EclatRoute2ResearchGate_MapScripts::
	.byte 0

EclatRoute2ResearchGate_Npc2::
	msgbox EclatRoute2ResearchGate_Npc2_Text, MSGBOX_NPC
	end

EclatRoute2ResearchGate_Npc2_Text:
	.string "Si un POKéMON a appris FLASH,\ntu pourras traverser la GROTTE.$"
EclatRoute2ResearchGate_Aide::
	lock
	faceplayer
	goto_if_set FLAG_ECLAT_ROUTE2_FLASH, EclatRoute2ResearchGate_AideAfter
	msgbox EclatRoute2ResearchGate_AideOffer, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, EclatRoute2ResearchGate_AideEnd
	setvar VAR_0x8004, 1
	special GetFrlgPokedexCount
	goto_if_lt VAR_0x8006, 10, EclatRoute2ResearchGate_NotEnough
	giveitem ITEM_TM70
	goto_if_eq VAR_RESULT, FALSE, EclatRoute2ResearchGate_AideEnd
	setflag FLAG_ECLAT_ROUTE2_FLASH
EclatRoute2ResearchGate_AideAfter:
	msgbox EclatRoute2ResearchGate_Flash, MSGBOX_DEFAULT
EclatRoute2ResearchGate_AideEnd:
	release
	end
EclatRoute2ResearchGate_NotEnough:
	msgbox EclatRoute2ResearchGate_CatchMore, MSGBOX_DEFAULT
	goto EclatRoute2ResearchGate_AideEnd
EclatRoute2ResearchGate_AideOffer:
	.string "I'm a professor's aide. If you've\n"
	.string "caught 10 different POKéMON,\l"
	.string "I have FLASH for you!\p"
	.string "Shall I check your POKéDEX?$"
EclatRoute2ResearchGate_Flash:
	.string "FLASH lights up dark caves.$"
EclatRoute2ResearchGate_CatchMore:
	.string "You still need to catch more kinds\n"
	.string "of POKéMON. Come back with 10!$"

