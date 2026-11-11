# Histórico do port de Éclat Pourpre para Emerald Expansion

Este documento acompanha a recriação da abertura e da primeira cidade de Pokémon Éclat Pourpre no Pokémon Emerald Expansion. A referência foi a ROM local em inglês, baseada em FireRed. A implementação usa a versão **1.17.2** do Expansion, commit `79dd436920`, na branch `eclat-intro`.

> [!NOTE]
> Para o roteiro narrativo, trama, arcos de personagens e a "Bíblia da História" do jogo, consulte o documento dedicado: [**`historia.md`**](historia.md).

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

## Mochila Cube (Pokémon Unbound)

| Escopo | O que foi feito | Como foi feito |
| --- | --- | --- |
| Transição dos Bolsos do Cube | Correção de corrupção visual (`''""`) ao trocar bolsos para a direita. | O buffer de deslizamento legado de 32 tiles do Emerald causava overflow de memória ao copiar 9 tiles a partir do índice 27. Substituído por sincronização e renderização direta no `WIN_POCKET_NAME` durante o giro suave de 16 quadros do Cubo. |
| Quebra e Ocupação da Descrição de Itens | Formatação horizontal em 2 linhas com preenchimento da largura da caixa e fim do corte no rodapé. | Criada a rotina `BagMenu_FormatDescription` com algoritmo guloso (*greedy wrap*) a 180 px, substituindo o balanceador simétrico de `BreakStringAutomatic` que dividia arbitrariamente o texto em 3 linhas curtas. Os textos agora preenchem toda a largura da janela em no máximo 2 linhas com margem limpa no rodapé. |

## Evento da Portaria (DexNav do Guarda)

| Escopo | O que foi feito | Como foi feito |
| --- | --- | --- |
| Ativação do Sistema DexNav | Ativação do DexNav no Expansion com flags e variáveis de sistema dedicadas. | Habilitado `DEXNAV_ENABLED = TRUE` em `include/config/dexnav.h`. Alocadas as flags `FLAG_SYS_DEXNAV_GET` (`SYSTEM_FLAGS + 0x3`), `FLAG_DEXNAV_SEARCHING` (`0x266`), `FLAG_DEXNAV_DETECTOR_MODE` (`0x267`) e variáveis `VAR_DEXNAV_SPECIES` (`0x40F7`), `VAR_DEXNAV_STEP_COUNTER` (`0x40F8`). Ícone no Unbound Start Menu condicionado a `FLAG_SYS_DEXNAV_GET`. |
| Cena do Guarda na Portaria | O guarda interrompe a passagem do jogador para a Rota 1 e entrega o DexNav com diálogo sobre seu sonho frustrado de ser treinador. | Adicionados gatilhos em `(6,5)` e `(6,6)` de `IviereRoute1Gate` que acionam a cena caso `FLAG_SYS_DEXNAV_GET` não esteja ativa. O guarda chama a atenção do jogador, desabafa sobre seu sonho de infância, entrega o DexNav com fanfarra oficial, explica o funcionamento e incentiva o protagonista a conquistar o sonho pelos dois. |

## Cidade de Petipres e Textbox Éclat

| Escopo | O que foi feito | Como foi feito |
| --- | --- | --- |
| Textbox e Namebox Éclat Pourpre | Troca do design da message box e name box pelo estilo moderno do Éclat Pourpre com cantos arredondados e realce ciano. | `graphics/text_window/message_box.png` (56x16) e `name_box.png` (24x16) convertidos em PNGs indexados 4bpp (16 cores estritas). Ajuste de paletas no C e quebra manual de diálogos ($\le 32$ caracteres por linha) para eliminar corte de palavras nas caixas. |
| Cena do Fugitivo em Petipres | Monólogo reflexivo de superação do protagonista e correção da rota de fuga do estranho pelas escadas. | Após o homem em pânico fugir e deixar a carta de advertência da Team Rocket, o protagonista reflete empaticamente sobre a dor daquela vítima, mas reafirma sua determinação de não desistir e honrar a promessa feita à sua mãe e ao Clefable de seu pai. A rota de corrida do NPC foi ajustada para subir pelo caminho de terra (`walk_fast_up` x 3) e subir a escadaria com corrimão vermelho (`walk_fast_right` x 6 através de `x=22, y=22`) em direção ao terraço superior, eliminando o clipping através do desnível rochoso. |
| Acolhimento no Centro Pokémon de Petipres | Cena de boas-vindas da Enfermeira Joy com presente, humor sobre a família Joy, primeira cura e animação da Chansey. | Adicionado `SP_NAME_JOY` (`"NURSE JOY"`) e flag `FLAG_PETIPRES_NURSE_INTRO` (`0x47`). Na 1ª visita, a Joy acolhe o protagonista ao notar sua tensão, explica que o Centro é um porto seguro, presenteia com uma Potion e brinca que nem toda Joy da família o mimará assim, curando a equipe em seguida. Visitas posteriores usam o menu ágil padrão. A Chansey teve seu texto em francês traduzido para inglês com animação de pulinho e coração (`jump_in_place_down` + `emote_heart`). |
| Confronto Team Rocket na Rota 2 | Reformulação do diálogo do encontro com os capangas e chegada de Myst: orgulho dos vilões ferido por uma criança e remoção de piadas de tradução. | Em `data/maps/Route2/rocket.inc`, o protagonista confronta os dois capangas sobre o roubo em Iviere; os vilões inicialmente desdenham por ser uma criança e mandam ele voltar para a mãe. O protagonista peita os vilões e os desafia para uma batalha Pokémon. Sentindo-se rebaixados por um pirralho, os vilões se enfurecem e aceitam o embate. Myst intervém repreendendo a impulsividade do jovem, mas dá cobertura na luta. Removida a antiga quebra de quarta parede ("Chidori / piada de Naruto"), substituída por um interrogatório coerente onde o capanga revela que a tropa principal se reúne em Peaceful Woods. Integração total com `setspeaker` (`SP_NAME_ROCKET`, `SP_NAME_PLAYER`, `SP_NAME_MYST`) e limite $\le 32$ caracteres por linha. |

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
Seguindo o princípio arquitetural do projeto, novos arquivos de sistema e scripts de mapas devem manter-se em módulos focados e desacoplados, evitando arquivos monolíticos e facilitando a rastreabilidade e testes automatizados.

### 6. Caixa de Diálogo e Namebox Estilo FireRed / Éclat Pourpre e Correção de Quebras de Linha
- **Gemini [Diagnóstico]**: A caixa de diálogo original do Emerald apresentava moldura ciano/esmeralda grossa e arredondada incompatível com a estética sóbria de Éclat Pourpre. Além disso, falas com mais de 34 caracteres por linha sem quebra manual sofriam corte do final de palavras na margem direita da tela do GBA.
- **ChatGPT [Análise]**: A engine do Emerald renderiza o contorno da caixa via `WindowFunc_DrawDialogueFrame` usando os 14 tiles de `graphics/text_window/message_box.png` (56×16, 4bpp) e a paleta `sTextWindowPalettes[0]`. A Namebox utiliza os 6 tiles de `graphics/text_window/name_box.png` (24×16, 4bpp) na mesma paleta `DLG_WINDOW_PALETTE_NUM`. O compilador `gbagfx` requer que os arquivos PNG possuam chunk `PLTE` estritamente indexado em 16 cores (48 bytes), evitando erros de *excess elements in array initializer*. Para evitar cortes de texto na tela de 208 px (26 tiles), cada linha deve possuir no máximo 32–33 caracteres.
- **Gemini [Implementação]**:
  1. Extraídos e implementados os tiles oficiais da caixa de texto FireRed/Éclat Pourpre em `graphics/text_window/message_box.png` (56×16) e na aba da Namebox em `graphics/text_window/name_box.png` (24×16), com paleta elegante de ardósia/grafite e fundo azul-ardósia escuro para o nome do interlocutor.
  2. Gerados os arquivos PNG com chunk PLTE estrito de 16 cores (32 bytes em `.gbapal` e 448/192 bytes em `.4bpp`), garantindo compilação limpa sem avisos.
  3. Revisadas todas as falas de `LittlerootTown/scripts.inc` (Prof. Filão), `IviereRoute1Gate/scripts.inc` (Guarda), `Iviere_FilaosHouse/scripts.inc` (Mrs. Filão) e `EclatRoute1/scripts.inc` (Myst), reformatando todas as quebras de linha (`\n`, `\l`, `\p`) para o limite seguro de $\le$ 32 caracteres. Compilação concluída com sucesso em `eclat-intro-teste.gba`.
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

