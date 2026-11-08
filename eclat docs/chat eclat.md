# Chat Éclat — guia técnico para trabalhar com o Gemini

Atualizado em 7 de novembro de 2026.

Este documento explica como investigamos e implementamos o port do Éclat Pourpre para a base Emerald. Foi escrito para outra IA conseguir retomar o trabalho, reproduzir os achados e verificar as próprias alterações. Consulte também o [modus](modus.md), o [histórico](historico.md) e a [metodologia](metodologia.md).

A identificação visual dos sprites começa com uma hipótese. A identificação técnica exige relacionar a imagem exibida aos tiles, à paleta, à tabela de quadros e à rotina que produz o movimento. A precisão alcançada depende desse cruzamento e das revisões do usuário. Houve implementações que compilavam e pareciam corretas em uma captura, mas falhavam em outros estados.

## 1. Reconhecer o projeto antes de editar

### Mapa das pastas principais

A raiz do trabalho é C:/Users/raulr/OneDrive/Desktop/zEmerald. A estrutura observada nesta sessão é a seguinte.

~~~text
zEmerald/
├── eclat/                            Referência do Éclat Pourpre
├── unbound/                          Referência do Pokémon Unbound
├── pokeemerald-expansion-1.17.2/      Repositório principal da base Emerald
├── eclat-intro-teste/                 Código-fonte do port em desenvolvimento
│   ├── eclat docs/                    Documentação específica deste projeto
│   ├── data/                         Mapas, layouts, scripts e tilesets
│   ├── graphics/                     Imagens, paletas e recursos gráficos
│   ├── src/                          Lógica da engine e dados compilados em C
│   ├── include/                      Interfaces, configurações e constantes
│   └── eclat-intro-teste.gba          ROM compilada para jogar e verificar
├── Pokemon - Emerald Version (USA, Europe).gba
├── Pokemon - Emerald Version (USA, Europe).sav
└── Pokemon - Emerald Version (USA, Europe).ss1
~~~

| Pasta ou arquivo | Para que serve | Como usar ao continuar o projeto |
| --- | --- | --- |
| [eclat](../../eclat/) | Contém a ROM original usada para recuperar mapas, personagens, paletas, textos e eventos do Éclat Pourpre. Também contém um save e estados do emulador da referência. | Consultar e executar como referência. As alterações do port são feitas no código-fonte do Emerald. |
| [unbound](../../unbound/) | Contém a ROM e o save do Pokémon Unbound. Foi usada para recuperar o fundo de grama e as molduras de batalha. | Consultar quando a tarefa envolver os recursos do Unbound escolhidos pelo usuário. Os mapas e a narrativa de Éclat vêm da outra referência. |
| [pokeemerald-expansion-1.17.2](../../pokeemerald-expansion-1.17.2/) | É o repositório principal da base Emerald, com a pasta real de metadados Git. Na inspeção, estava na branch eclat-emerald. | Usar para compreender a base e o histórico compartilhado. Verificar o estado antes de tratá-lo como referência limpa. Ele pode conter alterações próprias. |
| [eclat-intro-teste](../) | É a árvore de trabalho ativa do port, na branch eclat-intro durante esta inspeção. Contém a introdução, Iviere, Route 1, Petiprés e as adaptações de batalha. | Fazer aqui as alterações solicitadas pelo usuário e depois sincronizá-las com a cópia de compilação. O nome da pasta foi mantido desde os testes iniciais da abertura. |
| [eclat docs](./) | Guarda os documentos escritos para acompanhar a reconstrução e permitir a colaboração entre IAs. | Ler antes de editar e atualizar quando houver um achado, uma decisão ou um resultado de teste relevante. |
| ROM de Emerald na raiz | É o arquivo identificado pelo nome do jogo comercial, acompanhado de seu save e estado do emulador. | Conferir seu hash se uma tarefa exigir essa referência. O código do port e a ROM atual de teste estão nas pastas indicadas acima. |

### Relação entre a base e a pasta intro-teste

A pasta eclat-intro-teste é uma worktree do Git vinculada ao repositório pokeemerald-expansion-1.17.2. Seu arquivo .git aponta para os metadados em pokeemerald-expansion-1.17.2/.git/worktrees/eclat-intro-teste. As duas árvores podem ter branches e alterações de arquivos diferentes, enquanto compartilham o banco de objetos e o histórico do Git.

Essa relação explica por que copiar apenas eclat-intro-teste para outra máquina pode deixar o vínculo Git apontando para um caminho inexistente. Ao transferir o projeto, preserve ou reconstrua a ligação entre o repositório principal e a worktree. Ao fazer uma revisão, confira em qual árvore e branch o comando está sendo executado.

### Pastas dentro do projeto ativo

Os caminhos desta tabela são relativos a eclat-intro-teste.

