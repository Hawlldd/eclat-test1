# Metodologia do port

O port usa a ROM de Éclat Pourpre como referência para recuperar gráficos, mapas, falas e eventos. O Pokémon Emerald Expansion 1.17.2 fornece as rotinas que executam esses dados. O trabalho alternou entre leitura dos arquivos, observação quadro a quadro no mGBA e implementação no código-fonte.

## Como usar este documento

Se você está assumindo o port, comece pelo [histórico](historico.md) para descobrir qual commit introduziu o comportamento. Use as seções 6 e 7 para seguir os dados da ROM até o código. A seção 8 mostra como retomar a compilação e a validação local. A seção 9 explica como identificar um sprite pela memória de vídeo e decidir como portá-lo.

## 1. Fixar a referência e observar o jogo

A referência local usada foi `eclat/Eclat Pourpre ENG (Raizen).gba`, na pasta acima do projeto. O SHA-256 da cópia analisada é `9dadfec5e707e8be3e8f5f12dfd3bfd1b7448ae78c7d955f8e9593c91c7c045a`. Os offsets abaixo se referem a essa cópia.

Antes de editar, percorremos a cena no emulador com as mesmas entradas que o jogador faria. Registramos a ordem das falas, quando o controle volta, de onde a câmera parte, onde os objetos aparecem e como mudam de direção. Capturas sucessivas permitiram ver transições e animações que uma imagem isolada não mostra. Para entender a origem de um efeito, comparamos os frames da tela com as posições dos sprites e os gráficos que a ROM carregava na memória de vídeo.

## 2. Recuperar os dados que explicam a cena

- **Texto e eventos.** A tabela de caracteres permitiu decodificar as falas. A partir delas, seguimos referências na ROM até os scripts correspondentes e reconstruímos a ordem dos comandos, as esperas, os movimentos e as condições. A fala pós-roubo da mãe, por exemplo, foi localizada no offset de arquivo `0x96CFD8`.
- **Gráficos e paletas.** Recuperamos os tiles, a composição da tela e as paletas da transmissão. Para os sprites de campo, seguimos a tabela de gráficos até os descritores e as tabelas de quadros. As capturas ajudaram a conferir a paleta, a orientação e a ordem dos quadros.
- **Mapas.** Separamos layout, tiles, metatiles, paletas, atributos de terreno, objetos e destinos das portas. Os atributos de comportamento foram comparados separadamente do desenho. Um chão com o visual certo ainda podia acionar o efeito de grama ou de água se usasse o comportamento errado no Emerald.
- **Tempo e posição.** Medimos a ordem dos quadros, o deslocamento dos sprites e as fases de atualização dos tiles. Observamos tanto o que acontece com o jogador parado quanto o que muda quando a câmera se move ou quando o mapa é carregado de novo.

### Exemplo dos seis Pidgeot

A tabela de eventos de Iviere continha seis objetos com o gráfico `152` e o movimento horizontal `0x1B`. O ponteiro de gráficos no offset `0x80E4B4` levou ao descritor em `0xC06E6C` e à tabela de quadros em `0xC07E0C`. A análise do emulador confirmou, no trecho perto da casa, um deslocamento de um pixel por quadro e a ida e volta em 288 quadros para o alcance de nove tiles.

| X inicial | Y | Alcance para a esquerda em tiles |
| --- | --- | --- |
| 16 | 20 | 9 |
| 23 | 23 | 9 |
| 15 | 21 | 7 |
| 33 | 5 | 9 |
| 29 | 12 | 9 |
| 34 | 4 | 9 |

As coordenadas são do mapa. Em `src/field_specials.c`, cada instância guarda a rota, o avanço e a direção. A posição de tela deriva da posição no mapa e do deslocamento da câmera. A animação é espelhada na virada, e o script de Iviere cria as instâncias que estiverem faltando ao retomar o mapa.

## 3. Adaptar os dados ao Emerald

| Parte | Implementação |
| --- | --- |
| Transmissão e nomes | [Menu principal](../src/main_menu.c) para a sequência, o retorno à transmissão e a confirmação. [Tela de nomes](../src/naming_screen.c) para validar a entrada e mostrar o ícone apropriado. [Gráficos da abertura](../graphics/eclat_intro/) para os recursos recuperados. |
| Quarto | [Script do segundo andar](../data/maps/LittlerootTown_BrendansHouse_2F/scripts.inc) para menus, entrega do inicial e estado da introdução. [Código de Pokémon](../src/pokemon.c) para aplicar a taxa shiny escolhida, respeitando as regras e tentativas adicionais da engine. |
| Roubo | [Script do térreo](../data/maps/LittlerootTown_BrendansHouse_1F/scripts.inc) para as falas, os movimentos e o fim da cena. O estado da introdução muda antes de liberar o controle para impedir que a cena recomece. A porta do mapa faz a saída normal. |
| Iviere e interiores | [Mapa e scripts de Littleroot](../data/maps/LittlerootTown/) para a cidade portada, [casa de Filao](../data/maps/Iviere_FilaosHouse/) e [laboratório](../data/maps/Iviere_FilaosLab/) para os interiores. [Animações de tiles](../src/tileset_anims.c) para carregar os quadros nas fases medidas. |
| Interações | O Name Rater reaproveita as rotinas do Emerald para escolher um Pokémon da equipe ou do Box. O Town Map é um objeto sobre a mesa e só some após a entrega do item ser confirmada. |

