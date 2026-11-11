# Pokémon Éclat — Guia de Organização e Desenvolvimento

Este documento descreve a arquitetura, a organização dos arquivos e os fluxos de trabalho do projeto **Pokémon Éclat**, construído sobre a base [`pokeemerald-expansion`](https://github.com/rh-hideout/pokeemerald-expansion) (versão 1.17.2).

O objetivo deste guia é manter o projeto padronizado, limpo e 100% replicável caso seja hospedado no GitHub ou compartilhado com outros colaboradores.

---

## 📋 Sumário
1. [Visão Geral e Compilação](#-visão-geral-e-compilação)
2. [Estrutura de Pastas do Projeto](#-estrutura-de-pastas-do-projeto)
3. [Sistema de Summary Screen (Tela de Resumo)](#-sistema-de-summary-screen-tela-de-resumo)
4. [Ferramentas de Automação (`tools/eclat/`)](#-ferramentas-de-automação-toolseclat)
5. [Documentação de Design (`eclat docs/`)](#-documentação-de-design-eclat-docs)
6. [Boas Práticas de Versionamento (Git/GitHub)](#-boas-práticas-de-versionamento-gitgithub)

---

## ⚙️ Visão Geral e Compilação

O projeto utiliza o toolchain padrão de compilação da engine pokeemerald (`arm-none-eabi-gcc`), empacotado através do script `./build.sh`.

### Como Compilar
No ambiente Linux/WSL, execute na raiz do repositório:
```bash
./build.sh
```
O script executa:
1. Sincronização incremental de metatiles e tiles modificados.
2. Compilação dos arquivos fonte C alterados e geração de objetos.
3. Compressão de tilemaps (`compresSmolTilemap`) e gráficos 4bpp (`gbagfx`).
4. Linkagem do ELF e correção do cabeçalho da ROM via `gbafix`.
5. Geração da ROM pronta: `eclat-intro-teste.gba`.

---

## 📁 Estrutura de Pastas do Projeto

```
eclat-intro-teste/
├── asm/                     # Macros e rotinas em Assembly GBA
├── data/                    # Mapas, scripts de eventos e metatiles
│   ├── maps/                # Definições JSON e scripts .inc dos mapas
│   ├── layouts/             # Mapas de blocos (map.bin) de cada área
│   ├── scripts/             # Scripts globais (Centros Pokémon, Marts, etc.)
│   └── tilesets/            # Tilesets primários e secundários (metatiles.bin)
├── eclat docs/              # Documentos de lore, roteiro, notas técnicas e referências
├── graphics/                # Assets gráficos do jogo
│   ├── summary_screen/      # Gráficos da tela de resumo (tileset, tilemaps, fontes)
│   │   └── sources/         # PNGs mestres editáveis das abas (240x160 px)
│   ├── fonts/               # Fontes de texto (latin_short.png, normal, etc.)
│   └── text_window/         # Caixas de diálogo e caixas de nomes
├── include/                 # Arquivos de cabeçalho (.h) e configurações
│   ├── config/              # Toggles de funcionalidades (contest.h, battle.h, etc.)
│   └── constants/           # IDs de flags, vars, espécies, golpes e itens
├── src/                     # Código-fonte principal em C
│   ├── pokemon_summary_screen.c  # Implementação da tela de resumo
│   ├── unbound_start_menu.c      # Menu Iniciar estilo Unbound
│   └── quest_indicator.c        # Sistema de indicadores de missões
├── tools/                   # Compiladores e scripts auxiliares
│   └── eclat/               # Ferramentas exclusivas do Pokémon Éclat
├── README.md                # README oficial da engine pokeemerald-expansion
├── README_ECLAT.md          # Este documento (organização do Pokémon Éclat)
└── build.sh                 # Script mestre de compilação
```

---

## 🎨 Sistema de Summary Screen (Tela de Resumo)

A tela de resumo dos Pokémon foi remodelada com base no design DP/Pt (Gen 4) em tom azul-aço escuro (estilo *Pokémon Saffron / FireRed*), utilizando fontes compactas e legíveis.

### Características Implementadas:
- **3 Abas Ativas**:
  - **Aba 1 (Info)**: Número da Dex (3 dígitos com zeros à esquerda), Nome, Tipo (Badge colorido), OT, IDNo, Item segurado e Trainer Memo centralizado.
  - **Aba 2 (Skills)**: Estatísticas (`HP`, `ATTACK`, `DEFENSE`, `SP.ATK`, `SP.DEF`, `SPEED`) com notas de IV em letras coloridas (`S, A, B+, C+, D-, E`), destaque de Natureza (Vermelho para bônus, Azul para penalidade) e caixa inferior de Habilidade/EXP.
  - **Aba 3 (Battle Moves)**: Cards de golpes individuais com ícone de categoria/tipo, nome e contador de PP (`PP XX/YY`).
- **Aba Contest**: Desativada de forma limpa via configuração `#define C_HIDE_CONTEST_DATA TRUE` em `include/config/contest.h`.
- **Tipografia**: Utiliza a fonte compacta [`FONT_SHORT`](file:///C:/Users/raulr/OneDrive/Desktop/zEmerald/eclat-intro-teste/src/pokemon_summary_screen.c#L3060-L3075) para alinhamento e proporcionalidade idênticos ao FireRed.

### Como Editar os Fundos das Abas:
1. Abra e edite as imagens mestres em:
   📁 **`graphics/summary_screen/sources/`**
   - `page_info.png`: Fundo da Aba 1 (Info)
   - `page_skills.png`: Fundo da Aba 2 (Skills)
   - `page_battle_moves.png`: Fundo da Aba 3 (Golpes)
2. **Regras de Edição**:
   - Dimensão exata de **240 x 160 pixels**.
   - Não desenhe dados dinâmicos (como nome do mon ou número na dex), pois o motor C imprime esses textos por cima.
   - Utilize a paleta de cores GBA padrão do projeto.
3. Reimporte automaticamente para o jogo executando:
   ```bash
   python3 tools/eclat/import_summary_screen.py
   ```
4. Recompile a ROM:
   ```bash
   ./build.sh
   ```

---

## 🛠️ Ferramentas de Automação (`tools/eclat/`)

Todas as ferramentas criadas especificamente para este projeto estão consolidadas na pasta [`tools/eclat/`](file:///C:/Users/raulr/OneDrive/Desktop/zEmerald/eclat-intro-teste/tools/eclat/):

| Script | Função |
| :--- | :--- |
| **`import_summary_screen.py`** | Lê os PNGs de `graphics/summary_screen/sources/`, otimiza os blocos 8x8 (com detecção de espelhamento horizontal/vertical) e gera automaticamente o tileset `tiles.png` e os arquivos `.bin` de cada aba. |
| **`build_peaceful_woods_grim.py`** | Constrói e injeta os metatiles avançados da floresta *Peaceful Woods* / *Grim Woods*. |
| **`grim_woods_anim.py`** | Gerencia as animações de ambiente (rio, folhagens e grama alta). |
| **`grim_woods_exits_and_bg.py`** | Configura transições de saídas e fundos de batalha das florestas. |
| **`porymap_iviere_compat.py`** | Garante compatibilidade dos blocos e mapas de Iviere com o editor Porymap. |
| **`sync_petipres.py`** | Sincroniza metatiles e tilesets da cidade de Petipres. |

---

## 📖 Documentação de Design (`eclat docs/`)

A pasta [`eclat docs/`](file:///C:/Users/raulr/OneDrive/Desktop/zEmerald/eclat-intro-teste/eclat%20docs/) armazena o histórico do projeto, notas de desenvolvimento e design de fases:
- **`historia.md`**: Enredo, ambientação da região e planejamento de narrativa.
- **`historico.md`**: Registro cronológico de alterações e decisões tomadas no código.
- **`route2.md` / `route3.md` / `peaceful-woods.md`**: Especificações de level design de cada rota.
- **`quest-indicators.md`**: Documentação do sistema de marcadores de missões.

---

## 🔒 Boas Práticas de Versionamento (Git/GitHub)

Para manter o repositório organizado e fácil de versionar:
1. **Raiz Limpa**: Nunca deixe scripts de teste temporários soltos na raiz. Qualquer nova ferramenta de apoio deve ser criada dentro de `tools/eclat/`.
2. **Imagens Fontes Versionadas**: Sempre mantenha os PNGs mestres em `graphics/summary_screen/sources/` comitados junto com o projeto. Assim, qualquer membro da equipe pode alterar os gráficos e reexecutar a importação sem perdas.
3. **Build Replicável**: Qualquer modificação em código C ou gráficos deve compilar perfeitamente através de `./build.sh` com 0 erros antes de ser comitada.
