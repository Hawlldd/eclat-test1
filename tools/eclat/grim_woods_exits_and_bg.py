"""Sunlit concentric forest exit metatiles & Grim Woods battle backgrounds for Peaceful Woods."""
import math
import struct
import zlib
from pathlib import Path


class SimpleImage:
    def __init__(self, w: int, h: int, data: bytearray = None):
        self.width = w
        self.height = h
        self.data = bytearray(data) if data is not None else bytearray(w * h * 3)

    def getpixel(self, xy):
        i = (xy[1] * self.width + xy[0]) * 3
        return (self.data[i], self.data[i + 1], self.data[i + 2])

    def putpixel(self, xy, rgb):
        i = (xy[1] * self.width + xy[0]) * 3
        self.data[i : i + 3] = rgb

    def getdata(self):
        return [(self.data[i], self.data[i + 1], self.data[i + 2]) for i in range(0, len(self.data), 3)]

    def crop(self, box):
        x0, y0, x1, y1 = box
        cw, ch = x1 - x0, y1 - y0
        cd = bytearray(cw * ch * 3)
        for cy in range(ch):
            si = ((y0 + cy) * self.width + x0) * 3
            di = cy * cw * 3
            cd[di : di + cw * 3] = self.data[si : si + cw * 3]
        return SimpleImage(cw, ch, cd)

    def flip_lr(self):
        fd = bytearray(self.width * self.height * 3)
        for y in range(self.height):
            for x in range(self.width):
                si = (y * self.width + x) * 3
                di = (y * self.width + (self.width - 1 - x)) * 3
                fd[di : di + 3] = self.data[si : si + 3]
        return SimpleImage(self.width, self.height, fd)


def load_png_rgb(path: Path) -> SimpleImage:
    data = path.read_bytes()
    pos, idat = 8, bytearray()
    w, h, color_type = 0, 0, 0
    while pos < len(data):
        length, ctype = struct.unpack('>I4s', data[pos : pos + 8])
        cdata = data[pos + 8 : pos + 8 + length]
        pos += 12 + length
        if ctype == b'IHDR':
            w, h, _, color_type = struct.unpack('>IIBB', cdata[:10])
        elif ctype == b'IDAT':
            idat.extend(cdata)
        elif ctype == b'IEND':
            break
    raw = zlib.decompress(idat)
    bpp = 3 if color_type == 2 else 4
    stride = w * bpp
    unfilt = bytearray(stride * h)
    prev = bytearray(stride)
    rpos = 0
    for y in range(h):
        ft, curr = raw[rpos], bytearray(raw[rpos + 1 : rpos + 1 + stride])
        rpos += 1 + stride
        if ft == 1:
            for x in range(bpp, stride):
                curr[x] = (curr[x] + curr[x - bpp]) & 0xFF
        elif ft == 2:
            for x in range(stride):
                curr[x] = (curr[x] + prev[x]) & 0xFF
        elif ft == 3:
            for x in range(stride):
                a = curr[x - bpp] if x >= bpp else 0
                curr[x] = (curr[x] + ((a + prev[x]) >> 1)) & 0xFF
        elif ft == 4:
            for x in range(stride):
                a = curr[x - bpp] if x >= bpp else 0
                b, c = prev[x], prev[x - bpp] if x >= bpp else 0
                p = a + b - c
                pa, pb, pc = abs(p - a), abs(p - b), abs(p - c)
                pr = a if pa <= pb and pa <= pc else (b if pb <= pc else c)
                curr[x] = (curr[x] + pr) & 0xFF
        unfilt[y * stride : (y + 1) * stride] = curr
        prev = curr
    rgb = bytearray(w * h * 3)
    for i in range(w * h):
        rgb[i * 3 : i * 3 + 3] = unfilt[i * bpp : i * bpp + 3]
    return SimpleImage(w, h, rgb)



