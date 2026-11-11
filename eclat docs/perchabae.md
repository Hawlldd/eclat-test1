# Perchabae / Percharbre (Éclat Pourpre) — Implementação Completa

## 1. Visão Geral
**Perchabae** (denominada **Percharbre** no original francês de *Pokémon Éclat Pourpre*) é a primeira grande metrópole com ginásio da região, caracterizada por ser uma cidade construída sobre as copas das árvores e plataformas elevadas de madeira em perfeita harmonia com a flora local.

Na arquitetura do projeto `pokeemerald-expansion`, Perchabae substitui o slot de **Petalburg City** (`MAP_PETALBURG_CITY`), conectando-se diretamente a oeste com a **Rota 3** (`MAP_ROUTE103`) e a leste com a **Rota 4**.

- **ID do Mapa**: `MAP_PETALBURG_CITY`
- **Nome no Porymap**: `PetalburgCity`
- **Layout**: `LAYOUT_ECLAT_PERCHABAE` ($70 \times 40$ metatiles, 2800 blocos / 5600 bytes)
- **Música**: `MUS_RG_PEWTER` (Tema clássico adaptado em Éclat Pourpre)
- **Seção do Mapa**: `MAPSEC_PETALBURG_CITY` (com nome exibido no popup configurado como `"PERCHABAE"` em `src/data/region_map/region_map_sections.json`)
- **Tipo de Mapa**: `MAP_TYPE_CITY`
- **Clima**: `WEATHER_SUNNY`
- **Permissões**: Corrida (`allow_running: true`), Ciclismo (`allow_cycling: true`), Exibição de Nome (`show_map_name: true`)

---

## 2. Tilesets e Fidelidade Gráfica

- **Tileset Primário**: `gTileset_EclatIvierePrimary` (compartilhado com Ivière Town e Rota 1/2/3).
- **Tileset Secundário**: `gTileset_EclatRoute3Secondary` (`data/tilesets/secondary/eclat_route3/metatiles.bin`).
- **Compatibilidade 100%**: A análise binária da ROM de referência (`0x34aaa0`) comprovou que Perchabae utiliza exatamente os mesmos metatiles da Rota 3 (`0x2cef2c`). Não foi necessária nenhuma conversão ou remapeamento de blocos; todos os 182 metatiles distintos do mapa encaixam com precisão matemática na paleta de cores e estrutura já existente.
- **Continuidade Visual**: A junção de fronteira entre Rota 3 e Perchabae é contínua e sem corte visual, compartilhando a mesma iluminação e texturas de relva, passadiços e copas de carvalho.

---

## 3. Conexões entre Mapas

As conexões espaciais respeitam o sistema de coordenadas de borda de pokeemerald:

- **Conexão Oeste (Esquerda)**:
  - Destino: `MAP_ROUTE103` (Rota 3)
  - Direção: `left`
  - Offset: `-8`
- **Conexão Leste da Rota 3 (Direita)**:
  - No arquivo `data/maps/Route3/map.json`:
  - Destino: `MAP_PETALBURG_CITY`
  - Direção: `right`
  - Offset: `+8` (recíproca exata: $56 - 8 = 48 \ge 40$, alinhando perfeitamente a pista de terra batida entre os dois mapas).
- **Conexão Leste (Direita)**:
  - Rota 4 (bloqueada temporariamente pelo guarda local).

---

## 4. Warps e Edifícios (Interiores)

Os pontos de teletransporte para as construções e portas da cidade foram indexados para coincidir exatamente com os mapas de interior de Petalburg já existentes na engine:

| Warp ID | Coordenadas $(X, Y)$ | Destino no Projeto | Descrição / Função |
| :---: | :---: | :--- | :--- |
| **Warp 0** | `(34, 26)` | `MAP_PETALBURG_CITY_HOUSE1` | Casa Residencial 1 |
| **Warp 1** | `(25, 18)` | `MAP_PETALBURG_CITY_WALLYS_HOUSE` | Casa Residencial 3 |
| **Warp 2** | `(33, 6)` | `MAP_PETALBURG_CITY_GYM` | **Ginásio Pokémon de Perchabae** |
| **Warp 3** | `(41, 23)` | `MAP_PETALBURG_CITY_POKEMON_CENTER_1F` | **Centro Pokémon** (com cura e venda de Berries) |
| **Warp 4** | `(33, 11)` | `MAP_PETALBURG_CITY_HOUSE2` | Casa Residencial 2 |
| **Warp 5** | `(46, 12)` | `MAP_PETALBURG_CITY_MART` | **Poké Mart de Perchabae** |
| **Warp 6** | `(23, 30)` | `MAP_PETALBURG_CITY_HOUSE1` | Casa Residencial 4 |

---

## 5. Ginásio de Perchabae — Líder Roseline

- **Especialidade**: Tipo Planta (*Grass-type*) e estética botânica.
- **Título**: *"ROSELINE, the flower of PERCHABAE"*.
- **Insígnia Oficial**: **Insígnia Lys** (*Lys Badge*).
- **Identificação**: Placas dedicadas em frente ao ginásio na coordenada `(32, 7)` e afixada à estrutura em `(23, 3)`.
- **Progressão**: A conquista da Insígnia Lys (marcada pela flag `FLAG_BADGE01_GET`) é o requisito obrigatório para obter permissão de passagem na saída leste para a Rota 4.

---

## 6. População, Diálogos e Eventos (NPCs)

Todos os NPCs originais foram recriados com suas movimentações, gráficos e falas autênticas:

1. **Garota com Vertigem** (`(8, 17)`, `OBJ_EVENT_GFX_WOMAN_2`):
   - Diálogo: *"I would love to visit PERCHABAE city, but I have vertigo."*
