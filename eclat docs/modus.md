# Modus — como investigar e portar Éclat Pourpre para Emerald

Este documento descreve o método aplicado ao projeto `eclat-intro-teste`. Serve para outra pessoa ou IA reproduzir uma investigação, revisar uma alteração e reconhecer o que ainda falta. Não substitui a observação da ROM original nem transforma uma compilação bem-sucedida em prova de fidelidade visual ou funcional.

## 1. Referências e níveis de evidência

| Papel | Local |
| --- | --- |
| ROM de referência | `../eclat/Eclat Pourpre ENG (Raizen).gba`, a partir do projeto ativo |
| Projeto ativo | `eclat-intro-teste`, baseado em pokeemerald-expansion 1.17.2 |
| Base limpa | `../pokeemerald-expansion-1.17.2`, a partir do projeto ativo |
| Versão compilada | `eclat-intro-teste.gba` dentro do projeto |
| Backups | `../backups`, a partir do projeto ativo |

A ROM de referência usada nesta investigação tem SHA-256 `9DADFEC5E707E8BE3E8F5F12DFD3BFD1B7448AE78C7D955F8E9593C91C7C045A`. Offsets citados abaixo pertencem a **essa ROM modificada**. Eles não são garantidos para FireRed limpo, outra tradução ou outra revisão de Éclat. Registre o hash antes de reutilizar um offset.

Classifique cada afirmação:

1. **Observado:** cena percorrida no jogo, com mapa, posição, progresso e resultado registrados.
2. **Extraído:** bytes ou estrutura localizados na ROM, com ponteiro, tamanho e limites conferidos.
3. **Adaptado:** comportamento implementado nos arquivos do Emerald.
4. **Compilado:** a ROM foi gerada sem erro.
5. **Testado:** a alteração foi percorrida no emulador, incluindo retorno e estados alternativos.

Não escreva “perfeito” quando houver apenas um render ou uma build. O [histórico](historico.md), a [metodologia](metodologia.md) e as [validações da Route 2](validacao-route2.md) registram provas e pendências por etapa.

## 2. Onde cada tipo de dado mora

| Dado | Fonte ou destino no projeto |
| --- | --- |
| Lista e ordem dos mapas | `data/maps/map_groups.json` |
| ID, layout, objetos, portas, conexões, gatilhos | `data/maps/<Mapa>/map.json` |
| Diálogos, movimentos e condições locais | `data/maps/<Mapa>/scripts.inc` |
| Inclusão dos scripts na montagem | `data/event_scripts.s` |
| Dimensões, tilesets e arquivos de células | `data/layouts/layouts.json` e `data/layouts/<layout>/` |
| Seções e nomes do mapa regional | `src/data/region_map/region_map_sections.json` e `region_map_layout.h` |
| Sprites OBJ e paletas | `graphics/eclat_intro/` e `src/data/object_events/` |
| Espécies e frequência de encontros | `src/data/wild_encounters.json` |
| Equipes de treinadores | `src/data/trainers.party` e constantes correspondentes |
| Local de cura e desmaio | `src/data/heal_locations.json` |
| Comportamentos que exigem C | `src/`, por exemplo `field_effect_helpers.c` |

O Porymap lê a estrutura do projeto; a ROM lê os dados compilados. O nome da pasta no Porymap, o ID `MAP_...` e o nome mostrado ao jogador são coisas separadas. Ivière, por exemplo, usa o slot `MAP_LITTLEROOT_TOWN`, mas a seção desse mapa mostra “IVIERE TOWN”.

## 3. Rotina de investigação: da tela ao dado

