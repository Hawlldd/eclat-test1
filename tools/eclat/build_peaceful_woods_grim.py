"""Build the Grim Woods (Pokémon Unbound) tileset, tall grass & tree sway for Peaceful Woods."""
import json
import struct
from pathlib import Path

from grim_woods_anim import build_unbound_grass_and_tree_anim
from grim_woods_exits_and_bg import apply_sunlit_forest_exits, apply_user_smoothed_art, build_grim_woods_battle_bg, stamp_exit_metatiles_on_map
from grim_woods_river import configure_peaceful_woods_encounters, register_grim_woods_river_metatiles, stamp_dry_hollow_logs

ROOT = Path(__file__).resolve().parents[2]
UNBOUND_ROM = Path('C:/Users/raulr/OneDrive/Desktop/zEmerald/unbound/Pokémon - Unbound.gba')


def lz77_decompress(data: bytes, offset: int) -> bytes:
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
    return bytes(out)


def unpack_pal_block(raw: bytes, count: int = 16):
    pals = []
    for p in range(min(count, len(raw) // 32)):
        cols = []
        for c in range(16):
            val = struct.unpack_from('<H', raw, p * 32 + c * 2)[0]
            cols.append((
                (val & 0x1F) * 255 // 31,
                ((val >> 5) & 0x1F) * 255 // 31,
                ((val >> 10) & 0x1F) * 255 // 31,
            ))
        pals.append(cols)
    return pals


def pack_pal_block(pals) -> bytes:
    out = bytearray()
    for cols in pals:
        for r, g, b in cols:
            r5 = (r * 31 + 127) // 255
            g5 = (g * 31 + 127) // 255
            b5 = (b * 31 + 127) // 255
            out += struct.pack('<H', (r5 & 0x1F) | ((g5 & 0x1F) << 5) | ((b5 & 0x1F) << 10))
    return bytes(out)


def gba_snap(rgb):
    return tuple(((c * 31 + 127) // 255) * 255 // 31 for c in rgb)


def tint_for_grim_woods(rgb, sat_mul=0.82, r_mul=0.78, g_mul=0.84, b_mul=0.88):
    r, g, b = rgb
    lum = 0.299 * r + 0.587 * g + 0.114 * b
    r = max(0, min(255, int((lum + (r - lum) * sat_mul) * r_mul)))
    g = max(0, min(255, int((lum + (g - lum) * sat_mul) * g_mul)))
    b = max(0, min(255, int((lum + (b - lum) * sat_mul) * b_mul)))
    return gba_snap((r, g, b))


def mk_entry(t_id, pal_id, hflip=0, vflip=0):
    return (t_id & 0x3FF) | ((hflip & 1) << 10) | ((vflip & 1) << 11) | ((pal_id & 0xF) << 12)


def main():
    urom = UNBOUND_ROM.read_bytes()
    gw_prim_t = lz77_decompress(urom, 0xCF91A0)
    gw_prim_p = urom[0xEA1B68:0xEA1B68 + 16 * 32]
    gw_prim_m = urom[0x29F6C8:0x29F6C8 + 640 * 16]
    gw_prim_a = urom[0x2A1EC8:0x2A1EC8 + 640 * 4]

    gw_sec_t = lz77_decompress(urom, 0xC4AB58)
    gw_sec_t += b'\x00' * (384 * 32 - len(gw_sec_t))
    gw_sec_p = urom[0x296E90:0x296E90 + 16 * 32]
    gw_sec_m = urom[0xBD2234:0xBD2F34] + b'\x00' * ((384 - 208) * 16)
    gw_sec_a = urom[0xBD2F34:0xBD2F34 + 208 * 4] + b'\x00' * ((384 - 208) * 4)

    pw_prim_dir = ROOT / 'data/tilesets/primary/eclat_iviere'
    pw_sec_dir = ROOT / 'data/tilesets/secondary/peaceful_woods'
    pw_prim_t = (pw_prim_dir / 'tiles.bin').read_bytes()
    pw_prim_p = (pw_prim_dir / 'palettes.bin').read_bytes()
    pw_prim_m = (pw_prim_dir / 'metatiles.bin').read_bytes()
    pw_prim_a = (pw_prim_dir / 'metatile_attributes.bin').read_bytes()
    pw_sec_t = (pw_sec_dir / 'tiles.bin').read_bytes()
    pw_sec_p = (pw_sec_dir / 'palettes.bin').read_bytes()
    pw_sec_m = (pw_sec_dir / 'metatiles.bin').read_bytes()
    pw_sec_a = (pw_sec_dir / 'metatile_attributes.bin').read_bytes()

    gw_pals_p = unpack_pal_block(gw_prim_p, 16)
    gw_pals_s = unpack_pal_block(gw_sec_p, 16)
    pw_pals_p = unpack_pal_block(pw_prim_p, 13)
    pw_pals_s = unpack_pal_block(pw_sec_p, 13)

    new_pals = [list(gw_pals_p[p]) if p < 7 else (list(gw_pals_s[p]) if p < 12 else [(0, 0, 0)] * 16) for p in range(16)]

    new_pals[3] = [(0, 0, 0) if i == 0 else (16, 24, 24) if i == 1 else (98, 131, 115) if i == 4 else (82, 115, 98) if i == 5
                   else tint_for_grim_woods(col, 0.75, 0.74, 0.78, 0.82) for i, col in enumerate(pw_pals_s[11])]
    new_pals[5] = [(0, 0, 0) if i == 0 else (98, 131, 115) if i == 4 else (82, 115, 98) if i == 5
                   else tint_for_grim_woods(col, 0.75, 0.74, 0.78, 0.82) for i, col in enumerate(pw_pals_p[1])]
    wood_ramp = [(32, 24, 16), (65, 49, 32), (98, 74, 49), (123, 98, 65), (148, 115, 82)]
    new_pals[4] = [(0, 0, 0) if i == 0 else gba_snap(wood_ramp[i - 11]) if i >= 11
                   else tint_for_grim_woods(col, 0.65, 0.72, 0.80, 0.80) for i, col in enumerate(pw_pals_p[3])]
    new_pals[6] = list(gw_pals_p[0][:13]) + [(98, 131, 115), (82, 115, 98), (41, 82, 74)]
    new_pals[12] = [(0, 0, 0) if i == 0 else tint_for_grim_woods(col, 0.85, 0.80, 0.84, 0.88) for i, col in enumerate(pw_pals_s[7])]

    tile_pool = [gw_prim_t[i * 32:(i + 1) * 32] for i in range(640)] + [gw_sec_t[i * 32:(i + 1) * 32] for i in range(384)]
    pw_all_tiles = [pw_prim_t[i * 32:(i + 1) * 32] for i in range(640)] + [pw_sec_t[i * 32:(i + 1) * 32] for i in range(384)]

    gw_prim_used = {0, 0x263, 0x264, 0x265, 0x266, 0x273, 0x274, 0x275, 0x276}
    for mt in range(640, 848):
        for val in struct.unpack_from('<8H', gw_sec_m, (mt - 640) * 16):
            if (val & 0x3FF) < 640:
                gw_prim_used.add(val & 0x3FF)
    free_prim_slots = [i for i in range(1, 640) if i not in gw_prim_used]
    imported_map = {}

    def import_pw_tile(pw_tid):
        if pw_tid == 0:
            return 0
        if pw_tid not in imported_map:
            slot = free_prim_slots.pop(0)
            tile_pool[slot] = pw_all_tiles[pw_tid]
            imported_map[pw_tid] = slot
        return imported_map[pw_tid]

    def gw_sec_layers(gw_mt_id):
        e = struct.unpack_from('<8H', gw_sec_m, (gw_mt_id - 640) * 16)
        return list(e[:4]), list(e[4:])

    merged_pal8_records = []

    def merge_same_pal_8(bot_4, top_4):
        res = []
        for sub in range(4):
            bv, tv = bot_4[sub], top_4[sub]
            bt, tt = bv & 0x3FF, tv & 0x3FF
            if tt == 0:
                res.append(bv)
            elif bt == 0:
                res.append(tv)
            else:
                key = ('m8', bv, tv)
                if key not in imported_map:
                    bb, tb = tile_pool[bt], tile_pool[tt]
                    out_b = bytearray(32)
                    for i in range(32):
                        lo = (tb[i] & 0xF) or (bb[i] & 0xF)
                        hi = ((tb[i] >> 4) & 0xF) or ((bb[i] >> 4) & 0xF)
                        out_b[i] = lo | (hi << 4)
                    slot = free_prim_slots.pop(0)
                    tile_pool[slot] = bytes(out_b)
                    imported_map[key] = mk_entry(slot, 8)
                    merged_pal8_records.append((slot, bt, tt))
                res.append(imported_map[key])
        return res

    # Bake a 4-subtile Grim Woods tall grass top layer onto GW misty ground into single-layer Pal 6 tiles!
    gw_ground, _ = gw_sec_layers(640)

    def bake_grass_l0(grass_l1):
        res = []
        for sub in range(4):
            g_tid = gw_ground[sub] & 0x3FF
            t_val = grass_l1[sub]
            t_tid = t_val & 0x3FF
            key = ('grass6', g_tid, t_tid)
            if key not in imported_map:
                gb, tb = tile_pool[g_tid], tile_pool[t_tid]
                out_b = bytearray(32)
                for i in range(32):
                    p = []
                    for shift in (0, 4):
                        tc = (tb[i] >> shift) & 0xF
                        if tc != 0 and tc <= 12:
                            p.append(tc)
                        else:
                            gc = (gb[i] >> shift) & 0xF
                            col = gw_pals_s[7][gc]
                            p.append(13 if col[1] >= 127 else 14)
                    out_b[i] = p[0] | (p[1] << 4)
                slot = free_prim_slots.pop(0)
                tile_pool[slot] = bytes(out_b)
                imported_map[key] = mk_entry(slot, 6)
            res.append(imported_map[key])
        return res

    _, gw_tree_tl = gw_sec_layers(649)
    _, gw_tree_tr = gw_sec_layers(650)
    gw_dense_tl = gw_sec_layers(742)
    gw_dense_tr = gw_sec_layers(743)
    _, gw_mid_l = gw_sec_layers(657)
    _, gw_mid_r = gw_sec_layers(658)
    _, gw_ovh_l = gw_sec_layers(656)
    _, gw_ovh_r = gw_sec_layers(659)
    _, gw_trunk_l = gw_sec_layers(665)
    _, gw_trunk_r = gw_sec_layers(666)
    gw_both_ovh = merge_same_pal_8(gw_ovh_r, gw_ovh_l)
    gw_moss_opaque = [0x9306, 0x9307, 0x9316, 0x9317]

    new_mts = {
        652: (gw_ground, [0, 0, 0, 0]), 656: (gw_ground, [0, 0, 0, 0]),
        641: (gw_ground, gw_tree_tl), 642: (gw_ground, gw_tree_tr),
        643: gw_dense_tl, 644: gw_dense_tr,
        648: (gw_ground, gw_ovh_l), 649: (gw_ground, gw_mid_l),
        650: (gw_ground, gw_mid_r), 651: (gw_ground, gw_ovh_r),
        657: (gw_ground, gw_trunk_l), 658: (gw_ground, gw_trunk_r),
        660: (gw_ground, merge_same_pal_8(gw_mid_r, gw_ovh_l)),
        661: (gw_ground, merge_same_pal_8(gw_trunk_r, gw_tree_tl)),
        687: (gw_ground, merge_same_pal_8(gw_ovh_l, gw_tree_tr)),
        765: (gw_ground, merge_same_pal_8(gw_ovh_l, gw_tree_tr)),
        766: (gw_ground, merge_same_pal_8(gw_ovh_r, gw_tree_tl)),
        767: (gw_ground, merge_same_pal_8(gw_trunk_r, gw_ovh_l)),
        759: (gw_ground, merge_same_pal_8(gw_trunk_l, gw_ovh_r)),
        758: (gw_ground, gw_both_ovh),
        732: (gw_ground, merge_same_pal_8(gw_trunk_l, gw_tree_tr)),
        709: (gw_moss_opaque, gw_tree_tl), 710: (gw_moss_opaque, gw_tree_tr),
    }

    for pw_id, gw_id in {
        664: 644, 665: 645, 666: 646,
        672: 652, 673: 653, 674: 654,
        680: 660, 681: 661, 682: 662,
        667: 668, 668: 669, 675: 676, 676: 677,
        669: 653, 677: 653,
        683: 670, 684: 671, 691: 678, 692: 679,
        778: 647,
    }.items():
        new_mts[pw_id] = gw_sec_layers(gw_id)

    mush_l0, mush_l1 = gw_sec_layers(723)
    new_mts[752] = (mush_l0, mush_l1)
    new_mts[753] = (mush_l0, [mush_l1[1] ^ 0x400, mush_l1[0] ^ 0x400, mush_l1[3] ^ 0x400, mush_l1[2] ^ 0x400])

    exit_mt_attrs = apply_sunlit_forest_exits(
        new_pals, tile_pool, free_prim_slots, new_mts,
        gw_ground, gw_ovh_l, gw_ovh_r, gw_tree_tl, gw_tree_tr,
    )
    apply_user_smoothed_art(ROOT, new_pals, tile_pool, free_prim_slots, new_mts, exit_mt_attrs)
    river_mt_attrs = register_grim_woods_river_metatiles(
        new_pals, gw_pals_p, new_mts, gw_sec_layers, gw_ovh_l, gw_ovh_r,
    )

    pw_tree_l1_replace = {
        (644, 645, 660, 661): gw_tree_tl,
        (646, 647, 662, 663): gw_tree_tr,
        (680, 681, 696, 697): gw_ovh_r,
        (705, 704, 721, 720): merge_same_pal_8(gw_ovh_r, gw_tree_tl),
        (759, 680, 759, 696): gw_ovh_l,
        (0, 680, 0, 696): gw_ovh_l,
        (680, 0, 696, 0): gw_ovh_r,
        (680, 680, 696, 696): gw_both_ovh,
    }

    for mt in [688, 689, 690, 695, 696, 697, 698, 699, 700, 701, 702, 703, 704, 705, 706]:
        entries = struct.unpack_from('<8H', pw_sec_m, (mt - 640) * 16)
        l1 = [mk_entry(import_pw_tile(v & 0x3FF), 12, (v >> 10) & 1, (v >> 11) & 1) if (v & 0x3FF) else 0 for v in entries[4:]]
        new_mts[mt] = (gw_ground, l1)

    def remap_pw_entry(val):
        tid = val & 0x3FF
        if tid == 0:
            return 0
        hf, vf, pal = (val >> 10) & 1, (val >> 11) & 1, (val >> 12) & 0xF
        if pal == 10 and tid in (998, 999, 1014, 1015):
            return gw_ground[[998, 999, 1014, 1015].index(tid)]
        if pal == 11:
            return mk_entry(import_pw_tile(tid), 3, hf, vf)
        if pal == 1:
            return mk_entry(import_pw_tile(tid), 5, hf, vf)
        if pal == 3:
            return mk_entry(import_pw_tile(tid), 4, hf, vf)
        if pal == 9:
            return gw_ground[0]
        if pal == 8:
            if tid == 680:
                return gw_ovh_r[0]
            if tid == 696:
                return gw_ovh_r[2]
        return 0

    map_path = ROOT / 'data/layouts/peaceful_woods/map.bin'
    base_backup = ROOT / 'data/tilesets/secondary/peaceful_woods_grim/base_map.bin'
    base_backup.parent.mkdir(parents=True, exist_ok=True)
    if not base_backup.exists():
        init_map = bytearray(map_path.read_bytes())
        for idx in range(0, len(init_map), 2):
            v = struct.unpack_from('<H', init_map, idx)[0]
            if 792 <= (v & 0x3FF) < 888:
                struct.pack_into('<H', init_map, idx, (v & 0xFC00) | 652)
        base_backup.write_bytes(bytes(init_map))

    raw_map = bytearray(base_backup.read_bytes())
    all_used = sorted(set(v & 0x3FF for (v,) in struct.iter_unpack('<H', raw_map)))
    for mt in all_used:
        if mt in new_mts:
            continue
        entries = struct.unpack_from('<8H', pw_prim_m if mt < 640 else pw_sec_m, (mt if mt < 640 else mt - 640) * 16)
        l0_raw, l1_raw = entries[:4], entries[4:]
        l0 = list(gw_ground) if all((v & 0x3FF) in (0, 998, 999, 1014, 1015) for v in l0_raw) else [remap_pw_entry(v) for v in l0_raw]
        l1_tids = tuple(v & 0x3FF for v in l1_raw)
        l1 = list(pw_tree_l1_replace[l1_tids]) if l1_tids in pw_tree_l1_replace else [remap_pw_entry(v) for v in l1_raw]
        if mt == 785:
            l0, l1 = list(gw_ground), [mk_entry(864, 9), mk_entry(869, 9), mk_entry(798, 9), mk_entry(863, 9)]
        new_mts[mt] = (l0, l1)

    nesw_gw_l1 = {
        (0, 1, 1, 0): gw_sec_layers(760)[1], (0, 1, 1, 1): gw_sec_layers(761)[1], (0, 0, 1, 1): gw_sec_layers(762)[1],
        (1, 1, 1, 0): gw_sec_layers(768)[1], (1, 1, 1, 1): [0x0044, 0x0045, 0x0054, 0x0055], (1, 0, 1, 1): gw_sec_layers(770)[1],
        (1, 1, 0, 0): gw_sec_layers(776)[1], (1, 1, 0, 1): gw_sec_layers(777)[1], (1, 0, 0, 1): gw_sec_layers(778)[1],
        (0, 0, 1, 0): gw_sec_layers(771)[1], (1, 0, 1, 0): gw_sec_layers(772)[1], (1, 0, 0, 0): gw_sec_layers(779)[1],
        (0, 1, 0, 0): gw_sec_layers(764)[1], (0, 1, 0, 1): gw_sec_layers(780)[1], (0, 0, 0, 1): gw_sec_layers(765)[1],
        (0, 0, 0, 0): gw_sec_layers(769)[1],
    }
    baked_l0_by_nesw = {k: bake_grass_l0(v) for k, v in nesw_gw_l1.items()}

    # Porymap 8-col visual 3x3 stamp at 792..811 (no overhang)
    stamp_slots = {
        (0, 1, 1, 0): 792, (0, 1, 1, 1): 793, (0, 0, 1, 1): 794, (0, 0, 1, 0): 795,
        (0, 1, 0, 0): 796, (0, 1, 0, 1): 797, (0, 0, 0, 1): 798,
        (1, 1, 1, 0): 800, (1, 1, 1, 1): 801, (1, 0, 1, 1): 802, (1, 0, 1, 0): 803, (0, 0, 0, 0): 804,
        (1, 1, 0, 0): 808, (1, 1, 0, 1): 809, (1, 0, 0, 1): 810, (1, 0, 0, 0): 811,
    }
    grass_covered_mts = set()
    grass_normal_mts = set()
    nesw_list = list(nesw_gw_l1.keys())

    for nesw, mt_slot in stamp_slots.items():
        new_mts[mt_slot] = (baked_l0_by_nesw[nesw], [0, 0, 0, 0])
        grass_covered_mts.add(mt_slot)

    # Overhang + Tall Grass sets (840..855 = Ovh-L, 856..871 = Ovh-R, 872..887 = Both Ovh)
    ovh_grass_lookup = {}
    for base_mt, ovh_key, ovh_l1 in (
        (840, 648, gw_ovh_l),
        (856, 651, gw_ovh_r),
        (872, 758, gw_both_ovh),
    ):
        for i, nesw in enumerate(nesw_list):
            mt_slot = base_mt + i
            new_mts[mt_slot] = (baked_l0_by_nesw[nesw], ovh_l1)
            grass_normal_mts.add(mt_slot)
            ovh_grass_lookup[(ovh_key, nesw)] = mt_slot

    # Extra Grim Woods hollow logs & puddles in 816..835 for Porymap
    extra_gw = {
        816: 707, 817: 708, 818: 695, 819: 714,
        824: 715, 825: 716, 826: 703, 827: 722,
        832: 664, 833: 667, 834: 711, 835: 719,
    }
    for dst_mt, src_gw in extra_gw.items():
        new_mts[dst_mt] = gw_sec_layers(src_gw)

    prim_m_out = bytearray(640 * 16)
    prim_a_out = bytearray(gw_prim_a)
    for mt in range(640):
        if mt in new_mts:
            l0, l1 = new_mts[mt]
            struct.pack_into('<8H', prim_m_out, mt * 16, *(l0 + l1))
            struct.pack_into('<I', prim_a_out, mt * 4, struct.unpack_from('<I', pw_prim_a, mt * 4)[0])
        else:
            prim_m_out[mt * 16:(mt + 1) * 16] = gw_prim_m[mt * 16:(mt + 1) * 16]

    sec_m_out = bytearray(384 * 16)
    sec_a_out = bytearray(pw_sec_a)
    for mt in range(640, 1024):
        off = mt - 640
        if mt in new_mts:
            l0, l1 = new_mts[mt]
            struct.pack_into('<8H', sec_m_out, off * 16, *(l0 + l1))
        if mt in (687, 643, 644, 709, 710):
            struct.pack_into('<I', sec_a_out, off * 4, 0x00000000)
        elif mt in grass_covered_mts:
            struct.pack_into('<I', sec_a_out, off * 4, 0x20000002)
        elif mt in grass_normal_mts:
            struct.pack_into('<I', sec_a_out, off * 4, 0x00000002)  # L0 grass under player, L1 overhang over player!
        elif mt in exit_mt_attrs:
            struct.pack_into('<I', sec_a_out, off * 4, exit_mt_attrs[mt])
        elif mt in river_mt_attrs:
            struct.pack_into('<I', sec_a_out, off * 4, river_mt_attrs[mt])
        elif mt in extra_gw:
            gw_attr = struct.unpack_from('<I', gw_sec_a, (extra_gw[mt] - 640) * 4)[0]
            struct.pack_into('<I', sec_a_out, off * 4, 0x20000000 | (gw_attr & 0xFF))

    build_unbound_grass_and_tree_anim(ROOT, urom, tile_pool, merged_pal8_records)
    build_grim_woods_battle_bg(ROOT, urom)

    pal_bytes = pack_pal_block(new_pals)
    for role, t_slice, m_bytes, a_bytes in (
        ('primary', b''.join(tile_pool[:640]), bytes(prim_m_out), bytes(prim_a_out)),
        ('secondary', b''.join(tile_pool[640:]), bytes(sec_m_out), bytes(sec_a_out)),
    ):
        out_d = ROOT / 'data/tilesets' / role / 'peaceful_woods_grim'
        out_d.mkdir(parents=True, exist_ok=True)
        (out_d / 'tiles.bin').write_bytes(t_slice)
        (out_d / 'palettes.bin').write_bytes(pal_bytes)
        (out_d / 'metatiles.bin').write_bytes(m_bytes)
        (out_d / 'metatile_attributes.bin').write_bytes(a_bytes)

    map_json = json.loads((ROOT / 'data/maps/PeacefulWoods/map.json').read_text(encoding='utf-8'))
    stamp_exit_metatiles_on_map(raw_map)
    stamp_dry_hollow_logs(raw_map, map_json)
    place_tall_grass_patches(raw_map, map_json, stamp_slots, ovh_grass_lookup)
    map_path.write_bytes(bytes(raw_map))
    configure_peaceful_woods_encounters(ROOT)

    print(f"Built peaceful_woods_grim tilesets! Free primary tiles left: {len(free_prim_slots)}")


def place_tall_grass_patches(raw_map: bytearray, map_json: dict, stamp_slots: dict, ovh_grass_lookup: dict):
    W = 78

    blocked = set()
    for o in map_json['object_events']:
        ox, oy = o['x'], o['y']
        blocked.add((ox, oy))
        if 'BUG_CATCHER' in o['graphics_id'] or 'CUTTABLE_TREE' in o['graphics_id'] or 'ITEM' in o['graphics_id']:
            for dy in (-1, 0, 1):
                for dx in (-1, 0, 1):
                    blocked.add((ox + dx, oy + dy))
    for w in map_json['warp_events']:
        for dy in range(-2, 3):
            for dx in range(-2, 3):
                blocked.add((w['x'] + dx, w['y'] + dy))
    for b in map_json['bg_events']:
        blocked.add((b['x'], b['y']))
    for y in range(33, 43):
        for x in range(52, 63):
            blocked.add((x, y))

    boxes = [
        (28, 51, 36, 56), (35, 58, 43, 63), (46, 51, 52, 55), (53, 55, 58, 57),
        (7, 23, 13, 27), (7, 30, 12, 35), (14, 28, 20, 36), (18, 21, 25, 25), (6, 44, 12, 49),
        (28, 30, 34, 34), (34, 39, 46, 45), (33, 49, 41, 52), (36, 20, 44, 24), (27, 15, 34, 19), (17, 15, 23, 18),
        (6, 5, 13, 8), (16, 7, 24, 10), (27, 6, 35, 10), (36, 4, 44, 6),
        (45, 13, 55, 18), (48, 22, 56, 26), (46, 34, 51, 40), (62, 10, 67, 15),
    ]

    eligible = {652, 648, 651, 758, 752, 753}
    grass = {}
    for x0, y0, x1, y1 in boxes:
        for y in range(y0, y1 + 1):
            for x in range(x0, x1 + 1):
                if (x, y) in blocked:
                    continue
                v = struct.unpack_from('<H', raw_map, (y * W + x) * 2)[0]
                mt = v & 0x3FF
                if mt in eligible and ((v >> 10) & 3) == 0:
                    grass[(x, y)] = mt if mt in (648, 651, 758) else 652

    changed = True
    while changed:
        changed = False
        for (x, y) in list(grass.keys()):
            horiz = ((x - 1, y) in grass) or ((x + 1, y) in grass)
            vert = ((x, y - 1) in grass) or ((x, y + 1) in grass)
            if not (horiz and vert):
                del grass[(x, y)]
                changed = True

    for (x, y), orig_mt in sorted(grass.items()):
        nesw = (
            1 if (x, y - 1) in grass else 0,
            1 if (x + 1, y) in grass else 0,
            1 if (x, y + 1) in grass else 0,
            1 if (x - 1, y) in grass else 0,
        )
        mt = stamp_slots[nesw] if orig_mt == 652 else ovh_grass_lookup[(orig_mt, nesw)]
        idx = (y * W + x) * 2
        old_v = struct.unpack_from('<H', raw_map, idx)[0]
        struct.pack_into('<H', raw_map, idx, (old_v & 0xFC00) | mt)
    print(f"Placed {len(grass)} Grim Woods tall grass tiles across Peaceful Woods!")


if __name__ == '__main__':
    main()
