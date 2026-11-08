# Route 2 — port para Emerald

Esta etapa importa a primeira visita à Route 2 e ao Rocky Path. Não representa uma reprodução integral de toda a progressão futura da região. Consulte as pendências ao final antes de continuar o trabalho.

## Origem e mapas

ROM de referência: `../eclat/Eclat Pourpre ENG (Raizen).gba`. SHA-256 `9dadfec5e707e8be3e8f5f12dfd3bfd1b7448ae78c7d955f8e9593c91c7c045a`.

| Origem grupo/mapa | Cabeçalho ROM | Destino | ID grupo/mapa |
|---|---|---|---|
| 3/20 | `0x34AC98` | `EclatRoute2` | 1/14 |
| 1/122 | `0x34A3BC` | `EclatRockyPath` | 1/15 |
| 15/0 | `0x34BBB0` | `EclatRoute2Gate` | 1/16 |
| 15/2 | `0x34BBE8` | `EclatRoute2ResearchGate` | 1/17 |
| 16/0 | `0x34BC20` | `EclatRoute2Center` | 1/18 |
| salão superior reutilizado | layout já portado | `EclatRoute2Center_2F` | 1/19 |

Os IDs foram acrescentados ao final de `gMapGroup_IndoorLittleroot`. Os mapas anteriores mantêm seus números. Route 2 mede 54×44 e Rocky Path 70×40. Os layouts conservam o formato FRLG, com 640 metatiles primários e atributos de 32 bits. Não interpretar esses dados como os atributos de 16 bits do Emerald.

A fronteira leste de Petiprés usa deslocamento +4. A fronteira oeste da Route 2 usa -4. Sair de Petiprés em (59,18) leva à Route 2 em (0,14).

## Gráficos e animações

Os tiles e sprites são extraídos dos dados binários, sem reconstrução por captura de tela. A tabela de descritores de objetos fica em `0x80E254`. Para cada ID gráfico, ler o ponteiro do descritor, dimensões, tamanho, tag de paleta e a tabela de imagens em +28. O tamanho de uma imagem é um `u16` em +4; os dois bytes seguintes podem conter `FFFF` e não fazem parte do tamanho.

Há dez novos conjuntos de sprites terrestres em `graphics/eclat_intro/route2`. Reutilizamos os conjuntos já portados para os outros NPCs, para Rattata, Myst e para a enfermeira. A árvore cortável usa quatro quadros e `sAnimTable_CuttableTree`; não ler nove quadros dela como se fosse um treinador.

Os voos ficam em `src/eclat_route2.c`. São nove instâncias na Route 2 e dezoito Zubat no Rocky Path. As coordenadas de origem são fixas no mapa. A câmera só converte essas coordenadas para a tela. Os sprites são recriados em `MAP_SCRIPT_ON_RESUME` após portas e batalhas, sem duplicar instâncias. Os quadros laterais são os índices 2 e 7, e não 0 e 1. Espelhar somente ao mudar o sentido horizontal. A cadência usa a adaptação existente do projeto e ainda não foi comparada quadro a quadro com toda a referência.

## Treinadores e itens

A tabela correta de treinadores desta ROM é `0x238ED4`, com entradas de 40 bytes. O nome começa em +4, a quantidade em +32 e o ponteiro da equipe em +36. Para equipes com golpes personalizados desta ROM, usar passo de 16 bytes, inclusive quando não há item equipado. Em flags=1 os golpes começam em +6; em flags=3, o item fica em +6 e os golpes em +8. Não deduzir o passo de memória apenas pelo formato de uma ROM limpa.

| Treinador | Equipe |
|---|---|
| PIOTR | Caterpie 5, Weedle 5, Wurmple 5 |
| PERRINE | Glameow 5, Skitty 6 |
| GREGORY | Pikachu 7 |
| CHRIS | Geodude 7, Aron 7 |
| BILL | Rockruff 7, Cubone 7 |
| BETTY | Venonat 6, Morelull 6, Dunsparce 6 |
| ELIOT | Sandshrew 6, Baltoy 6, Onix 6 |
| Rocket, origem 541 | Rattata 10, Zubat 10, Houndour 11, todos com Oran Berry |

Os golpes foram traduzidos pelos nomes da origem, tabela `0xBEBD40`, passo 13. Os nomes de espécies ficam em `0xB24900`, passo 11. Os números expandidos das espécies não são iguais aos números da base Emerald. Os retratos de batalha usam os equivalentes FRLG disponíveis na engine, com as classes correspondentes.

Os itens desta ROM também foram reorganizados. A tabela em `0xBA8F00` tem entradas de 44 bytes, com nome em +0 e ponteiro da descrição em +20.