def lz77_decompress_stream(data: bytes, offset: int) -> tuple[bytes, int]:
    """Decompress GBA LZ77 stream at offset and return (decompressed_bytes, stream_end_offset)."""
    decomp_size = struct.unpack_from('<I', data, offset)[0] >> 8
    out = bytearray()
    pos = offset + 4
    while len(out) < decomp_size:
        flags = data[pos]
        pos += 1
        for bit in range(7, -1, -1):
            if len(out) >= decomp_size:
                break
            if (flags >> bit) & 1:
                b1, b2 = data[pos], data[pos + 1]
                pos += 2
                length = (b1 >> 4) + 3
                disp = (((b1 & 0x0F) << 8) | b2) + 1
                for _ in range(length):
                    out.append(out[-disp])
            else:
                out.append(data[pos])
                pos += 1
    while (pos - offset) % 4 != 0:
        pos += 1
    return bytes(out), pos


def lz77_compress_gba(raw: bytes) -> bytes:
    """Compress raw bytes into GBA BIOS Type 0x10 LZ77 format (VRAM-safe disp >= 2)."""
    n = len(raw)
    out = bytearray(struct.pack('<I', 0x10 | (n << 8)))
    pos = 0
    while pos < n:
        flag_idx = len(out)
        out.append(0)
        flags = 0
        for bit in range(7, -1, -1):
            if pos >= n:
                break
            best_len, best_disp = 0, 0
            max_len = min(18, n - pos)
            if pos >= 2 and max_len >= 3:
                start = max(0, pos - 4096)
                for cand in range(pos - 2, start - 1, -1):
                    l = 0
                    while l < max_len and raw[cand + l] == raw[pos + l]:
                        l += 1
                    if l > best_len:
                        best_len, best_disp = l, pos - cand
                        if best_len == max_len:
                            break
            if best_len >= 3:
                flags |= (1 << bit)
                disp_m1 = best_disp - 1
                out.append(((best_len - 3) << 4) | ((disp_m1 >> 8) & 0x0F))
                out.append(disp_m1 & 0xFF)
                pos += best_len
            else:
                out.append(raw[pos])
                pos += 1
        out[flag_idx] = flags
    while len(out) % 4 != 0:
        out.append(0)
    return bytes(out)