| Caminho | Conteúdo e responsabilidade |
| --- | --- |
| [data/maps](../data/maps/) | Cada mapa possui seu diretório. map.json descreve objetos, portas, conexões e propriedades. scripts.inc contém falas e eventos. map_groups.json registra os mapas e sua ordem nos grupos. Preserve os IDs já usados por saves e scripts. |
| [data/layouts](../data/layouts/) | Guarda as células e bordas dos mapas em arquivos binários. layouts.json relaciona dimensões, arquivos e tilesets. O layout define o cenário que um mapa utiliza. |
| [data/tilesets](../data/tilesets/) | Guarda os recursos primários e secundários, incluindo tiles, paletas, metatiles, atributos e animações extraídas. Um efeito de terreno incorreto costuma exigir uma leitura desta pasta junto do layout. |
| [data/scripts](../data/scripts/) | Rotinas de eventos compartilhadas, como o atendimento da enfermeira e os serviços de conexão. Uma chamada no script de mapa pode depender de inicialização feita por essas rotinas. |
| [graphics](../graphics/) | Recursos gráficos utilizados na compilação, em PNGs indexados, paletas e outros formatos. O processamento depende das declarações e regras de conversão da engine. |
| [graphics/eclat_intro](../graphics/eclat_intro/) | Recursos extraídos do Éclat para a abertura e para personagens e efeitos portados posteriormente. A subpasta petipres contém os sprites e as paletas dessa cidade. O nome histórico da pasta não limita seu uso à introdução. |
| [graphics/battle_interface](../graphics/battle_interface/) | Caixas de HP, barras, janelas e paletas da interface de batalha. Confira também as rotinas C que atualizam esses gráficos. |
| [graphics/battle_environment/unbound_grass](../graphics/battle_environment/unbound_grass/) | Recursos importados do cenário de grama do Unbound. A escolha de quando carregá-los fica na lógica de batalha. |
| [src](../src/) | Implementação C dos sistemas do jogo. Inclui movimento, texto, menus, batalha, serviços de mapas e tratamento de saves. |
| [src/data](../src/data/) | Tabelas e registros usados pelo código C, como descritores de sprites, cabeçalhos de tilesets, treinadores, encontros e locais de cura. Confira quais arquivos são gerados antes de editar um header diretamente. |
| [include](../include/) | Declarações e interfaces usadas pelos arquivos C. |
| [include/constants](../include/constants/) | IDs e constantes de flags, variáveis, objetos, movimentos, itens, mapas e outros sistemas. Consulte o símbolo do destino antes de reaproveitar um número da ROM original. |
| [include/config](../include/config/) | Configurações da base e das mecânicas. Uma mudança aqui pode afetar o jogo inteiro. |
| [asm](../asm/) | Macros e suporte em assembly, incluindo os comandos usados para montar os scripts de eventos. |
| [constants](../constants/) | Arquivos de constantes e definições em formato assembly, com extensão .inc. Eles complementam os headers C de include/constants. |
| [sound](../sound/) | Recursos de áudio e definições de músicas e efeitos usados pela engine. |
| [tools](../tools/) | Ferramentas de conversão e geração usadas pela compilação, como mapjson, gbagfx e processadores de dados. |
| [test](../test/) | Testes presentes no repositório da engine. Os roteiros locais de mGBA descritos neste documento foram guardados na área externa work. |
| [dev_scripts](../dev_scripts/) | Utilitários de desenvolvimento fornecidos pela base. Leia entradas, destinos e efeitos antes de executar. |
| [migration_scripts](../migration_scripts/) | Scripts de migração entre versões da base. Seu uso depende de uma tarefa de atualização e da versão de origem. |
| [libagbsyscall](../libagbsyscall/) | Suporte às chamadas de sistema da BIOS do GBA utilizado pelo processo de build. |
| [docs](../docs/) | Documentação que acompanha a base, com instalação, tutoriais, procedimentos e changelogs. |
| [eclat docs](./) | Documentação do port. historico.md registra a evolução, metodologia.md guarda os procedimentos e chat eclat.md apresenta o guia de transferência para outra IA. |
| [.github](../.github/) | Configurações e procedimentos de colaboração e automação do repositório. |

O diretório build, quando presente na árvore usada para compilar, contém produtos intermediários. A cópia do Windows inspecionada nesta etapa não tinha esse diretório. A cópia de build no WSL possui seus próprios arquivos gerados.

### Arquivos de jogo e áreas temporárias

Um arquivo .gba é uma ROM executável. Um .sav guarda o progresso normal do jogo. Os arquivos .ss1, .ss2 e semelhantes encontrados aqui são estados do emulador e guardam também memória de execução. Mantenha cada save associado à ROM correspondente ao testar as referências e o port.

O código e os recursos editáveis do projeto ativo ficam no Windows. A cópia de compilação no WSL fica no caminho registrado em /var/tmp/eclat-intro-build-path. Ela recebe as alterações para gerar pokeemerald.gba e pokeemerald.elf. A entrega testada é copiada para eclat-intro-teste/eclat-intro-teste.gba. Reconfira a sincronização antes de interpretar um resultado de teste.

