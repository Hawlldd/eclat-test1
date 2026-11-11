#!/usr/bin/env python3
import struct
import zlib
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]

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

def read_indexed_png(png_bytes):
    pos = 8
    width = height = None
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
    stride = width + 1
    pixels = bytearray(width * height)
    prev_row = bytearray(width)
    for y in range(height):
        filter_type = decompressed[y * stride]
        row_raw = decompressed[y * stride + 1 : (y + 1) * stride]
        row = bytearray(width)
        if filter_type == 0:
            row[:] = row_raw
        elif filter_type == 1:
            val = 0
            for x in range(width):
                val = (val + row_raw[x]) & 0xFF
                row[x] = val
        elif filter_type == 2:
            for x in range(width):
                row[x] = (row_raw[x] + prev_row[x]) & 0xFF
        else:
            raise NotImplementedError(f"Filter type {filter_type}")
        pixels[y * width : (y + 1) * width] = row
        prev_row = row
    return width, height, pixels, palette

def sync():
    sec_png_path = ROOT / 'data/tilesets/secondary/petipres_secondary/tiles.png'
    petipres_dir = ROOT / 'data/tilesets/secondary/petipres'
    bin_file = petipres_dir / 'tiles.bin'
    png_file = petipres_dir / 'tiles.png'
    pal_file = petipres_dir / 'palettes.bin'

    if not sec_png_path.exists():
        return

    sec_png = sec_png_path.read_bytes()
    w, h, pixels, _ = read_indexed_png(sec_png)
    total_tiles = (w // 8) * (h // 8)
    raw_4bpp = bytearray(total_tiles * 32)
    for tile in range(total_tiles):
        tx = tile % 16
        ty = tile // 16
        for y in range(8):
            for x in range(0, 8, 2):
                p0 = pixels[(ty * 8 + y) * w + (tx * 8 + x)] & 0xF
                p1 = pixels[(ty * 8 + y) * w + (tx * 8 + x + 1)] & 0xF
                raw_4bpp[tile * 32 + y * 4 + x // 2] = (p1 << 4) | p0

    petipres_tiles_new = bytes(raw_4bpp[128 * 32 : 512 * 32])
    old_bin = bin_file.read_bytes() if bin_file.exists() else b''

    if petipres_tiles_new != old_bin:
        bin_file.write_bytes(petipres_tiles_new)
        if pal_file.exists():
            palettes = pal_file.read_bytes()
            png_file.write_bytes(png_from_4bpp(petipres_tiles_new, palettes))
        print("Sincronizado petipres_secondary -> petipres/tiles.bin com sucesso!")
    else:
        print("petipres/tiles.bin já está atualizado.")

if __name__ == '__main__':
    sync()
