import struct
from pathlib import Path

rom_path = Path('/mnt/c/Users/raulr/OneDrive/Desktop/zEmerald/eclat/Eclat Pourpre ENG (Raizen).gba')
if not rom_path.exists():
    rom_path = Path('C:/Users/raulr/OneDrive/Desktop/zEmerald/eclat/Eclat Pourpre ENG (Raizen).gba')

rom = rom_path.read_bytes()

def u32(offset):
    return struct.unpack('<I', rom[offset:offset+4])[0]

def ptr(offset):
    val = u32(offset)
    if 0x08000000 <= val < 0x0a000000:
        return val - 0x08000000
    return None

charmap = {}
for i in range(26):
    charmap[0xBB + i] = chr(ord('A') + i)
    charmap[0xD5 + i] = chr(ord('a') + i)
for i in range(10):
    charmap[0xA1 + i] = chr(ord('0') + i)
charmap[0x00] = ' '
charmap[0xAB] = '!'
charmap[0xAC] = '?'
charmap[0xAD] = '.'
charmap[0xAE] = '-'
charmap[0xBA] = '/'
charmap[0xB8] = ','
charmap[0xB1] = '"'
charmap[0xB2] = '"'
charmap[0xB3] = "'"
charmap[0xB4] = "'"
charmap[0x1B] = 'e'  # é in frlg
charmap[0xF0] = ':'
charmap[0xFA] = '\n'
charmap[0xFB] = '\n'
charmap[0xFE] = '\n'
charmap[0xFF] = ''

def decode_str(off, max_len=100):
    s = []
    curr = off
    while curr < len(rom) and len(s) < max_len:
        b = rom[curr]
        if b == 0xFF:
            break
        s.append(charmap.get(b, f'[{hex(b)}]'))
        curr += 1
    return ''.join(s)

# FRLG trainer table is at 0x23957c (struct Trainer is 40 bytes)
# struct Trainer:
# u8 partyFlags;
# u8 trainerClass;
# u8 encounterMusic_gender;
# u8 trainerPic;
# u8 trainerName[12];
# u16 items[4];
# u8 doubleBattle;
# u8 padding[3];
# u32 aiFlags;
# u8 partySize;
# u8 padding2[3];
# const union TrainerMonPtr party;
gTrainers = 0x23957c
trainer_ids = [74, 89, 106, 107, 116, 117]

# Pokemon names table in FRLG is at 0x245ee0 (11 bytes per species)
gSpeciesNames = 0x245ee0

def get_species_name(species_id):
    off = gSpeciesNames + species_id * 11
    return decode_str(off, 11)

# Item table in FRLG is at 0x3db028 (44 bytes per item)
gItems = 0x3db028
def get_item_name(item_id):
    off = gItems + item_id * 44
    return decode_str(off, 14)

print("--- ITEMS ---")
for it in [13, 14, 0x18d]:
    print(f"Item {it} (0x{it:x}): {get_item_name(it)}")

print("\n--- TRAINERS DETAILS ---")
for tid in trainer_ids:
    t_off = gTrainers + tid * 40
    party_flags = rom[t_off]
    tr_class = rom[t_off+1]
    tr_pic = rom[t_off+3]
    tr_name = decode_str(t_off+4, 12)
    party_size = rom[t_off+32]
    party_ptr = ptr(t_off+36)
    print(f"Trainer {tid} ({tr_name}): class={tr_class}, pic={tr_pic}, size={party_size}, flags={party_flags}")
    if party_ptr:
        for p in range(party_size):
            if party_flags == 0: # TrainerMonNoItemDefaultMoves
                iv, lvl, species = struct.unpack('<HHHxx', rom[party_ptr + p*8 : party_ptr + (p+1)*8])
                print(f"  Mon {p}: Lvl {lvl} {get_species_name(species)}")
            elif party_flags == 1: # TrainerMonItemDefaultMoves
                iv, lvl, species, item = struct.unpack('<HHHH', rom[party_ptr + p*8 : party_ptr + (p+1)*8])
                print(f"  Mon {p}: Lvl {lvl} {get_species_name(species)} (held: {get_item_name(item)})")
            elif party_flags == 2: # TrainerMonNoItemCustomMoves
                iv, lvl, species = struct.unpack('<HHH', rom[party_ptr + p*16 : party_ptr + p*16 + 6])
                print(f"  Mon {p}: Lvl {lvl} {get_species_name(species)} (custom moves)")
            else: # TrainerMonItemCustomMoves
                iv, lvl, species, item = struct.unpack('<HHHH', rom[party_ptr + p*16 : party_ptr + p*16 + 8])
                print(f"  Mon {p}: Lvl {lvl} {get_species_name(species)} (held: {get_item_name(item)}, custom moves)")

# Wild encounters
# In FRLG, wild mon headers are at 0x3c9cb8
# struct WildPokemonHeader { u8 mapGroup; u8 mapNum; u16 pad; u32 landMons; u32 waterMons; u32 rockSmashMons; u32 fishingMons; };
gWildMonHeaders = 0x3c9cb8
print("\n--- WILD ENCOUNTERS FOR ROUTE 3 (Group 3, Map 2 or label 103) ---")
curr = gWildMonHeaders
while True:
    group = rom[curr]
    map_num = rom[curr+1]
    if group == 0xFF and map_num == 0xFF:
        break
    land_ptr = ptr(curr+4)
    water_ptr = ptr(curr+8)
    rock_ptr = ptr(curr+12)
    fish_ptr = ptr(curr+16)

    # Check if this is Route 3 (in FRLG group 3 map 2 is Route 3)
    if group == 3 and map_num == 2:
        print(f"Found Wild Header for Group {group}, Map {map_num} at {hex(curr)}:")
        if land_ptr:
            rate = u32(land_ptr)
            mons_ptr = ptr(land_ptr+4)
            print(f"  Land (rate {rate}):")
            for m in range(12):
                min_lvl = rom[mons_ptr + m*4]
                max_lvl = rom[mons_ptr + m*4 + 1]
                species = struct.unpack('<H', rom[mons_ptr + m*4 + 2 : mons_ptr + m*4 + 4])[0]
                print(f"    Slot {m}: Lvl {min_lvl}-{max_lvl} {get_species_name(species)}")
        if water_ptr:
            rate = u32(water_ptr)
            mons_ptr = ptr(water_ptr+4)
            print(f"  Water (rate {rate}):")
            for m in range(5):
                min_lvl = rom[mons_ptr + m*4]
                max_lvl = rom[mons_ptr + m*4 + 1]
                species = struct.unpack('<H', rom[mons_ptr + m*4 + 2 : mons_ptr + m*4 + 4])[0]
                print(f"    Slot {m}: Lvl {min_lvl}-{max_lvl} {get_species_name(species)}")
        if fish_ptr:
            rate = u32(fish_ptr)
            mons_ptr = ptr(fish_ptr+4)
            print(f"  Fish (rate {rate}):")
            for m in range(10):
                min_lvl = rom[mons_ptr + m*4]
                max_lvl = rom[mons_ptr + m*4 + 1]
                species = struct.unpack('<H', rom[mons_ptr + m*4 + 2 : mons_ptr + m*4 + 4])[0]
                print(f"    Slot {m}: Lvl {min_lvl}-{max_lvl} {get_species_name(species)}")
    curr += 20
