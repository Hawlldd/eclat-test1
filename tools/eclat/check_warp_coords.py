import struct
from pathlib import Path

rom = Path('/mnt/c/Users/raulr/OneDrive/Desktop/zEmerald/eclat/Eclat Pourpre ENG (Raizen).gba').read_bytes()

# Route 3 secondary tileset in ROM:
# Header was 0x2cef2c
# Let's inspect struct Tileset at 0x2cef2c
# struct Tileset { isCompressed, isSecondary, pad, tiles, palettes, metatiles, anim, attributes }
tileset_off = 0x2cef2c
tiles_p = struct.unpack('<I', rom[tileset_off+4:tileset_off+8])[0] & 0x1ffffff
pals_p = struct.unpack('<I', rom[tileset_off+8:tileset_off+12])[0] & 0x1ffffff
metatiles_p = struct.unpack('<I', rom[tileset_off+12:tileset_off+16])[0] & 0x1ffffff
attrs_p = struct.unpack('<I', rom[tileset_off+20:tileset_off+24])[0] & 0x1ffffff

print(f"Route 3 Secondary in ROM: tiles={hex(tiles_p)}, pals={hex(pals_p)}, metatiles={hex(metatiles_p)}, attrs={hex(attrs_p)}")

# In FRLG, metatile attributes is an array of u16 (or u32 in CFRU)!
# Let's check format of attrs at attrs_p:
for i in [134, 142]: # 774-640=134, 782-640=142
    val16 = struct.unpack('<H', rom[attrs_p + i*2 : attrs_p + (i+1)*2])[0]
    val32 = struct.unpack('<I', rom[attrs_p + i*4 : attrs_p + (i+1)*4])[0]
    print(f"Index {i} (metatile {640+i}): as_u16={hex(val16)}, as_u32={hex(val32)}")
