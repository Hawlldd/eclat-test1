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
	lock
	faceplayer
	goto_if_set FLAG_IVIERE_TOWN_MAP_TAKEN, Iviere_FilaosHouse_EventScript_WifeAfter
	setspeaker SP_NAME_FILAO_WIFE
	msgbox Iviere_FilaosHouse_Text_WifeOfferMap, MSGBOX_DEFAULT
	giveitem ITEM_TOWN_MAP
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	removeobject LOCALID_IVIERE_TOWN_MAP
	setflag FLAG_IVIERE_TOWN_MAP_TAKEN
	setspeaker SP_NAME_FILAO_WIFE
	msgbox Iviere_FilaosHouse_Text_WifeExplainMap, MSGBOX_DEFAULT
	release
	end

Iviere_FilaosHouse_EventScript_WifeAfter::
	setspeaker SP_NAME_FILAO_WIFE
	msgbox Iviere_FilaosHouse_Text_WifeAfter, MSGBOX_NPC
	end

Iviere_FilaosHouse_EventScript_TownMap::
	lockall
	playse SE_PIN
	applymovement LOCALID_IVIERE_FILAO_WIFE, Common_Movement_ExclamationMark
	waitmovement LOCALID_IVIERE_FILAO_WIFE
	applymovement LOCALID_IVIERE_FILAO_WIFE, Common_Movement_FaceRight
	waitmovement LOCALID_IVIERE_FILAO_WIFE
	setspeaker SP_NAME_FILAO_WIFE
	msgbox Iviere_FilaosHouse_Text_DontTouchMap, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_IVIERE_FILAO_WIFE, Common_Movement_FaceDown
	waitmovement LOCALID_IVIERE_FILAO_WIFE
	releaseall
	end

Iviere_FilaosHouse_Text_DontTouchMap:
	.string "Hey! Hands off that, kid!\p"
	.string "I know very well your mother\n"
	.string "taught you better manners than\l"
	.string "taking things without asking!\p"
	.string "If you want something, come\n"
	.string "over here and speak to me\l"
	.string "like a proper guest!$"

Iviere_FilaosHouse_Text_WifeOfferMap:
	.string "Hello, {PLAYER}…\p"
	.string "I heard what happened at your\n"
	.string "house, and no, I will not ask\l"
	.string "you to give up or stay behind.\p"
	.string "Those thugs had no right to do\n"
	.string "what they did in our town!\p"
	.string "My husband is always hoarding\n"
	.string "clutter, but at least this item\l"
	.string "will be truly useful to you.\p"
	.string "Here, take this MAP from the\n"
	.string "table to aid your journey.$"

Iviere_FilaosHouse_Text_WifeExplainMap:
	.string "That MAP will help guide your\n"
	.string "steps across the whole region.\p"
	.string "Go show those scumbags they\n"
	.string "messed with the wrong family!$"

Iviere_FilaosHouse_Text_WifeAfter:
	.string "My husband spends day and\n"
	.string "night locked in his LAB,\l"
	.string "so he won't miss that MAP.\p"
	.string "Stay safe out there, honey,\n"
	.string "and make those criminals pay!$"


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