A pasta work desta sessão fica fora de zEmerald, em C:/Users/raulr/Documents/Codex/2026-11-06/oi-chat-pdoeria-me-ajduar-a/work. Ela contém os roteiros de extração, diagnósticos, capturas e testes criados durante a investigação. Alguns desses roteiros modificam o projeto diretamente. A seção 10 descreve os arquivos que ajudam a retomá-los.

### Como decidir onde trabalhar

| Pedido recebido | Origem a consultar | Destino da implementação |
| --- | --- | --- |
| Portar uma cidade, fala ou personagem do Éclat | Pasta eclat e observação da ROM original | Mapas, recursos e código dentro de eclat-intro-teste |
| Ajustar o cenário ou HUD escolhido do Unbound | Pasta unbound e os recursos já extraídos | graphics/battle_interface, graphics/battle_environment e a lógica correspondente em src |
| Corrigir um erro do jogo portado | ROM atual, scripts e código em execução | Arquivos responsáveis em eclat-intro-teste, seguidos de compilação e teste |
| Entender a engine e seus formatos | Código da base e docs | Alteração localizada no projeto ativo quando a tarefa exigir |
| Registrar um resultado para a próxima IA | Evidências, diffs e logs da tarefa | Documentos em eclat docs |

O diretório de trabalho do código-fonte é este.

~~~text
C:/Users/raulr/OneDrive/Desktop/zEmerald/eclat-intro-teste
~~~

A base identificada no projeto é pokeemerald-expansion 1.17.2. As referências a “1.72” nas mensagens antigas precisam ser interpretadas junto dos arquivos atuais. Confira a versão e as alterações locais antes de atualizar dependências.

As três ROMs abaixo foram verificadas por SHA-256 na preparação deste documento.

| Papel | Caminho relativo a zEmerald | SHA-256 |
| --- | --- | --- |
| Referência de mapas, personagens e eventos | eclat/Eclat Pourpre ENG (Raizen).gba | 9dadfec5e707e8be3e8f5f12dfd3bfd1b7448ae78c7d955f8e9593c91c7c045a |
| Referência do cenário e das molduras de batalha | unbound/Pokémon - Unbound.gba | 7aa25bbf568f7cfcf6ee1cf2e9e6ff637350b3d0705c2375cabb6baa7d9739f7 |
| Resultado entregue após a correção dos diálogos | eclat-intro-teste/eclat-intro-teste.gba | 90f178c6e001a6146bac64ae16f5bafbf55e939d5f76e0e0c9c385725180dd6d |

O hash do resultado muda a cada alteração na ROM. Os offsets da referência só podem ser reaproveitados depois de confirmar que o arquivo de origem continua sendo o mesmo.

O histórico contém etapas intermediárias e relatos de trabalho de ambas as IAs. Expressões antigas como “perfeito”, “1:1” ou “100%” precisam ser lidas junto dos testes registrados e das correções posteriores. Uma etapa nova pode substituir uma tentativa anterior.

## 2. Como transformar uma captura em uma investigação

Comece registrando a situação observável. Por exemplo, “o Pidgeot se desloca para a direita enquanto a cabeça aponta para a esquerda”. Essa descrição permite formular uma hipótese verificável sobre o espelhamento.

Para cada diferença, registre estes dados.

| Campo | O que anotar |
| --- | --- |
| Origem | ROM, hash e versão observada |
| Cena | Mapa, coordenada e progresso do evento |
| Entrada | Botões e movimentos necessários para reproduzir |
| Tempo | Quadro ou sequência de quadros da captura |
| Recurso | BG, objeto da OAM, efeito de campo ou combinação |
| Diferença | Posição, direção, cor, duração ou estado incorreto |
| Critério de sucesso | O que deverá aparecer ou acontecer no teste |

Uma captura com filtro pode alterar cores e contornos. Quando a imagem parecer ampliada, confira se cada pixel original virou um bloco inteiro de 2 × 2, 3 × 3 ou 4 × 4. Só reduza por amostragem direta se essa relação estiver confirmada. Suavização e interpolação não servem para recuperar uma paleta exata.

Use o jogo de referência para observar a sequência. Um quadro parado não informa se a ave bate asas, se a câmera se move, se um objeto está preso ao mapa ou se a cena libera o jogador ao terminar.

## 3. Interpretar um sprite do GBA

### Tiles, cores e montagem

Nos recursos 4bpp usados neste trabalho, cada tile mede 8 × 8 pixels e ocupa 32 bytes. Um byte contém dois índices de cor. O nibble baixo corresponde ao primeiro pixel do par. O nibble alto corresponde ao segundo.

Um quadro de 16 × 32 ocupa 256 bytes. Um quadro de 32 × 32 ocupa 512 bytes. Esses tamanhos ajudam a validar um descritor, mas a largura e a altura também precisam ser lidas e conferidas.