def _decode_16x16(tile_pool: list[bytes], entries: list[int]) -> list[list[int]]:
    grid = [[0] * 16 for _ in range(16)]
    for sub, (sr, sc) in enumerate([(0, 0), (0, 1), (1, 0), (1, 1)]):
        val = entries[sub]
        tid, hf, vf = val & 0x3FF, (val >> 10) & 1, (val >> 11) & 1
        tb = tile_pool[tid]
        for py in range(8):
            sy = 7 - py if vf else py
            for px in range(8):
                sx = 7 - px if hf else px
                b = tb[sy * 4 + sx // 2]
                grid[sr * 8 + py][sc * 8 + px] = (b >> 4) & 0xF if (sx & 1) else (b & 0xF)
    return grid


def _pack_8x8_tile(grid: list[list[int]], x0: int, y0: int) -> bytes:
    buf = bytearray(32)
    for py in range(8):
        for px in range(0, 8, 2):
            c0 = grid[y0 + py][x0 + px] & 0xF
            c1 = grid[y0 + py][x0 + px + 1] & 0xF
            buf[py * 4 + px // 2] = c0 | (c1 << 4)
    return bytes(buf)


def apply_sunlit_forest_exits(
    new_pals: list,
    tile_pool: list[bytes],
    free_prim_slots: list[int],
    new_mts: dict,
    gw_ground: list[int],
    gw_ovh_l: list[int],
    gw_ovh_r: list[int],
    gw_tree_tl: list[int],
    gw_tree_tr: list[int],
) -> dict[int, int]:
    """Build 5-band sunlit concentric exit ovals (Pal 2) & return {mt_id: attr_u32}."""
    pal2 = [(0, 0, 0)] * 16
    # Copy exact Grim Woods ground base (6) & pebbles (10, 15) for zero-seam outer transition
    for idx in (6, 10, 15):
        pal2[idx] = new_pals[7][idx]
    # 5 smooth concentric sunlit bands (stepping from misty forest teal to warm sunlit clearing)
    pal2[1] = (115, 148, 123)  # R5=14, G5=18, B5=15
    pal2[2] = (131, 164, 123)  # R5=16, G5=20, B5=15
    pal2[3] = (148, 180, 131)  # R5=18, G5=22, B5=16
    pal2[4] = (164, 205, 139)  # R5=20, G5=25, B5=17
    pal2[5] = (189, 222, 148)  # R5=23, G5=27, B5=18
    new_pals[2] = pal2

    gnd_16 = _decode_16x16(tile_pool, gw_ground)
    tile_cache: dict[bytes, int] = {}

    def alloc_pal2_tile(raw_tile: bytes) -> int:
        if raw_tile not in tile_cache:
            slot = free_prim_slots.pop(0)
            tile_pool[slot] = raw_tile
            tile_cache[raw_tile] = slot
        return (tile_cache[raw_tile] & 0x3FF) | (2 << 12)

    def build_oval_2x2(open_east: bool) -> list[list[list[int]]]:
        cx = 28.5 if open_east else 2.5
        cy, rx, ry = 13.0, 26.5, 12.2
        canvas = [[gnd_16[y % 16][x % 16] for x in range(32)] for y in range(32)]
        for y in range(32):
            for x in range(32):
                x_eff = min(x, cx) if open_east else max(x, cx)
                d = math.hypot((x_eff - cx) / rx, (y - cy) / ry)
                if d <= 0.28:
                    canvas[y][x] = 5
                elif d <= 0.46:
                    canvas[y][x] = 4
                elif d <= 0.64:
                    canvas[y][x] = 3
                elif d <= 0.82:
                    canvas[y][x] = 2
                elif d <= 1.00:
                    canvas[y][x] = 1
        out_2x2 = [[None, None], [None, None]]
        for mr in range(2):
            for mc in range(2):
                out_2x2[mr][mc] = [
                    alloc_pal2_tile(_pack_8x8_tile(canvas, mc * 16 + sc * 8, mr * 16 + sr * 8))
                    for sr, sc in ((0, 0), (0, 1), (1, 0), (1, 1))
                ]
        return out_2x2

    east_l0 = build_oval_2x2(open_east=True)
    west_l0 = build_oval_2x2(open_east=False)

    # NE Exit to Route 3 at (68..69, 8..9) — fixes swapped gw_ovh_l on MT 772!
    new_mts[770] = (east_l0[0][0], [0, 0, 0, 0])
    new_mts[772] = (east_l0[0][1], gw_ovh_l)
    new_mts[775] = (east_l0[1][0], gw_tree_tl)
    new_mts[771] = (east_l0[1][1], gw_tree_tr)

    # SW Exit to Route 2 at (12..13, 56..57) — fixes swapped gw_ovh_r on MT 768!
    new_mts[768] = (west_l0[0][0], gw_ovh_r)
    new_mts[773] = (west_l0[0][1], [0, 0, 0, 0])
    new_mts[769] = (west_l0[1][0], gw_tree_tl)
    new_mts[774] = (west_l0[1][1], gw_tree_tr)

    return {
        768: 0x00000063,  # NORMAL (L1 overhang over player), MB_WEST_ARROW_WARP
        769: 0x00000063,  # NORMAL (L1 tree crown over player), MB_WEST_ARROW_WARP
        770: 0x20000000,  # COVERED, MB_NORMAL
        771: 0x00000062,  # NORMAL (L1 tree crown over player), MB_EAST_ARROW_WARP
        772: 0x00000062,  # NORMAL (L1 overhang over player), MB_EAST_ARROW_WARP
        773: 0x20000000,  # COVERED, MB_NORMAL
        774: 0x00000000,  # NORMAL (L1 tree crown over player), MB_NORMAL
        775: 0x00000000,  # NORMAL (L1 tree crown over player), MB_NORMAL
    }


def apply_user_smoothed_art(
    root: Path,
    new_pals: list,
    tile_pool: list[bytes],
    free_prim_slots: list[int],
    new_mts: dict,
    exit_attrs: dict,
) -> None:
    """Extract user-smoothed route 3 exit (canopy, sunlight, fence & wall) from saida_rota3_template_1x.png."""
    user_p = root / 'eclat docs/saida_rota3_template_1x.png'
    if not user_p.exists():
        return

    user_im = load_png_rgb(user_p)

    pal1_set = set(new_pals[1])
    pal3_set = set(new_pals[3])
    pal4_set = set(new_pals[4])
    pal7_set = set(new_pals[7])
    pal8_set = set(new_pals[8])

    tile_cache: dict[bytes, int] = {}

    def best_match_color(c, pal):
        return min(range(len(pal)), key=lambda idx: sum((c[i] - pal[idx][i])**2 for i in range(3)))

    def alloc_tile(img_sub, pal_idx, trans_zero=False):
        pal = new_pals[pal_idx]
        buf = bytearray(32)
        pixels = []
        for y in range(8):
            for x in range(8):
                rgb = img_sub.getpixel((x, y))
                idx = best_match_color(rgb, pal)
                pixels.append(idx)
        if trans_zero and all(p == 0 for p in pixels):
            return 0
        for py in range(8):
            for px in range(0, 8, 2):
                c0 = pixels[py * 8 + px]
                c1 = pixels[py * 8 + px + 1]
                buf[py * 4 + px // 2] = c0 | (c1 << 4)
        raw = bytes(buf)
        if raw in tile_cache:
            return (tile_cache[raw] & 0x3FF) | ((pal_idx & 0xF) << 12)
        buf_flip = bytearray(32)
        for py in range(8):
            for px in range(0, 8, 2):
                c0 = pixels[py * 8 + (7 - px)]
                c1 = pixels[py * 8 + (7 - (px + 1))]
                buf_flip[py * 4 + px // 2] = c0 | (c1 << 4)
        raw_flip = bytes(buf_flip)
        if raw_flip in tile_cache:
            return (tile_cache[raw_flip] & 0x3FF) | ((pal_idx & 0xF) << 12) | 0x0400
        slot = free_prim_slots.pop(0)
        tile_pool[slot] = raw
        tile_cache[raw] = slot
        return (tile_cache[raw] & 0x3FF) | ((pal_idx & 0xF) << 12)

    user_im_flip = user_im.flip_lr()

    # 13 exit metatiles directly from saida_rota3_template_1x.png:
    r3_blocks = [
        (940, 0, 1, 0x20000000),  # (66, 6) Wall top
        (941, 0, 2, 0x00000000),  # (66, 7) Fence top post & rail
        (942, 0, 3, 0x00000000),  # (66, 8) Fence vertical bars
        (943, 0, 4, 0x00000000),  # (66, 9) Fence bottom rail
        (944, 0, 5, 0x20000000),  # (66, 10) Wall base
        (945, 0, 6, 0x00000000),  # (66, 11) Wall foot & moss
        (946, 1, 2, 0x20000000),  # (67, 7) Fence rail end & sign (COVERED: layer 1 behind player)
        (770, 2, 3, 0x20000000),  # (68, 8) Sunlit exit NW
        (775, 2, 4, 0x00000000),  # (68, 9) Sunlit exit SW
        (772, 3, 3, 0x00000062),  # (69, 8) Warp NE (MB_EAST_ARROW_WARP)
        (771, 3, 4, 0x00000062),  # (69, 9) Warp SE (MB_EAST_ARROW_WARP)
        (776, 4, 3, 0x20000001),  # (70, 8) Tree canopy left
        (777, 4, 4, 0x20000001),  # (70, 9) Tree canopy bottom-left
    ]

    r2_blocks = [
        (949, 5, 1, 0x20000000),  # (15, 54) Wall top
        (950, 5, 2, 0x00000000),  # (15, 55) Fence top post & rail
        (951, 5, 3, 0x00000000),  # (15, 56) Fence vertical bars
        (952, 5, 4, 0x00000000),  # (15, 57) Fence bottom rail
        (953, 5, 5, 0x20000000),  # (15, 58) Wall base
        (954, 5, 6, 0x00000000),  # (15, 59) Wall foot & moss
        (955, 4, 2, 0x20000000),  # (14, 55) Fence rail end & sign (COVERED: layer 1 behind player)
        (773, 3, 3, 0x20000000),  # (13, 56) Sunlit exit NE
        (774, 3, 4, 0x00000000),  # (13, 57) Sunlit exit SE
        (768, 2, 3, 0x00000063),  # (12, 56) Warp NW (MB_WEST_ARROW_WARP)
        (769, 2, 4, 0x00000063),  # (12, 57) Warp SW (MB_WEST_ARROW_WARP)
        (947, 1, 3, 0x20000001),  # (11, 56) Tree canopy right
        (948, 1, 4, 0x20000001),  # (11, 57) Tree canopy bottom-right
    ]

    for src_im, blist in ((user_im, r3_blocks), (user_im_flip, r2_blocks)):
        for mt, c, r, attr in blist:
            l0_entries = []
            l1_entries = []
            for sr in (0, 1):
                for sc in (0, 1):
                    sx = c * 16 + sc * 8
                    sy = r * 16 + sr * 8
                    sub_im = src_im.crop((sx, sy, sx + 8, sy + 8))
                    colors = set(sub_im.getdata())

                    has_l1 = False
                    l0_pal = 0
                    l1_pal = 0
                    l0_im = SimpleImage(8, 8)
                    l1_im = SimpleImage(8, 8)

                    if (c in (0, 5)) and r in (2, 3, 4):
                        has_l1 = True
                        l1_pal = 4
                        l0_pal = 3
                        for y in range(8):
                            for x in range(8):
                                p = sub_im.getpixel((x, y))
                                if p in pal4_set:
                                    l1_im.putpixel((x, y), p)
                                    l0_im.putpixel((x, y), new_pals[3][1])
                                else:
                                    l1_im.putpixel((x, y), (0, 0, 0))
                                    l0_im.putpixel((x, y), p)
                    elif (c in (1, 4)) and r == 2:
                        has_l1 = True
                        l1_pal = 1
                        l0_pal = 7
                        for y in range(8):
                            for x in range(8):
                                p = sub_im.getpixel((x, y))
                                if p in pal1_set:
                                    l1_im.putpixel((x, y), p)
                                    l0_im.putpixel((x, y), new_pals[7][6])
                                else:
                                    l1_im.putpixel((x, y), (0, 0, 0))
                                    l0_im.putpixel((x, y), p)
                    else:
                        tree_cols = {col for col in colors if col in pal8_set and col not in ((0, 0, 0), (98, 131, 115))}
                        if tree_cols:
                            has_l1 = True
                            l1_pal = 8
                            ground_cols = [col for col in colors if col not in tree_cols]
                            l0_pal = min([2, 7, 3, 5], key=lambda p: sum(min(sum((tc[i] - pc[i])**2 for i in range(3)) for pc in new_pals[p]) for tc in ground_cols)) if ground_cols else 2
                            for y in range(8):
                                for x in range(8):
                                    p = sub_im.getpixel((x, y))
                                    if p in tree_cols:
                                        l1_im.putpixel((x, y), p)
                                        l0_im.putpixel((x, y), new_pals[l0_pal][6] if 6 < len(new_pals[l0_pal]) else new_pals[l0_pal][1])
                                    else:
                                        l1_im.putpixel((x, y), (0, 0, 0))
                                        l0_im.putpixel((x, y), p)
                        else:
                            l0_pal = min(range(13), key=lambda p: sum(min(sum((tc[i] - pc[i])**2 for i in range(3)) for pc in new_pals[p]) for tc in colors))
                            l0_im = sub_im

                    t0 = alloc_tile(l0_im, l0_pal)
                    t1 = alloc_tile(l1_im, l1_pal, trans_zero=True) if has_l1 else 0
                    l0_entries.append(t0)
                    l1_entries.append(t1)

            new_mts[mt] = (l0_entries, l1_entries)
            exit_attrs[mt] = attr

    print(f"Injected user-smoothed exit metatiles for Route 3 & Route 2 ({len(tile_cache)} tiles)!")


def stamp_exit_metatiles_on_map(raw_map: bytearray) -> None:
    """Place inner sunlit exit column metatiles onto Peaceful Woods 78x78 map."""
    w = 78
    placements = {
        # NE Exit to Route 3 (x=66..70, y=6..11)
        (66, 6): (940, 0x0400),
        (66, 7): (941, 0x0400),
        (66, 8): (942, 0x0400),
        (66, 9): (943, 0x0400),
        (66, 10): (944, 0x0400),
        (66, 11): (945, 0x3000),
        (67, 7): (946, 0x0400),
        (68, 8): (770, 0x3000),
        (68, 9): (775, 0x3000),
        (69, 8): (772, 0x3000),
        (69, 9): (771, 0x3000),
        (70, 8): (776, 0x0400),
        (70, 9): (777, 0x0400),

        # SW Entrance from Route 2 (x=11..15, y=54..59)
        (15, 54): (949, 0x0400),
        (15, 55): (950, 0x0400),
        (15, 56): (656, 0x3000),
        (15, 57): (656, 0x3000),
        (15, 58): (953, 0x0400),
        (15, 59): (954, 0x0400),
        (14, 55): (955, 0x0400),
        (14, 56): (656, 0x3000),
        (14, 57): (656, 0x3000),
        (14, 58): (952, 0x0400),
        (13, 56): (773, 0x3000),
        (13, 57): (774, 0x3000),
        (12, 56): (768, 0x3000),
        (12, 57): (769, 0x3000),
        (11, 56): (947, 0x0400),
        (11, 57): (948, 0x0400),
    }
    for (x, y), (mt, bits) in placements.items():
        idx = (y * w + x) * 2
        struct.pack_into('<H', raw_map, idx, bits | mt)


def build_grim_woods_battle_bg(root: Path, urom: bytes) -> None:
    """Extract Grim Woods Tall Grass (#25) & Plain (#34) battle backgrounds (Day/Evening/Night)."""
    out_dir = root / 'graphics/battle_environment/grim_woods'
    out_dir.mkdir(parents=True, exist_ok=True)

    # 1. Grim Woods Tall Grass (Entry 25)
    for fname, off in (
        ('grass_tiles.lz.bin', 0xED4738),
        ('grass_map.lz.bin', 0xED54B8),
        ('grass_entry_tiles.lz.bin', 0x248C68),
    ):
        _, end = lz77_decompress_stream(urom, off)
        (out_dir / fname).write_bytes(urom[off:end])

    ub_entry_map = (root / 'graphics/battle_environment/unbound_grass/entry_map.lz.bin').read_bytes()
    (out_dir / 'grass_entry_map.lz.bin').write_bytes(ub_entry_map)

    for fname, off in (
        ('grass_palette_day.gbapal.bin', 0xED5B10),
        ('grass_palette_evening.gbapal.bin', 0xED6F54),
        ('grass_palette_night.gbapal.bin', 0xED6FF0),
    ):
        pal, _ = lz77_decompress_stream(urom, off)
        assert len(pal) == 96, f"Unexpected palette size {len(pal)} for {fname}"
        (out_dir / fname).write_bytes(pal)

    # 2. Grim Woods Plain / Trainer Forest Floor (Entry 34)
    for fname, off in (
        ('plain_tiles.lz.bin', 0xED5B4C),
        ('plain_map.lz.bin', 0xED64B8),
        ('plain_entry_tiles.lz.bin', 0x24C314),
    ):
        _, end = lz77_decompress_stream(urom, off)
        (out_dir / fname).write_bytes(urom[off:end])

    raw_plain_entry_map, _ = lz77_decompress_stream(urom, 0x24C520)
    padded_plain_entry_map = raw_plain_entry_map + b'\x00' * (2048 - len(raw_plain_entry_map))
    comp_plain_entry_map = lz77_compress_gba(padded_plain_entry_map)
    verify_decomp, _ = lz77_decompress_stream(comp_plain_entry_map, 0)
    assert verify_decomp == padded_plain_entry_map
    (out_dir / 'plain_entry_map.lz.bin').write_bytes(comp_plain_entry_map)

    for fname, off in (
        ('plain_palette_day.gbapal.bin', 0xED6AC4),
        ('plain_palette_evening.gbapal.bin', 0xED6F90),
        ('plain_palette_night.gbapal.bin', 0xED702C),
    ):
        pal, _ = lz77_decompress_stream(urom, off)
        assert len(pal) == 96, f"Unexpected palette size {len(pal)} for {fname}"
        (out_dir / fname).write_bytes(pal)
