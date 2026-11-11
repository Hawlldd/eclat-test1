#!/usr/bin/env python3
import struct
import zlib
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
DESKTOP = Path("/mnt/c/Users/raulr/OneDrive/Desktop")

def chunk(kind, payload):
    return struct.pack('>I', len(payload)) + kind + payload + struct.pack('>I', zlib.crc32(kind + payload) & 0xffffffff)

def png_from_4bpp(raw, palette):
    tiles = len(raw) // 32
    width = 128
    height = ((tiles + 15) // 16) * 8
    pixels = bytearray(width * height)
    for tile in range(tiles):
        tx, ty = tile % 16, tile // 16
        for y in range(8):
            for x in range(8):
                byte = raw[tile * 32 + y * 4 + x // 2]
                pixels[(ty * 8 + y) * width + tx * 8 + x] = (byte >> (4 * (x % 2))) & 15
    rgb = bytearray()
    for value, in struct.iter_unpack('<H', palette[:32]):
        rgb.extend(((value & 31) * 255 // 31, ((value >> 5) & 31) * 255 // 31, ((value >> 10) & 31) * 255 // 31))
    scanlines = b''.join(b'\0' + pixels[y * width:(y + 1) * width] for y in range(height))
    return b'\x89PNG\r\n\x1a\n' + chunk(b'IHDR', struct.pack('>IIBBBBB', width, height, 8, 3, 0, 0, 0)) + chunk(b'PLTE', bytes(rgb)) + chunk(b'IDAT', zlib.compress(scanlines, 9)) + chunk(b'IEND', b'')

def read_png(png_bytes):
    pos = 8
    width = height = bit_depth = color_type = None
    palette = None
    idat_parts = []
    while pos < len(png_bytes):
        length, = struct.unpack(">I", png_bytes[pos:pos+4])
        kind = png_bytes[pos+4:pos+8]
        data = png_bytes[pos+8:pos+8+length]
        pos += 12 + length
        if kind == b'IHDR':
            width, height, bit_depth, color_type = struct.unpack(">IIBB", data[:10])
        elif kind == b'PLTE':
            palette = data
        elif kind == b'IDAT':
            idat_parts.append(data)

    decompressed = zlib.decompress(b''.join(idat_parts))
    bpp = 1 if color_type == 3 else (3 if color_type == 2 else 4)
    stride = width * bpp + 1
    raw_bytes = bytearray(width * height * bpp)
    prev_row = bytearray(width * bpp)
    for y in range(height):
        filter_type = decompressed[y * stride]
        row_raw = decompressed[y * stride + 1 : (y + 1) * stride]
        row = bytearray(width * bpp)
        if filter_type == 0:
            row[:] = row_raw
        elif filter_type == 1:
            for i in range(len(row_raw)):
                left = row[i - bpp] if i >= bpp else 0
                row[i] = (row_raw[i] + left) & 0xFF
        elif filter_type == 2:
            for i in range(len(row_raw)):
                row[i] = (row_raw[i] + prev_row[i]) & 0xFF
        elif filter_type == 3:
            for i in range(len(row_raw)):
                left = row[i - bpp] if i >= bpp else 0
                above = prev_row[i]
                row[i] = (row_raw[i] + ((left + above) // 2)) & 0xFF
        elif filter_type == 4:
            for i in range(len(row_raw)):
                a = row[i - bpp] if i >= bpp else 0
                b = prev_row[i]
                c = prev_row[i - bpp] if i >= bpp else 0
                p = a + b - c
                pa, pb, pc = abs(p - a), abs(p - b), abs(p - c)
                pr = a if (pa <= pb and pa <= pc) else (b if pb <= pc else c)
                row[i] = (row_raw[i] + pr) & 0xFF
        raw_bytes[y * width * bpp : (y + 1) * width * bpp] = row
        prev_row = row
    return width, height, color_type, raw_bytes, palette

def import_if_modified():
    desktop_32 = DESKTOP / 'entrada_e_arvore_32x32_editavel.png'
    desktop_64 = DESKTOP / 'entrada_e_arvore_64x64.png'
    sec_bin_file = ROOT / 'data/tilesets/secondary/petipres/tiles.bin'

    if not sec_bin_file.exists():
        return False

    s_mtime = sec_bin_file.stat().st_mtime

    target_desktop = None
    if desktop_32.exists() and desktop_32.stat().st_mtime > s_mtime:
        target_desktop = desktop_32
    elif desktop_64.exists() and desktop_64.stat().st_mtime > s_mtime:
        target_desktop = desktop_64

    if not target_desktop:
        return False

    w_crop, h_crop, ctype, crop_data, crop_pal = read_png(target_desktop.read_bytes())
    palettes_bin = (ROOT / 'data/tilesets/secondary/petipres/palettes.bin').read_bytes()

    # Palette 0 lookup
    pal_rgb = []
    for val, in struct.iter_unpack('<H', palettes_bin[:32]):
        pal_rgb.append((
            ((val & 31) * 255) // 31,
            (((val >> 5) & 31) * 255) // 31,
            (((val >> 10) & 31) * 255) // 31
        ))

    def closest_idx(r, g, b):
        best_i = 0
        best_d = 999999999
        for i in range(16):
            pr, pg, pb = pal_rgb[i]
            d = (r - pr)**2 + (g - pg)**2 + (b - pb)**2
            if d < best_d:
                best_d = d
                best_i = i
        return best_i

    # Extract 32x32 grid of indices:
    grid_32 = [[0xD]*32 for _ in range(32)]

    if w_crop == 32 and h_crop == 32:
        for y in range(32):
            for x in range(32):
                if ctype == 3:
                    idx = crop_data[y * 32 + x]
                    if crop_pal:
                        cr, cg, cb = crop_pal[idx*3 : idx*3+3]
                        idx = closest_idx(cr, cg, cb)
                else:
                    bpp = 3 if ctype == 2 else 4
                    cr, cg, cb = crop_data[(y * 32 + x) * bpp : (y * 32 + x) * bpp + 3]
                    idx = closest_idx(cr, cg, cb)
                grid_32[y][x] = idx
    elif w_crop == 64 and h_crop == 64:
        for y in range(32):
            for x in range(32):
                src_x = 16 + x
                src_y = 16 + y
                if ctype == 3:
                    idx = crop_data[src_y * 64 + src_x]
                    if crop_pal:
                        cr, cg, cb = crop_pal[idx*3 : idx*3+3]
                        idx = closest_idx(cr, cg, cb)
                else:
                    bpp = 3 if ctype == 2 else 4
                    cr, cg, cb = crop_data[(src_y * 64 + src_x) * bpp : (src_y * 64 + src_x) * bpp + 3]
                    idx = closest_idx(cr, cg, cb)
                grid_32[y][x] = idx

    # Apply to sec_bin_file:
    sec_tiles_new = bytearray(sec_bin_file.read_bytes())

    # Left 16x32: entrance tiles
    tiles_left = [
        (296, 0, 0), (297, 8, 0),
        (312, 0, 8), (313, 8, 8),
        (328, 0, 16), (329, 8, 16),
        (344, 0, 24), (345, 8, 24)
    ]
    for tid, bx, by in tiles_left:
        for ty in range(8):
            for tx in range(0, 8, 2):
                p0 = grid_32[by + ty][bx + tx]
                p1 = grid_32[by + ty][bx + tx + 1]
                sec_tiles_new[tid * 32 + ty * 4 + tx // 2] = (p1 << 4) | p0

    # Right 16x32: tree tiles
    tiles_right = [
        (200, 16, 0), (201, 24, 0),
        (202, 16, 8), (203, 24, 8),
        (204, 16, 16), (205, 24, 16),
        (206, 16, 24), (207, 24, 24)
    ]
    for tid, bx, by in tiles_right:
        for ty in range(8):
            for tx in range(0, 8, 2):
                p0 = grid_32[by + ty][bx + tx]
                p1 = grid_32[by + ty][bx + tx + 1]
                sec_tiles_new[tid * 32 + ty * 4 + tx // 2] = (p1 << 4) | p0

    sec_bin_file.write_bytes(sec_tiles_new)

    # Sync to petipres_secondary/tiles.png
    prim_tiles = (ROOT / 'data/tilesets/primary/eclat_iviere/tiles.bin').read_bytes()
    editor_tiles = prim_tiles[512 * 32:] + bytes(sec_tiles_new)
    (ROOT / 'data/tilesets/secondary/petipres_secondary/tiles.png').write_bytes(png_from_4bpp(editor_tiles, palettes_bin))

    print(f"Importação de {target_desktop.name} aplicada com sucesso (Entrada + Árvore sincronizadas)!")
    return True

if __name__ == '__main__':
    import_if_modified()