Os gráficos e as falas recuperados servem à fidelidade visual e narrativa. As rotinas, os identificadores de terreno e os scripts de execução foram adaptados ao Emerald. A inclusão dos iniciais de Paldea foi uma decisão de escopo do port.

## 4. Compilar e comparar

A branch `eclat-intro` mantém o port separado da base. Para compilar, abra o Ubuntu com `wsl` a partir do PowerShell e rode os comandos de Linux dentro dele. A compilação usada no projeto rodou na cópia de trabalho do Linux, com o compilador ARM disponível no WSL.

```bash
cd /caminho/da/copia-de-compilacao
make -j"$(nproc)" emerald
```

O alvo gera `pokeemerald.gba` e `pokeemerald.elf`. A ROM vai para a pasta de teste como `eclat-intro-teste.gba`. O ELF permite localizar símbolos e associar o que o teste observa na memória ao código da mesma compilação. Para reproduzir um teste de introdução, inicie um Novo Jogo no mGBA com a ROM recém-gerada.

Os testes automatizados desta etapa dirigiram o mGBA com sequências de botões e observaram os mapas, as coordenadas, o estado do evento e a memória gráfica. Os roteiros e capturas foram mantidos na área de trabalho usada para análise, fora do repositório.

## 5. Validar pelo percurso do jogador

- Nos dois nomes, tentar confirmar sem digitar ou usando somente espaços. Digitar depois da recusa, voltar da confirmação para corrigir e conferir a paleta ao retomar a transmissão.
- Observar os quatro quadros de encolhimento antes do quarto. Conferir a taxa shiny selecionada e a entrega do inicial. O teste mais recente escolheu Fuecoco.
- Descer a escada, acompanhar o movimento da câmera e terminar o roubo. Confirmar que o jogador fica no mesmo ponto dentro da casa, que a mãe usa a fala correta e que a porta funciona ao sair por conta própria.
- Em Iviere, conferir as seis instâncias de voo, a posição relativa ao mapa quando a câmera se move, o espelhamento, a animação do Pidgey e o retorno das aves depois de entrar e sair de uma casa. Passar pelos caminhos e pelas escadas secas para conferir os efeitos de terreno.
- Abrir as opções equipe e Box no Name Rater. Na casa de Filao, conversar com a esposa e depois examinar o objeto sobre a mesa para receber o Town Map. Entrar no laboratório, abrir o diálogo de cura e voltar à cidade.

A validação é por cena e por caminho testado. As duas execuções confirmaram a abertura da equipe e do Box pelo Name Rater e a gravação das duas escolhas de shiny. A frequência de shinies e os eventos posteriores ao trecho inicial ainda exigem testes próprios.

## 6. Reproduzir a extração em vez de adivinhar offsets

A ROM do GBA está mapeada a partir de `0x08000000`. Na região usada por esta referência, um ponteiro de ROM como `0x08712A2C` vira o offset de arquivo `0x712A2C`. Leia ponteiros e palavras em little-endian. Confira se cada ponteiro convertido cai dentro do arquivo e se a estrutura seguinte tem valores plausíveis. Uma ocorrência isolada de quatro bytes pode coincidir por acaso.

### Encontrar um mapa novo

1. Entre no mapa no emulador da ROM de referência. Capture a tela, EWRAM, VRAM de fundo, paletas e, se houver sprites relevantes, VRAM de objetos e OAM. Compare duas capturas com o jogador e a câmera no mesmo lugar.
2. Localize o cabeçalho de mapa ativo. Nesta ROM, as capturas usadas em `map-objects.py` tinham os ponteiros de layout e de eventos no offset `0x36DFC` do dump de EWRAM. Esse endereço é um achado específico da referência. Confira a largura, a altura e os ponteiros para borda, células e tilesets antes de usar a estrutura.
3. Siga os ponteiros para a ROM. Em Iviere, o layout é `0x08870E48` e mede 48 × 38 metatiles. O cabeçalho de mapa no offset `0x34AA68` aponta para esse layout e para os eventos em `0x08712E64`. O primeiro byte dos eventos é a contagem de objetos, e o ponteiro em `eventos + 4` leva à tabela em `0x08712A2C`.
4. Leia os objetos e portas antes de mover qualquer coisa no Emerald. Os registros de objeto usados aqui têm 24 bytes. O ID local está em `+0`, o gráfico em `+1`, X/Y em `+4/+6` como inteiros de 16 bits com sinal, o movimento em `+9` e os alcances X/Y nos nibbles baixo/alto de `+10`. Confira o ID local contra o script que chama `applymovement`, `removeobject` ou um comando equivalente.
5. Faça um render do mapa e compare-o com a captura. Use `width * height * 2` para as células de mapa. Para o layout usado aqui, os 10 bits baixos da célula são o ID do metatile, os bits 10–11 codificam colisão e os bits 12–15, elevação. No formato de tiles importado, cada metatile tem duas camadas de quatro entradas de 16 bits. Compare os bytes do tileset descomprimido com o que foi carregado na VRAM quando houver animação.

