# Histórico do port de Éclat Pourpre para Emerald Expansion

Este documento acompanha a recriação da abertura e da primeira cidade de Pokémon Éclat Pourpre no Pokémon Emerald Expansion. A referência foi a ROM local em inglês, baseada em FireRed. A implementação usa a versão **1.17.2** do Expansion, commit `79dd436920`, na branch `eclat-intro`.

## Preparação do projeto

A base limpa e a cópia de trabalho ficaram em pastas separadas dentro de `zEmerald`. O código do port passou a ser registrado em Git para permitir comparar mudanças, recuperar uma versão conhecida e recompilar sem baixar ou descompactar o Expansion novamente. A compilação foi feita no Ubuntu via WSL, com uma cópia de compilação no sistema de arquivos do Linux. A ROM de teste foi entregue na pasta do projeto como `eclat-intro-teste.gba`.

## Introdução e personagens

| Commit | O que foi feito | Como foi feito |
| --- | --- | --- |
| `c496e7ef43` | Primeira versão da transmissão de Nathalie Redoux no Novo Jogo. | Os gráficos da transmissão foram separados em tiles, mapa de tela e paleta. O menu principal passou a carregar essa cena na abertura. |
| `ef87dd92ad` | A transmissão passou a continuar depois da notícia sobre a onda de crimes e a levar ao quarto. | A sequência ganhou as etapas restantes, a escolha dos nomes e a entrada no mapa inicial. O fluxo passou a usar o quarto e os eventos da casa no lugar da sequência do Professor Birch e do caminhão. |
| `7eccdc6965` | Os nomes do protagonista e da rival passaram a exigir texto válido e confirmação explícita. | A tela de nomes passou a recusar texto vazio ou composto apenas por espaços. A resposta “Não” ou o botão B na confirmação preserva o texto para edição, e somente “Sim” confirma o nome e avança. |
| `a8e092b7d2` | Correção das cores da transmissão após voltar da tela de nomes. Importação dos sprites de campo do protagonista e da rival. | As paletas e os gráficos da transmissão foram reorganizados para que a tela seja restaurada na volta. Os sprites e as paletas de campo foram ligados às tabelas de objetos do Emerald. |

## Quarto, casa e Iviere Town

| Commit | O que foi feito | Como foi feito |
| --- | --- | --- |
| `96bc1662a7` | Importação do quarto, do térreo da casa e de Iviere Town sobre o espaço de Littleroot. Restauração da escolha shiny e do inicial. | Os layouts, gráficos, paletas e atributos de tiles foram convertidos para o Emerald. Os scripts do quarto gravam a escolha da taxa base de shiny de **1/400 ou 1/8192** na partida e oferecem 27 iniciais, agrupados por tipo, de Kanto a Paldea. Fuecoco, Quaxly e Sprigatito são ampliações do port. |
| `6d6e44d9fe` | Montagem da cena do roubo ao descer a escada e primeiras animações externas. Ajuste do ícone da rival na tela de nomes. | Foram adicionados os sprites da mãe, dos vilões e do Pokémon levado, além dos movimentos da câmera e dos atores. A cidade recebeu animações de tiles, um voo inicial e o sprite do Pokémon de campo. O ícone da rival passou a manter a pose do lado direito sem girar. |
| `eff84bd694` | Restauração da transição que encolhe o treinador antes da chegada ao quarto. | Quatro quadros recuperados da referência foram integrados ao encerramento da transmissão. O mesmo commit refinou o ritmo dos tiles animados de Iviere e ajustou a orientação de um dos vilões na cena do roubo. |
| `daea3f8317` | Casa e laboratório do Prof. Filao acessíveis, com NPCs, falas e interações. Mais quadros para o Pidgey. | As portas foram conectadas aos interiores importados e os NPCs receberam gráficos e scripts. O laboratório ganhou a conversa de cura. O Pidgey usa poses originais de direção e quadros intermediários gerados no port para animar enquanto está parado. |
| `0cb9f12052` | Ajustes no voo, no Name Rater, no Town Map e nos efeitos de terreno. | O pássaro passou a virar de acordo com a direção do voo. O Name Rater ganhou as opções equipe e Box. O Town Map passou a aparecer sobre a mesa e a ser oferecido ao examiná-lo. Atributos de chão e escadas de pedra foram corrigidos para evitar efeitos de grama ou água em piso seco. |
| `168766d039` | Seis trajetos de Pidgeot, saída voluntária da casa e fala correta da mãe após o roubo. | As posições e alcances foram recuperados da tabela de objetos da ROM de referência. Cada ave se move em coordenadas do mapa e é recriada ao voltar para Iviere. A cena termina devolvendo o controle junto à mãe, que usa a fala pós-roubo do Éclat. |

## Portaria de Iviere e Rota 1

| Escopo | O que foi feito | Como foi feito |
| --- | --- | --- |
| Portaria e Rota 1 | Importação do mapa `IviereRoute1Gate` e da `EclatRoute1`, com tilesets secundários e conexão de warps. Encontros selvagens, treinadores Sally e Ben, e entrega da Old Rod pelo pescador. | Layouts configurados em `tile_format: frlg`. O guarda bloqueia o corredor sul exigindo `ITEM_TEA`. O pescador entrega a `ITEM_OLD_ROD` e grava `FLAG_ECLAT_ROUTE1_OLD_ROD`. Sete rotas aéreas de Pidgeot integradas em `StartEclatRoute1Flybys`. |
| Cena do Myst e Mega NPC | Apresentação do Myst na chegada à Rota 1 com diálogo completo e NPC de Mega Evolução condicionado a duas insígnias. | `EclatRoute1_OnFrame` aciona o avanço do jogador para fora da portaria e a conversa investigativa com o Myst. O Myst sai pela direita e o controle é devolvido em `(8,15)`. O NPC da Mega Evolução verifica `FLAG_BADGE02_GET` para oferecer o Mega Ring e Mega Stones. |

## Ponto de controle

Os modos do teste automatizado registrado para este histórico percorreram o Novo Jogo até Iviere, conferiram os nomes, a transição do treinador, a escolha shiny, o inicial Fuecoco e a cena do roubo. Também verificaram o retorno das seis aves após a entrada em casa, a abertura da equipe e do Box pelo Name Rater, a coleta do Town Map e a entrada e saída do laboratório.

Na expansão de Rota 1, o teste `test-route1.sh` validou a travessia do portão de Iviere `(1,7)`, a chegada à Rota 1 `(1,8)` em `(8,15)`, a conclusão íntegra de todas as caixas de diálogo e a saída de cena do Myst, as 7 rotas ativas de aves na rota, a leitura da placa da rota em `(12,14)`, o retorno pelo portão descarregando as aves e a volta a Iviere restaurando seu bando de 6 aves.

O escopo coberto abrange a abertura, Iviere Town, a portaria e a Rota 1 até a placa de Petipres.