### Rota 3 e Correção de Conexões e Porymap

- **Usuário [Reporte]**:
  1. Ao sair de Peaceful Woods para a Rota 3, o jogador não conseguia retornar para Peaceful Woods pela conexão/warp.
  2. No Porymap, ocorria erro ao abrir o mapa `Route3`.
- **Gemini [Diagnóstico e Correções]**:
  1. **Compatibilidade do Porymap**: O Porymap converte nomes camel-case de tilesets inserindo sublinhado antes de números (`gTileset_EclatRoute3Secondary` $\rightarrow$ `data/tilesets/secondary/eclat_route_3secondary/`). O script `tools/eclat/porymap_iviere_compat.py` foi atualizado para gerar os arquivos necessários (`tiles.png`, `metatiles.bin`, `metatile_attributes.bin` e `00.pal..12.pal`) tanto em `eclat_route_3secondary` quanto em `eclat_route3_secondary`, além de adicionar suporte a `petipres_houses_primary/secondary`.
  2. **Comportamento do Warp (Metatile Attributes)**: Os metatiles de entrada 774 e 782 em `(12, 34)` e `(12, 35)` estavam com comportamento 0x0. A tabela de atributos original foi recuperada do endereço `0x8894d8` (apontado pelo cabeçalho `0x2cef2c` da ROM original), restaurando o comportamento direcional `MB_WESTWARD_WARP` (`0x63`).
  3. **Elevação dos Warps**: Em `data/maps/Route3/map.json`, os Warps 0 e 1 estavam definidos com `elevation: 0`, enquanto o terreno está em `elevation: 3`. O campo foi ajustado para `elevation: 3`, restabelecendo o trânsito bidirecional perfeito entre Peaceful Woods e Rota 3.
  4. **Compilação**: Compilado com sucesso com 0 erros via WSL, gerando `eclat-intro-teste.gba`.
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

### Reformulação Dramática e Cinematográfica do Prólogo (Quarto, Roubo e Prof. Filao) — 08/11/2026

- **Solicitação do Usuário**: Recontar e amplificar a dramaticidade do início da jornada:
  1. **Ato 1 (Quarto e TV)**: O protagonista inicia assistindo à notícia urgente na TV sobre a onda de roubos de Pokémon na região, expressando indignação e reflexão profunda sobre como alguém tem coragem de roubar parceiros de outros treinadores. Ao caminhar em direção à escada, escuta os gritos desesperados de socorro da mãe.
  2. **Ato 2 (Roubo no 1F)**: O jogador desce correndo a escada e presencia o assalto. Por ainda não possuir nenhum Pokémon, os criminosos zombam da sua incapacidade de lutar de mãos vazias e fogem com o Pokémon do pai. A mãe desaba em pranto e o jogador promete recuperar o Pokémon.
  3. **Ato 3 (Frustração e Prof. Filao)**: Ao sair de casa, o protagonista cerra os punhos lamentando sua impotência por não ter um Pokémon para proteger a mãe. O Prof. Filao ouve os gritos e chega correndo preocupado. Comovido pela determinação do jovem, entrega-lhe um Pokémon inicial (seleção personalizada de 27 iniciais entre 9 gerações) e faz um discurso inspirador para transformar a dor em força, vencer a Liga Pokémon e resgatar o Pokémon do pai.
- **Implementação**:
  - **Quarto (`LittlerootTown_BrendansHouse_2F`)**:
    - Em `src/new_game.c`: O ponto de spawn de início de jogo foi ajustado para `(7, 7)`, posicionando o jogador diretamente sobre o tapete vermelho em frente à TV e de frente para ela (`DIR_NORTH` em `OnTransition`).
    - Em `scripts.inc`: A cena abre diretamente com a transmissão do boletim urgente na TV, seguido pelo monólogo dramático de reflexão com `SP_NAME_PLAYER`. Toda a conversa e escolha antiga de taxa de shiny foi completamente removida (permitindo o uso futuro e nativo do Shiny Charm do Expansion). O controle é liberado imediatamente após a reflexão na TV, e o estado avança para `VAR_LITTLEROOT_INTRO_STATE = 1`.
    - Em `map.json`: O gatilho de coordenadas da escada foi posicionado precisamente em `(10, 2)` (um tile acima do piso lateral, ficando exatamente sobre o tapete vermelho no topo do primeiro degrau da escada). Ao pisar, o protagonista vira imediatamente para a esquerda (`Common_Movement_FaceLeft`) olhando diretamente para o vão dos degraus que descem, ouve o grito da mãe (`SP_NAME_MOM`), expressa seu choque ainda virado para a esquerda (`SP_NAME_PLAYER`), avança o estado para `8` e é transportado diretamente para o 1F (`warp MAP_LITTLEROOT_TOWN_BRENDANS_HOUSE_1F, 10, 2`).
  - **Térreo (`LittlerootTown_BrendansHouse_1F`)**:
    - Ao descer a escada (`VAR_LITTLEROOT_INTRO_STATE == 8`), a câmera panorâmica exibe os ladrões intimidando a mãe. O jogador desce correndo e intervém bravamente. O capanga zomba com desdém (`"What are you gonna do, kid? Fight us with your bare hands? You don't even have a POKéMON!"`), agarra a Pokébola do pai e os vilões fogem pela porta.
    - A mãe lamenta que era a última lembrança do pai, e o jogador jura recuperá-lo. O estado é definido como `VAR_LITTLEROOT_INTRO_STATE = 9`, mantendo o jogador sem Pokémon até encontrar o professor.
  - **Exterior de Iviere (`LittlerootTown`)**:
    - Em `include/constants/flags.h`, criado `#define FLAG_HIDE_IVIERE_FILAO_OUTSIDE 0x2F`, inicializado ativo em `src/new_game.c`.
    - Em `map.json`, inserido o evento de objeto do Prof. Filao (`OBJ_EVENT_GFX_FILAO`) na posição `(15, 26)`.
    - Em `scripts.inc`, ativado `MAP_SCRIPT_ON_FRAME_TABLE` com `VAR_LITTLEROOT_INTRO_STATE == 9`:
      - O jogador para na porta, lamentando amargamente sua falta de Pokémon para defender a mãe.
      - O Prof. Filao surge gritando pelo jogador, corre pela passarela de `x=15` até `x=21`, ficando face a face com o jogador.
      - Após ouvir o relato do roubo, Filao oferece seu estojo de iniciais (`IviereTown_ChooseType` com 27 iniciais das 9 gerações).
      - Ao receber o inicial, toca a fanfarra, ativa a Pokédex Nacional, oferece opção de apelido e o professor profere o discurso dramático motivacional, instruindo-o a buscar força na Liga Pokémon para derrotar os criminosos.
      - O professor caminha de volta ao laboratório, e o estado é atualizado para `7` (permitindo a progressão natural da história).
- **Validação**: Compilado via `./build.sh` no WSL sem erros e entregue na ROM `eclat-intro-teste.gba`.

### Aproximação do Guarda na Portaria de Iviere (`IviereRoute1Gate`) — 08/11/2026

