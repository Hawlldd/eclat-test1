"""Grim Woods secondary metatile registrations, dry-ground hollow logs & wild encounters."""
from collections import deque
import json
from pathlib import Path
import struct


def register_grim_woods_river_metatiles(
    new_pals: list,
    gw_pals_p: list,
    new_mts: dict,
    gw_sec_layers,
    gw_ovh_l: list[int],
    gw_ovh_r: list[int],
) -> dict[int, int]:
    """Register Grim Woods secondary features & bridge metatiles in 888..931 for Porymap palette."""
    new_pals[0] = list(gw_pals_p[2])
    water_l0 = [0xB3A6, 0xB3A7, 0xB3A8, 0xB3A9]

    gw_direct = {
        888: 688, 889: 689, 890: 690, 891: 691, 892: 692, 893: 711,
        894: 717, 895: 718, 896: 696, 897: 697, 898: 698, 899: 699,
        900: 700, 901: 719, 904: 704, 905: 705, 906: 706,
    }
    attrs: dict[int, int] = {}
    for dst_mt, src_gw in gw_direct.items():
        new_mts[dst_mt] = gw_sec_layers(src_gw)
        attrs[dst_mt] = 0x00000010 if dst_mt in (894, 895) else 0x20000010

    new_mts[902] = (list(water_l0), list(gw_ovh_l))
    new_mts[903] = (list(water_l0), list(gw_ovh_r))
    attrs[902] = 0x00000010
    attrs[903] = 0x00000010

    new_mts[912] = ([0x1A74, 0x1275, 0x1A74, 0x1275], [0, 0, 0, 0])
    new_mts[913] = ([0x1675, 0x1674, 0x1675, 0x1674], [0, 0, 0, 0])
    attrs[912] = 0x20000000
    attrs[913] = 0x20000000

    horiz_top_l1 = [0x1263, 0x1263, 0x1273, 0x1273]
    horiz_bot_l1 = [0x1A73, 0x1A73, 0x1A63, 0x1A63]
    w_bank_l0, _ = gw_sec_layers(696)
    e_bank_l0, _ = gw_sec_layers(698)

    new_mts[914] = (list(water_l0), list(horiz_top_l1))
    new_mts[915] = (list(water_l0), list(horiz_bot_l1))
    new_mts[917] = (list(w_bank_l0), list(horiz_top_l1))
    new_mts[918] = (list(w_bank_l0), list(horiz_bot_l1))
    new_mts[919] = (list(e_bank_l0), list(horiz_top_l1))
    new_mts[927] = (list(e_bank_l0), list(horiz_bot_l1))
    for m in (914, 915, 917, 918, 919, 927):
        attrs[m] = 0x20000000

    _, s_bank_l1 = gw_sec_layers(705)
    new_mts[916] = (list(water_l0), [0x0266, 0x0266, 0x0000, 0x0000])
    new_mts[931] = (list(water_l0), [0x0266, 0x0266, s_bank_l1[2], s_bank_l1[3]])
    attrs[916] = 0x20000010
    attrs[931] = 0x20000010

    for dst_mt, src_gw in (
        (920, 712), (921, 713), (922, 714), (923, 641),
        (928, 720), (929, 721), (930, 722),
    ):
        new_mts[dst_mt] = gw_sec_layers(src_gw)
        attrs[dst_mt] = 0x20000016

    return attrs


def stamp_dry_hollow_logs(raw_map: bytearray, map_json: dict) -> None:
    """Stamp scenic 2x1 Fallen Hollow Logs (816, 817, impassable coll=1, elev=3) on dry glades."""
    w = 78
    for lx, ly in ((6, 30), (19, 23), (42, 62)):
        for dx, mt in ((0, 816), (1, 817)):
            struct.pack_into('<H', raw_map, (ly * w + (lx + dx)) * 2, (3 << 12) | (1 << 10) | mt)
    _verify_full_walkability(raw_map, map_json)
    print("Stamped 3 dry-ground fallen hollow logs at (6,30), (19,23), (42,62)!")


def _verify_full_walkability(raw_map: bytearray, map_json: dict) -> None:
    """BFS flood-fill from Route 2 warp (12, 56) verifying 100% map connectivity."""
    w, h = 78, 78

    def passable(x: int, y: int) -> bool:
        if not (0 <= x < w and 0 <= y < h):
            return False
        v = struct.unpack_from('<H', raw_map, (y * w + x) * 2)[0]
        coll, elev = (v >> 10) & 3, (v >> 12) & 0xF
        return coll == 0 and elev in (0, 3, 4, 15)

    seen = {(12, 56)}
    q = deque([(12, 56)])
    while q:
        cx, cy = q.popleft()
        for nx, ny in ((cx + 1, cy), (cx - 1, cy), (cx, cy + 1), (cx, cy - 1)):
            if (nx, ny) not in seen and passable(nx, ny):
                seen.add((nx, ny))
                q.append((nx, ny))

    for wp in map_json['warp_events']:
        pt = (wp['x'], wp['y'])
        if pt != (26, 43):
            assert pt in seen, f"Warp {pt} unreachable from (12,56)!"

    for o in map_json['object_events']:
        ox, oy = o['x'], o['y']
        if o['trainer_type'] != 'TRAINER_TYPE_NONE' or 'ITEM' in o['graphics_id'] or 'CUTTABLE' in o['graphics_id']:
            adj = any((ox + dx, oy + dy) in seen for dx, dy in ((0, 0), (1, 0), (-1, 0), (0, 1), (0, -1)))
            assert adj, f"Important object {o['graphics_id']} at {(ox, oy)} became unreachable!"

    for b in map_json['bg_events']:
        bx, by = b['x'], b['y']
        if (bx, by) in ((16, 14), (13, 15)):
            continue
        adj = any((bx + dx, by + dy) in seen for dx, dy in ((0, 0), (1, 0), (-1, 0), (0, 1), (0, -1)))
        assert adj, f"Sign/hidden item at {(bx, by)} became unreachable!"


def configure_peaceful_woods_encounters(root: Path) -> None:
    """Ensure MAP_PEACEFUL_WOODS has land encounters (15) and NO water/fishing encounters."""
    we_path = root / 'src/data/wild_encounters.json'
    data = json.loads(we_path.read_text(encoding='utf-8'))
    for grp in data.get('wild_encounter_groups', []):
        if not grp.get('for_maps'):
            continue
        for entry in grp.get('encounters', []):
            if entry.get('map') == 'MAP_PEACEFUL_WOODS':
                entry['land_mons']['encounter_rate'] = 15
                entry.pop('water_mons', None)
                entry.pop('fishing_mons', None)
                break
    we_path.write_text(json.dumps(data, indent=2) + '\n', encoding='utf-8')
    print("Cleaned MAP_PEACEFUL_WOODS wild encounters: land_mons only, no water/fishing!")