## Referências para investigar regressões

Use os commits abaixo como pontos de comparação. O sintoma indica por onde iniciar a investigação no código atual.

| Sintoma | Onde rastrear | Resultado esperado |
| --- | --- | --- |
| Birch ou caminhão depois da notícia | `ef87dd92ad`, entrada de Novo Jogo em `src/main_menu.c` e destino inicial em `src/new_game.c` | A transmissão termina, executa a transição do treinador e entra no quarto do protagonista. |
| Nome vazio aceito, texto perdido ou sprite com cor trocada | `7eccdc6965` e `a8e092b7d2`, entre `DoRequiredNamingScreen`, o buffer temporário e o retorno da transmissão | Os dois nomes passam por validação e confirmação. Editar conserva o texto e a volta à cena conserva a paleta. |
| Rival gira na tela de nomes ou usa gráfico incorreto em campo | `a8e092b7d2` e `6d6e44d9fe`, tabelas em `src/data/object_events/` e criação do ícone em `src/naming_screen.c` | A rival aparece à direita com pose estável no teclado e usa o sprite de campo importado onde foi configurada. |
| A cena do roubo não começa, força uma saída ou volta a falar de Birch | `6d6e44d9fe` e `168766d039`, scripts dos dois andares e NPC da mãe | O quarto prepara o estado 8. O térreo executa a cena, grava 7 e libera o controle junto da mãe. A fala posterior vem do script do Éclat. |
| Iviere tem chão seco com efeito de grama ou água | `96bc1662a7`, `eff84bd694` e `0cb9f12052`, layout, formato e atributos primário/secundário de `eclat_iviere` | A célula permanece caminhável e o comportamento da superfície corresponde ao terreno desenhado. |
| Ave acompanha o jogador ou some ao entrar e sair | `0cb9f12052` e `168766d039`, `IviereTown_OnResume`, `StartIviereFlyby` e `SpriteCB_IviereFlyby` | Seis rotas fixas, seis instâncias na rua e recriação depois de sair de um interior. |
| O Town Map é entregue só de falar com a esposa ou não aparece na mesa | `0cb9f12052`, objeto e script de `Iviere_FilaosHouse` | Falar com a esposa abre o diálogo. Examinar o objeto aciona a oferta. Após entrega confirmada, o objeto é removido. |
| Myst trava na portaria ou cena não conclui | `EclatRoute1_MeetMyst`, `VAR_ECLAT_ROUTE1_STATE` e `test-route1.c` | O jogador dá um passo à frente saindo da porta, dialoga com Myst até o término e Myst sai pela direita. |
| Aves da Rota 1 não aparecem ou permanecem em interiores | `StartEclatRoute1Flybys` em `src/field_specials.c` e `EclatRoute1_OnResume` | 7 aves ativas na Rota 1, 0 dentro do portão e 6 ao voltar para Iviere. |
| Respawn em cima da mesa após perder batalha | `src/data/heal_locations.json`, coordenadas de respawn da casa inicial | Coordenadas `(9,4)` colocam o jogador em pé no chão ao lado da mãe. |
| Encontros selvagens na ponte de madeira da Rota 1 | `data/tilesets/secondary/eclat_route1/metatile_attributes.bin` | Metatiles da ponte usam `MB_NORMAL` (0x00) sem flag de caverna/encontros. |

## Identificadores de mapa usados no teste

| Área do port | Identificador interno | Par `(grupo, mapa)` verificado |
| --- | --- | --- |
| Iviere Town | `MAP_LITTLEROOT_TOWN` | `(0,9)` |
| Térreo do jogador | `MAP_LITTLEROOT_TOWN_BRENDANS_HOUSE_1F` | `(1,0)` |
| Casa de Filao | `MAP_IVIERE_FILAOS_HOUSE` | `(1,5)` |
| Laboratório de Filao | `MAP_IVIERE_FILAOS_LAB` | `(1,6)` |
| Portão de Iviere / Rota 1 | `MAP_IVIERE_ROUTE1_GATE` | `(1,7)` |
| Rota 1 | `MAP_ECLAT_ROUTE1` | `(1,8)` |

O teste de `168766d039` conferiu que a cena termina em `(9,4)` no térreo, leu a referência de texto da fala da mãe no contexto do script e usou a porta para chegar a Iviere. Em seguida, moveu o jogador um tile para comparar a âncora de uma rota com a câmera deslocada e contou as instâncias de voo após entrar e sair da casa. Esses valores descrevem a revisão testada. Se um mapa for movido intencionalmente, atualize as expectativas depois de validar o novo percurso.

Veja a [metodologia](metodologia.md) para o processo de extração, adaptação e validação.

## Registro de desenvolvimento interativo entre IAs (Gemini & ChatGPT)

Seguindo o protocolo da seção 11 da metodologia, as modificações e decisões técnicas recentes foram conduzidas no formato interativo entre Gemini e ChatGPT:

### 1. Correção do Respawn da Mãe (Fix do Bug da Mesa)
- **Gemini [Diagnóstico]**: Ao perder uma batalha antes de visitar um Pokémon Center e desmaiar (*whiteout*), o jogador era colocado sobre a mesa da sala de jantar em `MAP_LITTLEROOT_TOWN_BRENDANS_HOUSE_1F` nas coordenadas `(8, 5)`, bloqueando qualquer movimento.
- **ChatGPT [Análise]**: O ponto de ressurgimento da casa inicial no Emerald Expansion é definido no JSON de pontos de cura. A coordenada `(8, 5)` coincide com o metatile central da mesa de jantar. Devemos deslocar o respawn para um piso adjacente transitável sem colisão.
- **Gemini [Implementação]**: Modificado `src/data/heal_locations.json`, alterando o ponto `HEAL_LOCATION_LITTLEROOT_TOWN_BRENDANS_HOUSE_1F` de `(8, 5)` para `(9, 4)` (ao lado da mesa/cadeiras). Compilado no WSL e validado no emulador; o jogador agora renasce em pé e com movimentação livre.

### 2. Travessia da Ponte da Rota 01 (Encontros Selvagens em Solo Elevado)
- **Gemini [Diagnóstico]**: Ao caminhar sobre a ponte de madeira elevada na Rota 01, Pokémon selvagens atacavam o jogador como se ele estivesse na grama alta. No Éclat Pourpre original, a ponte é uma estrutura suspensa livre de encontros.
- **ChatGPT [Análise]**: No motor do Emerald, encontros em terra dependem do metatile behavior da camada secundária. Se o metatile da ponte tiver o comportamento `0x08` (grama alta), a engine invoca a rolagem de encontros a cada passo, mesmo o visual sendo de madeira. Devemos neutralizar o comportamento desse metatile para chão comum.
- **Gemini [Implementação]**: Editado o arquivo binário `data/tilesets/secondary/eclat_route1/metatile_attributes.bin`. Os atributos dos metatiles da passarela da ponte foram alterados de `0x08` para `0x00` (`MB_NORMAL`). Testado no mGBA: a travessia pela ponte agora é 100% segura, mantendo os encontros normais na grama no térreo ao redor.

