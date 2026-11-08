# Validação da Route 2 — 07/11/2026

ROM entregue: `eclat-intro-teste.gba`.

SHA-256: `a0e74f9780bdb18b237fa2fcc0dd3ff249c0647e90993cfcbaefd5d0ce161d94`.

Compilação Emerald concluída. Conferidos os arquivos alterados da engine entre a pasta de trabalho e a cópia utilizada pelo compilador WSL. ROM preenchida até 32 MiB; conteúdo ligado ocupa 27.351.080 bytes. O backup anterior está em `../backups/antes-route2-2026-11-07` a partir da pasta do projeto, com ROM, arquivos anteriores e manifesto.

## Verificação executada no mGBA

O teste percorreu um jogo novo, incluindo os nomes, a introdução, a escolha de Fuecoco, a casa, Ivière, a Route 1 e Petiprés. Na parte nova, confirmou:

- entrada pela fronteira leste de Petiprés e posição (0,14) na Route 2;
- nove voos ativos na Route 2;
- travessia das pontes, dos barrancos e do percurso até a entrada inferior do Rocky Path;
- dezoito voos ativos de Zubat na caverna;
- escada interna ligando as duas partes do Rocky Path;
- saída para a parte superior da Route 2 e recriação dos nove voos;
- entrada e saída do Centro Pokémon;
- três conversas com a enfermeira, alternando aceitar e recusar, com letras visíveis nas opções e controle devolvido;
- batalha contra os três Pokémon do Rocket e conclusão da conversa posterior;
- flag de vitória do treinador 864 em 0x4E0, sem alteração do byte das flags de sistema em 0x860;
- movimento do jogador após o fim da cena.

Para isolar a navegação, o teste mantém Repel e marca como derrotados os sete treinadores comuns. Isso não substitui jogar individualmente suas batalhas. Para agilizar a batalha Rocket, apenas na memória do teste, ajusta o Fuecoco ao nível 50, com experiência coerente, checksum válido e estatísticas ampliadas. Não há essa vantagem na ROM entregue.

O teste está em `C:/Users/raulr/Documents/Codex/2026-11-06/oi-chat-pdoeria-me-ajduar-a/work/test-route2.c`, com execução por `test-route2.sh`. A compilação foi feita na cópia apontada por `/var/tmp/eclat-intro-build-path`. As capturas estão no diretório `route2-check` da instalação mGBA apontada por `/var/tmp/eclat-mgba-path`.

## Limites da prova

Não foram testados individualmente todos os itens, serviços de conexão e a recompensa de FLASH. As portarias herdadas não foram apresentadas como portas ativas do percurso atual. A cena posterior de Ash/Misty/Brock e as áreas seguintes continuam pendentes. A encenação Rocket e a cadência dos voos não foram declaradas idênticas quadro a quadro à referência.

Ver [route2.md](route2.md) para endereços e decisões de implementação.