O trecho abaixo é somente leitura. Salve-o como arquivo temporário e execute com Python 3, passando o caminho da ROM entre aspas. Ele segue os ponteiros desde o cabeçalho e imprime `(id local, x, y, alcance horizontal)`.

```python
from pathlib import Path
from struct import unpack_from
from sys import argv

rom = Path(argv[1]).read_bytes()

def u32(offset):
    return unpack_from("<I", rom, offset)[0]

def offset(pointer):
    value = pointer - 0x08000000
    assert 0 <= value < len(rom), hex(pointer)
    return value

header = 0x34AA68
assert u32(header) == 0x08870E48
events = offset(u32(header + 4))
objects = offset(u32(events + 4))
assert rom[events] == 33

routes = []
for index in range(rom[events]):
    entry = objects + 24 * index
    if rom[entry + 1] != 152 or rom[entry + 9] != 0x1B:
        continue
    x, y = unpack_from("<hh", rom, entry + 4)
    routes.append((rom[entry], x, y, rom[entry + 10] & 0xF))

assert len(routes) == 6, routes
for route in routes:
    print(route)
```

A ordem esperada dos IDs é `5, 6, 7, 11, 13, 16`. Compare o restante de cada linha com a tabela da seção 2. Use a tabela de gráficos da ROM modificada. A tabela de uma base FireRed limpa levou a uma identificação errada do gráfico `152` durante a primeira análise.

### Recuperar uma fala e conferir seu contexto

Procure a sequência de bytes da fala codificada e as ocorrências little-endian do ponteiro para ela. Por exemplo, a fala pós-roubo está em `0x96CFD8`, que corresponde ao ponteiro `0x0896CFD8`. O script relacionado começa no offset `0x96CFA8`. Leia a condição que escolhe a fala e confira a mesma interação no jogo. A introdução portada usa o ramo pós-roubo inicial. O outro ramo depende de progressão posterior da ROM de origem.

Os scripts de análise usaram `0xBB..0xD4` para A–Z, `0xD5..0xEE` para a–z, `0xA1..0xAA` para 0–9, `0x00` para espaço e `0xFF` como terminador. Preserve os controles `0xFE` (nova linha), `0xFA` (continuação com rolagem) e `0xFB` (nova página) ao reescrever com `\n`, `\l` e `\p` em `.string`. Valide que a caixa de texto avança como na referência.

### Conferir gráficos e duração

Para sprites de campo, filtre a OAM pela posição, dimensão e visibilidade. Em sprites 4bpp, o índice de tile aponta para blocos de 32 bytes e a paleta de objeto contém 16 cores de 16 bits por banco. Compare os bytes na VRAM de objetos com a ROM, ou siga o descritor até os quadros quando a busca direta retornar vários resultados. Para blocos GBA LZ77, o primeiro byte esperado é `0x10`, seguido do tamanho descomprimido em três bytes little-endian. Confira o resultado contra o gráfico carregado.

Na água e nos tiles animados de Iviere, o script `export-town-timing.py` comparou sequências de blocos de 32 bytes na VRAM de fundo. A sequência capturada repetiu após 256 quadros. O exportador guardou apenas as variantes distintas de cada faixa em `animation.bin` e os 256 índices da faixa em `animation_phases.bin`. `sIviereAnimatedRanges` relaciona o tile de destino, o offset em palavras de 16 bits e a contagem de tiles. `TilesetAnim_Iviere` só enfileira a variante quando a fase muda, com atualização inicial para preencher a VRAM.

## 7. Verificar as transições antes de alterar um subsistema

