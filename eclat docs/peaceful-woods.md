# Altar de Celebi e Ajustes em Peaceful Woods (Bois Paisibles)

## 1. Visão Geral

Na região de *Pokémon Éclat Pourpre*, a floresta de **Peaceful Woods** (*Bois Paisibles*, equivalente à Viridian Forest) abriga uma clareira sagrada contendo o **Altar de Celebi** (*Autel des Bois Paisibles*) em `(57, 38)`.

No post-game do jogo original, após obter a **GS Ball** com o Prof. Flora e apresentá-la a Fargas em Johto, o jogador deposita a GS Ball neste altar para invocar e batalhar contra o Pokémon Mítico **Celebi**.

---

## 2. Diagnóstico dos Problemas Encontrados

Durante o port automatizado dos mapas e dados da ROM FireRed para o Emerald, a área do santuário em `(57, 36..40)` acumulou graves inconsistências visuais e lógicas:

| Elemento Visual | Causa Técnica no Port | Problema Observado |
|---|---|---|
| **Rayquaza no Telhado** | No FRLG, o objeto possuía `gfx: 184` e `flag: 64004` (invisível). No Emerald, o ID 184 coincidiu com `OBJ_EVENT_GFX_RAYQUAZA_STILL` e a flag foi zerada (`flag: 0`). | Um dragão Rayquaza verde estático aparecia pousado no telhado do santuário. |
| **3 Bug Catchers Bloqueando a Entrada** | Na ROM original, eram membros da Team Rocket com a flag de despawn `930`. No port, viraram 3 Bug Catchers permanentes com mensagem vazia (*"There is nothing to say here."*). | Três garotos Bug Catcher ombro a ombro impediam a aproximação e o exame do altar. |
| **Placas de Terra Roxa / Lilás** | Metatiles `664..666`, `672..677` e `680..682` utilizavam a Paleta 9 da camada secundária, cujas cores `#805050` e `#987068` possuem forte tonalidade de vinho/magenta. | Três manchas quadradas de terra roxa artificial pareciam tapetes estragados na grama. |

---

## 3. Correções Aplicadas (Opção 2 - Altar Autêntico)

1. **Remoção de Objetos Espúrios (`data/maps/PeacefulWoods/map.json`)**:
   - Removido o Rayquaza fantasma de `(57, 36)`.
   - Removidos os 3 Bug Catchers de `(56, 38)`, `(57, 38)` e `(58, 38)`.
   - Limpeza dos scripts correspondentes em `data/maps/PeacefulWoods/scripts.inc`.

2. **Ativação da Interação Oficial do Altar**:
   - O objeto de sinalização `PeacefulWoods_Sign6` em `(57, 37)` agora está totalmente acessível.
   - Ao se posicionar em `(57, 38)` virado para o norte e pressionar **A**, o jogador lê a mensagem oficial:
     > *“This altar is in homage to the guardian of the forest.”*

3. **Revitalização Paisagística do Solo (`data/layouts/peaceful_woods/map.bin`)**:
   - As três manchas roxas artificiais foram substituídas pelo metatile de grama verde viva `652` (`0x328C`, colisão normal e elevação 3).
   - Inseridos toques sutis de flores silvestres rosas `752` (`0x32F0`) em `(54, 37)`, `(60, 37)` e `(57, 40)`, transformando a clareira num ambiente sereno e sagrado.

---

## 4. Recriação Estética com Tileset de Grim Woods (*Pokémon Unbound*) + Gramas Altas

Para elevar o padrão visual de **Peaceful Woods** mantendo 100% da narrativa, *level design* ($78 \times 78$), treinadores, itens, Pokémon de ambiente e o Altar de Celebi de *Éclat Pourpre*, o mapa recebeu um par dedicado de tilesets extraído de **Grim Woods** (*Pokémon Unbound*):