| Objeto | Item na origem | Item no destino |
|---|---|---|
| Route 2, objeto 5 | 34, Ether | `ITEM_ETHER` |
| Route 2, objeto 6 | 18, Paralyz Heal | `ITEM_PARALYZE_HEAL` |
| Route 2, objeto 37 | 425, CHANTIBONBON | `ITEM_WHIPPED_DREAM` |
| Route 2, objeto 38 | 89, Lopunnite | `ITEM_LOPUNNITE` |
| Rocky Path, objeto 29 | 195, Everstone | `ITEM_EVERSTONE` |

A descrição do CHANTIBONBON identifica o doce de evolução de Swirlix, por isso foi associado ao Whipped Dream. Tratar o ID 89 como Escape Rope ou 195 como Hard Stone, usando uma lista de FireRed comum, daria itens errados. O NPC de TICKET usa uma flag própria da Route 2.

## Flags de treinadores e compatibilidade

A base já usava os IDs 855 e 856 para a Route 1. Os novos treinadores usam 857 a 864. O ID 864 não pode simplesmente usar `0x500 + id`: isso atingiria `FLAG_SYS_POKEMON_GET`, em 0x860.

A solução mantém as flags de sistema em seus endereços anteriores. `TRAINER_FLAG_ID(id)` conserva a fórmula original até 863 e reserva 0x4E0–0x4EF para os IDs 864–879. Essas dezesseis flags estavam declaradas como não usadas. `MAX_TRAINERS_COUNT_EMERALD` passa a 880, mas a fronteira das flags antigas permanece fixa em 864 entradas. As consultas e alterações em `src/battle_setup.c` e os comandos correspondentes de debug usam a mesma fórmula. O caminho de compilação FRLG mantém seu cálculo anterior.

Não aumentar novamente o limite sem reservar mais flags e atualizar o mapeamento. Não deslocar `SYSTEM_FLAGS`, pois isso reinterpretaria os saves existentes.

## Eventos e serviços

Os scripts locais importam diálogos, placas, itens, árvores de Cut e a interação dos Pokémon terrestres. Rattata e Geodude conservam as interações de texto e cry da referência desta rota. O encontro Rocket/Myst usa a equipe e os textos extraídos, com uma encenação adaptada ao Emerald. A flag de vitória é persistente. Ao entrar novamente no mapa, uma vitória já registrada também conclui a remoção dos Rockets, protegendo o caso de um save entre a batalha e o fim da conversa.

O Centro usa `CableClub_OnResume`, a enfermeira de dez quadros e a mesma rotina de cura já corrigida em Petiprés. O novo ponto de respawn é `HEAL_LOCATION_ECLAT_ROUTE2`. O salão superior reutiliza o layout existente e tem sua própria saída de volta ao Centro da Route 2. As escadas reutilizam os atributos corrigidos do tileset de Petiprés.

A tabela selvagem da Route 2 tem doze entradas de nível 5–6 e taxa 5. Rocky Path tem doze entradas de nível 5–7 e taxa 3. As listas foram extraídas dos cabeçalhos em `0x96AC30` e `0x96AEEC`, respectivamente. Os números são os da engine original, não porcentagens por passo.

## Limites desta entrega

- Peaceful Woods e a área ao norte ainda não foram portadas. As células de saída correspondentes estão bloqueadas. Não há redirecionamento para mapas de Hoenn.
- As duas portarias foram extraídas porque são referenciadas nos eventos antigos. Parte desses warps está sobre chão normal na versão modificada da origem, sem porta ativa. Sua existência no JSON não prova que sejam acessíveis no percurso atual. Não alterar o gráfico do chão para transformá-los em portas sem observar a ROM original.
- O evento posterior de Sacha/Ash, Misty e Brock, condicionado à progressão original, permanece pendente. Os objetos auxiliares ocultos e os objetos de preenchimento fora do piso útil não foram ativados.
- A encenação Rocket ainda exige comparação visual rigorosa de câmera e movimentos com a origem. Não afirmar equivalência quadro a quadro.
- O serviço de conexão do salão superior e a entrega de FLASH mediante dez capturas exigem validação específica. As batalhas normais de todos os sete treinadores não foram jogadas individualmente.

O inventário técnico em `route2-origem.json` permite revisar os endereços, coordenadas e IDs utilizados sem confiar na memória de outra IA.

## Validação e backup

O percurso, a cura e a batalha Rocket passaram no mGBA. Os detalhes, as condições do teste e o hash da ROM estão em [validacao-route2.md](validacao-route2.md). A versão anterior foi preservada em `../backups/antes-route2-2026-11-07`, a partir da pasta do projeto.
