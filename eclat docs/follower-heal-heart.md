# Notificação de Coração no Follower ao Curar a Equipe
**Projeto:** Pokémon Éclat Pourpre (pokeemerald)  
**Data:** 08/11/2026  

---

## 📌 Visão Geral

Inspirado na mecânica clássica de afeto e resposta de *Pokémon HeartGold & SoulSilver (HGSS)*, este recurso faz com que o Pokémon seguidor (*Follower Pokémon*) exiba uma notificação visual de coração (`FOLLOWER_EMOTION_LOVE`) e toque o chime característico de carinho (`SE_CONTEST_HEART`) sempre que o jogador cura a sua equipe.

Isso reforça o vínculo emocional entre o treinador e o parceiro, dando vida ao overworld e fornecendo feedback positivo imediato de descanso e recuperação.

---

## 📂 Arquivos Modificados

| Arquivo | Função / Modificação |
| :--- | :--- |
| [`include/event_object_movement.h`](file:///C:/Users/raulr/OneDrive/Desktop/zEmerald/eclat-intro-teste/include/event_object_movement.h) | Declaração da função pública `ShowFollowerHeartEmote(void)`. |
| [`src/event_object_movement.c`](file:///C:/Users/raulr/OneDrive/Desktop/zEmerald/eclat-intro-teste/src/event_object_movement.c) | Implementação de `ShowFollowerHeartEmote()` com validações estritas de estado e visibilidade. |
| [`data/event_scripts.s`](file:///C:/Users/raulr/OneDrive/Desktop/zEmerald/eclat-intro-teste/data/event_scripts.s) | Integração em `Common_EventScript_OutOfCenterPartyHeal` (casa da mãe, pontos de descanso, camas). |
| [`data/scripts/pkmn_center_nurse.inc`](file:///C:/Users/raulr/OneDrive/Desktop/zEmerald/eclat-intro-teste/data/scripts/pkmn_center_nurse.inc) | Integração em `EventScript_PkmnCenterNurse_TakeAndHealPkmn` (balcão de todos os Centros Pokémon). |
| [`data/scripts/pkmn_center_nurse_frlg.inc`](file:///C:/Users/raulr/OneDrive/Desktop/zEmerald/eclat-intro-teste/data/scripts/pkmn_center_nurse_frlg.inc) | Integração no fluxo de cura da enfermeira estilo FRLG. |

---

## ⚙️ Regras e Validações de Segurança

Para evitar falhas visuais ou travamentos em momentos onde o Pokémon não está acompanhando o jogador (ex: Pokémon na Pokébola, em cima da bicicleta, surfando, desmaiado ou em mapas internos onde espécies grandes ficam ocultas), a rotina verifica todas as condições antes de disparar o efeito:

```c
void ShowFollowerHeartEmote(void)
{
    struct ObjectEvent *follower = GetFollowerObject();
    if (follower != NULL && follower->active && !follower->invisible && IsFollowerVisible())
    {
        ObjectEventEmote(follower, FOLLOWER_EMOTION_LOVE);
        PlaySE(SE_CONTEST_HEART);
    }
}
```

- **`follower != NULL`**: Garante que o slot do evento de objeto do seguidor está alocado.
- **`follower->active`**: Garante que o objeto está ativo no mapa atual.
- **`!follower->invisible`**: Evita disparar em Pokémon que estejam ocultos ou recém-gerados na mesma coordenada do jogador.
- **`IsFollowerVisible()`**: Respeita flags de invisibilidade (bicicleta, metatile de água/surf, etc.).
- **`PlaySE(SE_CONTEST_HEART)`**: Toca o efeito sonoro suave de afeto sincronizado com a animação do balão.