1. **Tilesets Dedicados (`gTileset_PeacefulWoodsGrimPrimary` + `gTileset_PeacefulWoodsGrimSecondary`)**:
   - Gerados de forma reprodutível por `tools/eclat/build_peaceful_woods_grim.py` sem afetar nenhum outro mapa do projeto (preservando `data/tilesets/secondary/peaceful_woods/` intacto).
   - **Árvores, chão nebuloso, cogumelos vermelhos, placas, moitas $2 \times 2$, troncos ocos e poças**: Extraídos 1:1 de Grim Woods (`Pal 7..11`).
   - **Trilhas de musgo orgânicas**: Substituíram as antigas trilhas marrons/roxas por clareiras naturais de musgo (`Pal 9`).
   - **Barrancos, cavernas, escadas de madeira e Altar de Celebi**: Integrados nas paletas `Pal 3, 4, 5, 12` com corrimãos em madeira envelhecida e rochas harmonizadas à iluminação mística da floresta.
   - **Correções de sobreposição**: Copas densas empilhadas (`MT 643, 644` e `border.bin`) usam tronco sombreado opaco (`GW 742, 743`) e copas sobre musgo (`MT 709, 710`) usam base de musgo `Pal 9` sem recortes quadrados.

2. **Gramas Altas 9-Slice de Grim Woods e Encontros Selvagens**:
   - No `pokeemerald-expansion`, encontros terrestres exigem `MB_TALL_GRASS` (`0x02`); antes, o chão `652` possuía `MB_NORMAL` (`0x00`), impedindo encontros selvagens.
   - **Técnica de camada única na `Pal 6`**: As cores da grama alta de Grim Woods (`1..12`) e do solo nebuloso (`13, 14`) foram unificadas na `Pal 6` livre, assando todas as 16 variações 9-slice em `L0`. Isso liberou `L1` (`Pal 8`, `NORMAL`) para desenhar **bordas de copa de árvore sobre a grama alta** (`MT 840..887`) em hardware GBA padrão de 2 camadas (sem precisar do hack de 3 camadas da CFRU).
   - **Distribuição orgânica**: 205 blocos de grama alta em 22 clareiras espalhadas pelas 5 regiões da floresta, mantendo as trilhas principais de musgo, treinadores, itens, árvores de Cut e o Altar de Celebi (`x=52..62, y=33..42`) totalmente livres.
   - **Animação de grama 1:1 do Pokémon Unbound e taxa de encontro**:
     - Extraídos diretamente da ROM do *Pokémon Unbound* os 5 quadros $16 \times 16$ 4bpp de lâminas se abrindo ao caminhar (`graphics/eclat_intro/unbound_tall_grass.bin`, offset `0x39A008`, `640` bytes) e a paleta oficial verde-esmeralda/menta (`graphics/eclat_intro/grim_woods_tall_grass_palette.bin`, offset `0x398FC8`, `32` bytes), conectados em `gFieldEffectObjectTemplate_GrimWoodsTallGrass` (`sPicTable_UnboundTallGrass` + `gSpritePalette_GrimWoodsTallGrass`) e em `FldEff_JumpTallGrass()`.
     - Ajustado `encounter_rate` de `5` para `15` em `src/data/wild_encounters.json`.

3. **Animação de Balanço das Árvores ao Vento (`tree_anim.bin`)**:
   - Inspirada no balanço de folhagem da Rota 2, foi implementada `InitTilesetAnim_PeacefulWoodsGrim()` / `TilesetAnim_PeacefulWoodsGrim()` em `src/tileset_anims.c`, vinculada ao `.callback` de `gTileset_PeacefulWoodsGrimSecondary`.
   - Gerada via `tools/eclat/grim_woods_anim.py` (`data/tilesets/secondary/peaceful_woods_grim/tree_anim.bin`, `9.728` bytes): ciclo suave de brisa em 8 etapas $\times$ 14 frames (`112` frames por ciclo) com defasagem orgânica entre o topo (`y = 1..15`) e a base da copa (`y = 16..30`), mantendo os troncos e raízes 100% ancorados no chão.
   - Atualiza via DMA no VBlank tanto os 20 tiles puros de copa (`VRAM 713..716`, `729..732`, `744..749`, `760..765`) quanto os 18 tiles compostos de interseção/sobreposição de árvores (`VRAM 1..13` e `15..19`), animando sem costuras todas as árvores isoladas, fileiras horizontais, paredes densas e bordas sobre a grama alta.

