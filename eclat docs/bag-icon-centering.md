# Centralização do Ícone do Item na Bag
**Projeto:** Pokémon Éclat Pourpre (pokeemerald)  

---

## 📌 Visão Geral

Correção de alinhamento pixel-perfect do ícone 24x24 do item na caixa branca de descrição da Bag (Mochila / Cube).

---

## 📂 Arquivo Modificado

- [`src/item_menu_icons.c`](file:///C:/Users/raulr/OneDrive/Desktop/zEmerald/eclat-intro-teste/src/item_menu_icons.c#L480-L484)

---

## ⚙️ Detalhes Matemáticos da Correção
- **Caixa Branca**:
  - Horizontal: `X = 7..32` (26px, centro = 19.5).
  - Vertical: `Y = 123..149` (27px, centro = 136.0).
- **Sprite 32x32 com imagem 24x24**:
  - `x2 = 24`: Posiciona a imagem em `X = [8..31]` (1px de margem esquerda, 1px de margem direita).
  - `y2 = 140` (antes era 136): Posiciona a imagem em `Y = [124..147]` (1px de margem superior, 2px de margem inferior).
