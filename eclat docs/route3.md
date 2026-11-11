# Rota 3 (Éclat Pourpre) — Implementação Completa

## 1. Visão Geral
A **Rota 3** é a estrada que conecta a saída oriental de **Peaceful Woods** em direção a **Percharbre** e ao sopé da **Rota 4 / Mont Sélénite**. Na arquitetura do projeto pokeemerald-expansion, a Rota 3 substitui o slot da `Route103` (`MAP_ROUTE103` no grupo `gMapGroup_TownsAndRoutes`), seguindo rigorosamente o padrão arquitetural estabelecido pela Rota 1 (`MAP_ROUTE101`) e Rota 2 (`MAP_ROUTE102`).

- **ID do Mapa**: `MAP_ROUTE103`
- **Nome no Porymap**: `Route3`
- **Layout**: `LAYOUT_ECLAT_ROUTE3` ($66 \times 56$ metatiles, 3696 blocos)
- **Música**: `MUS_RG_ROUTE3`
- **Seção do Mapa**: `MAPSEC_ROUTE_103` ("ROUTE 3")
- **Tipo de Mapa**: `MAP_TYPE_ROUTE`
- **Clima**: `WEATHER_SUNNY`

---

## 2. Tilesets e Compatibilidade Porymap

- **Tileset Primário**: `gTileset_EclatIvierePrimary` (640 metatiles FRLG, compartilhado com Ivière e Rota 1).
- **Tileset Secundário da ROM**: `gTileset_EclatRoute3Secondary` em `data/tilesets/secondary/eclat_route3/` (384 metatiles FRLG / 12 KB tiles). Atributos extraídos do endereço original `0x8894d8` apontado pelo cabeçalho `0x2cef2c` da ROM original (com comportamentos de warp direcional `0x63`).
- **Tileset de Edição Porymap**: `data/tilesets/secondary/eclat_route_3secondary/` (e alias `eclat_route3_secondary/`) gerado via `tools/eclat/porymap_iviere_compat.py` (o Porymap converte `gTileset_EclatRoute3Secondary` inserindo underscore antes do número, requerendo `eclat_route_3secondary`). Contém `tiles.png`, paletas `00.pal` a `12.pal`, `metatiles.bin` e `metatile_attributes.bin` no formato 512/512 de Emerald.

---

## 3. Conexões e Warps

- **Warps com Peaceful Woods**:
  - `Warp 0` em `(10, 35)`, elevation 0 $\rightarrow$ `MAP_PEACEFUL_WOODS`, `dest_warp_id: 3`
  - `Warp 1` em `(10, 36)`, elevation 0 $\rightarrow$ `MAP_PEACEFUL_WOODS`, `dest_warp_id: 2`
  - Os metatiles 774 (`0x306`) e 782 (`0x30E`) da Rota 3 estão posicionados exatamente na abertura em `(10, 35)` e `(10, 36)` e possuem comportamento `MB_WEST_ARROW_WARP` (`0x63`), acionando o retorno para Peaceful Woods ao andar para a esquerda.
  - Em `MAP_PEACEFUL_WOODS`, os Warps 2 e 3 em `(69, 9)` e `(69, 8)` conectam de volta com perfeição aos Warps 1 e 0 da Rota 3.

- **Conexão Direta de Fronteira com Percharbre (Petalburg City)**:
  - Rota 3 conecta à direita com `MAP_PETALBURG_CITY` com `offset: +8`.
  - A estrada de saída da Rota 3 em `x=64..65, y=24..27` alinha com precisão matemática com a entrada ocidental de Percharbre em `x=0..1, y=16..19` ($Y_{\text{Route3}} - 8 = Y_{\text{Perchabae}}$), permitindo travessia contínua, sem degraus visuais ou colisões.

---

## 4. Treinadores e Batalhas

Foram portados os 6 treinadores de campo originais da Rota 3 com diálogos fiéis em inglês ($\le 32$ caracteres por linha):

1. **Youngster Calvin** (`(42, 25)`, visão 4, `TRAINER_ROUTE3_CALVIN`):
   - Time: Mankey Nível 11 (Golpes: *Low Kick*, *Fury Swipes*, *Leer*, *Karate Chop*).
   - Intro: *"Watch out for my new\nPOKéMON!"*
   - Derrota: *"Well, I need new ones!"*
   - Pós: *"Trained POKéMON are no longer\nstronger than wild ones!"*