4. **Saídas Iluminadas Concêntricas e Correção de Copas nas Saídas (`MT 768..775`)**:
   - **Causa da copa "comida" e do traço animado flutuante**: Os metatiles de warp `MT 768` (`12, 56`, saída SO para a Rota 2) e `MT 772` (`69, 8`, saída NE para a Rota 3) estavam com `gw_ovh_l` e `gw_ovh_r` invertidos em `L1`. Isso deixava tile transparente (`0x0000`) encostado na árvore vizinha — cortando a lateral da copa — e posicionava a borda animada externa da árvore `8 px` solta no ar. Além disso, `layerType = COVERED` (`0x20000062/63`) desenhava as copas abaixo do jogador.
   - **Correção estrutural**: `MT 772` agora usa `L1 = gw_ovh_l` (`0x00000062`, `NORMAL`, `MB_EAST_ARROW_WARP`) e `MT 768` usa `L1 = gw_ovh_r` (`0x00000063`, `NORMAL`, `MB_WEST_ARROW_WARP`), restaurando 100% da silhueta arredondada das árvores laterais, eliminando o traço vertical na animação e renderizando as copas sobre o jogador.
   - **Degradê oval iluminado de 5 faixas (`Pal 2`)**: Implementado em `tools/eclat/grim_woods_exits_and_bg.py` um semi-oval concêntrico $32 \times 32\text{ px}$ ($2 \times 2$ metatiles: `770, 772, 775, 771` em `(68..69, 8..9)` rumo à Rota 3 e `768, 773, 769, 774` em `(12..13, 56..57)` rumo à Rota 2) na `Pal 2` primária, clareando progressivamente do solo nebuloso `(98, 131, 115)` até luz solar suave `(189, 222, 148)` (com *tint* automático de dia/noite em sincronia com a `Pal 7`).

5. **Fundo de Batalha Oficial de Grim Woods (*Pokémon Unbound*)**:
   - Extraídos diretamente da ROM do *Pokémon Unbound* os dois cenários oficiais de batalha de Grim Woods (`graphics/battle_environment/grim_woods/`):
     - **Grama Alta (`Entry 25`)**: `grass_tiles.lz.bin` (`0xED4738`), `grass_map.lz.bin` (`0xED54B8`), `grass_entry_tiles.lz.bin` (`0x248C68`), `grass_entry_map.lz.bin` e paletas de 96 bytes para **Dia** (`0xED5B10`), **Entardecer** (`0xED6F54`) e **Noite** (`0xED6FF0`).
     - **Solo da Floresta / Treinadores (`Entry 34`)**: `plain_tiles.lz.bin` (`0xED5B4C`), `plain_map.lz.bin` (`0xED64B8`), `plain_entry_tiles.lz.bin` (`0x24C314`), `plain_entry_map.lz.bin` (`0x24C520` preenchido para `2048` bytes) e paletas de 96 bytes para **Dia** (`0xED6AC4`), **Entardecer** (`0xED6F90`) e **Noite** (`0xED702C`).
   - Conectados em `src/data/grim_woods_battle.h` e selecionados dinamicamente em `GetBattleEnvironmentGraphics()` (`src/battle_bg.c`) quando `gMapHeader.mapLayoutId == LAYOUT_PEACEFUL_WOODS`, respeitando `GetTimeOfDay()` tanto em encontros na grama alta quanto em batalhas contra treinadores no piso da floresta.