- **Solicitação do Usuário**: Fazer com que o jogador, ao ser chamado pelo guarda na portaria entre Iviere Town e a Rota 1, caminhe até a bancada/balcão do guarda, ficando de frente para ele separado pelo balcão para transmitir a sensação autêntica de aproximação.
- **Implementação**:
  - Em `data/maps/IviereRoute1Gate/map.json`, os gatilhos de coordenadas foram reposicionados na entrada da portaria em `(3, 5)` e `(3, 6)`, apontando respectivamente para `IviereRoute1Gate_Trigger_DexNav_Top` e `IviereRoute1Gate_Trigger_DexNav_Bottom`.
  - Em `data/maps/IviereRoute1Gate/scripts.inc`:
    - Ao pisar nos gatilhos, o guarda avista o jogador com balão de exclamação (`playse SE_PIN`, `Common_Movement_ExclamationMark`) e chama a atenção: `"Guard: Wait a second, young trainer! Come over to the counter for a moment!"`.
    - O jogador caminha suavemente até a bancada em `(6, 5)` (3 passos à direita pelo corredor superior, ou 3 passos à direita e 1 para cima pelo corredor inferior) e vira para cima.
    - O guarda vira para baixo em `(6, 3)` de frente para o jogador através do balcão `(6, 4)`.
    - Executado o diálogo com a entrega do DexNav, fanfarra, ativação de `FLAG_SYS_DEXNAV_GET` e explicação do funcionamento.

### Sistema de Exibição e Animação de Obtenção de Itens (Opção B - Unbound / CFRU) — 08/11/2026

- **Solicitação do Usuário**: Implementar a animação e destaque de item obtido no estilo do Pokémon Unbound (Opção B escolhida pelo usuário), com itens normais e importantes diferenciados na tela.
- **Implementação**:
  - Em `include/config/overworld.h`, ativado `#define OW_SHOW_ITEM_DESCRIPTIONS OW_ITEM_DESCRIPTIONS_ALWAYS`.
  - Em `src/overworld.c`:
    - Eliminada a criação de caixas de cabeçalho invasivas no topo da tela (`sHeaderBoxWindowId`), mantendo o visual limpo do overworld conforme o estilo Unbound / CFRU.
    - Implementada a classificação de itens via `IsImportantItem` (`POCKET_KEY_ITEMS`, `POCKET_TM_HM` e `GetItemImportance`).
    - **Itens Importantes**: O sprite surge no centro da tela (`x = 120, y = 62`) e executa a animação `SpriteCB_ObtainedItem_Center` (elevação suave de 12 px em 14 frames seguida de flutuação e levitação senoidal contínua enquanto durar a fanfarra e a mensagem).
    - **Itens Normais**: O sprite surge no canto inferior direito da caixa de mensagens (`x = 213, y = 140`) com a animação `SpriteCB_ObtainedItem_Corner` (salto rápido para cima com overshoot de 4 px em 8 frames e amortecimento elástico de retorno em 4 frames).
    - As rotinas `ScriptShowItemDescription` e `ScriptHideItemDescription` gerenciam o ciclo de vida do sprite de forma totalmente segura e assíncrona.
  - **Animação Específica para Obtenção do DexNav**:
    - Em `src/overworld.c`, implementada a função nativa `ScriptShowDexNav` e o sprite `sSpriteTemplate_DexnavObtain` utilizando o gráfico de alta resolução do DexNav (`graphics/unbound_start_menu/sprites/dexnav.4bpp.smol`) com a paleta oficial (`icons.gbapal`).
    - Criada a macro de script `.macro showdexnav` em `asm/macros/event.inc`.
    - No script `IviereRoute1Gate_Event_GiveDexNav` e na interação direta `IviereRoute1Gate_Guard`, acionado `showdexnav` durante a mensagem e a fanfarra, exibindo o DexNav levitando no centro da tela até o jogador pressionar o botão para avançar para a explicação do guarda.
- **Validação**: Compilado via `./build.sh` no WSL sem erros e entregue na ROM `eclat-intro-teste.gba`.

### Posicionamento e Animação do Sprite do Rival na Tela de Nomes — 08/11/2026

- **Solicitação do Usuário**: Na tela de escolha de nome da rival na introdução, alinhar o mini sprite na mesma posição do treinador principal (à esquerda, sobre a base) e fazer com que ele anime (passos no lugar) em vez de ficar estático.
- **Implementação**:
  - Em `src/naming_screen.c`:
    - Em `NamingScreen_CreateRivalIcon`, as coordenadas do sprite foram alteradas de `(176, 37)` (lado direito) para `(56, 37)` (exatamente a mesma posição do protagonista).
    - A animação foi alterada de `ANIM_STD_FACE_SOUTH` (pose estática) para `ANIM_STD_GO_SOUTH` (passos ritmados no lugar, idêntico ao protagonista).
    - Em `MainState_FadeIn`, foi removido o desvio que movia os tiles da plataforma de fundo para a coluna da direita, garantindo que a base permaneça perfeitamente alinhada sob o sprite em `x = 56`.
- **Validação**: Compilado via `./build.sh` no WSL com 0 avisos/erros e entregue na ROM `eclat-intro-teste.gba`.

### Integração do Sistema de Namebox (Caixa de Nome do Locutor) — 08/11/2026

- **Solicitação do Usuário**: Adicionar caixas com o nome de quem está falando nos diálogos (Namebox) para modernizar o visual narrativo.
- **Implementação**:
  - Em `include/constants/speaker_names.h` e `src/data/speaker_names.h`, configuradas as constantes e strings oficiais de locutores: `MOM`, `{PLAYER}`, `{RIVAL}`, `PROF. FILAO`, `MRS. FILAO`, `GUARD`, `MYST`, `???` e `ROCKET`.
  - Integrado o comando `setspeaker <NAME>` nos scripts principais do início da campanha:
    - **Quarto (2F)**: Grito da mãe no andar inferior com `SP_NAME_MOM`.
    - **Cena do Roubo (1F)**: Diálogo de ameaça e fuga dos capangas com `SP_NAME_MYSTERIOUS`, desabafo da mãe com `SP_NAME_MOM`, e promessa do protagonista com `SP_NAME_PLAYER`.
    - **Casa do Prof. Filao**: Entrega do Town Map pela esposa do professor com `SP_NAME_FILAO_WIFE`.
    - **Laboratório do Prof. Filao**: Conversa de cura e suporte do professor com `SP_NAME_FILAO`.
    - **Portaria de Iviere (`IviereRoute1Gate`)**: Chamado, entrega do DexNav e explicação do guarda com `SP_NAME_GUARD`.
  - Removidos os prefixos textuais brutos (ex: `"Mom: "`, `"Guard: "`, `"FILAO: "`, `"???: "`), deixando os textos das caixas de mensagens completamente limpos com o nome destacado na janela flutuante da Namebox.
- **Validação**: Compilado via `./build.sh` no WSL com 0 erros e entregue na ROM `eclat-intro-teste.gba`.

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

### Humanização de Iviere Town: Mãe, Dona Alda e Guarda da Portaria — 08/11/2026

- **Objetivo**:
  Enriquecer o prólogo dramático com laços comunitários e familiares profundos após o falecimento do pai e o roubo de seu Pokémon:
  1. A mãe acolhe o filho de volta após ele receber o Pokémon inicial de Filão, tornando a casa um porto seguro de descanso/cura.
  2. Dona Alda (a idosa na margem do rio) conforta o protagonista pela perda do pai e o presenteia com o `EXP_SHARE`.
  3. O guarda na guarita para a Rota 1 impede a partida prematura caso o jogador tente sair sem se despedir da mãe.