1. **Reproduza a cena original.** Anote mapa, coordenadas, direção do jogador, flags ou estágio narrativo, botões e sequência de quadros. Capture mais de um instante se houver movimento.
2. **Separe as camadas.** Descubra se o elemento é metatile de BG, objeto da OAM, efeito de campo, janela de texto ou composição de vários recursos.
3. **Encontre o recurso na ROM.** Siga o cabeçalho do mapa, o descritor do objeto ou um ponteiro de script. Converta ponteiros GBA válidos para offset de arquivo subtraindo `0x08000000`; rejeite offsets fora da ROM.
4. **Confirme por estrutura e contexto.** Largura, altura, tamanho, tag de paleta, coordenadas, ID local, fala e estado da cena precisam concordar. Um gráfico parecido isoladamente não basta.
5. **Implemente na semântica do Emerald.** Converta dados e fluxo; não copie IDs numéricos nem bytecode cegamente entre engines.
6. **Valide por percurso.** Compile, entre, interaja, saia, volte, salve/recarregue quando aplicável e compare com a referência no mesmo estado.

A captura ajuda a formular hipóteses; o acerto vem da combinação de comportamento observado, bytes da ROM e estruturas do projeto. Filtros de suavização do emulador podem alterar contornos e cores aparentes. Não use a cor de uma captura filtrada como paleta exata.

## 4. Como identifico e porto um sprite

Na referência analisada, a tabela de ponteiros dos descritores de gráficos dos objetos começa no **offset de arquivo** `0x80E254`. Para um ID gráfico `n`, leia um ponteiro little-endian em `0x80E254 + 4*n`. No descritor encontrado, os campos usados aqui são:

| Offset | Significado |
| --- | --- |
| `+2` | tag da paleta, 16 bits |
| `+6` | tamanho de um quadro, 16 bits |
| `+8`, `+10` | largura e altura, 16 bits cada |
| `+24` | tabela de animações |
| `+28` | tabela de imagens |

Confira o ponteiro e cada quadro antes de exportar. Em 4bpp, um tile de 8×8 ocupa 32 bytes; um quadro de 16×32 ocupa 256 bytes e um de 32×32 ocupa 512. Os pixels são armazenados em **ordem de tiles**, com dois índices de cor por byte. Cada paleta tem 16 cores BGR555 little-endian. Índice 0 costuma ser transparência de sprite OBJ.

Uma imagem de nove quadros 16×32 tem 2304 bytes; isso não quer dizer que toda imagem de nove quadros seja um treinador. Confira o ID do objeto no mapa, os comandos de movimento e a tabela de animação. Para objetos em movimento, verifique também direção, espelhamento, posição na OAM e efeito do deslocamento da câmera. Um pássaro pode ter o gráfico correto e ainda parecer voar de costas por usar o quadro ou o flip errado.

**Exemplo real: sprites da Route 2.** A build falhou com referências indefinidas para dez `gObjectEventGraphicsInfo_EclatRoute2Npc...`. Os binários em `graphics/eclat_intro/route2/` existiam, mas `eclat_route2_graphics.h`, `eclat_route2_pic_tables.h` e `eclat_route2_graphics_info.h` estavam vazios. Li os descritores da ROM de referência: 5F é 16×16, quatro quadros, paleta 1105 e árvore cortável; 9B é 32×32, nove quadros, paleta 1106; os demais têm nove quadros 16×32. As tags 1103/1105/1106 correspondem às paletas já extraídas como `PETIPRES_03/05/06`. Completei as três tabelas, usei `sAnimTable_CuttableTree` para 5F e removi três inclusões repetidas de cada cabeçalho. O link então passou. A build comprova que os símbolos existem e os formatos cabem; a fidelidade de cada animação ainda depende de comparação no emulador.

