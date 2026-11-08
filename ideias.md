# Ideias e Planejamento Futuro do Projeto

Este documento reúne melhorias, ideias e refatorações planejadas para execução em momentos oportunos, sem interromper as prioridades atuais do desenvolvimento.

---

## 🗺️ 1. Reorganização e Limpeza dos Mapas (Porymap & ROM)

### 🎯 Filosofia
Substituir os mapas originais de Hoenn/Emerald na ROM em vez de criar mapas extras soltos dentro de grupos errados. Isso garante:
- Árvore do Porymap limpa e intuitiva.
- Economia de slots, tabelas e ponteiros na ROM (sustentabilidade a longo prazo).
- Facilidade na integração com Pokenav, Town Map e fly spots.

---

### 📋 Plano de Migração dos Mapas

#### A. Rotas Principais (Mover para `gMapGroup_TownsAndRoutes`)
- [ ] **Rota 1:**
  - Migrar `EclatRoute1` para ocupar oficialmente o slot de **`MAP_ROUTE101`** no grupo principal (`gMapGroup_TownsAndRoutes`).
  - Remover a entrada duplicada `EclatRoute1` de `gMapGroup_IndoorLittleroot`.
  - Atualizar warps do portão de Iviere e conexões de mapa com Petipres.

- [ ] **Rota 2:**
  - Migrar `EclatRoute2` para ocupar oficialmente o slot de **`MAP_ROUTE102`** no grupo principal (`gMapGroup_TownsAndRoutes`).
  - Remover a entrada duplicada `EclatRoute2` de `gMapGroup_IndoorLittleroot`.
  - Atualizar conexões leste/oeste com Petipres e Peaceful Woods.

#### B. Florestas e Cavernas (Mover para `gMapGroup_Dungeons`)
- [ ] **Peaceful Woods:**
  - Tirar de `gMapGroup_IndoorLittleroot` e colocar em **`gMapGroup_Dungeons`** (mesmo grupo da Petalburg Woods).
- [ ] **EclatRockyPath:**
  - Tirar de `gMapGroup_IndoorLittleroot` e colocar em **`gMapGroup_Dungeons`** (junto de cavernas como Rusturf Tunnel / Granite Cave).

#### C. Interiores e Edifícios de Rota
- [ ] **Centro Pokémon da Rota 2:**
  - Mover `EclatRoute2Center` e `EclatRoute2Center_2F` para um grupo dedicado de interiores de rotas.
- [ ] **Portões de Rota:**
  - Mover `EclatRoute2Gate` e `EclatRoute2ResearchGate` para seus respectivos grupos de portões/interiores.

#### D. Limpeza Final de `gMapGroup_IndoorLittleroot`
- [ ] Deixar em `gMapGroup_IndoorLittleroot` apenas os interiores reais de Iviere/Littleroot (Casas do jogador/rival e Laboratório do Prof.).

---

## 💡 2. Novas Ideias & Próximas Prioridades

*(Espaço reservado para adicionar novos tópicos e mecânicas conforme formos discutindo)*