- **Nome e paleta.** `DoRequiredNamingScreen` ativa a validação específica da introdução. `SaveInputText` recusa uma entrada sem caracteres significativos e termina o texto válido com `EOS`. `CB2_EclatBroadcast_Resume` reconstrói a tela a partir de `sEclatPendingName`. `Task_EclatBroadcast_ConfirmName` escreve `gSaveBlock2Ptr->playerName` ou `gSaveBlock1Ptr->rivalName` só no “Sim”. A cena usa fundo 8bpp com paleta nos bancos baixos. A moldura de confirmação usa o banco BG 14 e o texto da transmissão, o 15. Reproduza a sequência vazio → texto válido → “Não” → editar → “Sim” antes de aceitar uma mudança de interface.
- **Quarto e roubo.** O novo jogo do Emerald aponta para `MAP_LITTLEROOT_TOWN_BRENDANS_HOUSE_2F` em `(6,5)`. O script do quarto roda em `VAR_LITTLEROOT_INTRO_STATE == 0`. A entrega do inicial altera a variável para `8`. O térreo usa esse valor para exibir os vilões e disparar a cena. A câmera temporária criada por `SpawnCameraObject` percorre `EclatHouse_Movement_PanToTable` e volta por `EclatHouse_Movement_PanToStairs`. Remova o objeto de câmera antes de devolver o controle. No fim, a variável vira `7` antes de `releaseall`. Os valores 8 e 7 são estados do script, com essa ordem intencional.
- **Shiny e iniciais.** O script do quarto alterna `FLAG_ECLAT_BOOSTED_SHINY_RATE` antes de `givemon`. Em `ComputePlayerShinyOdds`, a taxa de 1/400 usa `RandomUniform(RNG_NONE, 0, 399) == 0` para a tentativa inicial e para cada tentativa adicional concedida pela engine. A taxa normal usa `SHINY_ODDS`. O menu entrega o ID da espécie a `EclatIntro_ChooseStarter`. Teste voltar com B, recusar a confirmação e escolher novamente antes de conferir a entrega.
- **Formato do mapa e terreno.** Em `data/layouts/layouts.json`, os layouts portados usam `tile_format: frlg` para sinalizar o formato dos metatiles e bordas. `tools/mapjson/mapjson.cpp` passa essa informação ao cabeçalho gerado. Em caso de efeito de terreno incorreto, leia a célula em `map.bin`, encontre o ID do metatile e consulte o atributo no tileset primário ou secundário. Compare com `MapGridGetMetatileBehaviorAt` e com as constantes do Emerald. Os IDs 369, 925 e 933 foram checados com comportamento terrestre `0x4F`. Preserve o desenho e a colisão quando só o efeito estiver errado.
- **Voo e ciclo de vida.** `IviereTown_OnResume` chama `StartIviereFlyby`, que usa a função de callback de cada sprite para encontrar as rotas ativas. `SpriteCB_IviereFlyby` recebe em `data[0]` a rota, em `data[1]` a direção e em `data[2]` o avanço em pixels. O limite é `alcance * 16`. A coordenada do ponto inicial do trecho passa por `GetMapCoordsFromSpritePos` com `MAP_OFFSET`. O sprite usa `coordOffsetEnabled` e a visibilidade considera `gSpriteCoordOffsetX/Y`. Para detectar movimento que segue o jogador, compare a âncora em tela descontando `data[2]` antes e depois de andar um tile. Verifique também seis rotas únicas na rua, zero dentro da casa e seis na volta.
- **Pidgey e Pidgeot.** O Pidgey é um objeto de evento em `data/maps/LittlerootTown/map.json`. O tratamento de `OBJ_EVENT_GFX_ECLAT_TOWN_MON` em `MovementType_LookAround_Step2` mantém a animação ativa depois da virada. Os Pidgeot são sprites de campo iniciados pelo script de Iviere. Investigue a tabela de quadros do Pidgey quando ele parar de virar e a rotina de voo quando uma ave se deslocar ou espelhar incorretamente.
- **Portas e objetos persistentes.** A saída da casa usa os warps do mapa. Iviere tem portas para a casa do jogador em `(22,25)`, a casa de Filao em `(29,14)` e o laboratório em `(10,16)`. Ao editar um warp, verifique os índices nos dois mapas. A coleta do Town Map usa `FLAG_IVIERE_TOWN_MAP_TAKEN` e também verifica o inventário ao carregar o interior. Preserve a ordem entrega → verificação → remoção → flag para permitir nova tentativa quando a entrega falhar.

## 8. Retomar o trabalho em outra sessão

1. Abra o [histórico](historico.md) e compare o ponto de controle com `git status --short --branch` e `git log --oneline -12`. Preserve alterações locais que ainda estejam em andamento. Use `git show <commit> -- <caminho>` para entender a decisão antes de alterar o código.
2. Separe o que a ROM demonstra, o que o usuário pediu como adaptação e o que ainda precisa de confirmação. Registre o SHA da referência, o mapa, a coordenada, o evento, a evidência e a diferença encontrada no port.
3. Use o repositório do Windows como origem do código. No WSL, a última sessão guardou a localização da cópia de build em `/var/tmp/eclat-intro-build-path`. Antes de reutilizá-la, confira que os arquivos de entrada correspondem ao que está no Git e às mudanças locais. Caso o cache tenha sumido, prepare uma cópia de trabalho no sistema de arquivos do Linux com a mesma revisão e as alterações em teste. Compile antes de copiar o novo `.gba` para o Windows.
4. Na máquina deste projeto, os roteiros de análise ficaram em `C:/Users/raulr/Documents/Codex/2026-11-06/oi-chat-pdoeria-me-ajduar-a/work`. Entre eles estão `map-objects.py`, `export-maps.py`, `decode-transmission.py`, `record-flyers.c`, `export-town-timing.py` e `test-iviere-flock.c`. Alguns exportadores escrevem diretamente no projeto. Leia os destinos no código antes de executá-los.
5. Se a área de análise e os caches ainda existirem, rode o comando abaixo no WSL. O script usa a libmGBA localizada via `/var/tmp/eclat-mgba-path` e resolve símbolos do ELF por `arm-none-eabi-nm`. Os dois modos passaram. O modo `0` usa a taxa de 1/8192 e abre a equipe pelo Name Rater. O modo `1` usa 1/400 e abre o Box. Os arquivos temporários e a libmGBA ficam fora do Git. Se estiverem ausentes, use as verificações manuais da seção 5.

