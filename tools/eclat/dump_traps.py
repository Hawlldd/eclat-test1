import struct
from pathlib import Path

rom = Path('/mnt/c/Users/raulr/OneDrive/Desktop/zEmerald/eclat/Eclat Pourpre ENG (Raizen).gba').read_bytes()

def u32(off):
    return struct.unpack('<I', rom[off:off+4])[0]

def ptr(off):
    v = u32(off)
    return v - 0x08000000 if 0x08000000 <= v < 0x0a000000 else None

header = 0x34acb4
events_ptr = ptr(header + 4)
num_objs, num_warps, num_traps, num_bgs = rom[events_ptr:events_ptr+4]
traps_ptr = ptr(events_ptr + 12)

print(f"Num coord/traps: {num_traps} at {hex(traps_ptr)}")
# In FRLG, struct CoordEvent: { u16 x, u16 y, u8 elev, u8 pad, u16 trigger, u16 index, u16 pad2, u32 script }
for t in range(num_traps):
    t_off = traps_ptr + t * 16
    x, y, elev = struct.unpack('<hhB', rom[t_off:t_off+5])
    var = struct.unpack('<H', rom[t_off+6:t_off+8])[0]
    val = struct.unpack('<H', rom[t_off+8:t_off+10])[0]
    script = u32(t_off+12)
    print(f"Trap {t}: pos=({x},{y}), elev={elev}, var={hex(var)}, val={val}, script={hex(script)}")