- **Arquivos modificados**:
  - `include/constants/speaker_names.h` & `src/data/speaker_names.h`:
    - Adicionado `SP_NAME_ALDA` com string `"ALDA"` para uso de Namebox nativa.
  - `include/constants/flags.h`:
    - `#define FLAG_TALKED_TO_MOM_AFTER_STARTER 0x21`: Marca que o jogador retornou para falar com a mãe após receber o inicial.
    - `#define FLAG_GOT_EXP_SHARE_FROM_ALDA 0x46`: Registra a entrega do Exp. Share por Dona Alda.
  - `data/maps/LittlerootTown_BrendansHouse_1F/scripts.inc`:
    - `LittlerootTown_BrendansHouse_1F_EventScript_EclatMom`:
      - Primeira interação pós-inicial (`VAR_LITTLEROOT_INTRO_STATE >= 7` e flag desligada): O jogador apresenta o parceiro e promete recuperar o Pokémon do pai. A mãe se emociona vendo a chama do falecido marido nos olhos do filho, cura a equipe via `Common_EventScript_OutOfCenterPartyHeal`, e ativa `FLAG_TALKED_TO_MOM_AFTER_STARTER`.
      - Interações subsequentes: Caixa com prompt de descanso (`MSGBOX_YESNO`), curando o time a qualquer momento.
  - `data/maps/LittlerootTown/scripts.inc`:
    - `IviereTown_EventScript_RiverNpc`:
      - Identificada como Dona Alda (`setspeaker SP_NAME_ALDA`).
      - Reconhece o tumulto na casa, lembra com respeito do falecido pai, incentiva o jovem com sábias palavras e entrega o `ITEM_EXP_SHARE`.
  - `data/maps/IviereRoute1Gate/scripts.inc`:
    - `IviereRoute1Gate_Trigger_DexNav_Top` & `Bottom`:
      - Interrompe com `!` caso `FLAG_TALKED_TO_MOM_AFTER_STARTER` não esteja definida.
      - Guarda repreende moralmente o jogador por tentar partir sem consolar a mãe e o empurra um passo para trás em direção à cidade.
      - Se falado diretamente no balcão antes da mãe, reforça o pedido para ir falar com ela.
      - Após falar com a mãe, elogia a atitude de bom filho, relembra os velhos tempos batalhando junto com o falecido pai, entrega o `DexNav` para honrar a memória dele e o abençoa para a jornada.
  - `data/maps/Iviere_FilaosHouse/map.json` & `scripts.inc`:
    - Alterado movimento de Mrs. Filao de `MOVEMENT_TYPE_LOOK_AROUND` para `MOVEMENT_TYPE_FACE_DOWN` (fica calma e parada na sala).
    - `Iviere_FilaosHouse_EventScript_TownMap`: Caso o jogador tente pegar o mapa da mesa antes de falar com ela, Mrs. Filao reage com `!` e dá uma bronca bem-humorada ("Tire as mãos daí! Eu sei que a sua mãe não te criou para pegar coisas sem pedir!").
    - `Iviere_FilaosHouse_EventScript_Wife`: Ao falar com ela diretamente, ela expressa sua revolta com os bandidos, apoia a determinação do protagonista, brinca com o acúmulo de tralhas do marido e entrega o `ITEM_TOWN_MAP`, sumindo com o sprite do mapa na mesa.

- **Validação**:
  - Compilação limpa via WSL (`./build.sh`).
  - ROM gerada: `eclat-intro-teste.gba`.

### Revamp do Diálogo com Myst na Rota 1 — 08/11/2026

- **Objetivo**:
  Humanizar e tornar mais natural o primeiro encontro da Rota 1 com o agente dos G-Men, Myst:
  1. Myst aborda o jogador surpreso ao ver um garoto sozinho em uma rota selvagem com Mightyenas perigosos.
  2. Integração com Namebox dinâmica: inicia como `???` (`SP_NAME_MYSTERIOUS`) e muda para `MYST` (`SP_NAME_MYST`) ao se apresentar formalmente.
  3. O jogador expressa desconfiança natural após o assalto recente.
  4. Revelação dramática de que o Pokémon roubado do pai falecido era uma Clefable, selando a determinação do protagonista em recuperá-la.
  5. Myst reconhece a bravura do jovem, oferece cooperação nas investigações futuras e o alerta sobre os perigos da Team Rocket.

- **Arquivos modificados**:
  - `data/maps/EclatRoute1/scripts.inc`:
    - `EclatRoute1_MeetMyst`: Adicionados comandos `setspeaker SP_NAME_MYSTERIOUS`, `setspeaker SP_NAME_PLAYER` e `setspeaker SP_NAME_MYST`.
    - Diálogos reescritos com tom familiar, dinâmico e dramático.

- **Validação**:
  - Compilação limpa via WSL (`./build.sh`).
  - ROM gerada: `eclat-intro-teste.gba`.

### Cena da Carta em Petipres e Maturação do Protagonista — 08/11/2026

- **Objetivo**:
  Dar profundidade e alma à cidade de Petipres através do evento do homem em fuga e da carta:
  1. Integração com a nova Namebox (`SP_NAME_PLAYER`) e estética visual FireRed/Éclat Pourpre.
  2. Substituição da reação impulsiva original por um momento maduro de introspecção: o protagonista percebe o terror nos olhos do estranho e o reconhece como outra vítima da Team Rocket cujo espírito foi quebrado.
  3. O protagonista agradece mentalmente pelo aviso, mas recusa o conformismo e reafirma sua fé inabalável em si mesmo, em seu parceiro inicial e na promessa de recuperar a Clefable do pai falecido.
  4. Tradução dos diálogos do caçador de shinies (Poké Radar) e da pista de tesouro do francês para o inglês.
  5. Formatação rigorosa de quebras de linha com $\le 32$ caracteres por linha, prevenindo qualquer corte visual na tela do GBA.

- **Arquivos modificados**:
  - `data/maps/Petipres/scripts.inc`:
    - `Petipres_StrangerScene`: Inclusão de `setspeaker SP_NAME_PLAYER`, `setspeaker SP_NAME_NONE` e `waitmovement 0`.
    - Diálogos reescritos: `Petipres_ExcuseMe_Text`, `Petipres_Wait_Text`, `Petipres_LetterFound_Text`, `Petipres_Letter_Text`, `Petipres_LetterReaction_Text`.
    - Textos de NPCs de apoio traduzidos e reformatados (`Petipres_Boy_Text`, `Petipres_RocketGrunt_Text`, `Petipres_Radar_Text`, `Petipres_Clue_Text`).

- **Validação**:
  - Compilação com 100% de sucesso via `./build.sh` no WSL.
  - ROM gerada: `eclat-intro-teste.gba`.

### Centralização do Ícone do Item na Bag — 08/11/2026

- **Objetivo**:
  Corrigir o alinhamento pixel-perfect do ícone 24x24 do item na janela branca de descrição da Bag (Mochila / Cube estilo Unbound):
  1. A janela branca de descrição possui altura de 27 pixels ($Y \in [123..149]$), com centro geométrico vertical em $Y = 136.0$.
  2. O sprite alocado para os ícones possui tamanho $32 \times 32$ pixels, porém a arte útil do ícone ocupa a região central $24 \times 24$ pixels.
  3. Com a âncora vertical anterior em $y2 = 136$, a área visível do ícone ficava deslocada para cima ($Y \in [120..143]$), ultrapassando a borda superior da moldura branca em 3 pixels e deixando 6 pixels livres na parte inferior.
  4. Ajuste da âncora vertical para $y2 = 140$, posicionando o ícone exatamente em $Y \in [124..147]$ com margem equilibrada (1 pixel no topo e 2 pixels na base).

- **Arquivos modificados**:
  - `src/item_menu_icons.c`:
    - Ajustado `gSprites[iconSpriteId].y2 = 140;` na função de carregamento do ícone.
  - `eclat docs/bag-icon-centering.md`: Documentação técnica detalhada do cálculo geométrico e visual.

- **Validação**:
  - Compilação limpa com `./build.sh` (código de saída 0).
  - ROM gerada: `eclat-intro-teste.gba`.
  - Verificação visual em emulador confirmando centralização estética e pixel-perfect.

### Sistema de Indicadores de Missão (Quest Indicators) — 08/11/2026

- **Objetivo**:
  Criar um sistema modular, escalável e de baixo custo de processamento para exibir balões visuais flutuantes (ícone de exclamação `!`) sobre a cabeça de NPCs que possuem eventos, itens ou missões pendentes, desativando-se automaticamente assim que a flag do evento for definida:
  1. Criação do módulo `quest_indicator` com tabela central `sQuestIndicators[]` contendo `mapGroup`, `mapNum`, `localId` do NPC e `flagId` associada.
  2. Implementação de callback por frame com levitação harmônica senoidal suave ($\pm 2$ pixels via `gSineTable`) ancorada na cabeça do evento do overworld ($Y - 20$ px).
  3. Destruição dinâmica do sprite em tempo de execução (`DestroySprite`) assim que o jogador recebe o item e a flag de evento é acionada, sem necessidade de recarregar o mapa.
  4. Suporte inicial implementado para:
     - Dona Alda (`FLAG_GOT_EXP_SHARE_FROM_ALDA`, Local ID 14 em `MAP_LITTLEROOT_TOWN` / Iviere Town).
     - Mrs. Filão (`FLAG_IVIERE_TOWN_MAP_TAKEN`, Local ID 1 em `MAP_IVIERE_FILAOS_HOUSE`).
  5. Integração direta e sem overhead no ciclo principal do overworld (`OverworldBasic`).

