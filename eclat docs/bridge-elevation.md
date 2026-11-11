# Mecânica de Pontes e Elevação (Multi-Level Elevation)
**Projeto:** Pokémon Éclat Pourpre (pokeemerald)  

---

## 📌 Visão Geral

Documentação do sistema de pontes com elevação multinível (`ELEVATION_MULTI_LEVEL = 15`), permitindo que o jogador e o Pokémon seguidor (*Follower Pokémon*) caminhem por cima da ponte sem que a cabeça seja cortada ao entrar pela rampa, e passem por baixo da ponte sem colisões indevidas.

---

## 📂 Arquivos Modificados

| Arquivo | Descrição |
| :--- | :--- |
| `include/constants/metatile_behaviors.h` | Definição de comportamentos de ponte e elevação. |
| `src/event_object_movement.c` | Lógica de renderização de sprites e elevação de camadas. |
| `data/tilesets/primary/` | Metatiles com divisão de camadas (NORMAL no vão, COVERED na rampa). |

---

## ⚙️ Regras de Elevação
- **Vão da ponte**: Metatiles com comportamento normal de passagem e elevação elevada.
- **Rampa de entrada**: Ajustada para que o sprite do jogador e do follower não sofram clipping no topo do tile.
- **Passagem inferior**: Permite movimentação horizontal e vertical livre por baixo da estrutura.
