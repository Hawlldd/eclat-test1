# Documentação Técnica — Pokémon Summary Screen (Gen 4 Saffron Layout)
**Projeto:** Pokémon Éclat Pourpre (base `pokeemerald-expansion` v1.17.2)  
**Autor / Engenharia:** Equipe Éclat & Antigravity  
**Data:** 10/11/2026  

---

## 📌 1. Visão Geral e Objetivos

Este documento descreve toda a engenharia, arquitetura gráfica e implementação em C da nova **Tela de Resumo do Pokémon (Summary Screen)** de *Pokémon Éclat Pourpre*.

O design é inspirado na interface elegante e moderna da **Gen 4 (Diamond / Pearl / Platinum / HeartGold & SoulSilver)**, adaptada e aprimorada a partir do padrão de **Pokémon Saffron** e **CitrusBolt**.

### Principais Metas Atingidas:
1. **Estrutura Enxuta de 3 Abas**: Remoção definitiva da 4ª aba obsoleta de Contest Moves. O ciclo de navegação é travado estritamente em:
   - **Aba 1 (`PSS_PAGE_INFO`)**: Informações Gerais, Pílulas de Dados e Trainer Memo.
   - **Aba 2 (`PSS_PAGE_SKILLS`)**: Habilidade, Status Numéricos e Sistema de Notas de IV (S/A/B/C/D/E).
   - **Aba 3 (`PSS_PAGE_BATTLE_MOVES`)**: Golpes de Batalha, Badges de Tipo Saffron e PP.
2. **Transição Instantânea e Atômica (Zero Glitches / Sem "Esqueleto")**: Substituição do antigo efeito de scroll deslizante do Emerald vanilla por uma troca instantânea em um único frame V-Blank, eliminando qualquer cintilação ou resquício de tilemap.
3. **Pipeline de Extração e Quantização Gráfica Automatizado**: Script Python proprietário (`tools/eclat/import_summary_screen.py`) que converte imagens mestras PNG (240x160 px) em tilesets 4bpp otimizados com flips e tilemaps binários compactos.
4. **Correção de Cobertura do Retrato do Pokémon**: Área inferior esquerda sob o sprite do Pokémon perfeitamente azul sólida na aba de golpes, sem invasão visual de caixas de outras abas.

---

## 📂 2. Arquivos Envolvidos