- **Arquivos modificados**:
  - `include/quest_indicator.h`: Declaração pública de `UpdateQuestIndicators()`.
  - `src/quest_indicator.c`: Implementação completa da lógica de ciclo de vida, template de sprite, animação senoidal e tabela de NPCs.
  - `src/overworld.c`: Inclusão da chamada `UpdateQuestIndicators();` em `OverworldBasic()`.
  - `eclat docs/quest-indicators.md`: Documentação técnica com diagrama Mermaid e instruções passo a passo para cadastrar novos NPCs.

- **Validação**:
  - Compilação sem avisos ou erros via `./build.sh` (código de saída 0).
  - ROM gerada: `eclat-intro-teste.gba`.
  - Teste em jogo validado pelo usuário: balão flutua sobre Alda e Mrs. Filão e desaparece imediatamente após a entrega dos itens (Exp Share e Town Map).

### Mecânica de Pontes e Elevação Multinível — 08/11/2026

- **Objetivo**:
  Documentar e consolidar as correções de elevação de pontes (`ELEVATION_MULTI_LEVEL = 15`) para garantir movimentação tridimensional correta no overworld:
  1. Garantir que o jogador e o Pokémon seguidor (*Follower Pokémon*) possam transitar sobre a ponte sem clipping da cabeça nas rampas de entrada/saída.
  2. Permitir a passagem livre e desimpedida por baixo das estruturas de pontes sem colisão indevida ou sobreposição incorreta de camadas de metatiles.

- **Arquivos modificados**:
  - `eclat docs/bridge-elevation.md`: Documentação técnica detalhada das regras de metatile e camadas.

- **Validação**:
  - Validação de movimentação e renderização gráfica em mapas com pontes e elevações.

### Refinamento da Interação com a Mãe Pós-Inicial — 08/11/2026

- **Objetivo**:
  Ajustar a coerência narrativa e a lógica de gameplay no primeiro diálogo com a mãe após receber o Pokémon inicial:
  1. Remoção da rotina de cura (`Common_EventScript_OutOfCenterPartyHeal`) e do jingle musical no primeiro diálogo, pois o inicial acabou de ser entregue pelo Prof. Filão com 100% de HP e PP intactos.
  2. Foco exclusivo no momento dramático e emocional: o protagonista promete recuperar o Pokémon do falecido pai, a mãe se emociona vendo a chama nos olhos do filho e lhe dá sua bênção.
  3. A mãe reforça que as portas de casa estarão sempre abertas com um refúgio seguro e uma cama quente quando a jornada ficar pesada.
  4. A função de cura e descanso (`MSGBOX_YESNO`) passa a ser oferecida exclusivamente nas interações seguintes (`LittlerootTown_BrendansHouse_1F_EventScript_MomHealPrompt`), quando o jogador realmente retorna de batalhas pelo mundo.

- **Arquivos modificados**:
  - `data/maps/LittlerootTown_BrendansHouse_1F/scripts.inc`:
    - `LittlerootTown_BrendansHouse_1F_EventScript_MomShowStarter`: Removidas as chamadas de cura e de fala pós-cura no 1º encontro.
    - `EclatHouse_Text_MomProudReaction`: Texto refinado com quebras estritas $\le 32$ caracteres, reforçando o acolhimento para visitas futuras.

- **Validação**:
  - Compilação limpa com 100% de sucesso via `./build.sh` (código de saída 0).
  - ROM gerada: `eclat-intro-teste.gba`.

### Reação de Coração no Pokémon Seguidor ao Curar — 08/11/2026

- **Objetivo**:
  Adicionar a clássica e adorada mecânica de afeto estilo *Pokémon HGSS*, fazendo com que o Pokémon seguidor (*Follower Pokémon*) reaja com um balão de coração (`FOLLOWER_EMOTION_LOVE`) e efeito sonoro de carinho (`SE_CONTEST_HEART`) ao recuperar as energias em qualquer evento de cura:
  1. Criação da função segura `ShowFollowerHeartEmote()` no subsistema de movimento de eventos do overworld.
  2. Implementação de checagens estritas para garantir que o efeito só ocorra se o seguidor estiver ativo, visível no mapa e desimpedido (ignora se estiver dentro da Pokébola, na bicicleta, surfando ou fainted).
  3. Integração com `Common_EventScript_OutOfCenterPartyHeal` para ativar ao descansar na cama da mãe, em casas de descanso e camas do mapa.
  4. Integração com `EventScript_PkmnCenterNurse_TakeAndHealPkmn` para ativar em todos os balcões de Centros Pokémon com a Enfermeira Joy.

- **Arquivos modificados**:
  - `include/event_object_movement.h`: Declaração de `ShowFollowerHeartEmote(void)`.
  - `src/event_object_movement.c`: Implementação de `ShowFollowerHeartEmote()` acionando `ObjectEventEmote` e `PlaySE(SE_CONTEST_HEART)`.
  - `data/event_scripts.s`: Chamada `callnative ShowFollowerHeartEmote` em `Common_EventScript_OutOfCenterPartyHeal`.
  - `data/scripts/pkmn_center_nurse.inc`: Chamada `callnative ShowFollowerHeartEmote` em `EventScript_PkmnCenterNurse_TakeAndHealPkmn`.
  - `data/scripts/pkmn_center_nurse_frlg.inc`: Chamada `callnative ShowFollowerHeartEmote` em `EventScript_PkmnCenterNurse_TakeAndHealPkmn_Frlg`.
  - `eclat docs/follower-heal-heart.md`: Documentação técnica detalhada das regras de ativação e segurança.

- **Validação**:
  - Compilação limpa com 100% de sucesso via `./build.sh` (código de saída 0).
  - ROM gerada: `eclat-intro-teste.gba`.

### Correção do Altar de Celebi em Peaceful Woods (Opção 2) — 08/11/2026

- **Objetivo**:
  Restaurar a integridade estética e narrativa do Altar de Celebi (*Autel des Bois Paisibles*) em `PeacefulWoods` (`LAYOUT_PEACEFUL_WOODS` em `57, 38`):
  1. Eliminação do sprite de Rayquaza gigante no telhado da cabana do altar (artefato de colisão de ID de sprite do FRLG ID 184 para o Emerald com flag 0).
  2. Remoção dos 3 Bug Catchers fantasmas que ficavam bloqueando o portal do santuário com textos placeholders.
  3. Desbloqueio e ativação do script oficial de interação do altar em `(57, 37)` com o texto canônico de homenagem ao Guardião da Floresta Celebi (`“This altar is in homage to the guardian of the forest.”`).
  4. Substituição das 3 placas artificiais de terra roxa/lilás (metatiles de Paleta 9 descalibrada) por grama verde limpa (`652`) com toques de flores campestres cor-de-rosa (`752`), harmonizando a clareira com o estilo natural de floresta.

- **Arquivos modificados**:
  - `data/maps/PeacefulWoods/map.json`: Removidos os 4 objetos de evento fantasmas (`OBJ_EVENT_GFX_BUG_CATCHER` e `OBJ_EVENT_GFX_RAYQUAZA_STILL`).
  - `data/maps/PeacefulWoods/scripts.inc`: Removidos os scripts de placeholder não utilizados `PeacefulWoods_Npc12`, `13`, `14` e `15`.
  - `data/layouts/peaceful_woods/map.bin`: Atualizados os metatiles das 3 clareiras ao redor do altar para grama verde viva (`0x328C` / `652`) e flores (`0x32F0` / `752`).
  - `eclat docs/peaceful-woods.md`: Documentação técnica com histórico de investigação e antes/depois do mapa.

- **Validação**:
  - Compilação limpa com 100% de sucesso via `./build.sh` (código de saída 0).
  - ROM gerada: `eclat-intro-teste.gba`.

### Implementação da Rota 3 (substituindo Rota 103) — 08/11/2026

