"""Generate Porymap's Emerald-sized view of Ivière and Route 1 tilesets.

The ROM continues to build from the original FRLG-format *.bin assets.
"""
from pathlib import Path
import struct
import zlib

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


sources = {
    role: ROOT / 'data' / 'tilesets' / role / 'eclat_iviere'
    for role in ('primary', 'secondary')
}
tiles = {role: (source / 'tiles.bin').read_bytes() for role, source in sources.items()}
metatiles = {role: (source / 'metatiles.bin').read_bytes() for role, source in sources.items()}
attributes = {role: (source / 'metatile_attributes.bin').read_bytes() for role, source in sources.items()}
palettes = {role: (source / 'palettes.bin').read_bytes() for role, source in sources.items()}

assert len(tiles['primary']) == 640 * 32 and len(tiles['secondary']) == 384 * 32
assert len(metatiles['primary']) == 640 * 16 and len(metatiles['secondary']) == 384 * 16

# The map's global tile/metatile IDs stay 0..1023. Only the boundary between
# primary and secondary source files changes from 640 to 512 for Porymap.
editor_tiles = {
    'primary': tiles['primary'][:512 * 32],
    'secondary': tiles['primary'][512 * 32:] + tiles['secondary'],
}
editor_metatiles = {
    'primary': metatiles['primary'][:512 * 16],
    'secondary': metatiles['primary'][512 * 16:] + metatiles['secondary'],
}

def convert_attributes(raw):
    output = bytearray()
    for value, in struct.iter_unpack('<I', raw):
        editor_value = (value & 0xff) | (((value >> 29) & 3) << 12)
        output += struct.pack('<H', editor_value)
    return bytes(output)

all_attributes = convert_attributes(attributes['primary']) + convert_attributes(attributes['secondary'])
editor_attributes = {
    'primary': all_attributes[:512 * 2],
    'secondary': all_attributes[512 * 2:],
}