6. **Refinamento do Terreno: Remoção de Rios e Preservação de Troncos Caídos em Terra Seca**:
   - Conforme diretriz do usuário de manter a essência da floresta sem divisões hídricas agressivas e preservar a serenidade da caminhada contínua pelo bosque:
     - **Rios, Pontes e Piscinas Removidos**: Todos os corpos d'água, pontes de madeira e piscinas de água ao redor do Altar de Celebi foram inteiramente retirados do mapa, restaurando 100% do chão florestal contínuo de terra e musgo original (`0 metatiles de rio em map.bin`).
     - **Troncos Ocos Caídos em Terra Seca (`MT 816, 817`)**: Mantidos os 3 troncos caídos de Grim Woods $2 \times 1$ em clareiras amplas sem obstruir caminhos:
       1. Clareira Oeste: `(6, 30)`
       2. Corredor Central Norte: `(19, 23)`
       3. Clareira Sul de Entrada: `(42, 62)`
     - **Encontros Selvagens Exclusivamente Terrestres**: `MAP_PEACEFUL_WOODS` em `src/data/wild_encounters.json` conta exclusivamente com `land_mons` (`encounter_rate: 15`), sem tabelas de surfar (`water_mons`) ou pescar (`fishing_mons`).
     - **Animação Pura de Folhagem (38 Tiles)**: `src/tileset_anims.c` opera com `GRIM_WOODS_TREE_STEP_STRIDE = 38 * 16`, animando as árvores em 8 etapas com `tree_anim.bin` de `9.728` bytes.
     - **Cenários de Batalha de Grim Woods**: `src/battle_bg.c` mapeia todos os combates em Peaceful Woods dinamicamente para os fundos oficiais de Grim Woods (Grama Alta e Solo Florestal para Dia, Entardecer e Noite).

7. **Restauração 1:1 da Geografia Original e Correção dos Contornos**:
   - **Extração Fiel da ROM Éclat Pourpre (`0x87BC08`)**: Extraída a matriz binária original completa de $78 \times 78$ metatiles (12.168 bytes), revertendo todas as 358 alterações acidentais prévias onde desníveis e paredões tinham sido substituídos por chão plano (`MT 652`).
   - **Reconstrução Limpa dos Contornos e Desníveis de Terra/Rocha**:
     - Desníveis horizontais de salto (`MT 713` / `0x00000032`, `MB_JUMP_SOUTH` na `Pal 3`) e cantos curvos de relevo (`MT 712` canto esquerdo, `MT 714` canto direito) mapeados com bordas arredondadas e transições suaves sem quebras secas.
     - Sobreposição orgânica de copas de árvore sobre desníveis (`MT 707` e `MT 708`, com `L0 = ledge` e `L1 = gw_tree_tl/tr`), mantendo a linha contínua do barranco sob a folhagem sem criar vãos quadrados.
     - Bordas e curvas de platôs rochosos (`MT 715, 716, 720, 722, 723, 728, 729`) e escadas de madeira (`MT 744..746`) integradas organicamente ao relevo.

8. **Compatibilidade com o Porymap**:
   - Espelhos 512/512 gerados via `tools/eclat/porymap_iviere_compat.py` em `data/tilesets/primary/peaceful_woods_grim_primary/` e `data/tilesets/secondary/peaceful_woods_grim_secondary/`.
   - No seletor do Porymap (8 colunas), o carimbo $3 \times 3$ da grama alta está organizado visualmente em `792..811`, troncos ocos/poças extras em `816..835`, variantes sob copas de árvores em `840..887` e metatiles decorativos em `888..931`.

---

## 5. Validação

- Compilação realizada com sucesso sem erros via script WSL `./build.sh` (uso de ROM: `82,04%`).
- Presença binária exata verificada na ROM (`map.bin` com 0 rios, 6 metatiles de tronco e 196 gramas altas; `tree_anim.bin` de `9.728` bytes).
- Todos os scripts Python mantidos rigorosamente $\le 500$ linhas (`build_peaceful_woods_grim.py`: 499 linhas; `grim_woods_river.py`: 138 linhas; `grim_woods_exits_and_bg.py`: 249 linhas; `grim_woods_anim.py`: 136 linhas).
- ROM de teste atualizada e validada: `eclat-intro-teste.gba`.