2. **Lass Meg** (`(41, 3)`, visão 5, `TRAINER_ROUTE3_MEG`):
   - Time: Pidgey Nível 12, Oddish Nível 12, Bellsprout Nível 12.
   - Intro: *"Hey! Are you going to PETIPRES?\nNot without having a battle!"*
   - Derrota: *"You're not nice!"*
   - Pós: *"This is the only road that\nconnects to the next town."*

3. **Youngster Elijah** (`(39, 3)`, visão 2, `TRAINER_ROUTE3_ELIJAH`):
   - Time: Rattata Nível 11, Ekans Nível 11.
   - Intro: *"I captured my POKéMON in the\nPEACEFUL WOODS."*
   - Derrota: *"Maybe I should have captured\nmore POKéMON."*
   - Pós: *"Sometimes it's worth it to\nspend hours searching for rare\nPOKéMON."*

4. **Bug Catcher Colton** (`(36, 40)`, visão 1, `TRAINER_ROUTE3_COLTON`):
   - Time: Caterpie Nível 9, Weedle Nível 9, Caterpie Nível 10.
   - Intro: *"Hey! You don't have any shorts!\nYou suck!"*
   - Derrota: *"Lost! Lost, I lost!"*
   - Pós: *"I always wear shorts, even\nin the winter!"*

5. **Lass Lana** (`(38, 3)`, visão 5, `TRAINER_ROUTE3_LANA`):
   - Time: Nidoran♂ Nível 16, Nidoran♀ Nível 16.
   - Intro: *"Do you want my picture?"*
   - Derrota: *"Hey, that's my picture!"*
   - Pós: *"Don't make eye contact if you\ndon't wanna battle!"*

6. **Bug Catcher Cedric** (`(37, 3)`, visão 2, `TRAINER_ROUTE3_CEDRIC`):
   - Time: Weedle Nível 9, Kakuna Nível 9, Caterpie Nível 10, Metapod Nível 10.
   - Intro: *"I'm sick of these shorts, the\ntall grass is always scratching!"*
   - Derrota: *"Anyway, I still prefer shorts."*
   - Pós: *"It's so dumb that I have to\nwear shorts even in winter!"*

---

## 5. NPCs, Itens e Placas

- **NPC Mulher** (`(44, 22)`): *"You know, CLEFAIRY can sometimes\nhold MOON STONEs."*
- **NPC Secreto da Loteria** (`(14, 18)`): *"Yo-oh!\nWell done, you found me!\pTake this!"* $\rightarrow$ Entrega `ITEM_EON_TICKET` (Ticket da loteria).
- **Poké Balls com Itens**:
  - `(40, 42)`: `ITEM_SUPER_POTION` (`FLAG_ITEM_ROUTE_3_SUPER_POTION`)
  - `(37, 32)`: `ITEM_AWAKENING` (`FLAG_ITEM_ROUTE_3_AWAKENING`)
  - `(19, 22)`: `ITEM_RARE_CANDY` (`FLAG_ITEM_ROUTE_3_RARE_CANDY`)
- **Item Oculto**:
  - `(9, 4)`: `ITEM_ORAN_BERRY` (`FLAG_HIDDEN_ITEM_ROUTE_3_ORAN_BERRY`)
- **Placas**:
  - `(58, 23)` e `(14, 32)`: *"ROUTE 3\nPEACEFUL WOODS - PERCHARBRE"*
- **Pokémon do Cenário (Ambiente)**:
  - Rattatas terrestres com cry nativo.
  - Pokémon aquáticos no rio com cry nativo.

---

## 6. Encontros Selvagens

Atualizados em `src/data/wild_encounters.json` para `MAP_ROUTE103`:

- **Grama (Land, taxa 5%)**:
  - Surskit (Nív. 7-9)
  - Dewpider (Nív. 7-9)
  - Igglybuff (Nív. 7-8)
  - Cherubi (Nív. 7-8)
  - Taillow (Nív. 7-8)
  - Poliwag (Nív. 7-9)
  - Ledyba (Nív. 7-9)
  - Minccino (Nív. 7-9)
  - Lotad (Nív. 7-9)
  - Eevee (Nív. 7-9)
  - Sunkern (Nív. 7-9)
  - Cleffa (Nív. 7-9)
- **Água (Surfing, taxa 6%)**:
  - Psyduck (Nív. 20-40)
- **Pesca (Fishing, taxa 20%)**:
  - Magikarp (Nív. 5-15)
  - Goldeen (Nív. 5-15)
  - Poliwag (Nív. 5-25)
  - Poliwhirl (Nív. 20-30)
  - Froakie (Nív. 15-25)
  - Luvdisc (Nív. 15-25)
  - Frogadier (Nív. 25-30)