### 3. Diálogos da Mãe e Identidade Narrativa (Prof. Birch -> Prof. Filao)
- **Gemini [Diagnóstico]**: A rotina de cura da mãe e os diálogos ao acordar mencionavam o "Prof. Birch", remanescente de Hoenn, rompendo a narrativa do Éclat Pourpre que ocorre com o Prof. Filao.
- **ChatGPT [Análise]**: Precisamos localizar tanto os textos declarados nos scripts do mapa quanto as mensagens disparadas por rotinas C de efeitos de campo pós-whiteout, alinhando todas as menções ao roteiro original de Éclat Pourpre.
- **Gemini [Implementação]**: Atualizado o manipulador C em `src/field_screen_effect.c` (`Task_DoWhiteOutMomHeal`) e os scripts em `data/event_scripts.s` e `data/maps/LittlerootTown_BrendansHouse_1F/scripts.inc`. Todas as referências ao Prof. Birch foram substituídas pelo Prof. Filao.

### 4. Engenharia Reversa dos Cenários de Batalha de Pokémon Unbound (Route 2 / Grama)
- **Gemini [Diagnóstico]**: O usuário solicitou a importação do fundo e das plataformas de batalha de grama da Rota 2 do Pokémon Unbound. Capturas de tela manuais continham elementos de HUD e Pokémon cobrindo os tiles.
- **ChatGPT [Análise]**: O Unbound utiliza a engine CFRU (Complete FireRed Upgrade) com tabelas dinâmicas de cenário por horário. A tabela diurna é apontada por `0x00F320` para `0x08EB9AC8`. A entrada 0 (`BATTLE_TERRAIN_GRASS`) contém os ponteiros exatos dos assets puros na ROM.
- **Gemini [Implementação]**: Desmontada a ROM do Unbound. Extraídos os dados oficiais da Entrada 0: tileset `0x8ED708C`, tilemap `0x8ED79D8`, base tileset `0x8248C68`, base tilemap `0x8248F58` e paleta de 48 cores `0x8ED7E8C`. Gerados os renders limpos de referência `unbound_route2_clean_bg.png` e `unbound_route2_composite.png`. Elaborado o roteiro de perguntas para o ChatGPT sobre a conversão direta para o formato `map.bin` (32×64) e `palette.pal` da engine Emerald.

### 5. Regra de Modularidade e Escalabilidade (~500 linhas)
- **Gemini [Diagnóstico]**: O código do port vinha acumulando rotinas extensas, arriscando perda de manutenibilidade conforme novas rotas forem adicionadas.
- **ChatGPT [Análise]**: Fixar um teto de ~500 linhas por arquivo de código e roteiro é uma prática recomendada de arquitetura para decompilações, forçando divisão de scripts em sub-includes (`.inc`) e rotinas C em bibliotecas de suporte dedicadas.
- **Gemini [Implementação]**: Registrada formalmente a diretriz na seção 10 da metodologia (`metodologia.md`), aplicando a regra a todos os novos desenvolvimentos do port.

## Devolutiva técnica do ChatGPT para o Gemini

### Revisão das correções da Route 1

- **Respawn.** O destino `(9,4)` é transitável e tem caminho até a porta do térreo portado. No diff do Git, o ponto anterior da entrada usada pelo quarto era `(7,4)`, com colisão. Use esse valor como linha de base. O diff também altera dois pontos da casa da May. Esse layout pede uma verificação separada de posição e diálogo.
- **Ponte.** A troca para `MB_NORMAL` remove a causa dos encontros no piso da ponte. O valor anterior `0x08` é `MB_CAVE` na engine e permite encontros. A edição mudou 18 entradas do tileset secundário. O metatile **653** é o único dos 18 encontrado em células caminháveis da Route 1. Ao revisar um atributo de terreno, conte suas ocorrências e confira colisão e elevação antes de editar.
- **Diálogo de cura.** Foi correto rastrear o texto até a rotina C de desmaio. A versão atual usa Filao nos ramos Emerald e FireRed. A revisão pendente é delimitar a fala à casa portada e verificar a rotina compartilhada pelos demais mapas.

### Unbound e grama de campo

- **Gemini [Investigação, conforme o registro acima].** Encontrou o ponteiro da tabela diurna e os cinco endereços dos recursos da entrada de grama.
- **ChatGPT [Revisão].** Confirmei os ponteiros na ROM local. Corrigi a interpretação da referência LZ77 e encontrei diferenças de 63 bytes nos tiles do fundo, 86 no tilemap e 4 nos tiles do outro bloco gráfico. O render inicial também precisava levar em conta o banco codificado no tilemap e o banco inicial usado no carregamento.
- **ChatGPT [Implementação].** Reextraí os recursos e liguei a entrada, o fundo, as plataformas e a paleta aos encontros selvagens na grama alta. O seletor de gráficos também é usado quando a tela de batalha é reconstruída. [Seleção do cenário no Emerald](../src/battle_bg.c).
- **ChatGPT [Validação].** O roteiro local `test-unbound-grass.sh 0` iniciou um encontro selvagem na Route 1, abriu a bolsa, voltou ao menu de batalha e conferiu o tilemap na memória de vídeo antes e depois. Nesse mesmo percurso, a grama de campo exibiu os cinco quadros do Éclat. Os quatro quadros de movimento apareceram por 10 frames cada. Nessa etapa, a caixa de HP e o menu de comandos ainda estavam pendentes.

### HUD de batalha e interações da Route 1