```bash
cd /mnt/c/Users/raulr/Documents/Codex/2026-11-06/oi-chat-pdoeria-me-ajduar-a/work
bash test-iviere-flock.sh 0
bash test-iviere-flock.sh 1
```

O teste acessa endereços derivados dos símbolos da compilação. Os offsets usados dentro de estruturas, como `struct Sprite` e o contexto do script, pertencem à revisão testada. Se a engine mudar, reconfira as estruturas no fonte e no ELF antes de interpretar uma falha. Para guardar o resultado, registre a revisão da ROM testada, as opções percorridas e quais verificações passaram.

## 9. Como reconhecer o sprite certo e portar o comportamento

Cada cena foi investigada em três etapas. Primeiro observamos o que aparece e quando muda. Depois identificamos os dados que a ROM realmente colocou na memória para produzir aquele quadro. Por fim, escolhemos o ponto correspondente no Emerald e validamos o resultado em movimento. Uma captura de tela permite localizar a cena e formular a hipótese. O cruzamento com a memória identifica qual recurso foi exibido.

### Ler um objeto da tela até a ROM

1. Capture o quadro com o objeto visível. Na OAM do GBA, a partir de `0x07000000`, cada entrada ocupa oito bytes. Leia `attr0`, `attr1` e `attr2` como palavras de 16 bits. Filtre por posição, dimensões, modo do sprite e visibilidade. Considere as dimensões completas ao inspecionar um objeto parcialmente fora da tela.
2. Nos sprites 4bpp estudados aqui, em modo OBJ 1D, o quadro começa em `0x06010000 + (attr2 & 0x3FF) * 32`. O banco de paleta começa em `0x05000200 + (attr2 >> 12) * 32`. Cada byte gráfico contém dois índices de cor. O nibble baixo representa o primeiro pixel do par. Para comparar um sprite completo, leia `largura * altura / 2` bytes de tiles e 32 bytes de paleta.
3. Procure os bytes dos tiles na ROM. Se a busca devolver mais de um endereço, confira outros quadros, a paleta e a tabela de imagens apontada pelo descritor. Para um quadro comprimido, compare os bytes depois de descomprimir ou rastreie o ponteiro usado pelo jogo. A posição na tela identifica qual entrada da OAM observar. A tabela de imagens identifica a origem do quadro.
4. Nos objetos comuns sem transformação afim, o bit 12 de `attr1` indica espelhamento horizontal. Ao comparar dois quadros, registre também X/Y, o ID do quadro, a paleta e o espelhamento. O mesmo tile em VRAM pode aparecer orientado para lados diferentes sem que os bytes do gráfico mudem.

### Casos que explicam as decisões

| Caso | Evidência e adaptação |
| --- | --- |
| **Protagonista e rival** | A referência guarda pares de ponteiro e tamanho, de oito bytes por entrada. As tabelas começam nos offsets `0x39A490` e `0x39A860`, com paletas em `0x355D58` e `0x367C78`. Cada quadro de 16 × 32 tem 256 bytes em 4bpp. O exportador `export-walk.py` reordena a segunda parte da tabela do protagonista para encaixar nos índices usados pelo Emerald. Para a rival, os nove quadros originais foram repetidos para preencher as 18 posições da tabela usada no port. O teste compara na memória de vídeo os bytes dos tiles e as 16 cores. A tela de nome e os objetos de campo têm pontos de configuração separados. |
| **Ícone da rival** | `NamingScreen_CreateRivalIcon` configura a pose do ícone. `MainState_FadeIn` também desloca dois tiles do fundo para acompanhar sua posição à direita. O teste verifica a posição e compara o quadro exibido em momentos separados, o que detecta a rotação mesmo que o primeiro quadro pareça correto. |
| **Pidgeot voando** | Dos nove quadros de 512 bytes listados no descritor do gráfico 152, os dois quadros selecionados para o voo no port correspondem byte a byte aos offsets `0xC0825C` e `0xC08C5C`. `sIviereFlybyAnims` alterna esses quadros a cada oito ticks. A animação sem espelhamento é usada ao ir para a esquerda e a espelhada ao ir para a direita. O teste observa a direção pela variação de X e confere o bit de espelhamento. O número de aves vem da tabela de objetos. A orientação vem da OAM e do movimento observado. |
| **Pidgey no chão** | Os três quadros de direção de 16 × 16 têm 128 bytes cada e correspondem aos offsets `0x38E698`, `0x38E718` e `0x38E798`. Para montar o ciclo de campo no Emerald, `iviere-houses-assets.py` gerou três quadros intermediários deslocando a pose um pixel para cima e deixando a última linha transparente. O script e a rotina de movimento do objeto determinam quando a ave vira. Essa parte combina poses recuperadas com quadros intermediários construídos para o port. |
| **Transmissão e encolhimento** | A transmissão foi recomposta de quadros capturados em 240 × 160. `broadcast-assets.py` converte as cores para 15 bits, indexa a paleta, deduplica tiles 8 × 8 em 8bpp e escreve um mapa de 32 × 32 por cena. O Emerald desenha o diálogo com a própria janela. Na transição, `export-trainer-shrink.py` isolou quatro silhuetas de 64 × 64 das capturas e as empacotou em 4bpp. `sEclatShrinkAnim` usa durações de 10, 20, 20 e 40 ticks. São recursos reconstruídos a partir da saída do emulador. |