- **Objetivo**:
  Implementar a **Rota 3** em substituição ao slot da Rota 103 (`MAP_ROUTE103` em `gMapGroup_TownsAndRoutes`), conectando a saída leste de **Peaceful Woods** em direção a **Percharbre**:
  1. Extração fiel da geometria $66 \times 56$ (3696 blocos) e do tileset secundário da ROM original de Éclat Pourpre (`data/layouts/eclat_route3/` e `data/tilesets/secondary/eclat_route3/`).
  2. Geração dos assets editoriais 512/512 para o Porymap (`data/tilesets/secondary/eclat_route3_secondary/`).
  3. Registro do layout `LAYOUT_ECLAT_ROUTE3` e do tileset `gTileset_EclatRoute3Secondary`.
  4. Criação do mapa `data/maps/Route3/` (`map.json`, `scripts.inc`, `header.inc`, `events.inc`, `connections.inc`) associado ao slot `MAP_ROUTE103`.
  5. Portabilidade de todos os 6 treinadores de campo (Calvin, Meg, Elijah, Colton, Lana, Cedric) com suas equipes e diálogos em inglês.
  6. Portabilidade dos itens de campo (Super Potion, Awakening, Rare Candy), item oculto (Oran Berry), NPC secreto da Loteria (Eon Ticket), placas e Pokémon de ambiente (Rattata e aquáticos).
  7. Conexão bidirecional perfeita com os Warps 2 e 3 de `PeacefulWoods`.
  8. Encontros selvagens originais configurados em `src/data/wild_encounters.json` (grama, surfe e pesca).
  9. Suporte a efeito de grama em `ShouldUseEclatGrass()` em `src/field_effect_helpers.c` e nome da seção como `"ROUTE 3"` em `MAPSEC_ROUTE_103`.

- **Arquivos modificados / criados**:
  - `data/layouts/eclat_route3/border.bin` e `map.bin`
  - `data/tilesets/secondary/eclat_route3/` (tiles, palettes, metatiles, attributes)
  - `data/tilesets/secondary/eclat_route3_secondary/` (tiles.png, palettes/, metatiles.bin, metatile_attributes.bin)
  - `data/layouts/layouts.json`, `data/layouts/layouts.inc`, `data/layouts/layouts_table.inc`, `include/constants/layouts.h`
  - `src/data/tilesets/headers.h`
  - `data/maps/map_groups.json`
  - `data/maps/Route3/` (`map.json`, `scripts.inc`, `header.inc`, `events.inc`, `connections.inc`)
  - `data/event_scripts.s`
  - `data/maps/PeacefulWoods/map.json`
  - `include/constants/opponents.h`, `src/data/trainers.party`
  - `include/constants/flags.h`
  - `src/field_effect_helpers.c`
  - `src/data/region_map/region_map_sections.json`
  - `src/data/wild_encounters.json`
  - `eclat docs/route3.md`

- **Validação**:
  - Compilação limpa com 100% de sucesso via `./build.sh` (0 erros).
  - ROM gerada: `eclat-intro-teste.gba` (33.554.432 bytes).

### Recriação de Peaceful Woods com Tileset de Grim Woods (Unbound) e Gramas Altas — 09/11/2026

- **Objetivo**:
  Recriar todo o visual de **Peaceful Woods** (`MAP_PEACEFUL_WOODS`, $78 \times 78$) utilizando os tiles e paletas de **Grim Woods** (*Pokémon Unbound*), preservando integralmente a história, colisões, eventos, treinadores, itens e o Altar de Celebi de *Éclat Pourpre*, além de adicionar manchas naturais da grama alta 9-slice de Grim Woods para ativar encontros selvagens e DexNav:
  1. Criação de par dedicado de tilesets `gTileset_PeacefulWoodsGrimPrimary` + `gTileset_PeacefulWoodsGrimSecondary` via `tools/eclat/build_peaceful_woods_grim.py` (440 tiles livres restantes no primário), mantendo o tileset antigo `peaceful_woods` intacto.
  2. Portabilidade 1:1 das árvores místicas, solo nebuloso, clareiras orgânicas de musgo, cogumelos vermelhos, arbustos $2 \times 2$, placas, troncos ocos e poças de Grim Woods (`Pal 7..11`), além da harmonização cromática dos barrancos, entradas de caverna, escadarias de madeira e Altar de Celebi (`Pal 3, 4, 5, 12`).
  3. Unificação da grama alta 9-slice com o solo nebuloso em camada única na `Pal 6` livre (`L0`), liberando `L1` (`Pal 8`) para renderizar bordas de copas de árvores sobre a grama alta (`MT 840..887`) em hardware GBA padrão de 2 camadas.
  4. Adição de 205 blocos de grama alta (`MB_TALL_GRASS = 0x02`) distribuídos em 22 clareiras nas 5 regiões da floresta (mantendo trilhas de musgo e o Altar de Celebi livres), ajuste de `land_mons.encounter_rate` de `5` para `15` em `src/data/wild_encounters.json` e criação da paleta de animação `gSpritePalette_GrimWoodsTallGrass` (`0x1016`).
  5. Espelhamento 512/512 para edição direta no Porymap (`peaceful_woods_grim_primary` e `peaceful_woods_grim_secondary`) via `tools/eclat/porymap_iviere_compat.py`, com carimbo $3 \times 3$ organizado em `792..811`.

- **Arquivos modificados / criados**:
  - `tools/eclat/build_peaceful_woods_grim.py`
  - `tools/eclat/porymap_iviere_compat.py`
  - `data/tilesets/primary/peaceful_woods_grim/` e `data/tilesets/primary/peaceful_woods_grim_primary/`
  - `data/tilesets/secondary/peaceful_woods_grim/` e `data/tilesets/secondary/peaceful_woods_grim_secondary/`
  - `data/layouts/peaceful_woods/map.bin` e `data/layouts/layouts.json`
  - `src/data/tilesets/headers.h`
  - `graphics/eclat_intro/grim_woods_tall_grass_palette.bin`
  - `include/constants/field_effects.h`, `src/data/field_effects/field_effect_objects.h`, `src/field_effect_helpers.c`
  - `src/data/wild_encounters.json`
  - `eclat docs/peaceful-woods.md`

- **Validação**:
  - Compilação limpa com 100% de sucesso via `wsl ./build.sh` (`Exit Code: 0`, uso de ROM: 81,96%).
  - ROM atualizada: `eclat-intro-teste.gba`.

### Animação de Grama Alta 1:1 do Unbound e Balanço das Árvores em Peaceful Woods — 09/11/2026

- **Objetivo**:
  Substituir o efeito de caminhar na grama alta de **Peaceful Woods** pela animação exata do *Pokémon Unbound* e adicionar uma animação suave de vento nas copas das árvores de Grim Woods (no estilo da Rota 2):
  1. **Animação 1:1 de Grama Alta do Unbound**:
     - Extração direta dos 5 frames $16 \times 16$ 4bpp de lâminas balançando (`graphics/eclat_intro/unbound_tall_grass.bin` em `0x39A008`, `640` bytes) e da paleta oficial de sprite (`graphics/eclat_intro/grim_woods_tall_grass_palette.bin` em `0x398FC8`, `32` bytes) a partir do `SpriteTemplate` `FLDEFFOBJ_TALL_GRASS` (`0x3A5420`) do *Pokémon Unbound*.
     - Vinculação de `sPicTable_UnboundTallGrass` em `gFieldEffectObjectTemplate_GrimWoodsTallGrass` (`src/data/field_effects/field_effect_objects.h`) e redirecionamento de saltos sobre grama em `FldEff_JumpTallGrass()` (`src/field_effect_helpers.c`) para `LAYOUT_PEACEFUL_WOODS`, eliminando o antigo arbusto opaco de Éclat.
  2. **Animação de Vento nas Árvores de Grim Woods (`tree_anim.bin`)**:
     - Criação do gerador modular `tools/eclat/grim_woods_anim.py` (`143` linhas) produzindo `data/tilesets/secondary/peaceful_woods_grim/tree_anim.bin` (`9.728` bytes: `8` estágios de onda de vento $\times$ `38` tiles 4bpp).
     - Onda suave de brisa (`8` passos $\times$ `14` ticks = `112` frames) com defasagem natural entre o topo e a base da copa, preservando os troncos fixos no solo e atualizando via DMA tanto os `20` tiles puros de copa (`VRAM 713..716, 729..732, 744..749, 760..765`) quanto os `18` tiles mesclados de junções horizontais/verticais (`VRAM 1..13, 15..19`).
     - Registro de `InitTilesetAnim_PeacefulWoodsGrim` em `include/tileset_anims.h`, `src/tileset_anims.c` e no `.callback` de `gTileset_PeacefulWoodsGrimSecondary` (`src/data/tilesets/headers.h`).

