Route2_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Route2_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, Route2_OnResume
	.byte 0

Route2_OnTransition:
EclatRoute2_OnTransition::
	setflag FLAG_ECLAT_ROUTE2_MYST_HIDDEN
	checktrainerflag TRAINER_ECLAT_ROUTE2_ROCKET
	goto_if_eq VAR_RESULT, TRUE, Route2_RocketsAlreadyBeaten
	end

Route2_RocketsAlreadyBeaten:
EclatRoute2_RocketsAlreadyBeaten::
	setflag FLAG_ECLAT_ROUTE2_ROCKETS_DONE
	end

Route2_OnResume:
EclatRoute2_OnResume::
	special StartEclatRoute2Flybys
	end

Route2_Npc5::
EclatRoute2_Npc5::
	finditem ITEM_ETHER
	end

Route2_Npc6::
EclatRoute2_Npc6::
	finditem ITEM_PARALYZE_HEAL
	end

Route2_Npc11::
EclatRoute2_Npc11::
	lock
	faceplayer
	playmoncry SPECIES_RATTATA, CRY_MODE_NORMAL
	msgbox EclatRoute2_Npc11_Text, MSGBOX_DEFAULT
	waitmoncry
	release
	end
EclatRoute2_Npc11_Text:
	.string "RATTATA : Crr crr crr.$"

Route2_Trainer12::
EclatRoute2_Trainer12::
	trainerbattle_single TRAINER_ECLAT_ROUTE2_PIOTR, EclatRoute2_Trainer12_Intro, EclatRoute2_Trainer12_Defeat
	msgbox EclatRoute2_Trainer12_After, MSGBOX_AUTOCLOSE
	end

EclatRoute2_Trainer12_Intro:
	.string "I'm catching bugs in the forest,\nso let me know what you find.$"
EclatRoute2_Trainer12_Defeat:
	.string "What? I must not have trained\nmy insects enough...$"
EclatRoute2_Trainer12_After:
	.string "There are a lot of different\nPOKéMON in the forest!$"

Route2_Npc13::
EclatRoute2_Npc13::
	lock
	faceplayer
	playmoncry SPECIES_RATTATA, CRY_MODE_NORMAL
	msgbox EclatRoute2_Npc13_Text, MSGBOX_DEFAULT
	waitmoncry
	release
	end
EclatRoute2_Npc13_Text:
	.string "RATTATA : Crr crr crr.$"

Route2_Npc14::
EclatRoute2_Npc14::
	lock
	faceplayer
	playmoncry SPECIES_RATTATA, CRY_MODE_NORMAL
	msgbox EclatRoute2_Npc14_Text, MSGBOX_DEFAULT
	waitmoncry
	release
	end
EclatRoute2_Npc14_Text:
	.string "RATTATA : Crr crr crr.$"

Route2_Npc17::
EclatRoute2_Npc17::
	lock
	faceplayer
	playmoncry SPECIES_RATTATA, CRY_MODE_NORMAL
	msgbox EclatRoute2_Npc17_Text, MSGBOX_DEFAULT
	waitmoncry
	release
	end
EclatRoute2_Npc17_Text:
	.string "RATTATA : Crr crr crr.$"

Route2_Trainer21::
EclatRoute2_Trainer21::
	trainerbattle_single TRAINER_ECLAT_ROUTE2_PERRINE, EclatRoute2_Trainer21_Intro, EclatRoute2_Trainer21_Defeat
	msgbox EclatRoute2_Trainer21_After, MSGBOX_AUTOCLOSE
	end

EclatRoute2_Trainer21_Intro:
	.string "Do you want my picture?$"
EclatRoute2_Trainer21_Defeat:
	.string "Hey, that's my picture!$"
EclatRoute2_Trainer21_After:
	.string "Si tu évites les regards des DRESS\nEURS, ils ne te défieront pas.\lSinon, tu dois accepter le combat.$"

Route2_Npc25::
EclatRoute2_Npc25::
	lock
	faceplayer
	playmoncry SPECIES_RATTATA, CRY_MODE_NORMAL
	msgbox EclatRoute2_Npc25_Text, MSGBOX_DEFAULT
	waitmoncry
	release
	end
EclatRoute2_Npc25_Text:
	.string "RATTATA : Crr crr crr.$"

Route2_Trainer26::
EclatRoute2_Trainer26::
	trainerbattle_single TRAINER_ECLAT_ROUTE2_GREGORY, EclatRoute2_Trainer26_Intro, EclatRoute2_Trainer26_Defeat
	msgbox EclatRoute2_Trainer26_After, MSGBOX_AUTOCLOSE
	end

EclatRoute2_Trainer26_Intro:
	.string "I captured my POKéMON in the\nPEACEFUL WOODS.\lIt's really cool.$"
EclatRoute2_Trainer26_Defeat:
	.string "Maybe I should have captured more\nPOKéMON.$"
EclatRoute2_Trainer26_After:
	.string "Sometimes it's worth it\nto spend hours searching for\lrare POKéMON.$"

Route2_Npc27::
EclatRoute2_Npc27::
	msgbox EclatRoute2_Npc27_Text, MSGBOX_NPC
	end

EclatRoute2_Npc27_Text:
	.string "Want a tip?\nUse HEADBUTT on\lcertain trees.\nSometimes a POKéMON\ncan fall out.$"

Route2_Npc28::
EclatRoute2_Npc28::
	lock
	faceplayer
	playmoncry SPECIES_RATTATA, CRY_MODE_NORMAL
	msgbox EclatRoute2_Npc28_Text, MSGBOX_DEFAULT
	waitmoncry
	release
	end
EclatRoute2_Npc28_Text:
	.string "RATTATA : Crr crr crr.$"

Route2_Npc37::
EclatRoute2_Npc37::
	finditem ITEM_WHIPPED_DREAM
	end

Route2_Npc38::
EclatRoute2_Npc38::
	finditem ITEM_LOPUNNITE
	end

Route2_Npc39::
EclatRoute2_Npc39::
	lock
	faceplayer
	msgbox EclatRoute1_Text_LotteryFound, MSGBOX_DEFAULT
	giveitem ITEM_ECLAT_LOTTERY_TICKET
	goto_if_eq VAR_RESULT, FALSE, EclatRoute2_Npc39_Done
	setflag FLAG_ECLAT_ROUTE2_TICKET
	msgbox EclatRoute1_Text_LotteryUse, MSGBOX_DEFAULT
	removeobject VAR_LAST_TALKED
EclatRoute2_Npc39_Done:
	release
	end

Route2_Sign0::
EclatRoute2_Sign0::
	msgbox EclatRoute2_Sign0_Text, MSGBOX_NPC
	end

EclatRoute2_Sign0_Text:
	.string "Welcome to ROUTE 2$"

Route2_Sign1::
EclatRoute2_Sign1::
	msgbox EclatRoute2_Sign1_Text, MSGBOX_NPC
	end

EclatRoute2_Sign1_Text:
	.string "ROUTE 2\nPETIPRES - PERCHARBRE$"

Route2_Sign2::
EclatRoute2_Sign2::
	msgbox EclatRoute2_Sign2_Text, MSGBOX_NPC
	end

EclatRoute2_Sign2_Text:
	.string "PEACEFUL WOODS entrance$"

Route2_Sign3::
EclatRoute2_Sign3::
	msgbox EclatRoute2_Sign3_Text, MSGBOX_NPC
	end

EclatRoute2_Sign3_Text:
	.string "Entrée du CHEMIN ROCAILLEUX$"

	.include "data/maps/Route2/rocket.inc"
