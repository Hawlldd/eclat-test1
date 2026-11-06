Iviere_FilaosHouse_MapScripts::
	.byte 0

Iviere_FilaosHouse_EventScript_Wife::
	lock
	faceplayer
	checkitem ITEM_TOWN_MAP, 1
	goto_if_eq VAR_RESULT, TRUE, Iviere_FilaosHouse_EventScript_AlreadyHasMap
	msgbox Iviere_FilaosHouse_Text_OfferMap, MSGBOX_DEFAULT
	giveitem ITEM_TOWN_MAP
	release
	end

Iviere_FilaosHouse_EventScript_AlreadyHasMap:
	msgbox Iviere_FilaosHouse_Text_AlreadyHasMap, MSGBOX_DEFAULT
	release
	end

Iviere_FilaosHouse_EventScript_Books::
	msgbox Iviere_FilaosHouse_Text_Books, MSGBOX_SIGN
	end

Iviere_FilaosHouse_EventScript_ClefairyBook::
	msgbox Iviere_FilaosHouse_Text_ClefairyBook, MSGBOX_SIGN
	end

Iviere_FilaosHouse_Text_OfferMap:
	.string "My husband, PROF. FILAO,\n"
	.string "spends his days in his laboratory.\p"
	.string "If you are going on an adventure,\n"
	.string "take this MAP. It'll be very useful.$"

Iviere_FilaosHouse_Text_AlreadyHasMap:
	.string "I hope the MAP helps you on\n"
	.string "your journey through the region.$"

Iviere_FilaosHouse_Text_Books:
	.string "Books about POKéMON!$"

Iviere_FilaosHouse_Text_ClefairyBook:
	.string "The gentle and adorable CLEFAIRY.$"
