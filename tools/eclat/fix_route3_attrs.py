import struct
from pathlib import Path

rom = Path('/mnt/c/Users/raulr/OneDrive/Desktop/zEmerald/eclat/Eclat Pourpre ENG (Raizen).gba').read_bytes()

# Route 3 Secondary at 0x2cef2c:
# attrs is at 0x8894d8! 384 * 4 = 1536 bytes
attrs_off = 0x8894d8
attrs_bytes = rom[attrs_off : attrs_off + 384 * 4]
assert len(attrs_bytes) == 1536

target_bin = Path('data/tilesets/secondary/eclat_route3/metatile_attributes.bin')
target_bin.write_bytes(attrs_bytes)
print(f"Written {len(attrs_bytes)} bytes from {hex(attrs_off)} to {target_bin}")

# Verify 774 and 782
val774 = struct.unpack('<I', attrs_bytes[(774-640)*4 : (774-640)*4 + 4])[0]
val782 = struct.unpack('<I', attrs_bytes[(782-640)*4 : (782-640)*4 + 4])[0]
print(f"Metatile 774 (behavior): {hex(val774)}")
print(f"Metatile 782 (behavior): {hex(val782)}")
