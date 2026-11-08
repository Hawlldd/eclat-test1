# Porymap e Ivière

O mapa de Ivière usa `MAP_LITTLEROOT_TOWN` e `LAYOUT_LITTLEROOT_TOWN`, mas os tilesets são extraídos do Éclat Pourpre em formato binário. Na ROM, as declarações em `src/data/tilesets/headers.h` leem `data/tilesets/{primary,secondary}/eclat_iviere/*.bin`.

O Porymap 6.3.1 não localizou esses arquivos através das declarações personalizadas e inferiu os nomes `eclat_iviere_primary` e `eclat_iviere_secondary`. Seu log registrou a ausência de `tiles.png`, `palettes/00.pal` a `12.pal`, `metatiles.bin` e `metatile_attributes.bin` nessas pastas. Foram criadas representações compatíveis nesses dois caminhos a partir dos dados binários originais, sem alterar as entradas usadas pela compilação da ROM.

O primeiro espelho abria com manchas pretas e magenta. O log mostrou que o Porymap impunha 512 tiles/metatiles primários e seis paletas primárias, enquanto o layout FRLG de Ivière usa 640/640 e sete. Uma tentativa de colocar `define_*_primary` diretamente em `porymap.project.cfg` falhou porque o Porymap só aceita essas opções com o prefixo `ident/` e regravou o arquivo. Não mantemos essa mudança global.

O conversor `tools/eclat/porymap_iviere_compat.py` reorganiza somente as cópias do editor. Mantém a ordem global dos 1024 tiles e metatiles, mas desloca os 128 últimos do primário FRLG para o início do secundário Porymap. Assim cada tileset de visualização tem 512 entradas. A sétima paleta primária FRLG é oferecida ao Porymap como primeira paleta secundária. Cada atributo FRLG de 32 bits é convertido para 16 bits, mantendo o comportamento básico e o tipo de camada. Os binários originais da ROM permanecem intactos.

Os arquivos compatíveis permitem visualizar o mapa. Edições de mapa, eventos e conexões feitas no Porymap afetam seus respectivos arquivos de projeto. Edições no editor de tilesets feitas sobre `eclat_iviere_primary` ou `eclat_iviere_secondary` não atualizam automaticamente os binários `eclat_iviere` usados pela ROM; esses gráficos exigem sincronização antes de compilar.

O conversor também gera prévias PNG dos três sprites de NPC que o Porymap buscava. Após recarregar o projeto, Ivière abriu sem manchas pretas ou magenta e o painel de eventos mostrou o sprite do NPC selecionado. O log da nova abertura não registrou mais avisos de tilesets ou desses sprites. Os mapas Hoenn continuam usando a configuração padrão de Emerald.

Para regenerar as cópias após alterar os binários originais, execute `python3 tools/eclat/porymap_iviere_compat.py` na raiz do projeto e recarregue o projeto no Porymap. Edições feitas diretamente nos tilesets de visualização do Porymap ainda não são convertidas de volta para os binários da ROM.