O quadro é composto por tiles. Ler todos os bytes como uma imagem linear produz blocos fora de ordem. Para um quadro retangular organizado por linhas de tiles, o pixel pode ser consultado assim.

~~~python
def pixel_4bpp(frame, width, x, y):
    tiles_per_row = width // 8
    tile = (y // 8) * tiles_per_row + x // 8
    byte_offset = tile * 32 + (y % 8) * 4 + (x % 8) // 2
    value = frame[byte_offset]
    return (value >> (4 * (x % 2))) & 0x0F
~~~

Essa função pressupõe o arranjo descrito. Sprites compostos por subsprites, recursos 8bpp e outras organizações de imagem exigem outra leitura.

Cada banco de paleta 4bpp possui 16 entradas de 16 bits. Na cor GBA, os bits 0–4 representam vermelho, 5–9 representam verde e 10–14 representam azul. Os bytes são armazenados em little-endian.

~~~python
def gba_color_to_rgb(value):
    return tuple(((value >> shift) & 31) * 255 // 31
                 for shift in (0, 5, 10))
~~~

O índice zero normalmente representa transparência em sprites OBJ. Um PNG indexado também conserva a ordem dos índices. Reordenar sua paleta sem remapear os pixels pode trocar roupa, pele e contorno, mesmo que a imagem pareça correta no editor.

### Identificação pela OAM

A OAM começa em 0x07000000 e possui 128 entradas de oito bytes. Cada entrada contém attr0, attr1 e attr2 de 16 bits, além do campo reservado à matriz afim.

Para um sprite 4bpp em modo OBJ 1D, use estas relações depois de confirmar o modo de vídeo.

~~~text
tile inicial = attr2 & 0x03FF
endereço OBJ = 0x06010000 + tile inicial × 32
banco de paleta = (attr2 >> 12) & 15
endereço da paleta = 0x05000200 + banco de paleta × 32
~~~

Leia a forma em attr0 e o tamanho em attr1 para resolver as dimensões. Confira posição, visibilidade, transformação e prioridade. As coordenadas têm wrap e podem representar objetos parcialmente fora da tela.

Nos sprites sem transformação afim, os bits 12 e 13 de attr1 controlam os espelhamentos horizontal e vertical. Quando a transformação afim está ativa, esses bits participam de outro campo. Confira o modo antes de concluir que o sprite está espelhado.

Compare os bytes da VRAM com os quadros candidatos da ROM, após descompressão quando necessário. A aparência localiza o objeto. A coincidência dos dados e dos ponteiros confirma sua origem.

### Identificação pelo descritor da ROM original

Na cópia analisada do Éclat, a tabela de ponteiros de gráficos usada pelos mapas modificados começa no offset 0x80E254. Um ID de gráfico leva ao descritor por uma entrada de quatro bytes.

Os campos usados nos descritores desta referência são os seguintes.

| Offset no descritor | Campo |
| --- | --- |
| +2 | Tag da paleta, 16 bits |
| +6 | Tamanho de um quadro em bytes, 16 bits |
| +8 | Largura, 16 bits |
| +10 | Altura, 16 bits |
| +24 | Ponteiro da tabela de animações |
| +28 | Ponteiro da tabela de imagens |

A tabela de imagens contém pares de ponteiro e tamanho, com oito bytes por entrada. Confirme limites e tamanhos antes de extrair. Uma tabela de FireRed limpa pode apontar para outro recurso porque o hack alterou os dados.

O leitor abaixo é somente leitura. Ele consulta o descritor da enfermeira e as dez entradas utilizadas pelo port. Execute com Python 3 e forneça o caminho da ROM original.

~~~python
from pathlib import Path
from struct import unpack_from
from sys import argv

rom = Path(argv[1]).read_bytes()

def file_offset(pointer):
    offset = pointer - 0x08000000
    if not 0 <= offset < len(rom):
        raise ValueError(f"Ponteiro fora da ROM: {pointer:#x}")
    return offset

def u32(offset):
    return unpack_from("<I", rom, offset)[0]

descriptor = file_offset(u32(0x80E254 + 0x40 * 4))
frame_size, width, height = unpack_from("<3H", rom, descriptor + 6)
frames = file_offset(u32(descriptor + 28))
assert (frame_size, width, height) == (256, 16, 32)

for index in range(10):
    pointer, size = unpack_from("<II", rom, frames + index * 8)
    offset = file_offset(pointer)
    assert size == frame_size and offset + size <= len(rom)
    print(index, hex(offset), size)
~~~

Esse exemplo não transforma dez quadros numa regra geral. A quantidade necessária vem das animações e das ações executadas pelo personagem.

## 4. Descobrir a animação que pertence ao sprite

Registre o ID do quadro, a duração, a direção e o deslocamento ao longo do tempo. Compare também o estado parado, a caminhada, a virada e as ações especiais.

Os quadros de um personagem podem repetir ponteiros. A enfermeira original, por exemplo, reutiliza imagens de orientação nas entradas direcionais. Uma extração deve preservar essa correspondência, mesmo que alguns blocos sejam iguais.

O travamento após curar em Petiprés mostrou o que acontece quando só os nove quadros direcionais são portados. O script chama nurse_joy_bow ao terminar o atendimento. A tabela sAnimTable_Nurse usa o quadro de índice 9 nessa reverência. Ele vem do offset 0x385418, com 256 bytes. Foi preciso importar esse décimo quadro e trocar a tabela genérica pela tabela da enfermeira.

O teste de abrir a pergunta de cura havia passado. O teste de aceitar, assistir à animação, encerrar o diálogo e voltar a andar revelou o erro. Ao portar uma classe de NPC, procure as ações especiais que seus scripts executam.

Para as aves de Iviere, medimos o movimento em relação ao mapa. Se a posição de tela depender diretamente da posição do jogador, a ave pode parecer segui-lo. Registre a âncora no mapa, o avanço da rota e o deslocamento da câmera separadamente. Confira o espelhamento nas duas direções e a recriação das instâncias após entrar e sair de um prédio.

O exportador de animações de Iviere comparou blocos de tiles em quadros consecutivos. Guardou as variantes distintas e uma tabela de fases de 256 quadros. Isso preservou a sequência observada sem supor que todos os efeitos tivessem a mesma duração.

## 5. Recuperar mapas, portas e eventos

Na região de ROM usada aqui, um ponteiro GBA vira offset de arquivo pela subtração de 0x08000000. Verifique também o final de cada bloco lido. Um ponteiro plausível isolado não confirma uma estrutura.

O caminho de investigação foi este.

~~~text
cabeçalho do mapa
  → layout, tilesets, eventos, scripts e conexões
  → células, metatiles, atributos, paletas e tiles
  → objetos, portas, gatilhos e placas
  → falas, movimentos, condições e alterações persistentes
~~~

Nas células de 16 bits dos layouts portados, os bits 0–9 são o ID do metatile, os bits 10–11 são a colisão e os bits 12–15 são a elevação. O desenho não determina o comportamento do terreno. Um piso pode parecer seco e ainda disparar um efeito de água ou um encontro se o atributo estiver errado.

Os layouts importados usam tile_format frlg no projeto. O suporte local lê os atributos de metatiles desse formato. Ainda é necessário conferir o significado do comportamento na engine de destino, os tamanhos das tabelas e os limites entre tilesets primário e secundário.

Para alterar um comportamento, encontre a célula, resolva o metatile e conte onde mais ele aparece. Preserve os demais campos do atributo. Prefira uma mudança no bloco específico quando o problema for a colisão de uma única porta.

Petiprés foi localizada seguindo a conexão leste da Route 1. O cabeçalho da rota em 0x34AC7C aponta para o grupo 3, índice 1, com deslocamento zero. A tabela em 0x34C744 leva ao cabeçalho da cidade em 0x34AA84. O layout em 0x87230C mede 60 × 44.

A ligação testada passa de (65,22) na Route 1 para (0,22) em Petiprés. Os interiores vêm dos cabeçalhos 0x34B248, 0x34B264, 0x34B280 e 0x34B29C. Um deles contém três casas separadas dentro do mesmo layout. Por isso, a entrada precisa apontar para o índice correto de warp.

Preserve os índices de portas referenciadas por outros mapas. Verifique entrada, saída, posição final e elevação. Em Petiprés, três portas residenciais precisaram ter a colisão liberada para executar o warp sem animação. A escada do Centro também precisou do comportamento de escada e de uma elevação compatível com o bloco de destino.

Para um mapa novo, compare os serviços esperados com os de um mapa equivalente já funcional na base. Isso inclui os scripts de transição, retorno e carregamento, além dos objetos visíveis.

## 6. Reescrever os scripts com suas dependências

Leia o fluxo de execução junto das falas. Registre o ponto de entrada, as condições, as chamadas, os retornos, as esperas, a câmera, os movimentos e o momento em que o controle volta ao jogador.

Na referência analisada, os controles de texto incluem 0xFF para EOS, 0xFE para nova linha, 0xFA para continuação com rolagem e 0xFB para nova página. Eles correspondem a convenções que precisam ser traduzidas para as strings da engine. Controle de cor, nome do jogador e outros parâmetros exigem tratamento próprio.

Um decodificador parcial pode produzir uma frase legível e ainda perder uma condição, uma cor ou uma quebra de página. Compare a interação completa na referência. Preserve o texto original quando essa for a intenção do port e identifique as traduções ou adaptações que forem necessárias.

Os IDs de flags, variáveis, itens, movimentos, músicas e espécies precisam ser resolvidos no destino. Um número da ROM original pode representar outro recurso no Emerald. Preserve os IDs locais usados pelos scripts de movimento dos objetos.

Em eventos de entrega, marque a conclusão após confirmar que o jogador recebeu o item. Teste a mochila sem espaço quando o item puder falhar. Em eventos de remoção temporária, confira qual estado permite recriar o objeto ao recarregar o mapa.

O Poké Radar de Petiprés já é entregue como item, mas sua função de busca está pendente na base. Entregar o recurso e implementar a mecânica são etapas diferentes que precisam constar no relatório.

## 7. Investigar um erro visual pela memória

O erro de texto em branco após a cura é um exemplo de investigação completa.

1. O teste repetiu o atendimento. Na primeira pergunta havia letras. Depois de aceitar a cura, a segunda pergunta e as opções YES/NO ficaram brancas.
2. A paleta continuava com branco, cinza escuro e sombra corretos. A comparação dos buffers mostrou ausência de pixels das letras tanto na RAM quanto na VRAM.
3. O ponteiro gFonts e os dados sLastTextColor e sFontHalfRowLookupTable tinham sido zerados. Isso delimitou o problema à geração dos caracteres e à memória que ela usa.
4. O teste salvou o estado antes de cada quadro durante o atendimento. Ao detectar a perda de gFonts, restaurou o último estado e avançou instrução por instrução até a escrita.
5. Na compilação investigada, o PC estava em 0x0820B896, dentro de StringCopy, e o LR em 0x0822F857, no retorno de BufferUnionRoomPlayerName. O ELF da mesma compilação permitiu identificar as funções.
6. O Centro portado não tinha MAP_SCRIPT_ON_RESUME com CableClub_OnResume. O nome pendente da sala de conexão permanecia sem o terminador EOS. A cópia para gStringVar1 passava do limite e apagava os dados das fontes.
7. A correção adicionou o gancho ao mapa e validou a presença de EOS dentro dos 12 bytes do nome antes da cópia. O teste final alternou aceitar e recusar seis vezes, com texto visível e movimento liberado em todas.

Os endereços de RAM, PC e LR acima pertencem à compilação investigada. Gere os símbolos novamente quando o código mudar.

Uma falha visual pode começar em uma rotina de texto, de animação, de mapa ou de serviço compartilhado. Localize primeiro o dado incorreto. Trocar a cor do texto não corrigiria a cópia que zerava as fontes.

## 8. Portar a interface de batalha sem perder as camadas

O cenário, a janela de mensagem, os menus, as caixas de HP e as barras são recursos distintos. Uma moldura estática pode estar certa enquanto a rotina que atualiza HP ou EXP escreve tiles diferentes por cima.

As molduras originais do Unbound foram recuperadas dos blocos LZ77 nos offsets 0xEEF0AC e 0xEEF380. A paleta está em 0xEEF360. As referências da tabela de sprites foram encontradas em 0x260208 e 0x260210. As caixas usam metades de 64 pixels armazenadas em sequência.

A primeira adaptação por desenho precisou de revisões. A reextração permitiu recuperar a silhueta original, incluindo as pontas. O port remapeou os índices de paleta e conservou o espaço necessário aos textos e às barras dinâmicas do Emerald.

O callback SpriteCB_HealthBar posiciona a barra separada. A junção superior precisou de um ajuste de um pixel nas caixas individuais. Na caixa do jogador, o primeiro pixel transparente do overlay também exigiu preservar a coluna correspondente do desenho de fundo.

No EXP, MoveBattleBarGraphically atualiza oito tiles. Os nove estados de expbar.png precisam conservar as linhas fixas da moldura enquanto variam o preenchimento interno. Uma captura sem EXP parcial não verifica todo esse comportamento.

Na compilação observada, os três sprites de caixa produziram artefatos transparentes usando a opção SMOL. A troca desses recursos para LZ77 removeu os artefatos no mesmo percurso de teste. A causa interna dessa diferença de pipeline não foi isolada. Esse achado não justifica uma troca geral do compressor do projeto.

O painel roxo de texto no menu de ações foi ajustado separadamente, pelo tilemap e pela paleta do textbox. O teste comparou as cores de dez linhas da borda e do fundo ao longo dos 120 pixels de largura do painel. A combinação das camadas precisa ser conferida em batalha e depois de entrar e sair da bolsa.

## 9. Descompressão e formatos

O cabeçalho GBA LZ77 começa com 0x10 e três bytes little-endian de tamanho descomprimido. Nos pares de referência, calcule o comprimento e a distância assim.

~~~python
length = (first >> 4) + 3
distance = (((first & 0x0F) << 8) | second) + 1
~~~

A soma de 1 vem depois de combinar os 12 bits. Com first igual a 0x01 e second igual a 0xFF, a distância deve ser 512. Durante a cópia, o bloco pode referenciar bytes que estão sendo produzidos pela própria repetição.

Valide o tamanho final, os limites de leitura e a igualdade com um recurso carregado pelo jogo. Uma imagem reconhecível pode conter bytes incorretos.

Ao exportar PNG, conserve o modo indexado e a semântica de cada índice. Confira também a organização em tiles exigida pela conversão do projeto. A aparência do PNG precisa corresponder aos bytes que a engine realmente recebe.

## 10. Ferramentas e arquivos para retomar a investigação

Os pontos de entrada abaixo ajudam a localizar a implementação atual no código-fonte.

| Assunto | Arquivos para ler |
| --- | --- |
| Abertura e confirmação dos nomes | [main_menu.c](../src/main_menu.c), [naming_screen.c](../src/naming_screen.c) |
| Quarto e cena da mãe | [segundo andar](../data/maps/LittlerootTown_BrendansHouse_2F/scripts.inc), [térreo](../data/maps/LittlerootTown_BrendansHouse_1F/scripts.inc) |
| Rotas das aves | [field_specials.c](../src/field_specials.c) |
| Leitura dos atributos e animação dos tiles | [fieldmap.c](../src/fieldmap.c), [tileset_anims.c](../src/tileset_anims.c) |
| Quadros e descritores de Petiprés | [petipres_pic_tables.h](../src/data/object_events/petipres_pic_tables.h), [petipres_graphics_info.h](../src/data/object_events/petipres_graphics_info.h) |
| Animação especial da enfermeira | [object_event_anims.h](../src/data/object_events/object_event_anims.h), [pkmn_center_nurse.inc](../data/scripts/pkmn_center_nurse.inc) |
| Inicialização do Centro e cópia do nome pendente | [scripts do Centro](../data/maps/Petipres_PokemonCenter/scripts.inc), [union_room.c](../src/union_room.c) |
| Caixas e atualizações do HUD | [battle_interface.c](../src/battle_interface.c), [graphics.c](../src/graphics.c) |
| Cenário de batalha | [battle_bg.c](../src/battle_bg.c) |
| Destinos após desmaiar | [heal_locations.json](../src/data/heal_locations.json) |

Os roteiros locais desta sessão estão nesta área externa ao repositório.

~~~text
C:/Users/raulr/Documents/Codex/2026-11-06/oi-chat-pdoeria-me-ajduar-a/work
~~~

| Grupo | Exemplos verificados nesta sessão | Uso |
| --- | --- | --- |
| Abertura e observação | extract-intro.py, full-animation-audit.py | Recuperação e comparação de recursos |
| Tempo das animações | export-town-timing.py | Variantes de tiles e fases observadas |
| Eventos da origem | route-source.py | Leitura de cabeçalhos, objetos e conexões |
| Port de Petiprés | import-petipres.py | Registro do procedimento de importação |
| HUD original | unbound-rom-hud.py, scan-native-hud.py, apply-native-hud.py | Leitura, localização e aplicação dos gráficos |
| Regressão da cura | test-petipres-text.sh, test-petipres-font.c | Repetição do diálogo e rastreio da corrupção |
| Batalha | test-unbound-grass.sh | Encontro natural e retorno da bolsa |
| Entrega da ROM | deliver_rom.py | Cópia do resultado compilado para o Windows |

Leia os roteiros antes de executar. Vários gravam diretamente no projeto, acrescentam entradas e pressupõem o estado de uma etapa antiga. Uma nova execução de um importador pode duplicar mapas ou sobrescrever correções posteriores. Use-os para entender a transformação e prepare uma execução que seja segura no estado atual.

Esses arquivos temporários e os caches do WSL podem desaparecer. O código-fonte, os recursos portados e a documentação são as referências duráveis para reconstruir o ambiente.

A cópia de compilação usada estava em /var/tmp/eclat-intro.ooJzjN. O caminho foi registrado em /var/tmp/eclat-intro-build-path. O pacote local de libmGBA estava em /var/tmp/eclat-mgba.HxfCZu, com marcador em /var/tmp/eclat-mgba-path. Confira existência, conteúdo e sincronização antes de reutilizar qualquer um desses caminhos.

Os caminhos /mnt/c pertencem ao WSL. Os caminhos C:/ pertencem ao Windows. Ao enviar código de shell pelo PowerShell, cuide da expansão de variáveis antes que o Bash receba o comando. Prefira um arquivo de script ou entrada literal quando houver várias camadas de aspas.

## 11. Compilar, testar e entregar

Mantenha a cópia do Windows como origem das alterações. Sincronize para a cópia de build do Linux todos os arquivos envolvidos, incluindo os recursos binários, os headers de registro e os arquivos de mapa. Leia o log quando a compilação falhar.

Na cópia de build, o comando usado foi este.

~~~bash
make -j8 emerald
~~~

A compilação gera pokeemerald.gba e pokeemerald.elf. O teste precisa usar os dois artefatos da mesma compilação. Endereços de funções, posições de globais e offsets internos de estruturas podem mudar.

Os testes locais usam libmGBA para executar entradas normais, capturar vídeo e consultar memória. O ELF permite resolver símbolos com arm-none-eabi-nm e investigar instruções com arm-none-eabi-objdump. Use um teste pequeno que reproduza o problema e confira o que o jogador vê, além do estado interno.

Para cada alteração, exercite o percurso completo que ela afeta. Em uma cura, confirme a animação, a fala final, o movimento e um novo atendimento. Em uma porta, confira ida e volta. Em uma interface de batalha, confira as atualizações dinâmicas e o retorno de outra tela.

Só copie a ROM testada para eclat-intro-teste.gba depois da compilação e da verificação. Compare o SHA-256 da origem e do destino. Um arquivo entregue com o nome certo ainda pode ser uma compilação antiga.

Um save state guarda memória de execução e pode conservar corrupção ou ponteiros de outra compilação. Para verificar uma correção desse tipo, reinicie a ROM atualizada e carregue o save normal pelo Continue. Para mudanças na abertura, use Novo Jogo.

## 12. Como o Gemini pode registrar e revisar o trabalho

As etapas abaixo são tarefas concretas de investigação. Qualquer IA pode executá-las. A revisão fica mais eficiente quando o próximo participante consegue reproduzir o achado sem depender da interpretação de uma imagem.

Antes de modificar o código, produza uma nota curta com a referência, o caminho até os dados, o fluxo do evento e o critério de sucesso. Mostre quais informações foram observadas e quais continuam sendo hipóteses.

Depois da implementação, registre o arquivo alterado, o símbolo relevante, a adaptação feita e o teste executado. Identifique explicitamente os casos que ficaram pendentes. Não atribua a outra IA uma verificação que ela não executou.

Use este modelo de passagem de trabalho.

~~~text
Tarefa e comportamento esperado
ROM de origem e SHA-256
Mapa, coordenada e estado do evento
Captura ou sequência que demonstra a diferença
Ponteiros, offsets, estruturas e símbolos encontrados
Quadros, dimensões, paleta e ação especial, quando houver sprite
Arquivos modificados e motivo de cada alteração
Adaptações feitas à engine de destino
Compilação, hash entregue e teste realizado
Resultado observado e casos ainda pendentes
~~~

Ao receber uma correção do usuário, transforme a captura e a descrição em um novo caso de reprodução. Preserve a intenção do pedido e investigue a causa antes de alterar outra camada.

## 13. Exercícios para verificar a compreensão

### Enfermeira

Leia o descritor do gráfico 0x40, recupere o quadro de índice 9 e confira seus 256 bytes no offset 0x385418. Encontre a referência a esse quadro em sAnim_NurseBow. Explique por que uma conversa inicial não detectaria sua ausência.

### Texto em branco

Leia BufferUnionRoomPlayerName e o script de retorno do Centro. Explique como uma sequência de zeros sem EOS ultrapassava gStringVar1 e atingia as fontes. No teste final, confirme que os seis atendimentos mantêm letras nas duas janelas e devolvem o controle ao jogador.

### Nova cidade

Siga a conexão a partir do mapa já portado. Registre o cabeçalho, o layout, os objetos, as portas e os scripts da origem. Resolva cada entrada e saída com seus índices e elevações. Porte os recursos e os serviços do mapa, depois percorra a fronteira e cada interior antes de declarar a etapa concluída.

## 14. Limites conhecidos para orientar a próxima etapa

O port contém adaptações deliberadas e casos que ainda precisam de verificação. Em Petiprés, a cadência da animação dos interiores e a execução da cena do desconhecido foram adaptadas ao Emerald. O Poké Radar ainda precisa de sua função de busca. A continuação da caça ao tesouro e as próximas rotas permanecem pendentes. As sessões multijogador do salão superior não foram validadas de ponta a ponta.

No HUD, as caixas com porcentagem de HP, as batalhas duplas e o preenchimento durante ganho de EXP exigem revisão visual específica. Os menus de batalha conservam adaptações da engine de destino. O teste de uma batalha simples não cobre automaticamente essas variantes.

Ao continuar, use essas pendências para delimitar o que falta e compare a referência com o código atual. A combinação de dados extraídos, execução observada e testes repetíveis é o procedimento que permite melhorar a fidelidade do port.

### Continuidade da Route 2

A análise da etapa seguinte está em [route2.md](route2.md) e [route2-origem.json](route2-origem.json). São as referências para os mapas 1/14 a 1/19. Atenção especial ao passo de 16 bytes das equipes personalizadas, ao tamanho `u16` das imagens de objeto e à tabela de itens reorganizada. Conferir o nome e a descrição evitou colocar Escape Rope no lugar de Lopunnite.

O teste de percurso desativa encontros aleatórios e marca os treinadores comuns como já derrotados para isolar a navegação. Isso não demonstra que cada batalha comum foi jogada. A prova da cena Rocket é uma verificação separada, com estatísticas e experiência do Pokémon de teste ajustadas de forma coerente. Essas alterações existem somente na memória do emulador de teste. Não entram na ROM distribuída.
