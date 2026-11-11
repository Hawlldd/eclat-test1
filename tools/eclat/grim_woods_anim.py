"""Generate Unbound Tall Grass field-effect assets and Grim Woods swaying tree tileset animation."""
from pathlib import Path

CANOPY_GRID = [
    [None, 713, 714, 715, 716, None],
    [None, 729, 730, 731, 732, None],
    [ 744, 745, 746, 747, 748,  749],
    [ 760, 761, 762, 763, 764,  765],
]

# 8-step gentle forest breeze wave: (dx_top, dx_bot)
WIND_WAVE_STEPS = [
    ( 0,  0),
    (-1,  0),
    (-1, -1),
    ( 0, -1),
    ( 0,  0),
    (+1,  0),
    (+1, +1),
    ( 0, +1),
]


def _decode_tile(raw32: bytes):
    grid = []
    for y in range(8):
        row = []
        for x in range(0, 8, 2):
            b = raw32[y * 4 + x // 2]
            row.extend([b & 0xF, (b >> 4) & 0xF])
        grid.append(row)
    return grid


def _encode_tile(grid) -> bytes:
    out = bytearray(32)
    for y in range(8):
        for x in range(0, 8, 2):
            out[y * 4 + x // 2] = (grid[y][x] & 0xF) | ((grid[y][x + 1] & 0xF) << 4)
    return bytes(out)


def _shift_row_clamped(row, x_min: int, x_max: int, dx: int):
    if dx == 0:
        return list(row)
    seg = row[x_min:x_max]
    if dx == -1:
        shifted = ([seg[0]] + seg[2:] + [0]) if seg[0] != 0 else (seg[1:] + [0])
    else:
        shifted = ([0] + seg[:-2] + [seg[-1]]) if seg[-1] != 0 else ([0] + seg[:-1])
    out = list(row)
    out[x_min:x_max] = shifted
    return out


def _shift_interior_runs(row, x_min: int, x_max: int, dx: int):
    if dx == 0:
        return list(row)
    out = list(row)
    x = x_min
    while x < x_max:
        if row[x] not in (0, 14, 1, 6, 7):
            s = x
            while x < x_max and row[x] not in (0, 14, 1, 6, 7):
                x += 1
            if x - s >= 3:
                run = row[s:x]
                out[s:x] = (run[1:] + [run[-1]]) if dx == -1 else ([run[0]] + run[:-1])
        else:
            x += 1
    return out


def _make_canopy_step(canvas0, dx_top: int, dx_bot: int):
    c = [list(r) for r in canvas0]
    for y in range(1, 16):
        c[y] = _shift_row_clamped(c[y], 8, 40, dx_top)
    for y in range(16, 29):
        c[y] = _shift_row_clamped(c[y], 0, 48, dx_bot)
    if dx_bot != 0:
        c[29] = _shift_interior_runs(c[29], 10, 39, dx_bot)
        c[30] = _shift_interior_runs(c[30], 11, 38, dx_bot)
    return c


def _merge_tile_bytes(bot32: bytes, top32: bytes) -> bytes:
    out = bytearray(32)
    for i in range(32):
        lo = (top32[i] & 0xF) or (bot32[i] & 0xF)
        hi = ((top32[i] >> 4) & 0xF) or ((bot32[i] >> 4) & 0xF)
        out[i] = lo | (hi << 4)
    return bytes(out)


def build_unbound_grass_and_tree_anim(root: Path, urom: bytes, tile_pool: list, merged_pal8_records: list):
    """Write Unbound tall grass sprite/palette and 8-step Grim Woods swaying tree animation binary."""
    # 1. Exact 1:1 Pokémon Unbound Tall Grass 5-frame sprite (0x39A008) & palette (0x398FC8)
    gfx_dir = root / 'graphics/eclat_intro'
    (gfx_dir / 'unbound_tall_grass.bin').write_bytes(urom[0x39A008:0x39A008 + 5 * 128])
    (gfx_dir / 'grim_woods_tall_grass_palette.bin').write_bytes(urom[0x398FC8:0x398FC8 + 32])

    # 2. Build base 48x32 Grim Woods tree canopy canvas
    canvas0 = [[0] * 48 for _ in range(32)]
    for tr in range(4):
        for tc in range(6):
            tid = CANOPY_GRID[tr][tc]
            if tid is not None:
                g = _decode_tile(tile_pool[tid])
                for y in range(8):
                    for x in range(8):
                        canvas0[tr * 8 + y][tc * 8 + x] = g[y][x]

    # Verify merged_pal8_records occupy expected primary slots [1..13, 15..19] (tile 14 is used by GW_PRIM)
    slots = [rec[0] for rec in merged_pal8_records]
    expected_slots = list(range(1, 14)) + list(range(15, 20))
    assert slots == expected_slots, f"Expected slots {expected_slots}, got {slots}"

    anim_out = bytearray()
    for step_idx, (dx_top, dx_bot) in enumerate(WIND_WAVE_STEPS):
        step_canvas = _make_canopy_step(canvas0, dx_top, dx_bot)
        canopy_tiles = {}
        for tr in range(4):
            for tc in range(6):
                tid = CANOPY_GRID[tr][tc]
                if tid is not None:
                    sub = [step_canvas[tr * 8 + y][tc * 8:tc * 8 + 8] for y in range(8)]
                    canopy_tiles[tid] = _encode_tile(sub)

        # Group 0: Contiguous merged Pal 8 composite tiles (slots 1..18)
        for _slot, bt, tt in merged_pal8_records:
            bb = canopy_tiles.get(bt, tile_pool[bt])
            tb = canopy_tiles.get(tt, tile_pool[tt])
            anim_out += _merge_tile_bytes(bb, tb)

        # Groups 1..4: Canopy rows 713..716 (4), 729..732 (4), 744..749 (6), 760..765 (6)
        for seq in ([713, 714, 715, 716], [729, 730, 731, 732],
                    [744, 745, 746, 747, 748, 749], [760, 761, 762, 763, 764, 765]):
            for tid in seq:
                anim_out += canopy_tiles[tid]

    anim_path = root / 'data/tilesets/secondary/peaceful_woods_grim/tree_anim.bin'
    anim_path.write_bytes(bytes(anim_out))
    print(f"Built Grim Woods tree anim ({len(anim_out)} B, 38 tiles x 8 steps)!")


