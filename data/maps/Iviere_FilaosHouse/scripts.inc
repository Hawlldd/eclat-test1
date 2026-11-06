Iviere_FilaosHouse_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Iviere_FilaosHouse_OnTransition
	.byte 0

Iviere_FilaosHouse_OnTransition:
	checkitem ITEM_TOWN_MAP, 1
	goto_if_eq VAR_RESULT, FALSE, Iviere_FilaosHouse_EndTransition
	setflag FLAG_IVIERE_TOWN_MAP_TAKEN
Iviere_FilaosHouse_EndTransition:
	end

Iviere_FilaosHouse_EventScript_Wife::
	msgbox Iviere_FilaosHouse_Text_Wife, MSGBOX_NPC
	end

Iviere_FilaosHouse_EventScript_TownMap::
	lockall
	msgbox Iviere_FilaosHouse_Text_InspectMap, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_IVIERE_FILAO_WIFE, Common_Movement_FaceRight
	waitmovement LOCALID_IVIERE_FILAO_WIFE
	msgbox Iviere_FilaosHouse_Text_OfferMap, MSGBOX_DEFAULT
	giveitem ITEM_TOWN_MAP
	checkitem ITEM_TOWN_MAP, 1
	goto_if_eq VAR_RESULT, FALSE, Iviere_FilaosHouse_FinishMap
	removeobject LOCALID_IVIERE_TOWN_MAP
	setflag FLAG_IVIERE_TOWN_MAP_TAKEN
Iviere_FilaosHouse_FinishMap:
	releaseall
	end

Iviere_FilaosHouse_Text_Wife:
	.string "My husband, PROF. FILAO,\n"
	.string "spends his days in his\l"
	.string "laboratory.\l"
	.string "So he no longer needs his\l"
	.string "MAP.$"

Iviere_FilaosHouse_Text_InspectMap:
	.string "It's a MAP of the region.$"

Iviere_FilaosHouse_Text_OfferMap:
	.string "If you are going on an adventure,\n"
	.string "take this MAP. It'll be very\l"
	.string "useful. It's not a lot\l"
	.string "but I hope it helps you on\l"
	.string "your journey through\l"
	.string "the region.$"


Iviere_FilaosHouse_EventScript_Books::
	msgbox Iviere_FilaosHouse_Text_Books, MSGBOX_SIGN
	end

Iviere_FilaosHouse_EventScript_ClefairyBook::
	msgbox Iviere_FilaosHouse_Text_ClefairyBook, MSGBOX_SIGN
	end

Iviere_FilaosHouse_Text_Books:
	.string "Books about POKéMON!$"

Iviere_FilaosHouse_Text_ClefairyBook:
	.string "The gentle and adorable CLEFAIRY.$"