| Arquivo | Descrição e Responsabilidade |
| :--- | :--- |
| [`src/pokemon_summary_screen.c`](file:///C:/Users/raulr/OneDrive/Desktop/zEmerald/eclat-intro-teste/src/pokemon_summary_screen.c) | Toda a lógica em C: janelas de texto, troca de abas, formatação de IVs, cálculo de PP e renderização de sprites. |
| [`tools/eclat/import_summary_screen.py`](file:///C:/Users/raulr/OneDrive/Desktop/zEmerald/eclat-intro-teste/tools/eclat/import_summary_screen.py) | Script de automação que processa os PNGs das abas e gera tileset e tilemaps. |
| `graphics/summary_screen/sources/` | Imagens mestras em PNG 240x160 px: `page_info.png`, `page_skills.png` e `page_battle_moves.png`. |
| `graphics/summary_screen/tiles.png` | Tileset mestre indexado de 4bpp (128px de largura) compartilhado entre as abas. |
| `graphics/summary_screen/page_*.bin` | Tilemaps binários 32x32 em formato little-endian de 16 bits para cada página. |
| `graphics/types/` | Sprites OAM das badges de tipos redesenhadas com ícones temáticos e siglas (ex: FIR, DRG, WAT, NRM). |

---

## 📐 3. Anatomia Detalhada das 3 Abas

### 🏷️ Aba 1: Info & Trainer Memo (`page_info.png`)
- **Painel Superior Esquerdo**: Nível do Pokémon (`LvXX`), Nome da Espécie e Ícone de Gênero (♂ / ♀).
- **Moldura do Retrato**: Retrato retangular com scanlines cinzas suaves onde o sprite do monstrinho e sua Pokébola são exibidos.
- **Painel Direito (Pílulas Arredondadas)**:
  - `No.` (Número da Pokédex)
  - `Name` (Apelido ou Nome)
  - `Type` (Badges de Tipo 1 e Tipo 2 lado a lado)
  - `OT` (Nome do Treinador Original colorido por gênero: azul para masculino, rosa para feminino)
  - `IDNo.` (ID do Treinador formatado em 5 dígitos)
  - `Item` (Nome do item segurado ou `"None"`)
- **Painel Inferior (Trainer Memo)**:
  - Exibe Natureza (com cores destacadas se afetar atributos), local de encontro/captura, nível em que foi obtido e personalidade.
  - Tipografia: Utiliza a fonte compacta e nítida `FONT_SHORT` para evitar estouro de texto e eliminar sombras pretas pesadas.

---

### 📊 Aba 2: Habilidades, Stats & IV Grades (`page_skills.png`)
- **Painel Esquerdo (Ability Box)**:
  - Cabeçalho cinza `"ABILITY"` acima de um box delimitado.
  - Nome da habilidade impresso em negrito e descrição de 2 a 3 linhas auto-quebrada e perfeitamente ajustada.
- **Painel Direito (Tabela de Atributos)**:
  - Colunas:
    1. **Nome do Atributo**: `HP`, `ATTACK`, `DEFENSE`, `SP. ATK`, `SP. DEF`, `SPEED`.
    2. **Nota de IV (Saffron Letter Grade)**: Letras com classificação visual de potencial genético:
       - **S** (31 IVs): Roxo (`#A541A5`)
       - **A** (30–26 IVs): Verde (`#7BCE52`)
       - **B / B+** (25–16 IVs): Azul (`#6B96F7`)
       - **C / C+** (15–8 IVs): Amarelo (`#DEE731`)
       - **D** (7–1 IVs): Laranja (`#F78631`)
       - **E** (0 IVs): Vermelho (`#EF3100`)
    3. **Valor Numérico**: Valor real do stat atual, colorido de acordo com a Natureza:
       - **Vermelho (`#EF3100`)**: Stat amplificado pela Natureza (+10%).
       - **Azul (`#6B96F7`)**: Stat reduzido pela Natureza (-10%).
       - **Escuro padrão (`#424142`)**: Stat neutro ou HP.
  - **Experiência**: Box inferior com pontos de experiência acumulados e pontos restantes para o próximo nível.

---

### ⚔️ Aba 3: Battle Moves (`page_battle_moves.png`)
- **4 Slots Verticais de Golpes**:
  - Cada slot possui altura exata de **28 pixels** (`i * 28`).
  - **Coordenadas de Renderização**:
    ```c
    u8 yName = 5 + moveIndex * 28;
    u8 yPP   = 16 + moveIndex * 28;
    ```
  - **Badges de Tipo Saffron**:
    - Sprite OAM posicionado em `X = 123`, `Y = 21 + (i * 28)`.
    - Apresenta ícone circular característico do tipo + sigla de 3 caracteres (ex: `FIR` para Fogo, `WAT` para Água, `DRG` para Dragão).
  - **Nome e PP**:
    - Nome do golpe impresso com `PrintTextOnWindowToFitPx` (largura máxima de 74px com compressão automática caso ultrapasse).
    - PP impresso no formato `{PP} XX/YY`, com alinhamento à direita finalizando em `window X = 76`.
- **Correção da Área sob o Pokémon (Blue Background Fix)**:
  - Na aba de golpes não há caixa de habilidades nem notas embaixo do Pokémon.
  - O quadrante inferior esquerdo (`X: 0..120, Y: 96..160`) foi uniformizado com a cor azul de fundo oficial (`#528ED6`), garantindo que nenhum fragmento da caixa de habilidades apareça ao trocar para esta aba.
- **Tela de Aprendizado e Seleção de Golpes (`SUMMARY_MODE_SELECT_MOVE`)**:
  - **Eliminação da Barra Glitchada Central**: O Emerald vanilla invocava `TilemapFiveMovesDisplay` sobrescrevendo o tilemap no offset `0x56A` com índices de tiles obsoletos (gerando o bloco com Pokébola e listras pretas/roxas no meio da tela). A função foi neutralizada para manter o fundo limpo.
  - **5º Slot de Golpe / Golpe Novo a Aprender**:
    - O badge de tipo Saffron do novo golpe agora é posicionado em `(123, 133)` (`Y = 21 + 4 * 28`), perfeitamente alinhado com o nome e PP do 5º slot.
  - **Caixa de Seleção OAM 16x32 Ajustada**:
    - Sprites redesenhados de 16x16 para 16x32 (`SPRITE_SHAPE(16x32)` / `SPRITE_SIZE(16x32)`), garantindo que a borda vermelha e amarela envolva perfeitamente os 24px de altura de cada card (`Y = 21..46`, `49..74`, etc.).
    - Composta por 8 segmentos horizontais com cantos arredondados cobrindo `X = 121..237`, sem invadir os números de PP.
  - **Painel de Detalhes Inferior Esquerdo**:
    - `PSS_LABEL_WINDOW_MOVES_POWER_ACC`: Janela expandida para altura 4 (32px), exibindo `POWER` (linha 1, Y=97) e `ACCURACY` (linha 2, Y=113) de forma nítida e sem cortes.
    - Ícone de Categoria (Físico / Especial / Status): Reposicionado para `(98, 112)` ao lado de Poder e Acurácia, sem mais sobrepor o texto de descrição do golpe.
    - `PSS_DATA_WINDOW_MOVE_DESCRIPTION`: Alocada em `tilemapTop = 16` (`Y = 128..160`), reservando as linhas inferiores inteiramente para a descrição do golpe.

---

## ⚡ 4. Transição Instantânea sem Glitches (Atomic Page Switching)

### O Problema do Emerald Vanilla:
No código original do Emerald, a troca de abas utilizava tarefas assíncronas com scroll de BG (`PssScrollRight`, `PssScrollLeft`) que deslocavam as coordenadas de `BG1` e `BG2` gradativamente. Isso causava:
1. O "esqueleto" do fundo antigo visível por alguns frames enquanto o novo era carregado.
2. Rasgos visuais (*tearing*) e caixas pretas vazias durante o deslocamento de coordenadas.
3. Atraso perceptual de vários frames entre o toque no D-Pad e a exibição dos dados.

### A Solução Atômica Implementada:
Em `src/pokemon_summary_screen.c`, a função `ChangePage` foi reescrita para realizar todas as atualizações no mesmo frame V-Blank:

```c
static void ChangePage(u8 taskId, s8 delta)
{
    struct PokeSummary *summary = &sMonSummaryScreen->summary;
    u32 currPageIndex;

    if (summary->isEgg)
        return;
    else if (delta == -1 && sMonSummaryScreen->currPageIndex <= sMonSummaryScreen->minPageIndex)
        return;
    else if (delta == 1 && sMonSummaryScreen->currPageIndex >= sMonSummaryScreen->maxPageIndex)
        return;

    PlaySE(SE_SELECT);
    ClearPageWindowTilemaps(sMonSummaryScreen->currPageIndex);
    currPageIndex = sMonSummaryScreen->currPageIndex += delta;

    HidePageSpecificSprites();

    if (currPageIndex == PSS_PAGE_SKILLS || (currPageIndex + delta) == PSS_PAGE_SKILLS)
    {
        struct Pokemon *mon = &sMonSummaryScreen->currentMon;
        if (sMonSummaryScreen->skillsPageMode != SUMMARY_SKILLS_MODE_STATS)
            sMonSummaryScreen->skillsPageMode = SUMMARY_SKILLS_MODE_STATS;

        ShowUtilityPrompt(sMonSummaryScreen->skillsPageMode);
        ExtractMonSkillStatsData(mon, summary);
        BufferLeftColumnStats();
        BufferRightColumnStats();
    }
    else
    {
        ShowUtilityPrompt(SUMMARY_MODE_NORMAL);
    }

    // 1. Troca instantânea do buffer de tilemap no BG1
    SetBgAttribute(1, BG_ATTR_PRIORITY, 1);
    ChangeBgX(1, 0, BG_COORD_SET);
    ChangeBgY(1, 0, BG_COORD_SET);
    SetBgTilemapBuffer(1, sMonSummaryScreen->bgTilemapBuffers[currPageIndex][0]);
    ScheduleBgCopyTilemapToVram(1);

    // 2. Atualização imediata dos textos, janelas e ícones
    PrintPageSpecificText(currPageIndex);
    PutPageWindowTilemaps(currPageIndex);
    SetTypeIcons();
    TryDrawExperienceProgressBar();
    ScheduleBgCopyTilemapToVram(0);
}
```

### Remoção da 4ª Aba:
Para restringir estritamente a 3 abas, `maxPageIndex` foi travado:
```c
u32 maxPageIndex = PSS_PAGE_BATTLE_MOVES; // Limita estritamente a 3 páginas (Info, Skills, Moves)
```
Isso impede que a navegação com o direcional direito ultrapasse a tela de golpes ou tente renderizar a aba de Contest.

---

## 🎨 5. Pipeline Gráfico & Script de Automação

O script [`tools/eclat/import_summary_screen.py`](file:///C:/Users/raulr/OneDrive/Desktop/zEmerald/eclat-intro-teste/tools/eclat/import_summary_screen.py) automatiza a conversão dos PNGs para o hardware do GBA.

### Estrutura das Paletas de Cores (8 sub-paletas de BG):
1. **Paleta 0 (Fundo Oficial Saffron)**:
   - Cores base: Branco, Cinza Claro, Azul Aço Principal (`#528ED6`), Azul Cabeçalho (`#4A79A5`), Cinza Escuro de preenchimento e destaques.
2. **Paletas 1 a 5**:
   - Paletas dinâmicas do motor para barras de progresso, ícones de status e marcações.
3. **Paleta 6 (Text Windows & IV Grades)**:
   - Configurada especialmente para tipografia nítida (sem borda preta grossa) e para abrigar as 6 cores do sistema de notas de IV (E, D, C, B, A, S) e os indicadores de Natureza (+ / -).
4. **Paleta 7 (Prompts e Comandos)**:
   - Prompts de botões A/B e mensagens de ajuda.

### Otimização de VRAM (Tile deduplication & Flips):
- A tela do GBA tem 240x160 pixels = 30 colunas x 20 linhas = 600 tiles teóricos por tela.
- O limite de VRAM para um CharBlock de Background é de **576 tiles** de 8x8 pixels em 4bpp.
- O script realiza:
  1. Detecção de tiles idênticos entre todas as 3 abas.
  2. Detecção de tiles espelhados horizontalmente (`flip_h`), verticalmente (`flip_v`) ou ambos (`flip_hv`), ativando os bits 10 e 11 na entrada do tilemap de 16 bits.
  3. Resultado: As 3 abas somadas consom **menos de 300 tiles únicos**, cabendo com folga em um único CharBlock!

### Como executar o importador após alterar os PNGs:
```bash
python3 tools/eclat/import_summary_screen.py
```
O script gera os arquivos prontos em `graphics/summary_screen/`:
- `tiles.png`
- `page_info.bin`
- `page_info_egg.bin`
- `page_skills.bin`
- `page_battle_moves.bin`

---

## 🛠️ 6. Como Replicar Esta Engenharia em Outro Projeto

Caso deseje implementar este mesmo sistema em outra hackrom baseada em pokeemerald:

1. **Copiar o Script de Automação**:
   - Copie `tools/eclat/import_summary_screen.py` para a sua pasta de ferramentas.
2. **Exportar / Editar os PNGs Mestres**:
   - Crie imagens de 240x160 px em `graphics/summary_screen/sources/`:
     - `page_info.png`
     - `page_skills.png`
     - `page_battle_moves.png`
   - Certifique-se de que a paleta de cores respeite a lista de 16 cores de `OFFICIAL_PALETTE`.
3. **Gerar os Gráficos**:
   - Execute o script Python para compilar os `.bin` e o `tiles.png`.
4. **Ajustar `src/pokemon_summary_screen.c`**:
   - **Remover abas excedentes**: Altere `maxPageIndex` para `PSS_PAGE_BATTLE_MOVES`.
   - **Aplicar Transição Atômica**: Substitua o corpo de `ChangePage` pelo modelo sem tarefas de scroll.
   - **Ajustar Janelas de Texto**: Atualize as structs `sPageInfoTemplate`, `sPageSkillsTemplate` e `sPageMovesTemplate` para as posições X/Y correspondentes às suas caixas gráficas.
   - **Atualizar Coordenadas dos Golpes**: Defina `yName = 5 + i * 28` e `yPP = 16 + i * 28`.
   - **Carregar Paleta de Texto Nitida**: Certifique-se de que `sSkillsTextPalette` seja carregada no `BG_PLTT_ID(6)`.
5. **Compilar e Testar**:
   - Execute `./build.sh` (ou `make -j$(nproc)`).
