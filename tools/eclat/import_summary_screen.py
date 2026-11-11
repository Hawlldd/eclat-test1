#!/usr/bin/env python3
"""
import_summary_screen.py
-------------------------
Automação para importação das telas de fundo do Pokémon Summary Screen.
Lê as imagens PNG das abas em 'graphics/summary_screen/sources/' (240x160 pixels)
e gera automaticamente:
  - graphics/summary_screen/tiles.png (Tileset 4bpp indexado)
  - graphics/summary_screen/page_info.bin (Tilemap da Aba 1)
  - graphics/summary_screen/page_info_egg.bin (Tilemap do Ovo)
  - graphics/summary_screen/page_skills.bin (Tilemap da Aba 2 - Skills)
  - graphics/summary_screen/page_battle_moves.bin (Tilemap da Aba 3 - Golpes)

Uso:
  python3 tools/eclat/import_summary_screen.py
"""

import os
import sys
import struct
from pathlib import Path
from PIL import Image

REPO_ROOT = Path(__file__).resolve().parent.parent.parent
GFX_DIR = REPO_ROOT / "graphics" / "summary_screen"
SOURCES_DIR = GFX_DIR / "sources"

# Paleta oficial de 16 cores do Summary Screen estilo Saffron (Gen 4 DP/Pt)
OFFICIAL_PALETTE = [
    (0, 0, 0),         # 0: Transparente
    (255, 255, 255),   # 1: Branco
    (214, 215, 214),   # 2: Cinza claro (scanlines e pílulas)
    (82, 142, 214),    # 3: Azul aço principal
    (74, 121, 165),    # 4: Azul aço cabeçalho
    (156, 158, 156),   # 5: Borda cinza média
    (173, 174, 173),   # 6: Borda cinza suave
    (99, 97, 99),      # 7: Cinza escuro preenchimento de pílulas
    (58, 57, 58),      # 8: Sombra de texto escura
    (66, 65, 66),      # 9: Fonte escura
    (74, 81, 82),      # 10: Borda escura
    (189, 190, 189),   # 11: Anti-alias cinza
    (214, 222, 189),   # 12: Destaque
    (180, 164, 57),    # 13: Dourado
    (213, 32, 32),     # 14: Vermelho
    (172, 0, 0),       # 15: Vermelho escuro
]

# Paletas completas para o Summary Screen (8 paletas = 128 cores para tiles.png)
# Paleta 0: Fundo oficial
PAL_0 = OFFICIAL_PALETTE

# Paletas 1 a 5: Paletas padrão do pokeemerald
PAL_1 = [
    (180, 180, 115), (74, 65, 57), (255, 255, 255), (90, 106, 0), (123, 139, 32), (255, 246, 205),
    (115, 106, 98), (90, 222, 123), (90, 205, 98), (90, 189, 74), (98, 172, 49), (98, 156, 24),
    (238, 164, 255), (189, 156, 164), (148, 148, 82), (106, 148, 0)
]
PAL_2 = [
    (180, 180, 115), (74, 65, 57), (255, 255, 255), (172, 180, 172), (205, 205, 197), (246, 255, 238),
    (115, 106, 98), (238, 222, 24), (213, 189, 57), (189, 164, 98), (164, 139, 131), (139, 115, 172),
    (205, 222, 123), (172, 172, 148), (139, 131, 180), (115, 90, 213)
]
PAL_3 = [
    (180, 180, 115), (74, 65, 57), (255, 255, 255), (0, 0, 0), (0, 0, 0), (246, 255, 246),
    (115, 106, 98), (222, 24, 238), (189, 16, 238), (164, 8, 238), (131, 8, 246), (106, 0, 246),
    (172, 222, 222), (139, 148, 230), (106, 74, 238), (82, 0, 255)
]
PAL_4 = [
    (180, 180, 115), (74, 65, 57), (148, 32, 172), (189, 106, 213), (230, 189, 255), (255, 255, 255),
    (213, 148, 246), (189, 139, 205), (255, 230, 255), (230, 180, 213), (0, 0, 0), (0, 0, 0),
    (0, 0, 0), (0, 0, 0), (0, 0, 0), (115, 98, 90)
]
PAL_5 = [
    (180, 180, 115), (74, 65, 57), (255, 255, 255), (222, 230, 246), (246, 246, 222), (246, 255, 255),
    (115, 106, 98), (255, 205, 74), (230, 164, 98), (205, 123, 123), (180, 82, 148), (156, 41, 172),
    (230, 156, 255), (197, 98, 238), (164, 49, 222), (131, 0, 205)
]