Para investigar um sprite **já desenhado na tela**, a OAM começa em `0x07000000`. Em OBJ 4bpp 1D, `attr2 & 0x3FF` fornece o tile inicial e `(attr2 >> 12) & 15`, o banco de paleta. Compare VRAM de objeto e paleta carregada com os candidatos da ROM. Antes de interpretar os bits de flip de `attr1`, confira se o objeto usa transformação afim. O procedimento detalhado está em [chat eclat.md](chat%20eclat.md#3-interpretar-um-sprite-do-gba).

## 5. Como porto mapas de FireRed para Emerald

Siga o cabeçalho do mapa da ROM de origem até layout, tilesets, objetos, warps, conexões e scripts. Confira a largura e a altura antes de ler `width * height * 2` bytes das células. No layout portado aqui, os dez bits baixos da célula identificam o metatile; os bits 10–11 tratam da colisão e os bits 12–15, da elevação. O desenho do chão não define sozinho seu comportamento: um atributo errado pode produzir grama, água, Dive ou encontro selvagem sobre piso seco.

Este projeto usa `tile_format: frlg` em layouts importados. Isso exige compatibilizar os atributos e o empacotamento de tilesets na engine Emerald. Para o **Porymap**, Ivière também precisou de uma representação própria em `data/tilesets/{primary,secondary}/eclat_iviere_*`: os IDs globais foram preservados, mas o particionamento de tiles e paletas foi ajustado para o editor. Os binários que a ROM compila e os arquivos de visualização do Porymap não devem ser confundidos. Veja [porymap-iviere.md](porymap-iviere.md).

**Exemplo de localização, não fórmula universal:** em Ivière, o cabeçalho da ROM original no offset `0x34AA68` aponta para o layout `0x08870E48` (48×38 metatiles) e para os eventos `0x08712E64`. O ponteiro em `eventos + 4` chega à tabela de objetos `0x08712A2C`. Cada registro de objeto analisado tem 24 bytes; `+0` é o ID local, `+1` o ID gráfico, `+4/+6` as coordenadas, `+9` o movimento e `+10` os alcances. A contagem observada foi 33. Esses valores foram aceitos porque os objetos reconstruídos coincidiam com as posições vistas no jogo. Em outra versão, releia cabeçalho, contagem e ponteiros; não reaproveite esses endereços por hábito.

No caso do Porymap, o problema preto/magenta de Ivière não era um defeito da ROM compilada. O editor esperava outro particionamento de tilesets: o espelho de visualização reorganizou 640+384 entradas FRLG em 512+512, preservando os IDs globais. Também adaptou atributos de 32 para 16 bits e ofereceu ao secundário a paleta necessária. Uma correção no espelho do editor não altera automaticamente os binários usados pelo jogo.

**Warp não é conexão de borda.** Ivière → portaria → Route 1 usa warps com índices de destino nos dois lados. Esses mapas não aparecem colados na aba Connections. Route 1 ↔ Petiprés usa conexão direta de borda, com direção e deslocamento recíprocos; o Porymap pode mostrá-los juntos. Nunca crie uma conexão só para satisfazer a visualização do editor se a referência atravessa uma porta.

## 6. Como leio e reescrevo scripts e eventos

Desenhe o fluxo antes de editar:

```text
entrada no mapa / interação / gatilho
  → condição de flag ou variável
  → fala, câmera, som, movimento e esperas
  → entrega ou batalha
  → alteração persistente
  → liberação do controle
  → comportamento ao sair e retornar
```

No Emerald, `map.json` posiciona cada objeto e guarda seu `local_id`, gráfico, movimento, script e flag de visibilidade. `scripts.inc` contém o fluxo. `data/event_scripts.s` precisa incluir o arquivo. O mapa gerado referencia um símbolo `<NomeDoMapa>_MapScripts`. Se o símbolo ou um descritor gráfico faltar, a montagem ou o link falha. Se o `local_id` usado por `applymovement` ou `removeobject` não corresponder ao objeto, a cena pode mover ou remover a entidade errada.

Para recuperar o script da origem, começo pelo objeto ou gatilho da tabela de eventos, sigo seu ponteiro de script e marco cada comando de controle, salto, chamada e retorno. Não paro ao encontrar uma fala legível: procuro os ramos que levam a ela, as condições que os selecionam e os efeitos posteriores. Uma busca pela sequência codificada da fala e pelas ocorrências little-endian do ponteiro ajuda a encontrar chamadas relacionadas, mas uma ocorrência isolada pode ser falso positivo. Em seguida comparo o percurso no emulador, inclusive ao conversar de novo e ao regressar ao mapa. Na codificação examinada, `0xFF` encerra o texto, `0xFE` quebra a linha, `0xFA` rola a caixa e `0xFB` abre uma nova página; no destino esses controles devem virar convenções de string equivalentes e ser testados visualmente.

O script novo não precisa ter os mesmos bytes nem os mesmos endereços de FireRed. Precisa preservar as condições observáveis: quem fala, onde o jogador e a câmera ficam, quanto cada movimento espera, qual item ou batalha ocorre, qual estado fica salvo e quando o controle retorna. Confiro também os ganchos `MAP_SCRIPT_ON_TRANSITION`, `MAP_SCRIPT_ON_FRAME_TABLE` e `MAP_SCRIPT_ON_RESUME` conforme a finalidade. Uma cena que funciona na primeira entrada pode falhar depois de uma batalha se o retorno não recriar sprites ou reinicializar um serviço.

No evento de Myst da Route 1, `EclatRoute1_OnFrame` chama a cena enquanto `VAR_ECLAT_ROUTE1_STATE == 0`. `EclatRoute1_MeetMyst` trava o controle, espera movimentos, apresenta as falas, move e remove Myst, grava `FLAG_ECLAT_ROUTE1_MET_MYST`, muda a variável para 1 e libera o controle. O estado impede repetir a cena ao voltar. `OnResume` reinicia os sprites voadores após retornar de batalha ou interior. Para verificar, teste antes da cena, durante, após, depois de sair/voltar e após recarregar um save.

Traduza **significado**, não número bruto. IDs de espécies, itens, golpes, treinadores, flags, variáveis, músicas e comportamentos de metatile podem ter outra enumeração no Emerald. Na Route 2, por exemplo, a tabela de itens modificada em `0xBA8F00` e a tabela de treinadores em `0x238ED4` foram conferidas pelos nomes e descrições da ROM de Éclat. Um ID de FireRed limpo teria atribuído itens e equipes errados. Veja [route2.md](route2.md#treinadores-e-itens).

Em entregas, confirme que o item ou Pokémon foi recebido **antes** de fixar a flag de conclusão. Em cenas de câmera, espere o movimento, devolva a câmera e remova o objeto temporário antes de `releaseall`. Em cura, whiteout e serviços de sala de conexão, copie também os ganchos de ciclo de vida necessários. Um script aparentemente correto pode falhar na segunda interação se a inicialização compartilhada estiver ausente.

## 7. Estudo de caso: Route 1 no slot de Route 101

O problema observado foi estrutural: a primeira rota jogável de Éclat estava em `MAP_ECLAT_ROUTE1`, enquanto `MAP_ROUTE101` ainda apontava para a antiga Route 101. O Porymap listava as duas. Verifiquei `map_groups.json`, os dois `map.json`, as entradas da portaria, a conexão de Petiprés, encontros selvagens, scripts e usos de `MAP_ROUTE101` no C.

A migração foi feita assim:

1. Criei `data/maps/Route1/map.json` com o layout, objetos, portas e placas de Éclat, mas com ID `MAP_ROUTE101`.
2. Troquei **somente a entrada do índice 16 do grupo 0** de `Route101` para `Route1`. Não removi `EclatRoute1` do grupo: sua exclusão deslocaria IDs de mapas posteriores e afetaria saves.
3. Criei `Route1_MapScripts` como ponte para os eventos já implementados em `EclatRoute1/scripts.inc`; mantive os dois arquivos incluídos em `data/event_scripts.s`, pois o gerador ainda monta o cabeçalho do mapa antigo.
4. Apontei os dois warps de saída da portaria e a conexão oeste de Petiprés para `MAP_ROUTE101`. Removi a antiga conexão sul de Oldale, que ligaria Hoenn ao mapa novo.
5. Associei os encontros de Éclat ao slot principal e mantive a mesma tabela no slot legado. Atualizei o efeito de grama que testava o ID do mapa.
6. Dei à seção `MAPSEC_ROUTE_101` o nome “ROUTE 1”, coloquei-a ao lado de Ivière no mapa regional e usei a seção no mapa novo e na portaria.
7. Verifiquei os dados gerados: `Route1` continua no índice 16, usa `EclatRoute1_Layout` e aponta para `Route1_MapScripts`. A portaria tem dois warps para esse ID, Petiprés tem a conexão recíproca, Oldale não o referencia e as duas entradas de encontros coincidem.

O slot `MAP_ECLAT_ROUTE1` permanece como legado para saves que já estavam nele. A build e essas conferências estruturais passaram; **não houve novo percurso completo em emulador após a migração**. Esse teste ainda é necessário para afirmar que todos os encontros e transições se comportam como a referência.

## 8. Como encontro erros sem adivinhar

Parta do primeiro sintoma reproduzível e procure a camada que realmente falhou. Exemplos deste projeto:

| Sintoma | Prova que separou a causa | Correção |
| --- | --- | --- |
| Texto branco após curar | A paleta ainda estava correta, mas os pixels das letras faltavam na RAM e VRAM; o rastreio chegou a `StringCopy` após nome pendente sem `EOS` | Adicionar `CableClub_OnResume` e validar o terminador antes da cópia |
| Personagem sobre grama ou água invisível | O gráfico da célula era piso, mas o atributo do metatile acionava efeito de terreno | Corrigir o comportamento do metatile ou da célula afetada, preservando os outros campos |
| Porymap mostra `LittlerootTown` | Esse é o nome interno/pasta; a seção regional já dizia “IVIERE TOWN” | Explicar os três nomes antes de renomear IDs ou pastas |
| Ivière não aparece colada à Route 1 | Há uma portaria intermediária e warps, não conexão direta de borda | Conferir índices dos warps; não inventar conexão |
| Build: `HEAL_LOCATION_ECLAT_ROUTE2` redefinida | Três objetos idênticos em `heal_locations.json` | Remover as duas cópias extras |
| Build: campo de treinador sobrescrito | Omar, Alfred e Sammy tinham blocos repetidos em `trainers.party` | Comparar os blocos e remover apenas os idênticos |
| Link: dez `gObjectEventGraphicsInfo_EclatRoute2Npc...` ausentes | Ponteiros declarados, binários presentes, três cabeçalhos de definição vazios | Completar tiles, quadros e descritores com os dados da ROM |

O erro de texto está detalhado em [chat eclat.md](chat%20eclat.md#7-investigar-um-erro-visual-pela-memoria). Endereços de PC e RAM ali dependem da compilação investigada; depois de uma mudança de código, gere símbolos novos. Um erro de cor aparente pode ser um buffer de fonte corrompido; um sprite invisível pode ser um símbolo não ligado; um warp errado pode ser um índice de destino, não uma coordenada de borda.

## 9. Build, prova e entrega

No PowerShell, compile dentro do WSL usando um caminho Linux:

```powershell
wsl --exec bash -lc 'cd /mnt/c/Users/raulr/OneDrive/Desktop/zEmerald/eclat-intro-teste && make -j4 emerald'
```

Neste worktree, o arquivo `.git` aponta para um caminho Windows que o Git do WSL não resolve. O próprio `check_history.sh` aceita `.histignore` para essa situação; o marcador foi criado na raiz do projeto. Ele permite compilar, mas não conserta o acesso do Git no WSL. Use o Git do Windows para inspecionar o histórico deste worktree.

A saída do alvo é `pokeemerald.gba`. Na migração da Route 1, a build terminou, a ROM anterior foi guardada em `zEmerald/backups/antes-migracao-route1-2026-11-07.gba`, e a nova foi copiada para `eclat-intro-teste.gba`. O SHA-256 desta cópia é `DCD4D3B7B5F0EBC1D2B8FB1E0017FD327055019BEE95A8943D3D1BAF8251415C`. O save `.sav` não foi alterado.

Para cada entrega seguinte, registre: hash da ROM de origem, arquivos modificados, bytes ou estruturas que fundamentam a extração, estado inicial, percurso testado, estados alternativos, resultado da build e limitações. Um teste mínimo de mapa com evento deve cobrir entrada, interação, saída, retorno e save/reload. Para sprite animado, compare múltiplos quadros e a relação com a câmera. Para batalha, teste barras parcialmente preenchidas e retorno aos menus.

## 10. Instruções para outra IA continuar

- Leia primeiro este arquivo, [metodologia.md](metodologia.md), [chat eclat.md](chat%20eclat.md) e o documento da área em questão. Confira o código atual; um relato antigo pode ter sido superado.
- Antes de editar, escreva uma hipótese falsificável: “o warp de saída aponta ao ID errado”, “a paleta tem tag errada”, “o script não roda no retorno”. Registre a prova que confirmaria ou derrubaria a hipótese.
- Para cada ID da ROM de origem, procure o **nome** do recurso e mapeie-o para uma constante do Emerald. Confira tamanho, limites, endereço e versão da ROM.
- Preserve a ordem de mapas, flags persistentes e IDs usados em saves. Uma mudança de nome no Porymap não é automaticamente uma migração de dados.
- Mantenha extração, adaptação e validação como etapas separadas. Se uma etapa não passou, diga exatamente qual e por quê.
- Ao corrigir erro de build, encontre a primeira mensagem útil do compilador ou linker; não altere gráficos ou scripts por suposição. Se o erro for anterior à mudança atual, registre isso e faça a menor correção comprovada.
- Compare o resultado com a ROM original no mesmo estado do jogo. Não declare equivalência quadro a quadro com base apenas em compilação, screenshot ou memória de uma jogada.

## 11. Método aplicado à integração do Cube na Bag

Para portar uma interface de outra ROM sem quebrar a lógica existente, a análise foi feita em quatro camadas:

1. **Referência jogável**: abrir o Unbound no mesmo tamanho de tela da GBA e observar o Cube em uso, anotando a função de cada região (título, sprite, lista, descrição e comando de ordenação).
2. **Mapeamento para a engine**: localizar as janelas Emerald responsáveis por essas regiões em `sDefaultBagWindows`, identificar os callbacks que imprimem itens/quantidades e separar geometria visual de regras de inventário.
3. **Mudança mínima comprovável**: manter os bolsos, índices, callbacks, capacidades e estados de seleção originais; alterar somente textos, larguras/posições de janelas e a âncora do sprite já usado pela Bag. Isso evita introduzir uma segunda implementação de inventário.
4. **Build e rastreabilidade**: recompilar a ROM, registrar o uso de memória, copiar o artefato final e salvar o SHA-256. A compilação comprova integração de código, mas não substitui a inspeção visual em emulador.

### Como interpretar a tela do Cube

O ponto importante é não tratar a imagem como um único bitmap. O Unbound combina um sprite animado do dispositivo, molduras de janela, texto e uma lista dinâmica. Por isso, a adaptação correta começa identificando quais pixels pertencem a gráficos persistentes e quais são desenhados pela engine a cada estado. No projeto atual, `graphics/bag/bag_male.png`/`bag_female.png` já são sprites de seis quadros com aparência de dispositivo; reutilizá-los preserva animação e paleta enquanto a geometria é ajustada no código.

### Próxima verificação obrigatória

Abrir a Bag da ROM recém-compilada e conferir: alinhamento do título, largura da lista, quantidades no limite direito, descrição de itens longos, troca de bolso, animação do Cube e retorno com `B`. Só depois dessa conferência deve ser adicionada a arte dedicada do painel ou a indicação `START Sort`; caso contrário, um ajuste gráfico pode mascarar um erro de janela ou de callback.

### Diagnóstico da primeira falha visual

A captura com listras não foi tratada como defeito aleatório de paleta. O padrão se repetia exatamente no intervalo acrescentado à direita da lista, o que apontou para uma incompatibilidade entre a largura da `WindowTemplate` e a margem reservada no tilemap da Bag. A conferência do recurso `graphics/bag/menu.bin` e do limite original de 16 tiles confirmou a hipótese; a largura foi restaurada sem alterar o callback da lista.

O texto inferior exigiu uma segunda correção independente. O sprite do item é criado fora da janela, em `x2 = 24`, enquanto todas as rotinas de descrição imprimiam com deslocamento local de 3 pixels. Ao mover a janela para o painel inferior completo, esse deslocamento passou a colocar as letras sobre o ícone. A constante `BAG_DESCRIPTION_TEXT_LEFT` centraliza a decisão e foi aplicada também às mensagens de erro e aos caminhos de uso/troca, para não corrigir apenas o caso normal.