### Escolher a implementação no Emerald

Comece pelo tipo de comportamento. Um NPC com colisão, diálogo e direção precisa de um objeto de evento com ID local e tabela de gráficos. Um voo decorativo precisa de sprite de campo com posição ancorada no mapa e ciclo de vida ligado à entrada na cidade. Um elemento de cenário que muda na mesma célula, como água, precisa de atualização de tiles na VRAM. Um evento com câmera e atores precisa de script, esperas e condições de disparo. Implementar os quatro como imagens sobrepostas perderia as regras que o jogador percebe ao se mover e interagir.

Ao validar, compare separadamente **identidade visual**, **sequência** e **interação**. A identidade visual cobre os tiles, a paleta e a pose. A sequência cobre direção, duração, câmera e ordem de falas. A interação cobre colisão, entrada e saída de mapas, retorno do controle e persistência de estado. Registre a origem de cada parte como extração direta, reconstrução por captura ou adaptação para a engine. Os testes deste projeto sustentam a fidelidade dos casos que exercitaram. A equivalência de cada quadro e de cada evento da campanha exigiria uma verificação mais ampla.

## 10. Modularidade de código e escalabilidade (Regra de ~500 linhas)

Para manter o projeto fácil de manter, escalável e com baixo risco de regressões ou conflitos de merge, adotamos a regra de **modularidade**:

1. **Meta de tamanho por arquivo**: Arquivos novos ou modificados de scripts, lógica C ou dados customizados devem manter um teto alvo de ~500 linhas.
2. **Decomposição por responsabilidade**:
   - **Scripts de mapas grandes (`scripts.inc`)**: Se um mapa acumular múltiplos fluxos (cutscenes, NPCs gerais, treinadores, portarias), deve-se subdividir em arquivos menores incluídos via `.include` (ex: `scripts_cutscenes.inc`, `scripts_npcs.inc`, `scripts_trainers.inc`).
   - **Código C de subsistemas**: Funções auxiliares, tabelas de dados/estruturas e rotinas de controle devem ficar em arquivos dedicados e coesos, expondo interfaces limpas via headers em `include/`.
   - **Tabelas de assets e gráficos**: Evitar concentrar descritores gigantescos em um único arquivo de código; separar por subsistema (ex: `battle_environment/`, `eclat_intro/`, `overworld_sprites/`).
3. **Refatoração contínua**: Sempre que um arquivo existente ultrapassar significativamente esse limite durante a adição de uma nova rota/cidade/recurso, planejar a separação em módulos antes de acumular mais complexidade.

## 11. Revisão técnica entre Gemini e ChatGPT

Use o [histórico](historico.md) para trocar achados entre sessões. Qualquer uma das IAs pode investigar, implementar ou revisar. Registre o nome de quem realmente executou cada etapa e identifique relatos trazidos pelo usuário. Uma pergunta para a outra IA fica em aberto até haver uma resposta registrada.

Para cada mudança, passe três informações à próxima pessoa ou IA:

1. **Evidência.** Informe a ROM e seu SHA-256, o mapa e a posição, o offset de arquivo ou endereço GBA e o que o emulador mostrou. Para gráficos, anote também a camada, o banco de paleta e quais quadros ou estados foram capturados.
2. **Implementação.** Informe os arquivos e símbolos alterados, as adaptações à engine e o comportamento esperado. Se houver trabalho local sem commit, inclua `git status --short` ou descreva o diff relevante para que a próxima IA preserve as alterações.
3. **Verificação.** Registre a ROM compilada, o roteiro executado, o resultado e os casos pendentes. Sempre que houver uma regra de escopo, inclua um controle fora dela. Por exemplo, para um cenário exclusivo de encontros selvagens no mato, compare uma batalha de treinador e uma batalha na água.

A referência de ~500 linhas da seção 10 é uma heurística para módulos novos ou que cresceram demais. Em arquivos existentes da engine, mantenha a alteração focada. Separe o código quando houver responsabilidades distintas que possam ser testadas por conta própria.

### Exercício de revisão para o Gemini

**Unbound.** Reproduza a leitura do ponteiro em `0x00F320` e da entrada em `0xEB9AC8` no arquivo da ROM local. Um ponteiro de ROM GBA perde a base `0x08000000` quando é convertido para offset de arquivo. Nos pares de referência do LZ77, combine primeiro os 12 bits e some 1 depois.

```python
distance = (((b1 & 0x0F) << 8) | b2) + 1
```

Teste o caso `b1 = 0x01`, `b2 = 0xFF`. A distância deve ser `512`. Os cinco blocos descomprimidos da entrada de grama têm, em ordem, **7776, 4096, 2560, 896 e 96 bytes**. No tilemap, os bits 12–15 selecionam o banco. Confira em qual banco a engine carrega o primeiro grupo de 16 cores antes de calcular o índice. Neste cenário, o primeiro grupo é carregado no banco BG 2. As duas plataformas já estão desenhadas no tilemap de fundo. O outro par de tiles e tilemap é usado na entrada da batalha.