# Paleta 6: Paleta das janelas de texto (Text Window Palette)
# Crucial para o texto ficar fino e nitido (sem sombra preta borrando o texto)
PAL_6 = [
    (98, 180, 106),    # 0: Transparente
    (66, 65, 66),      # 1: Texto escuro padrao (Saffron)
    (213, 213, 205),   # 2: Cinza claro (sombra do texto)
    (255, 255, 255),   # 3: Branco (fonte do topo/cabecalho)
    (98, 98, 98),      # 4: Cinza escuro (sombra do cabecalho)
    (239, 49, 0),      # 5: Vermelho (Grade E / Nature Up)
    (247, 134, 49),    # 6: Laranja (Grade D)
    (222, 231, 49),    # 7: Amarelo (Grade C / C+)
    (107, 150, 247),   # 8: Azul (Grade B / B+ / Nature Down)
    (123, 206, 82),    # 9: Verde (Grade A)
    (165, 65, 165),    # 10: Roxo (Grade S)
    (139, 246, 255),   # 11: Ciano (nome do OT masculino)
    (24, 164, 213),    # 12: Sombra OT masculino
    (255, 189, 180),   # 13: Rosa (nome da OT feminina)
    (213, 115, 106),   # 14: Sombra OT feminina
    (0, 0, 0)          # 15
]

# Paleta 7: Paleta de prompts e utilidades
PAL_7 = [
    (98, 180, 106),    # 0: Transparente
    (0, 0, 0),         # 1: Preto
    (213, 213, 205),   # 2: Cinza claro
    (255, 255, 255),   # 3: Branco
    (98, 98, 98),      # 4: Cinza escuro
    (230, 8, 8),       # 5: Vermelho
    (255, 189, 115),   # 6: Laranja
    (238, 238, 172),   # 7: Amarelo
    (164, 164, 98),    # 8: Sombra amarela
    (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 0),
    (255, 0, 0), (164, 0, 0), (255, 255, 255)
]

FULL_8_PALETTES = [PAL_0, PAL_1, PAL_2, PAL_3, PAL_4, PAL_5, PAL_6, PAL_7]


COLOR_TO_IDX = {color: idx for idx, color in enumerate(OFFICIAL_PALETTE)}

def snap_to_palette(rgb_tuple):
    if rgb_tuple in COLOR_TO_IDX:
        return COLOR_TO_IDX[rgb_tuple]
    # Encontra a cor mais próxima por distância euclidiana
    best_dist = float('inf')
    best_idx = 0
    for idx, pal_col in enumerate(OFFICIAL_PALETTE):
        dist = sum((a - b) ** 2 for a, b in zip(pal_col, rgb_tuple))
        if dist < best_dist:
            best_dist = dist
            best_idx = idx
    return best_idx

def get_indexed_pixels(im):
    w, h = im.size
    pixels = []
    for y in range(h):
        for x in range(w):
            c = im.getpixel((x, y))[:3]
            pixels.append(snap_to_palette(c))
    return pixels

def flip_h(t):
    return tuple(t[y * 8 + (7 - x)] for y in range(8) for x in range(8))

def flip_v(t):
    return tuple(t[(7 - y) * 8 + x] for y in range(8) for x in range(8))

def flip_hv(t):
    return tuple(t[(7 - y) * 8 + (7 - x)] for y in range(8) for x in range(8))