for role in ('primary', 'secondary'):
    target = ROOT / 'data' / 'tilesets' / role / f'eclat_iviere_{role}'
    target.mkdir(parents=True, exist_ok=True)
    (target / 'tiles.png').write_bytes(png_from_4bpp(editor_tiles[role], palettes[role]))
    palette_dir = target / 'palettes'
    palette_dir.mkdir(exist_ok=True)
    for index in range(13):
        palette_source = palettes['primary'] if role == 'secondary' and index == 6 else palettes[role]
        colors = []
        for value, in struct.iter_unpack('<H', palette_source[index * 32:(index + 1) * 32]):
            colors.append(f'{(value & 31) * 255 // 31} {((value >> 5) & 31) * 255 // 31} {((value >> 10) & 31) * 255 // 31}')
        (palette_dir / f'{index:02}.pal').write_text('JASC-PAL\n0100\n16\n' + '\n'.join(colors) + '\n', encoding='ascii')
    (target / 'metatiles.bin').write_bytes(editor_metatiles[role])
    (target / 'metatile_attributes.bin').write_bytes(editor_attributes[role])
    print(role, len(editor_tiles[role]) // 32, 'tiles', len(editor_metatiles[role]) // 16, 'metatiles')


# Route 1 shares Ivière's primary tileset but has its own FRLG secondary.
# Porymap's 512/512 split needs the last 128 primary entries prepended to it.
route1_source = ROOT / 'data' / 'tilesets' / 'secondary' / 'eclat_route1'
route1_tiles = (route1_source / 'tiles.bin').read_bytes()
route1_metatiles = (route1_source / 'metatiles.bin').read_bytes()
route1_attributes = (route1_source / 'metatile_attributes.bin').read_bytes()
route1_palettes = (route1_source / 'palettes.bin').read_bytes()
assert len(route1_tiles) == 384 * 32
assert len(route1_metatiles) == 384 * 16
assert len(route1_attributes) == 384 * 4
assert len(route1_palettes) >= 13 * 32

route1_target = ROOT / 'data' / 'tilesets' / 'secondary' / 'eclat_route_1secondary'
route1_target.mkdir(parents=True, exist_ok=True)
route1_editor_tiles = tiles['primary'][512 * 32:] + route1_tiles
route1_editor_metatiles = metatiles['primary'][512 * 16:] + route1_metatiles
route1_editor_attributes = convert_attributes(attributes['primary'][512 * 4:] + route1_attributes)
assert len(route1_editor_tiles) == 512 * 32
assert len(route1_editor_metatiles) == 512 * 16
assert len(route1_editor_attributes) == 512 * 2
(route1_target / 'tiles.png').write_bytes(png_from_4bpp(route1_editor_tiles, route1_palettes))
(route1_target / 'metatiles.bin').write_bytes(route1_editor_metatiles)
(route1_target / 'metatile_attributes.bin').write_bytes(route1_editor_attributes)
route1_palette_dir = route1_target / 'palettes'
route1_palette_dir.mkdir(exist_ok=True)
for index in range(13):
    palette_source = palettes['primary'] if index == 6 else route1_palettes
    colors = []
    for value, in struct.iter_unpack('<H', palette_source[index * 32:(index + 1) * 32]):
        colors.append(f'{(value & 31) * 255 // 31} {((value >> 5) & 31) * 255 // 31} {((value >> 10) & 31) * 255 // 31}')
    (route1_palette_dir / f'{index:02}.pal').write_text(
        'JASC-PAL\n0100\n16\n' + '\n'.join(colors) + '\n', encoding='ascii')
print('route1 secondary', len(route1_editor_tiles) // 32, 'tiles',
      len(route1_editor_metatiles) // 16, 'metatiles')

# Petipres secondary (used by Petipres and Route 2)
petipres_source = ROOT / 'data' / 'tilesets' / 'secondary' / 'petipres'
petipres_tiles = (petipres_source / 'tiles.bin').read_bytes()
petipres_metatiles = (petipres_source / 'metatiles.bin').read_bytes()
petipres_attributes = (petipres_source / 'metatile_attributes.bin').read_bytes()
petipres_palettes = (petipres_source / 'palettes.bin').read_bytes()
assert len(petipres_tiles) == 384 * 32
assert len(petipres_metatiles) == 384 * 16
assert len(petipres_attributes) == 384 * 4
assert len(petipres_palettes) >= 13 * 32

petipres_target = ROOT / 'data' / 'tilesets' / 'secondary' / 'petipres_secondary'
petipres_target.mkdir(parents=True, exist_ok=True)
petipres_editor_tiles = tiles['primary'][512 * 32:] + petipres_tiles
petipres_editor_metatiles = metatiles['primary'][512 * 16:] + petipres_metatiles
petipres_editor_attributes = convert_attributes(attributes['primary'][512 * 4:] + petipres_attributes)
assert len(petipres_editor_tiles) == 512 * 32
assert len(petipres_editor_metatiles) == 512 * 16
assert len(petipres_editor_attributes) == 512 * 2
(petipres_target / 'tiles.png').write_bytes(png_from_4bpp(petipres_editor_tiles, petipres_palettes))
(petipres_target / 'metatiles.bin').write_bytes(petipres_editor_metatiles)
(petipres_target / 'metatile_attributes.bin').write_bytes(petipres_editor_attributes)
petipres_palette_dir = petipres_target / 'palettes'
petipres_palette_dir.mkdir(exist_ok=True)
for index in range(13):
    palette_source = palettes['primary'] if index == 6 else petipres_palettes
    colors = []
    for value, in struct.iter_unpack('<H', palette_source[index * 32:(index + 1) * 32]):
        colors.append(f'{(value & 31) * 255 // 31} {((value >> 5) & 31) * 255 // 31} {((value >> 10) & 31) * 255 // 31}')
    (petipres_palette_dir / f'{index:02}.pal').write_text(
        'JASC-PAL\n0100\n16\n' + '\n'.join(colors) + '\n', encoding='ascii')
print('petipres secondary', len(petipres_editor_tiles) // 32, 'tiles',
      len(petipres_editor_metatiles) // 16, 'metatiles')

for name in ('peaceful_woods', 'eclat_rocky_path'):
    sec_source = ROOT / 'data' / 'tilesets' / 'secondary' / name
    sec_tiles = (sec_source / 'tiles.bin').read_bytes()
    sec_metatiles = (sec_source / 'metatiles.bin').read_bytes()
    sec_attributes = (sec_source / 'metatile_attributes.bin').read_bytes()
    sec_palettes = (sec_source / 'palettes.bin').read_bytes()
    assert len(sec_tiles) == 384 * 32
    assert len(sec_metatiles) == 384 * 16
    assert len(sec_attributes) == 384 * 4
    assert len(sec_palettes) >= 13 * 32

    sec_target = ROOT / 'data' / 'tilesets' / 'secondary' / f'{name}_secondary'
    sec_target.mkdir(parents=True, exist_ok=True)
    sec_editor_tiles = tiles['primary'][512 * 32:] + sec_tiles
    sec_editor_metatiles = metatiles['primary'][512 * 16:] + sec_metatiles
    sec_editor_attributes = convert_attributes(attributes['primary'][512 * 4:] + sec_attributes)
    assert len(sec_editor_tiles) == 512 * 32
    assert len(sec_editor_metatiles) == 512 * 16
    assert len(sec_editor_attributes) == 512 * 2
    (sec_target / 'tiles.png').write_bytes(png_from_4bpp(sec_editor_tiles, sec_palettes))
    (sec_target / 'metatiles.bin').write_bytes(sec_editor_metatiles)
    (sec_target / 'metatile_attributes.bin').write_bytes(sec_editor_attributes)
    sec_palette_dir = sec_target / 'palettes'
    sec_palette_dir.mkdir(exist_ok=True)
    for index in range(13):
        palette_source = palettes['primary'] if index == 6 else sec_palettes
        colors = []
        for value, in struct.iter_unpack('<H', palette_source[index * 32:(index + 1) * 32]):
            colors.append(f'{(value & 31) * 255 // 31} {((value >> 5) & 31) * 255 // 31} {((value >> 10) & 31) * 255 // 31}')
        (sec_palette_dir / f'{index:02}.pal').write_text(
            'JASC-PAL\n0100\n16\n' + '\n'.join(colors) + '\n', encoding='ascii')
    print(f'{name} secondary', len(sec_editor_tiles) // 32, 'tiles',
          len(sec_editor_metatiles) // 16, 'metatiles')


def sprite_png(raw, width, height, palette):
    frame_size = width * height // 2
    assert len(raw) == frame_size * 9
    pixels = bytearray(width * 9 * height)
    for frame in range(9):
        for tile_y in range(height // 8):
            for tile_x in range(width // 8):
                tile = (tile_y * (width // 8) + tile_x) * 32
                for y in range(8):
                    for x in range(8):
                        byte = raw[frame * frame_size + tile + y * 4 + x // 2]
                        index = (y + tile_y * 8) * (width * 9) + frame * width + tile_x * 8 + x
                        pixels[index] = (byte >> (4 * (x % 2))) & 15
    rgb = bytearray()
    for value, in struct.iter_unpack('<H', palette[:32]):
        rgb.extend(((value & 31) * 255 // 31, ((value >> 5) & 31) * 255 // 31, ((value >> 10) & 31) * 255 // 31))
    scanlines = b''.join(b'\0' + pixels[y * width * 9:(y + 1) * width * 9] for y in range(height))
    return (b'\x89PNG\r\n\x1a\n'
            + chunk(b'IHDR', struct.pack('>IIBBBBB', width * 9, height, 8, 3, 0, 0, 0))
            + chunk(b'PLTE', bytes(rgb))
            + chunk(b'tRNS', bytes([0] + [255] * 15))
            + chunk(b'IDAT', zlib.compress(scanlines, 9))
            + chunk(b'IEND', b''))


sprite_folder = ROOT / 'graphics' / 'eclat_intro'
for filename, width, height, palette_file in (
    ('npc_ivierenamerater', 16, 32, 'rival_overworld_palette.bin'),
    ('npc_iviererivernpc', 16, 32, 'rival_overworld_palette.bin'),
    ('town_mon', 16, 16, 'mother_palette.bin'),
):
    data = (sprite_folder / f'{filename}.bin').read_bytes()
    colors = (sprite_folder / palette_file).read_bytes()
    (sprite_folder / f'{filename}.png').write_bytes(sprite_png(data, width, height, colors))
    print(filename, 'preview', width * 9, 'x', height)