**Animações de campo.** Confirme a posição do jogador antes de concluir que um efeito está ausente. Na Route 1, a primeira captura ficou em `(12,14)`, no caminho. Um passo para a direita, em `(13,14)`, revelou o sprite de grama na OAM. A VRAM mostrou a sequência `1, 2, 3, 4, 0`. Os quatro quadros de movimento duram 10 frames cada. A tabela de quadros começa em `0x39F7CC`, os tiles em `0x3943F8` e a paleta sem efeitos em `0x3933B8`, todos offsets do Éclat em inglês. Confirme o terreno e o movimento antes de comparar arte ou contagem de frames.

**Próxima revisão.** Para o HUD do Unbound, capture três estados com a mesma batalha e anote os recursos que mudam entre eles. Use a mensagem de abertura, o menu Fight/Bag/Pokémon/Run e a tela de escolha de golpe. A caixa de HP deve ser conferida também após perder HP ou receber um status. Separe o que vem das camadas BG do que vem da OAM. A integração do fundo já pode servir como controle, inclusive no retorno da bolsa.

## 12. HUD de batalha e tiles atualizados em tempo real

Compare primeiro o mesmo estado da batalha. O menu de ações aparece ao lado do diálogo roxo. Ao entrar em **Fight**, a tela mostra a lista de golpes e o painel de PP e tipo. As caixas de HP permanecem nos dois estados. Nesta implementação, as cores e as molduras foram adaptadas a partir das capturas do Unbound. Os controles e a disposição das janelas usam a engine do Emerald.

- **Janelas e texto.** `graphics/battle_interface/textbox_0.pal` e `textbox_1.pal` fornecem as cores da interface de fundo. Em `text.pal`, o índice 14 é o fundo branco dos comandos e dos golpes. Os índices 13 e 15 dão o texto escuro e a sombra. `gText_BattleMenu` define os rótulos. [Texto de batalha](../src/battle_message.c)
- **Caixas de HP.** Os PNGs indexados `healthbox_singles_player.png`, `healthbox_singles_opponent.png` e `healthbox_singles_opponent_large.png` mantêm as dimensões e os índices que a engine espera. O contorno branco, escuro e dourado é desenhado a partir da união entre o retângulo e a ponta. Isso evita uma borda vertical sobrando no encontro entre os dois. A paleta da caixa vem de `ball_status_bar.png`. [Gráficos da interface](../graphics/battle_interface/)
- **Faixa de experiência.** No ramo `EXP_BAR` de `MoveBattleBarGraphically`, a engine copia oito tiles de 8 × 8 sobre a parte inferior da caixa do jogador. Assim, os pixels pintados somente na imagem estática são substituídos. Os nove quadros de `expbar.png` cobrem preenchimentos de zero a oito pixels por tile. Cada quadro mantém as mesmas linhas de moldura. Somente os pixels internos variam. Confira as emendas com os pixels adjacentes da caixa em `x=31` e `x=96`. [Atualização da barra](../src/battle_interface.c)

Para repetir a revisão, execute o roteiro local `test-unbound-grass.sh 0` na área indicada na seção 8. O roteiro captura o menu de batalha e a seleção de golpes após voltar da bolsa. Compare a junção da ponta com a caixa e a linha inferior do EXP na tela do emulador. Se alterar `expbar.png`, compare as linhas fixas de todos os quadros e confira o preenchimento com EXP parcial. A variante adversária ampliada também recebeu o contorno atualizado. Ela precisa ser conferida no emulador com a configuração de porcentagem de HP ativada.


### Reextração das molduras originais

A segunda revisão leu os blocos LZ77 do Unbound 2.1.1.1 diretamente da ROM. Os offsets de arquivo são `0xEEF0AC` para a caixa do jogador, `0xEEF380` para a caixa adversária e `0xEEF360` para a paleta BGR555. A tabela de ponteiros dos sprites referencia os blocos em `0x260208` e `0x260210`. Cada caixa usa duas metades de 64 pixels de largura, armazenadas em sequência no binário. A captura serviu para confirmar a identificação das peças.

O port preserva a silhueta original, incluindo as pontas e a borda de EXP. Os índices de cor foram remapeados para manter o fundo no índice 2 e preservar os índices dinâmicos do Emerald. A região atrás da barra de HP recebe o fundo roxo. A engine continua a desenhar a barra, os nomes, os níveis e os números de HP. A faixa de EXP usa nove quadros derivados do trilho extraído. A caixa adversária com porcentagem de HP ganhou uma extensão de oito pixels para acomodar os números.

A compilação com SMOL produziu pixels indevidos nas regiões transparentes em batalha. Ao compilar os três sprites de caixa como LZ77, essas regiões voltaram a mostrar o cenário no mesmo teste. O teste no mGBA percorreu a Route 1, entrou em um encontro selvagem e conferiu os menus de ação e golpes após abrir e fechar a bolsa. A caixa de porcentagem de HP, as batalhas duplas e o preenchimento durante o ganho de EXP ainda precisam de verificação visual. O menu de ações usa a arte adaptada anteriormente. [Declarações dos sprites](../src/graphics.c)