def build_summary_screen():
    print("=== Importando Pokémon Summary Screen ===")
    SOURCES_DIR.mkdir(parents=True, exist_ok=True)

    info_path = SOURCES_DIR / "page_info.png"
    skills_path = SOURCES_DIR / "page_skills.png"
    moves_path = SOURCES_DIR / "page_battle_moves.png"
    details_path = SOURCES_DIR / "page_move_details.png"

    for p in [info_path, skills_path, moves_path, details_path]:
        if not p.exists():
            print(f"Erro: Arquivo fonte não encontrado: {p}")
            sys.exit(1)

    im_info = Image.open(info_path).convert("RGB")
    im_skills = Image.open(skills_path).convert("RGB")
    im_moves = Image.open(moves_path).convert("RGB")
    im_details = Image.open(details_path).convert("RGB")

    master_tiles = []
    # Tile 0 vazio
    blank_tile = tuple([0] * 64)
    master_tiles.append(blank_tile)

    def process_image(im):
        pix = get_indexed_pixels(im)
        tilemap_entries = []
        for ty in range(32):
            for tx in range(32):
                if ty < 20 and tx < 30:
                    t = tuple(pix[(ty * 8 + y) * 240 + (tx * 8 + x)] for y in range(8) for x in range(8))
                    if t in master_tiles:
                        entry = master_tiles.index(t)
                    elif flip_h(t) in master_tiles:
                        entry = master_tiles.index(flip_h(t)) | (1 << 10)
                    elif flip_v(t) in master_tiles:
                        entry = master_tiles.index(flip_v(t)) | (1 << 11)
                    elif flip_hv(t) in master_tiles:
                        entry = master_tiles.index(flip_hv(t)) | (1 << 10) | (1 << 11)
                    else:
                        entry = len(master_tiles)
                        master_tiles.append(t)
                else:
                    entry = 0
                tilemap_entries.append(entry)
        return tilemap_entries

    tm_info = process_image(im_info)
    tm_skills = process_image(im_skills)
    tm_moves = process_image(im_moves)
    tm_details = process_image(im_details)

    print(f"Total de tiles únicos combinados: {len(master_tiles)} (limite VRAM: 576)")
    if len(master_tiles) > 576:
        print("AVISO: O número de tiles ultrapassa o espaço padrão do CharBlock (576 tiles)!")

    # Criação do tiles.png (16 tiles por linha = 128px de largura)
    rows = (len(master_tiles) + 15) // 16
    tiles_img = Image.new("P", (128, rows * 8), 0)

    # Cria paleta completa de 8 paletas (128 cores = 384 bytes)
    full_pal = []
    for pal in FULL_8_PALETTES:
        for r, g, b in pal:
            full_pal.extend([r, g, b])
    tiles_img.putpalette(full_pal)

    for idx, t in enumerate(master_tiles):
        tx = (idx % 16) * 8
        ty = (idx // 16) * 8
        for y in range(8):
            for x in range(8):
                tiles_img.putpixel((tx + x, ty + y), t[y * 8 + x])

    tiles_out = GFX_DIR / "tiles.png"
    tiles_img.save(tiles_out)
    print(f"Salvo: {tiles_out} ({tiles_img.size[0]}x{tiles_img.size[1]} px)")

    # Salva os tilemaps .bin
    def save_tilemap(entries, filename):
        data = struct.pack(f"<{len(entries)}H", *entries)
        out_file = GFX_DIR / filename
        out_file.write_bytes(data)
        print(f"Salvo: {out_file} ({len(data)} bytes)")

    save_tilemap(tm_info, "page_info.bin")
    save_tilemap(tm_info, "page_info_egg.bin")
    save_tilemap(tm_skills, "page_skills.bin")
    save_tilemap(tm_moves, "page_battle_moves.bin")
    save_tilemap(tm_details, "page_move_details.bin")
    save_tilemap(tm_details, "page_contest_moves.bin")
    print("=== Concluído com sucesso! ===")

if __name__ == "__main__":
    build_summary_screen()