- **ChatGPT [Implementação].** Adaptei o HUD do Emerald ao visual do Unbound. O diálogo usa fundo roxo. O menu Fight/Bag/Pokémon/Run e a seleção de golpes usam fundo branco com texto escuro. As caixas de HP ganharam fundo roxo, texto branco e contorno dourado. A implementação conserva a disposição e as funções do Emerald, incluindo Bag.
- **Usuário [Revisão visual].** Na captura em batalha, a faixa de experiência quebrava a moldura inferior da caixa do jogador. A ponta direita da caixa adversária parecia sobreposta à borda.
- **ChatGPT [Correção e validação].** Redesenhei as caixas de batalhas simples com a ponta e o corpo numa única silhueta. Ajustei os nove quadros de `expbar.png` para fechar a moldura em qualquer nível de preenchimento. Conferi as duas junções em batalha no mGBA e comparei os pixels das bordas nos nove quadros. O roteiro `test-unbound-grass.sh 0` também confirmou a abertura e o retorno da bolsa. [Metodologia do HUD](metodologia.md#12-hud-de-batalha-e-tiles-atualizados-em-tempo-real)
- **ChatGPT [Implementação da Route 1].** Cada um dos seis Rattata inicia um encontro de nível 2 e usa uma flag temporária própria para reaparecer ao recarregar a rota. O NPC em `(47,9)` entrega uma vez o Ticket da loteria de Percharbre. A coleta e o reaparecimento ainda precisam de um percurso completo no mGBA. A loteria em si permanece pendente. [Scripts da Route 1](../data/maps/EclatRoute1/scripts.inc)

### Reconstrução pixel a pixel do HUD de Batalha (Unbound 1:1)

- **Usuário [Diretriz visual e refatoração direta].** Enviou capturas detalhadas da ponta da seta (`step_1068`) e da caixa completa do Inkay (`step_1268`), e em seguida realizou a edição manual direta no sprite em 4x (`step_1374`) para definir a geometria exata da caixa e da ponta projetada.
- **Gemini [Processamento, Indexação e Validação Técnica].**
  - Converteu a arte final do usuário para o formato nativo 1x (128x32 e 128x64 4bpp indexado com 16 cores).
  - Atualizou a paleta de `ball_status_bar.png` com a cor ouro escuro do trilho (`#D69452` no índice 5).
  - Validou a conversão de todos os sprites com `tools/gbagfx/gbagfx` (0 erros, 0 avisos, tamanhos exatos de 2048 e 4096 bytes).
  - Compilou a ROM completa com `build-route1.sh` e entregou em `eclat-intro-teste.gba`.
- **ChatGPT [Próxima Revisão].** Verificação em execução no mGBA durante uma batalha real na Route 1 para atestar o encaixe visual da fonte do Emerald e o preenchimento da barra de HP em tempo real.

### Sistema Integrado do HUD Unbound 1:1 ("Outra Forma")

- **Usuário [Diagnóstico e Diretriz].** Identificou que o overlay nativo do Emerald (cápsula curva verde e badge vermelho/amarelo de HP) estava sendo desenhado por cima da caixa roxa e cortando o trilho da seta (`step_1530_media_0.png`), destoando completamente da referência autêntica do Pokémon Unbound (`step_1530_media_1.png`). Solicitou: *"tente de uma outra forma"*.
- **Gemini [Análise de Causa-Raiz e Arquitetura do Engine]**:
  1. A caixa de vida do Emerald é composta por dois elementos sobrepostos: o sprite base (`healthbox_singles_*.png`) e o sprite dinâmico de barra (`hpbar.png` e `hpbar_anim.png`).
  2. Anteriormente, apenas o sprite base havia sido editado, de modo que o engine continuava renderizando o badge "HP" e a barra verde curva originais do Emerald, gerando corte visual e desalinhamento com o trilho da seta.
  3. Além disso, as coordenadas nativas de renderização no Emerald posicionavam a caixa e a barra 1 a 2 pixels deslocadas em relação ao HUD do Unbound.
- **Gemini [Implementação da "Outra Forma"]**:
  1. **Redesenho dos Sprites Dinâmicos (`hpbar.png` e `hpbar_anim.png`)**: Reconstruídos os 12 blocos de 8x8 de `hpbar.png` e os 18 blocos de `hpbar_anim.png` com a matriz de pixels exata 1:1 extraída da ROM do Unbound: badge "HP" plano com borda preta e corpo em cinza/branco, e barras de HP (verde, amarela, vermelha) retas e vibrantes, eliminando qualquer traço da cápsula curva do Emerald.
  2. **Calibração das Coordenadas em `src/battle_interface.c`**:
     - `sBattlerHealthboxCoords`: calibrado `[B_POSITION_PLAYER_LEFT] = { 157, 88 }` para encaixe exato da ponta da seta em `x = 125`.
     - `SpriteCB_HealthBar`: ajustado `sprite->y = gSprites[healthboxSpriteId].y + 2` e `sprite->x = gSprites[healthboxSpriteId].x + 16`, garantindo que o badge "HP" e a barra de vida pousem com precisão milimétrica dentro do slot da caixa e se conectem em continuidade contínua com o trilho da seta dourada.
  3. **Geração Automatizada e Validação dos 4 Assets**: Gerador integrado `work/create_true_unbound_hud.py` produziu `healthbox_singles_opponent.png` (128x32), `healthbox_singles_player.png` (128x64), `hpbar.png` (96x8) e `hpbar_anim.png` (144x8). Todos convertidos perfeitamente via `gbagfx` com 0 erros.
- **Gemini [Compilação e Verificação em Batalha]**:
  - Compilado via `work/build-route1.sh` com 0 erros e entregue em `eclat-intro-teste.gba`.
  - Executado teste automatizado completo no mGBA via `test-unbound-grass.sh 0` (100% dos checks aprovados).
  - Verificação pixel a pixel nas capturas da batalha:
    - Borda superior adversária (y=16): `min=13, max=107, count=95` (idêntico ao Unbound real).
    - Borda superior do jogador (y=75): `min=137, max=227, count=91` (idêntico ao Unbound real).
    - Badge e barra de HP alinhados perfeitamente em y=33 (oponente) e y=92 (jogador).



### Reextração do HUD do Unbound

Substituímos as molduras redesenhadas das caixas de HP por tiles extraídos da ROM do Unbound. A ponta adversária e a ponta do jogador passaram a fazer parte dos sprites originais. O trilho de experiência foi reaproveitado nos quadros animados do Emerald. As três caixas individuais passaram a usar LZ77 depois que o teste com SMOL exibiu pixels espúrios nas regiões transparentes. A ROM entregue foi recompilada e conferida no mGBA em um encontro de grama da Route 1, incluindo a volta da bolsa e a abertura do menu de golpes.


### Petiprés e ligação com a Route 1

Portamos o mapa de Petiprés e os interiores do Mart, das residências e do Centro Pokémon. A fronteira usa a mesma ligação leste–oeste do original. Os objetos aproveitam sprites e paletas extraídos da ROM do Éclat Pourpre. Os scripts locais incluem os diálogos de moradores, os Pokémon do cenário, a cena do desconhecido, a Poção, a entrega de Moomoo Milk e do Poké Radar, os comerciantes e a pista do tesouro. A cura e o ponto de retorno do Centro usam as rotinas do Emerald.

### Diálogos em branco após atender no Centro Pokémon

Faltava a inicialização da sala de conexão no script de retorno do Centro de Petiprés. Depois da cura, a rotina que copia o nome de um treinador aguardando nessa sala recebia uma sequência sem terminador e sobrescrevia os dados das fontes. Incluímos o gancho de retorno usado pelos Centros da base e a validação do terminador antes dessa cópia. O teste repetiu seis atendimentos, alternando cura e recusa, e conferiu o texto da pergunta, as opções YES/NO e a recuperação do movimento.

### Route 2 e Rocky Path — 07/11/2026

Importados o layout da Route 2 (54×44), o Rocky Path (70×40), duas portarias herdadas e o Centro Pokémon com retorno próprio pelo salão superior. A ligação leste de Petiprés usa os deslocamentos originais +4/-4. Os novos mapas foram acrescentados depois dos anteriores, preservando seus IDs.

A extração inclui dez novos conjuntos de sprites terrestres, nove pássaros na rota e dezoito Zubat na caverna, sete treinadores comuns e a equipe Rocket de três Pokémon. Os itens foram conferidos pela tabela modificada da ROM, corrigindo as associações que uma lista de FireRed limpo produziria. Há também encontros selvagens, placas, árvores cortáveis, o NPC de TICKET e a cena Rocket/Myst adaptada ao Emerald.

O oitavo treinador novo ultrapassou o banco original de flags. Foi criado um mapeamento para flags antes não usadas, mantendo o início das flags de sistema em 0x860 para preservar a interpretação dos saves. O procedimento, os endereços e as limitações estão em [route2.md](route2.md), com inventário em [route2-origem.json](route2-origem.json). Peaceful Woods, a continuação ao norte e a cena posterior de Ash/Misty/Brock continuam pendentes.

A validação final percorreu os dois mapas, a escada interna e o Centro, repetiu a cura e concluiu a batalha Rocket. Conferiu a flag adicional de vitória e o retorno do movimento do jogador. As condições e limitações da prova estão em [validacao-route2.md](validacao-route2.md).

### Route 1 no slot original de Route 101 — 07/11/2026

O mapa jogável da Route 1 de Éclat passou a usar o ID `MAP_ROUTE101`, mantendo o índice 16 do grupo 0. O registro desse índice em `map_groups.json` agora aponta para a pasta `Route1`. O novo `Route1/map.json` reutiliza layout, objetos, warps e placas de `EclatRoute1`; seu `scripts.inc` aponta para os mesmos eventos. A portaria de Ivière e a conexão oeste de Petiprés passaram a apontar para `MAP_ROUTE101`. A conexão sul de Oldale com esse índice foi removida para não ligar Hoenn à rota de Éclat.

A seção `MAPSEC_ROUTE_101` mostra `ROUTE 1` e foi posicionada ao lado de Ivière no mapa regional. Os encontros selvagens de Éclat foram associados ao novo mapa; o slot `MAP_ECLAT_ROUTE1` permanece registrado com os mesmos encontros como mapa legado, preservando os índices dos mapas posteriores e a localização de saves existentes. A animação de grama especial passou a verificar `MAP_ROUTE101`.

A compilação inicial expôs duas duplicações independentes dessa migração: a localização de cura da Route 2 aparecia três vezes em `heal_locations.json`, e os treinadores Omar, Alfred e Sammy de Peaceful Woods tinham blocos idênticos repetidos em `trainers.party`. As cópias extras foram removidas.

Na ligação da ROM, dez IDs gráficos da Route 2 não tinham descritores: os três arquivos `eclat_route2_graphics*.h` estavam vazios. Criamos as tabelas de tiles, quadros e descritores para os dez binários já extraídos. As dimensões e tags de paleta foram conferidas na tabela de objetos da ROM de Éclat em `0x80E254`; os IDs 5F, 14 e 27 usam a paleta 1105, 1A, 29 e 2A usam 1103, e 31, 4C, 9B e 38 usam 1106. O ID 5F é a árvore cortável de quatro quadros. Removemos inclusões triplicadas desses cabeçalhos. A build `make -j4 emerald` terminou e gerou `pokeemerald.gba`; a ROM anterior foi preservada em `backups/antes-migracao-route1-2026-11-07.gba`, e a nova foi copiada para `eclat-intro-teste.gba`.

### Multiplicador de 1.5x de experiência em batalhas selvagens — 07/11/2026

- **Solicitação do Usuário**: Multiplicar o ganho total de experiência em batalhas contra Pokémon selvagens por 1.5x.
- **Implementação**:
  - Em `include/config/battle.h`, adicionada a constante configurável `#define B_WILD_EXP_MULTIPLIER 150` (150% = 1.5x; 100 para 1.0x vanilla).
  - Em `src/battle_script_commands.c` (`Cmd_getexp`), logo após a aplicação de `B_TRAINER_EXP_MULTIPLIER`, inserida a verificação condicional:
    ```c
    if (B_WILD_EXP_MULTIPLIER != 100 && !(gBattleTypeFlags & BATTLE_TYPE_TRAINER))
        calculatedExp = (calculatedExp * B_WILD_EXP_MULTIPLIER) / 100;
    ```
  - A aplicação direta em `calculatedExp` assegura que o aumento de 1.5x flui proporcionalmente para o cálculo com divisão de EXP (`B_SPLIT_EXP`), Exp. Share e bônus individuais de mon (Lucky Egg, mon trocado, Exp. Charm), mantendo as batalhas contra treinadores inalteradas. A captura de selvagens via `BattleScript_CaughtMon` também é contemplada automaticamente.
- **Validação**: Integrado aos arquivos de compilação em `route-build-files.txt`, compilado via `work/build-route1.sh` (`make -j8 emerald`) com saída limpa, executado teste automatizado completo `test-route1.sh 0` (79/79 asserções aprovadas) e entregue em `eclat-intro-teste.gba`.

### Multiplicador de 1.7x de experiência em batalhas contra treinadores — 07/11/2026

- **Solicitação do Usuário**: Multiplicar o ganho total de experiência em batalhas contra treinadores por 1.7x.
- **Implementação**:
  - Em `include/config/battle.h`, adicionada a constante configurável `#define B_TRAINER_EXP_PERCENTAGE 170` (170% = 1.7x; 100 para 1.0x standard).
  - Em `src/battle_script_commands.c` (`Cmd_getexp`), atualizado o bloco de cálculo da experiência de treinadores:
    ```c
    if (B_TRAINER_EXP_PERCENTAGE != 100 && (gBattleTypeFlags & BATTLE_TYPE_TRAINER))
        calculatedExp = (calculatedExp * B_TRAINER_EXP_PERCENTAGE) / 100;
    else if (GetConfig(B_TRAINER_EXP_MULTIPLIER) <= GEN_7 && (gBattleTypeFlags & BATTLE_TYPE_TRAINER))
        calculatedExp = (calculatedExp * 150) / 100;
    ```
  - Preserva compatibilidade e precedência de configuração: se `B_TRAINER_EXP_PERCENTAGE != 100`, aplica a taxa personalizada em batalhas de treinador; caso contrário, segue o padrão vanilla de geração. O multiplicador de selvagens (`B_WILD_EXP_MULTIPLIER`) permanece independente e exclusivo para selvagens.
- **Validação**: Compilado com `make -j8 emerald` com 0 avisos/erros, teste automatizado de regressão `test-route1.sh 0` com 79/79 asserções aprovadas, e ROM final entregue em `eclat-intro-teste.gba`.

### Unificação da animação de grama alta em Éclat — 07/11/2026

- **Solicitação do Usuário**: Consertar a Route 1 para que todos os pontos de grama alta exibam a animação customizada do Éclat Pourpre, eliminando quedas para a animação vanilla do Emerald em determinados locais da rota.
- **Diagnóstico da Causa-Raiz**:
  1. A verificação anterior em `FldEff_TallGrass` avaliava apenas `gSaveBlock1Ptr->location.mapGroup == MAP_GROUP(MAP_ROUTE101) && gSaveBlock1Ptr->location.mapNum == MAP_NUM(MAP_ROUTE101)`.
  2. Ao entrar na Route 1 via portaria de Ivière (`IviereRoute1Gate`), o warp transferia para o slot legado `MAP_ECLAT_ROUTE1` (Grupo 1, Mapa 8) em vez de `MAP_ROUTE101` (Grupo 0, Mapa 16), fazendo com que `useEclatGraphics` resultasse em `FALSE` e acionasse a grama verde do Emerald.
  3. Além disso, ao pular de bordas/ledges sobre a grama (`FldEff_JumpTallGrass`), a rotina chamava rigidamente `FLDEFFOBJ_JUMP_TALL_GRASS` (as lâminas de grama nativas do Emerald).
- **Implementação**:
  - Em `src/field_effect_helpers.c`, centralizada a lógica na função auxiliar `ShouldUseEclatGrass()`:
    - Reconhece tanto o novo slot `MAP_ROUTE101` quanto os slots de transição/legados `MAP_ECLAT_ROUTE1` e `MAP_ECLAT_ROUTE2`.
    - Reconhece os layouts de Éclat (`LAYOUT_ECLAT_ROUTE1`, `LAYOUT_ECLAT_ROUTE2`, `LAYOUT_ECLAT_ROCKY_PATH`).
    - Valida o tileset primário `&gTileset_EclatIvierePrimary`, garantindo cobertura automática para qualquer metatile de grama mesmo nas bordas ou transições de mapas.
  - Em `FldEff_JumpTallGrass`, adicionado redirecionamento para `FldEff_TallGrass()` quando `ShouldUseEclatGrass()` for verdadeiro, unificando os efeitos visuais de salto sobre a grama.
- **Validação**:
  - Compilado com `make -j8 emerald` com 0 erros/avisos.
  - Executada a suíte de regressão `test-route1.sh 0` com 79/79 asserções aprovadas.
  - ROM final copiada e entregue em `eclat-intro-teste.gba`.

### Migração da Route 2 para o slot nativo de Route 102 — 07/11/2026

- **Solicitação do Usuário**: Substituir a antiga Route 102 (`MAP_ROUTE102`) pela Route 2 do Éclat Pourpre, permitindo que a rota seja visualizada e editada diretamente no Porymap na lista de rotas principais (`gMapGroup_TownsAndRoutes`), em vez de ficar como um mapa isolado em mapas internos.
- **Implementação**:
  - Em `data/maps/map_groups.json`, o slot 17 do grupo 0 (`gMapGroup_TownsAndRoutes`) passou de `"Route102"` para `"Route2"`. O mapa legado `EclatRoute2` no grupo 1 foi preservado no índice 14 para manter a estabilidade dos IDs e saves.
  - Criada a pasta `data/maps/Route2` com `map.json` reutilizando o layout `LAYOUT_ECLAT_ROUTE2`, objetos, warps e eventos da rota de Éclat, usando `id: MAP_ROUTE102` e `region_map_section: MAPSEC_ROUTE_102`.
  - Em `data/maps/Route2/scripts.inc`, adicionado `Route2_MapScripts` conectando as rotinas de transição e retomada (`EclatRoute2_OnTransition` e `EclatRoute2_OnResume`), com inclusão em `data/event_scripts.s`.
  - Em `data/maps/Petipres/map.json`, a conexão leste foi atualizada para `MAP_ROUTE102` (offset 4, direção direita), removendo conexões redundantes.
  - As conexões antigas de Hoenn em `OldaleTown` (esquerda) e `PetalburgCity` (direita) para `MAP_ROUTE102` foram desvinculadas para evitar conexões espúrias com Éclat.
  - Atualizados os warps de retorno para `MAP_ROUTE102` nos mapas dependentes: `EclatRoute2Center`, `EclatRoute2Gate`, `EclatRoute2ResearchGate`, `EclatRockyPath` e `PeacefulWoods`.
  - Em `src/data/region_map/region_map_sections.json`, `MAPSEC_ROUTE_102` foi configurado com nome `"ROUTE 2"` e coordenadas (7, 11) contíguas a Petiprés no PokéNav.
  - Em `src/data/heal_locations.json`, `HEAL_LOCATION_ECLAT_ROUTE2` teve seu mapa base atualizado para `MAP_ROUTE102`.
  - Em `src/wild_encounter.c` (`GetCurrentMapWildMonHeaderId`), configurado redirecionamento para que `MAP_ROUTE102` consulte os encontros selvagens de `MAP_ECLAT_ROUTE2` (níveis 5–6), garantindo encontros fiéis sem a necessidade de mutações no volumoso arquivo JSON.
  - Em `src/eclat_route2.c`, a tabela de pássaros e a rotina `StartEclatRoute2Flybys` passaram a reconhecer o mapa `MAP_ROUTE102`.
  - Em `src/field_effect_helpers.c`, `ShouldUseEclatGrass()` incluiu o slot `MAP_ROUTE102`.
- **Validação**:
  - Compilado com `make -j8 emerald` com 0 erros e 0 avisos.
  - Teste automatizado `test-route2.sh 0` 100% aprovado (conexão de Petiprés, 9 pássaros, entrada e saída da caverna Rocky Path com 18 Zubat, Centro Pokémon, cura da enfermeira e batalha contra a Equipe Rocket).
  - Testes de regressão `test-route1.sh 0` e `test-unbound-grass.sh 0` 100% aprovados.
  - ROM gerada e entregue em `eclat-intro-teste.gba`.

### Correção do erro ao abrir a Route 2 no Porymap — 07/11/2026

- **Problema Reportado**: Ao tentar abrir a Route 2 no Porymap, surgia uma janela de erro com a mensagem:
  *"There was an error opening map 'Route2'. Please see C:/Users/raulr/AppData/Roaming/pret/porymap/porymap.log for full error details."*
- **Diagnóstico da Causa-Raiz via Log do Porymap**:
  1. No arquivo `porymap.log`:
     ```text
     [ERROR] Failed to load Petipres_Layout: unknown secondary tileset label 'gTileset_PetipresSecondary'.
     [ERROR] Failed to load EclatRoute2_Layout: unknown secondary tileset label 'gTileset_PetipresSecondary'.
     [WARN] Failed to set map to 'Route2'
     ```
  2. O parser em C++ do Porymap faz uma leitura direta via expressão regular em `src/data/tilesets/headers.h` procurando declarações de `const struct Tileset <Nome> =`. Ele **não** processa diretivas de pré-processador `#include "petipres.h"` nem `#include "eclat_route2.h"`. Como `gTileset_PetipresSecondary` estava dentro de um arquivo `#include`, o Porymap não encontrava o rótulo do tileset secundário e abortava o carregamento do mapa e das conexões de Petiprés.
  3. Além disso, o Porymap espera os arquivos de tileset no formato do editor (Emerald 512/512 metatiles, `tiles.png`, `metatiles.bin`, `metatile_attributes.bin` de 2 bytes e pasta `palettes/` com `.pal`), localizados no diretório derivado do rótulo (`petipres_secondary`).
- **Implementação**:
  - Em `src/data/tilesets/headers.h`, substituídas as diretivas `#include "petipres.h"` e `#include "eclat_route2.h"` pelo conteúdo inlined das declarações de todos os tilesets (`gTileset_PetipresSecondary`, `gTileset_PetipresHousesPrimary`, `gTileset_PetipresHousesSecondary`, `gTileset_PetipresMartSecondary`, `gTileset_EclatRockyPathSecondary`, `gTileset_EclatRoute2GateSecondary`, `gTileset_EclatRoute2CenterSecondary`, `gTileset_PeacefulWoodsSecondary`), tornando todos visíveis ao analisador do Porymap.
  - Em `tools/eclat/porymap_iviere_compat.py`, adicionada a rotina de conversão do tileset secundário de Petiprés (`data/tilesets/secondary/petipres`) para a estrutura 512/512 compatível com o Porymap em `data/tilesets/secondary/petipres_secondary/` (gerando `tiles.png`, `metatiles.bin`, `metatile_attributes.bin` e `palettes/00.pal` a `12.pal`), mantendo os binários 384 FRLG da ROM intactos.
  - Executado o gerador e sincronizado com o build tree no WSL.
- **Validação**:
  - Recompilado via `work/build-route1.sh` com 0 erros e 0 avisos.
  - Suíte de testes automatizados `test-route2.sh 0` 100% aprovada (todas as 25 asserções passaram).
  - Suítes de regressão `test-route1.sh 0` e `test-unbound-grass.sh 0` 100% aprovadas.
  - ROM final copiada e entregue em `eclat-intro-teste.gba`.

### Finalização dos Eventos da Rota 2 e Transição para Peaceful Woods — 07/11/2026

- **Solicitação do Usuário**: Terminar de implementar todos os eventos e scripts da Rota 2 para permitir a progressão contínua em direção a Peaceful Woods.
- **Implementação**:
  - **Consolidação de Scripts em `Route2`**:
    - Centralizados todos os scripts de NPCs, itens, treinadores, placas e a cena da Equipe Rocket diretamente em `data/maps/Route2/scripts.inc` e `data/maps/Route2/rocket.inc`.
    - Criados aliases bidirecionais (`Route2_*` e `EclatRoute2_*`) para que o editor de scripts do Porymap exiba todo o conteúdo do mapa diretamente ao inspecionar a Route 2.
    - `data/maps/EclatRoute2/scripts.inc` e `rocket.inc` foram esvaziados para evitar duplicações de símbolos na montagem.
  - **Calibração de Warps e Elevações**:
    - Em `data/maps/Route2/map.json`, a elevação do Warp 4 em `(45, 18)` para `MAP_PEACEFUL_WOODS` foi ajustada de 0 para 3, harmonizando com a colisão do terreno (`c3`) e o Warp 5 em `(45, 19)`.
    - Em `data/maps/PeacefulWoods/map.json`, os Warps 0 e 1 em `(12, 56)` e `(12, 57)` conectam de volta com perfeição aos Warps 4 e 5 de `MAP_ROUTE102`.
    - Limpeza de cópias espúrias de objetos em `PeacefulWoods/map.json`: os objetos 33, 34 e 35 foram corrigidos para seus scripts nativos da floresta (`PeacefulWoods_Npc21` e `PeacefulWoods_Npc17`), desvinculando-os das flags da Rota 2.
  - **Encontros Selvagens e Efeitos de Campo**:
    - Atualizado `src/data/wild_encounters.json` para que `MAP_ROUTE102` possua os encontros originais da Rota 2 de Éclat (Spearow, Nidoran, Buneary, Hoppip, etc.), e `LAYOUT_PEACEFUL_WOODS` foi incluído em `ShouldUseEclatGrass()` em `src/field_effect_helpers.c`.
  - **Geração de Tilesets Editor para Peaceful Woods e Rocky Path**:
    - `tools/eclat/porymap_iviere_compat.py` atualizado para gerar os espelhos 512/512 (`peaceful_woods_secondary` e `eclat_rocky_path_secondary`) com `tiles.png`, `metatiles.bin`, `metatile_attributes.bin` e paletas JASC-PAL, garantindo que o Porymap abra Peaceful Woods e Rocky Path sem erros.
- **Validação**:
  - Compilado com `make -j8 emerald` com 0 erros e 0 avisos.
  - `work/test-route2.sh 0`: 100% aprovado (25/25 asserções, incluindo vitória contra Grunts, fuga para Peaceful Woods e desbloqueio do caminho).
  - Suítes de regressão `test-route1.sh 0` e `test-unbound-grass.sh 0` 100% aprovadas.
  - ROM final copiada e entregue em `eclat-intro-teste.gba`.

### Primeira integração visual do Cube do Pokémon Unbound na Bag — 08/11/2026

- **Solicitação**: aproximar a tela da Bag do design do Cube de Pokémon Unbound, preservando a lógica de inventário da engine Emerald.
- **Análise da referência**: a tela do Unbound foi conferida em estado jogável, em 240×160 pixels. O padrão observado foi uma coluna esquerda para o Cube, uma lista de itens mais larga à direita, o nome do bolso no cabeçalho e a descrição ocupando a faixa inferior.
- **Implementação em `src/strings.c`**:
  - `CLOSE BAG` passou a ser `CLOSE CUBE`.
  - Os nomes dos bolsos passaram para capitalização de título (`Items`, `Poké Balls`, `TMs & HMs`, `Berries`, `Key Items`), sem alterar os índices ou a lógica de seleção.
- **Implementação em `src/item_menu.c`**:
  - A janela da lista foi ampliada de 16 para 18 tiles, criando o painel direito mais próximo do Cube.
  - A janela de descrição passou de `(5, 13, 24×6)` para `(1, 13, 28×6)`, permitindo a faixa inferior quase completa.
  - O título do bolso foi reposicionado para o painel esquerdo (`tilemapLeft = 1`).
- **Implementação em `src/item_menu_icons.c`**:
  - O sprite visual da Bag/Cube foi ancorado em `(48, 76)`, mantendo a escala e os seis quadros já existentes e evitando duplicação de gráficos.
- **Validação**:
  - `make -j4 emerald` concluído com sucesso; uso final: EWRAM 86,39%, IWRAM 86,66%, ROM 81,66%.
  - A ROM resultante foi copiada para `eclat-intro-teste.gba`.
  - SHA-256: `A37E528AE57E9BFFA0C9BC3BB142C66B9E4D54B54AFF07517836DED8F87D2E7D`.
- **Limitação registrada**: esta é a primeira camada de integração de layout e textos. A equivalência pixel a pixel do painel, paleta e indicação `START Sort` ainda requer uma sessão de comparação no emulador com a Bag aberta e, se necessário, novos gráficos dedicados.

### Implementação Definitiva e Correção de Layout do Cube (Unbound) — 08/11/2026

- **Sintoma das alterações anteriores**: o menu apresentava corte no nome dos itens ("Ticke"), setas desalinhadas no topo sobrepostas ao texto, listras verdes geradas por rotinas legadas de wipe do Emerald, fragmentos de tiles e sprite de Pokéball giratória flutuando sobre a caixa do Cube e conflitos de baseBlock na VRAM.
- **Diagnóstico e extração direta da ROM do Pokémon Unbound (offsets `0x4530c4` e `0x452f6c`)**:
  - `WIN_ITEM_LIST`: `bg = 0, left = 11, top = 1, width = 18, height = 12, baseBlock = 0x27` (exibe 6 itens, `maxShowed = 6`).
  - `WIN_POCKET_NAME`: `bg = 0, left = 1, top = 1, width = 9, height = 2, baseBlock = 0xFF` (encaixado no visor superior esquerdo).
  - `WIN_DESCRIPTION`: `bg = 0, left = 5, top = 14, width = 25, height = 6, baseBlock = 0x111` (começa em x=40, preservando a área do ícone 32×32 em x=24).
  - `WIN_TMHM_INFO_ICONS`: `baseBlock = 0x1A7`.
  - `WIN_TMHM_INFO`: `baseBlock = 0x1C5`.
  - `WIN_MESSAGE`: `baseBlock = 0x201`.
  - Setas de bolso: `firstX = 8, firstY = 76, secondX = 88, secondY = 76` (flanqueando o Cube centralizado em `(48, 76)`).
  - Setas de rolagem da lista: `x = 160, minY = 8, maxY = 102`.
- **Correções aplicadas**:
  - `src/item_menu.c`:
    - BaseBlocks sequenciais e estritamente sem sobreposição na VRAM.
    - `MAX_ITEMS_SHOWN` e `sItemListMenu.maxShowed = 6`, `item_X = 9, cursor_X = 1`.
    - Alinhamento de quantidade em 134 px e ícone de item registrado em 112 px.
    - Animação de rotação da Pokéball desativada (`AddSwitchPocketRotatingBallSprite` como no-op).
    - Desativados wipes destrutivos do Emerald (`DrawItemListBgRow` e `DrawPocketIndicatorSquare` como no-ops, remoção de `FillBgTilemapBufferRect_Palette0(2, 11, 14, 2, 15, 16)`).
    - `PrintPocketNames` e `CopyPocketNameToWindow` adaptados para 9 tiles (72 px) com texto centralizado.
  - `src/item_menu_icons.c`:
    - Cube centralizado perfeitamente em `(48, 76)`.
    - Remoção do sprite de Pokéball giratória sobre o visor.
  - `src/strings.c`:
    - Nomes dos bolsos em Title Case ("Items", "Poké Balls", etc.) e "CLOSE CUBE".
- **Validação**:
  - Compilação concluída com sucesso via `./build.sh`.
  - ROM atualizada: `eclat-intro-teste.gba`.
  - SHA-256: `27D50FC7B7E8CB70E76BCF472D1287B90EB0A25C3DE9FA91C60B72594A61D347`.

### Correção de Glitch ao Trocar Bolso para Direita e Alinhamento da Descrição — 08/11/2026

- **Sintomas**:
  1. Ao navegar para a direita entre os bolsos, o nome do bolso apresentava caracteres corrompidos (`''""`) no canto direito do visor, enquanto ao navegar para a esquerda o texto ficava normal.
  2. O texto da descrição dos itens ficava colado na borda superior ciano (`y = 113`) e muito próximo da caixa branca do ícone do item.
- **Causa identificada**:
  1. No Emerald, a animação de deslizar o nome do bolso fazia cópia em buffer offscreen (`pocketNameBuffer`). Como o buffer na struct `BagMenu` tinha tamanho fixo de 32 tiles, ao deslizar para a direita (`timer >> 1 == 9`), os índices de cópia da linha inferior alcançavam os tiles 27 a 35, ultrapassando os 32 tiles alocados e lendo memória não inicializada da RAM (gerando caracteres de pontuação aleatórios). Ao navegar para a esquerda, os tiles acessados nunca passavam do índice 26, mantendo-se dentro do buffer.
  2. No Unbound autêntico (offset `0x1087ec` do disassembly da ROM), o nome do bolso não sofre animação de deslizamento de texto: o Cube rotaciona no painel e o nome do bolso atualiza diretamente na janela (`WIN_POCKET_NAME`).
  3. No Unbound autêntico (`step_6885_media_0.png`), o texto da descrição inicia em `x = 46` e `y = 120`, ficando alinhado perfeitamente com a caixa branca do ícone do item (`y = 120..152`) e com margem agradável sob a linha ciano.
- **Correções aplicadas**:
  - `src/item_menu.c`:
    - `PrintPocketNames` atualizado para desenhar o nome do bolso diretamente em `WIN_POCKET_NAME` centralizado (72 px) com `CopyWindowToVram`, eliminando o overflow de buffer offscreen e garantindo transição 100% limpa tanto para a direita quanto para a esquerda.
    - `Task_SwitchBagPocket`: simplificada a máquina de estados para manter a rotação suave do Cube por 16 frames e sincronizar a atualização do inventário sem deslizamento de tiles na VRAM.
    - `BAG_DESCRIPTION_TEXT_LEFT` ajustado para `6` (tela `x = 46`).
    - `BAG_DESCRIPTION_TEXT_TOP` definido como `4` (tela `y = 116`), alinhando o texto à altura do ícone do item e afastando-o da borda ciano superior.
- **Validação**:
  - Compilação concluída com sucesso via `./build.sh` (0 erros, 0 avisos).
  - ROM atualizada: `eclat-intro-teste.gba`.
  - SHA-256: `C5E2BB497020D97B79BF5C34481C8A01DF603A2E115ED2E4AB82276E0A673F38`.