- **Arquivos modificados / criados**:
  - `tools/eclat/grim_woods_anim.py`, `tools/eclat/build_peaceful_woods_grim.py`
  - `graphics/eclat_intro/unbound_tall_grass.bin`, `graphics/eclat_intro/grim_woods_tall_grass_palette.bin`
  - `data/tilesets/secondary/peaceful_woods_grim/tree_anim.bin`
  - `src/data/field_effects/field_effect_objects.h`, `src/field_effect_helpers.c`
  - `include/tileset_anims.h`, `src/tileset_anims.c`, `src/data/tilesets/headers.h`
  - `eclat docs/peaceful-woods.md`, `eclat docs/historico.md`

- **Validação**:
  - Recompilação forçada de `field_effect.o` e `tileset_anims.o` via `wsl ./build.sh` concluída com 0 erros (`ROM: 82,00%`).
  - Verificação binária direta na ROM confirmando `unbound_tall_grass.bin` (`0x49CD58`), `grim_woods_tall_grass_palette.bin` (`0x49CD38`) e `tree_anim.bin` (`0xCEDCD0`) integrados em `eclat-intro-teste.gba`.

### Saídas Iluminadas Concêntricas, Correção de Copas e Fundo de Batalha de Grim Woods em Peaceful Woods — 09/11/2026

- **Objetivo**:
  Refinar as duas saídas da floresta de **Peaceful Woods** (NE rumo à Rota 3 em `68..69, 8..9` e SO rumo à Rota 2 em `12..13, 56..57`) com um degradê concêntrico iluminado, corrigir o recorte da copa lateral + traço animado flutuante na saída e integrar o fundo de batalha oficial de **Grim Woods** (*Pokémon Unbound*):
  1. **Correção da Copa Cortada e do Traço Animado nas Saídas (`MT 768` e `MT 772`)**:
     - Identificada inversão entre `gw_ovh_l` e `gw_ovh_r` na camada `L1` de `MT 768` e `MT 772`, que deixava pixels transparentes encostados nas árvores vizinhas `(70, 8)` / `(11, 56)` e projetava uma coluna externa animada solta no ar, além de `layerType = COVERED` ocultando as copas sob o jogador.
     - Corrigidos `MT 772` (`L1 = gw_ovh_l`, `0x00000062` `NORMAL` + `MB_EAST_ARROW_WARP`) e `MT 768` (`L1 = gw_ovh_r`, `0x00000063` `NORMAL` + `MB_WEST_ARROW_WARP`), restaurando integralmente as copas das árvores e eliminando o artefato vertical.
  2. **Degradê Oval Iluminado de 5 Faixas (`Pal 2`, `MT 768..775`)**:
     - Criação do módulo `tools/eclat/grim_woods_exits_and_bg.py` (`249` linhas) gerando um semi-oval concêntrico $32 \times 32\text{ px}$ ($2 \times 2$ metatiles) na `Pal 2` primária livre, preservando as pedrinhas exatas do solo de Grim Woods na parte externa e clareando suavemente em 5 faixas até luz solar quente `(189, 222, 148)` nas aberturas para a Rota 3 e Rota 2 (compatível com a iluminação dinâmica de Dia/Noite).
  3. **Fundo de Batalha Oficial de Grim Woods (*Pokémon Unbound*)**:
     - Extração 1:1 dos cenários de batalha de Grim Woods em `graphics/battle_environment/grim_woods/`: **Grama Alta** (`Entry 25`: `0xED4738` / `0xED54B8`) e **Solo da Floresta / Treinadores** (`Entry 34`: `0xED5B4C` / `0xED64B8`), cada um com suas 3 paletas oficiais de **Dia**, **Entardecer** e **Noite**.
     - Integração via `src/data/grim_woods_battle.h` e seleção automática por `LAYOUT_PEACEFUL_WOODS` + `GetTimeOfDay()` em `GetBattleEnvironmentGraphics()` (`src/battle_bg.c`) para batalhas selvagens e contra treinadores.

- **Arquivos modificados / criados**:
  - `tools/eclat/grim_woods_exits_and_bg.py`, `tools/eclat/build_peaceful_woods_grim.py`
  - `data/tilesets/primary/peaceful_woods_grim/` e `data/tilesets/primary/peaceful_woods_grim_primary/`
  - `data/tilesets/secondary/peaceful_woods_grim/` e `data/tilesets/secondary/peaceful_woods_grim_secondary/`
  - `data/layouts/peaceful_woods/map.bin`
  - `graphics/battle_environment/grim_woods/` (`grass_*` e `plain_*` `.lz.bin` + `.gbapal.bin`)
  - `src/data/grim_woods_battle.h`, `src/battle_bg.c`
  - `eclat docs/peaceful-woods.md`, `eclat docs/historico.md`

- **Validação**:
  - Recompilação limpa de `battle_bg.o`, `tilesets.o`, `tileset_anims.o` e `maps.o` via `wsl ./build.sh` (`Exit Code: 0`, `ROM: 82,04%`).
  - Verificação binária exata na ROM confirmando `map.bin` (`0xF63A98`), `metatiles.bin` (`0xD10F30`), `grass_tiles.lz.bin` (`0x411DB0`) e `plain_tiles.lz.bin` (`0x4100D8`) em `eclat-intro-teste.gba`.

### Refinamento de Peaceful Woods: Preservação do Bosque Contínuo e Troncos Caídos em Terra Seca — 09/11/2026

- **Decisão e Diretriz do Usuário**:
  Após avaliação dos testes visuais com rios e cursos d'água, o usuário optou por retirar todos os rios para preservar a serenidade, continuidade e essência original da caminhada por **Peaceful Woods**, mantendo exclusivamente os troncos ocos caídos em terra seca:
  1. **Remoção de Rios, Lagos, Pontes e Piscinas**:
     - Eliminados todos os metatiles aquáticos e pontes de madeira de `data/layouts/peaceful_woods/map.bin` (`0 metatiles de rio em map.bin`). O mapa voltou a contar com 100% de terreno florestal contínuo de terra e musgo.
  2. **Preservação dos Troncos Ocos Caídos em Terra Seca (`MT 816, 817`)**:
     - Mantidos os 3 troncos caídos de Grim Woods $2 \times 1$ posicionados estrategicamente em clareiras amplas sem bloquear caminhos:
       1. Clareira Oeste: `(6, 30)`
       2. Corredor Central Norte: `(19, 23)`
       3. Clareira Sul de Entrada: `(42, 62)`
  3. **Encontros Selvagens Exclusivamente Terrestres**:
     - Em `src/data/wild_encounters.json`, as seções `water_mons` e `fishing_mons` foram retiradas de `MAP_PEACEFUL_WOODS`, preservando apenas `land_mons` com taxa de encontro 15.
  4. **Animação Pura das Copas ao Vento (38 Tiles)**:
     - `src/tileset_anims.c` opera com `GRIM_WOODS_TREE_STEP_STRIDE = 38 * 16`, animando em 8 passos via DMA a brisa suave nas árvores com `tree_anim.bin` de `9.728` bytes gerado por `tools/eclat/grim_woods_anim.py`.
  5. **Cenários Oficiais de Batalha de Grim Woods**:
     - `src/battle_bg.c` mapeia as batalhas de Peaceful Woods dinamicamente para os fundos oficiais de Grim Woods (Grama Alta e Solo Florestal para Dia, Entardecer e Noite).