## 13. Petiprés e a fronteira leste da Route 1

O cabeçalho original da Route 1 em `0x34AC7C` conecta a leste ao mapa do grupo 3, índice 1, com deslocamento zero. A tabela em `0x34C744` resolve esse destino para o cabeçalho `0x34AA84`. O layout de Petiprés em `0x87230C` mede 60 × 44. O cruzamento confirmado no Emerald vai de `(65,22)` na Route 1 a `(0,22)` em Petiprés, com volta pela mesma borda.

Os interiores vêm dos cabeçalhos `0x34B248`, `0x34B264`, `0x34B280` e `0x34B29C`. O primeiro contém três interiores no mesmo mapa, por isso as entradas apontam a índices de warp diferentes. Antes de remover entradas vazias, preserve os índices de todas as entradas referenciadas. No exterior, as três portas residenciais usavam o comportamento de porta sem animação e colisão 1. Foi preciso liberar apenas esses três blocos para o Emerald executar o warp ao pisar. As escadas do Centro receberam o comportamento de escada da engine.

Para os NPCs, a tabela de gráficos em `0x80E254` leva aos descritores de tamanho e às tabelas de quadros. As paletas originais vêm da tabela próxima de `0x39F558`. O port usa os nove quadros direcionais nos movimentos nativos do Emerald. O exterior reaproveita a animação primária de Iviere. Os interiores residenciais usam os quadros extraídos das tabelas `0x8BDE50` e `0x8BA858`, com cadência adaptada. A cena do desconhecido usa movimentos e câmera do Emerald, com uma flag persistente para impedir que ela se repita.

O Poké Radar é entregue como item. A função de busca do item permanece pendente na base. A pista do tesouro é local, e os destinos a leste e ao norte pertencem a mapas ainda fora do port. O salão superior reaproveita os scripts de conexão do Emerald. As sessões multijogador ainda precisam ser testadas.


A enfermeira usa `sAnimTable_Nurse`. O comando `nurse_joy_bow`, executado após a cura, exige o quadro de índice 9. Esse quadro de 256 bytes vem do offset `0x385418` na ROM original e é o décimo quadro de `npc_40.bin`. A tabela genérica de nove quadros permite conversar com a enfermeira, mas trava ao executar a reverência. Ao testar um Centro Pokémon, aceite a cura, encerre o diálogo e confirme que o jogador volta a andar.


### Texto em branco após a cura

O teste precisa repetir o atendimento depois de aceitar a cura. Na primeira pergunta, o texto podia aparecer normalmente. Na segunda, o texto e as opções ficavam brancos porque a primeira cura zerava a memória das fontes.

O rastreio no mGBA identificou a escrita em `StringCopy`, chamada por `BufferUnionRoomPlayerName`. O Centro de Petiprés não tinha o gancho `MAP_SCRIPT_ON_RESUME, CableClub_OnResume`, presente nos Centros da base. O nome pendente da sala de conexão continuava preenchido com zeros, sem o terminador `EOS`. A cópia para `gStringVar1` ultrapassava o buffer e atingia `gFonts`, `sLastTextColor` e `sFontHalfRowLookupTable`.

A correção adiciona o gancho de inicialização ao mapa e verifica se o nome tem `EOS` dentro dos 12 bytes antes da cópia. Essa verificação também protege chamadas com um nome pendente inválido. As alterações ficam em `data/maps/Petipres_PokemonCenter/scripts.inc` e `src/union_room.c`. O teste `test-petipres-text.sh` alterna aceitar e recusar seis vezes e verifica os pixels das letras nas duas janelas, além de confirmar que o jogador recupera o movimento.

### Conferência das tabelas modificadas na Route 2

Não transportar IDs de itens ou espécies diretamente entre as ROMs. A tabela de itens da origem desta etapa foi encontrada em 0xBA8F00, com passo 44. Ler nome e descrição confirmou que 18 é Paralyz Heal, 89 é Lopunnite e 195 é Everstone. A equipe BILL revelou outro erro possível: nesta ROM a equipe com golpes e sem item também usa passo 16, embora o conteúdo dos golpes comece em +6. A segunda espécie é Cubone, e não os valores incoerentes obtidos com passos 12 ou 14.

Na navegação, a colisão dos blocos é apenas parte da regra. O percurso de teste precisou considerar barrancos que saltam duas células, lados bloqueados no bloco atual e no bloco seguinte, elevação de transição 0 e pontes de elevação 15. Uma busca que ignora essas regras pode declarar um mapa desconectado ou propor atravessar uma cerca. Não modificar o mapa só porque essa busca falhou.

Ao acrescentar treinadores, conferir também onde a flag de vitória será gravada. O ID 864 somado a 0x500 colide com as flags de sistema. O mapeamento desta etapa reserva 0x4E0–0x4EF e mantém os endereços antigos. Testar uma vitória nesse banco e verificar as flags de sistema antes e depois. Ver [route2.md](route2.md).