2. **Menino das Frutas (Berry Boy)** (`(21, 15)`, `OBJ_EVENT_GFX_BOY_1`):
   - Diálogo: *"Hi! Did you know that you can buy Berries at the POKéMON center? You should think about buying some. It can be decisive in a match."*
3. **Garoto de Hoenn / Fortree** (`(22, 31)`, `OBJ_EVENT_GFX_FAT_MAN`):
   - Diálogo: *"How do you like this town? It's nestled in the trees, which reminds me of a town in HOENN, but I can't remember which."* (Alusão nostálgica a Fortree City).
4. **Moradora de Peaceful Woods** (`(39, 26)`, `OBJ_EVENT_GFX_GIRL_1`):
   - Diálogo: *"I chose to live in this city because the PEACEFUL WOODS are close by."*
5. **Especialista em Pedras Evolutivas** (`(42, 15)`, `OBJ_EVENT_GFX_SCIENTIST_1`):
   - Diálogo: *"Evolution stones are quite rare. However, you can find MOON STONE on Clefairy and SUNSTONES on Sunkern, even if it takes luck."*
6. **Organizador da Grande Loteria** (`(53, 25)`, `OBJ_EVENT_GFX_GENTLEMAN`):
   - Diálogo: *"Hi there! You're probably thinking that my outfit is strange. And it's true. I'm organizing a big game for POKéMON TRAINERS. All you have to do is find some TICKETS to play my great lotto! My friends will give you some if you find them. You can try to win a POKéMON egg from over 300 different species."*
7. **NPC Secreto dos Tickets** (`(12, 0)`, `OBJ_EVENT_GFX_NINJA_BOY`):
   - Localização: Escondido atrás da folhagem densa no topo norte da cidade, bloqueado pela árvore de Cut.
   - Script: Ao ser encontrado, parabeniza o jogador, entrega o `ITEM_MYSTIC_TICKET` (salvo na flag `FLAG_PERCHABAE_TICKET`), explica a utilidade na loteria e desaparece na folhagem via animação de fade.
8. **Guarda da Rota 4** (`(67, 12)`, `OBJ_EVENT_GFX_POLICEMAN`):
   - Sem a Insígnia: Bloqueia a passagem afirmando que desafia apenas quem possui a insígnia local (*"Weak trainers don't interest me."*).
   - Com a Insígnia: O jogador exibe a Insígnia Lys (*"{PLAYER} flashes the LYS BADGE."*) e o guarda elogia a competência do treinador (*"You didn't get your BADGE on a stroke of luck it seems."*).
9. **Árvore de Cut** (`(12, 2)`, `OBJ_EVENT_GFX_CUTTABLE_TREE`):
   - Utiliza a flag `FLAG_PERCHABAE_CUT_TREE` e o script nativo `EventScript_CutTree`.

---

## 7. Vida Selvagem Integrada (Overworld Pokémon)

Perchabae reflete o convívio direto entre humanos e Pokémon nativos através de sprites dinâmicos utilizando a macro `OBJ_EVENT_GFX_SPECIES`:

- **Bando de Bellossom**:
  - Distribuídas pelas plataformas em `(20, 26)`, `(22, 20)`, `(34, 22)`, `(51, 28)`, `(55, 24)`, `(48, 25)` e `(43, 9)`.
  - Ao interagir, executam o cry sonoro característico de Bellossom (`playmoncry SPECIES_BELLOSSOM`) e exibem *"Bellossom: Bellooosom!"*.
  - Exemplares distraídos em `(9, 0)` e `(44, 19)` ignoram o jogador (*"This wild POKéMON seems to be ignoring you. It's best to leave it alone."*).
- **Vileplume Guardião**:
  - Localizado em `(36, 7)` junto ao jardim do ginásio.
  - Executa o cry de Vileplume e expressa *"VILEPLUME: Rafleeeesia !"*.

---

## 8. Sinalização e Placas (BG Events)

- `(10, 15)`: *"Head west on ROUTE 3."*
- `(64, 10)`: *"Head east on ROUTE 4."*
- `(38, 21)`: *"PERCHABAE - Elevated above the lush greenery."*
- `(23, 3)`: Placa na parede do Ginásio Pokémon.
- `(42, 23)`: Placa luminosa do Centro Pokémon.
- `(47, 12)`: Placa luminosa do Poké Mart.

---

## 9. Arquivos Modificados no Repositório

1. `data/layouts/layouts.json`: Registro de `LAYOUT_ECLAT_PERCHABAE` ($70 \times 40$).
2. `data/layouts/eclat_perchabae/map.bin`: 5600 bytes do mapa com os 2800 metatiles de Perchabae.
3. `data/layouts/eclat_perchabae/border.bin`: 8 bytes de borda de árvores.
4. `data/maps/PetalburgCity/map.json`: Configuração de layout, warps, objetos, placas e conexão com Rota 3.
5. `data/maps/PetalburgCity/scripts.inc`: Implementação completa dos scripts de eventos, diálogos, cries e verificação de insígnia.
6. `data/maps/Route3/map.json`: Conexão de borda à direita com `MAP_PETALBURG_CITY` no offset `+8`.
7. `include/constants/flags.h`: Alocação de `FLAG_PERCHABAE_TICKET` (`0x48`) e `FLAG_PERCHABAE_CUT_TREE` (`0x49`).
8. `src/data/region_map/region_map_sections.json`: Atualização do nome de `MAPSEC_PETALBURG_CITY` para `"PERCHABAE"`.
9. `eclat docs/perchabae.md`: Esta documentação técnica integral.