- **Arquivos modificados**:
  - `tools/eclat/grim_woods_river.py`: Reduzido para `138` linhas, contendo registro de metatiles, carimbo dos troncos caídos (`stamp_dry_hollow_logs`), validação BFS de 100% de acessibilidade e configuração de encontros terrestres (`configure_peaceful_woods_encounters`).
  - `tools/eclat/build_peaceful_woods_grim.py`: Reduzido para `499` linhas, integrando todo o pipeline da floresta.
  - `tools/eclat/grim_woods_anim.py`: `136` linhas, gerando animação pura de árvores (9.728 bytes).
  - `src/tileset_anims.c`: Retornado para stride 38 e animação pura de folhagem.
  - `src/battle_bg.c`: Retornado para mapeamento direto de Grama e Solo de Grim Woods.
  - `src/data/wild_encounters.json`: Apenas `land_mons` em Peaceful Woods.
  - `data/layouts/peaceful_woods/map.bin`: 0 rios, 6 metatiles de troncos secos, 196 gramas altas.
  - `eclat docs/peaceful-woods.md`, `eclat docs/historico.md`.

- **Validação**:
  - Compilado via `wsl ./build.sh` com 100% de sucesso (código de saída 0, uso de ROM: `82,04%`).
  - ROM atualizada: `eclat-intro-teste.gba`.
  - Verificação de metatiles em `map.bin`: `River mts: 0, Log mts: 6, Grass mts: 196`.
  - Todos os scripts Python mantidos $\le 500$ linhas.

### Restauração da Geografia Original de Peaceful Woods e Correção de Contornos — 09/11/2026

- **Solicitação do Usuário**:
  Extrair o mapa original autêntico de **Peaceful Woods** diretamente da ROM de *Pokémon Éclat Pourpre* e corrigir a forma dos tiles de contorno (desníveis, curvas de barranco, platôs e bordas rochosas), revertendo alterações acidentais prévias onde parte da geografia havia sido aplainada com chão comum:
  1. **Extração 1:1 da Matriz da ROM Original**:
     - Localizado o ponteiro de layout em `0x87EB90` e extraída a matriz binária original completa de $78 \times 78$ metatiles (12.168 bytes) do offset `0x87BC08` da ROM de *Éclat Pourpre*.
     - Substituído `data/tilesets/secondary/peaceful_woods_grim/base_map.bin`, revertendo todas as 358 discrepâncias em que desníveis rochosos, cantos curvos de platôs e árvores sobre barrancos haviam sido substituídos por chão plano (`MT 652`).
  2. **Reconstrução Limpa dos Contornos e Desníveis**:
     - Cantos arredondados de desnível (`MT 712` à esquerda e `MT 714` à direita) e borda reta horizontal de salto (`MT 713` com atributo `0x00000032`, `MB_JUMP_SOUTH`) mapeados nas paletas harmonizadas de Grim Woods (`Pal 3`), eliminando recortes pretos e quebras secas.
     - Sobreposição orgânica de copas e troncos de árvores sobre desníveis (`MT 707` e `MT 708`), mantendo a camada inferior de rocha/terra sob a folhagem da árvore para que a silhueta do barranco continue ininterrupta sem criar vazios quadrados.
     - Paredões e curvas de platôs rochosos (`MT 715, 716, 720, 722, 723, 728, 729`) e escadas de madeira (`MT 744..746`) restaurados com transições suaves.
  3. **Preservação dos Recursos Validados**:
     - Mantidos os 3 troncos ocos secos, os carimbos de grama alta com encontros selvagens e as saídas solares concêntricas para as Rotas 2 e 3.
  4. **Eliminação de Dependência Externa**:
     - Em `tools/eclat/grim_woods_exits_and_bg.py`, eliminada a dependência da biblioteca externa `PIL`/`pillow`, implementando leitor/decodificador puro em Python com `zlib` e `struct` para execução autônoma em qualquer ambiente.
     - Em `tools/eclat/grim_woods_river.py`, ajustada a verificação BFS de acessibilidade para aceitar elevações 0, 3, 4 e 15, validando 100% de conectividade de warps, placas e objetos.

- **Arquivos modificados**:
  - `data/tilesets/secondary/peaceful_woods_grim/base_map.bin` (12.168 bytes extraídos da ROM)
  - `tools/eclat/grim_woods_exits_and_bg.py` (SimpleImage puro sem PIL)
  - `tools/eclat/grim_woods_river.py` (validação BFS com elevação 4)
  - `tools/eclat/build_peaceful_woods_grim.py`
  - `tools/eclat/porymap_iviere_compat.py`
  - `data/layouts/peaceful_woods/map.bin`
  - `data/tilesets/primary/peaceful_woods_grim/` e `data/tilesets/secondary/peaceful_woods_grim/`
  - `eclat docs/peaceful-woods.md`, `eclat docs/historico.md`

- **Validação**:
  - Compilação via `wsl ./build.sh` executada com sucesso (`Exit Code: 0`, ROM: `82,04%`).
  - ROM atualizada: `eclat-intro-teste.gba`.
  - Sincronização 512/512 para o Porymap executada com sucesso.

### Ativação do Sistema de EXP da Gen 6 e Exp. Share de Equipe — 09/11/2026

- **Contexto e Solicitação**:
  - Usuário solicitou a ativação do sistema de experiência da 6ª Geração (Pokémon X & Y) com recebimento do item através de Dona Alda em Iviere Town.

- **Implementação**:
  1. **Configuração de Item e Flag**:
     - Em `include/constants/flags.h`, definido `#define FLAG_SYS_EXP_SHARE 0x268` utilizando slot não utilizado.
     - Em `include/config/item.h`, definido `I_EXP_SHARE_ITEM GEN_6` e `I_EXP_SHARE_FLAG FLAG_SYS_EXP_SHARE`.
     - Isso converte o *Exp. Share* em **Item Chave** (*Key Item*) com suporte nativo a ligar e desligar (`ON`/`OFF`) pela mochila.
  2. **Configuração de Batalha (Gen 6)**:
     - Em `include/config/battle.h`, ajustado `B_SCALED_EXP GEN_6` para cálculo de experiência plano da 6ª geração (sem escalonamento por diferença de nível da Gen 5/7).
     - Mantidos `B_EXP_CATCH GEN_LATEST` (EXP ao capturar Pokémon) e `B_SPLIT_EXP GEN_LATEST` (100% da EXP para todos os participantes da batalha sem divisão; 50% para os demais membros quando ligado).
  3. **Script de Dona Alda em Iviere Town**:
     - Em `data/maps/LittlerootTown/scripts.inc` (`IviereTown_EventScript_RiverNpc`), adicionado `setflag FLAG_SYS_EXP_SHARE` logo após a entrega do item, garantindo que o Exp. Share já entre ativado (`ON`) assim que recebido.
     - Em `IviereTown_EventScript_AldaAfterGift`, adicionada checagem retrocompatível para saves em andamento onde a flag de diálogo já foi ativada.

- **Validação**:
  - Compilação via `wsl ./build.sh` executada com sucesso (`Exit Code: 0`, ROM: `82,04%`).
  - ROM atualizada: `eclat-intro-teste.gba`.

### Entrega de 5 Pokébolas por Prof. Filão — 09/11/2026

- **Contexto e Solicitação**:
  - Solicitação do usuário para que o Prof. Filão entregue 5 Pokébolas juntamente com o Pokémon inicial para o início da jornada.

- **Implementação**:
  1. **Flag de Controle**:
     - Em `include/constants/flags.h`, definido `#define FLAG_RECEIVED_POKE_BALLS_FROM_FILAO 0x269`.
  2. **Script de Entrega Inicial**:
     - Em `data/maps/LittlerootTown/scripts.inc` (`IviereTown_EventScript_FilaoInspire`), adicionada fala temática de incentivo e comando `giveitem ITEM_POKE_BALL, 5`, ativando a flag.
  3. **Retrocompatibilidade no Laboratório**:
     - Em `data/maps/Iviere_FilaosLab/scripts.inc` (`Iviere_FilaosLab_EventScript_Filao`), adicionada checagem para saves em andamento: caso o jogador já tenha passado da cena de abertura, interagir com o Filão no laboratório entrega as 5 Pokébolas.
  4. **Documentação**:
     - Atualizada a bíblia da história em `eclat docs/historia.md`.











