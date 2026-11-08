Petipres_Houses_MapScripts::
	.byte 0

Petipres_Houses_Eugene::
	lock
	faceplayer
	goto_if_set FLAG_GOT_PETIPRES_MILK, Petipres_Houses_EugeneAfter
	msgbox Petipres_Houses_Eugene_Text, MSGBOX_DEFAULT
	giveitem ITEM_MOOMOO_MILK
	goto_if_eq VAR_RESULT, FALSE, Petipres_Houses_Release
	setflag FLAG_GOT_PETIPRES_MILK
	goto Petipres_Houses_Release

Petipres_Houses_EugeneAfter:
	msgbox Petipres_Houses_EugeneAfter_Text, MSGBOX_DEFAULT
	goto Petipres_Houses_Release

Petipres_Houses_Marianne::
	lock
	faceplayer
	msgbox Petipres_Houses_Marianne_Text, MSGBOX_DEFAULT
	showmoneybox 0, 0
	msgbox Petipres_Houses_MilkOffer_Text, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, Petipres_Houses_LeaveShop
	checkmoney 500
	goto_if_eq VAR_RESULT, FALSE, Petipres_Houses_NoMoney
	checkitemspace ITEM_MOOMOO_MILK, 1
	goto_if_eq VAR_RESULT, FALSE, Petipres_Houses_BagFull
	additem ITEM_MOOMOO_MILK, 1
	removemoney 500
	updatemoneybox
	playse SE_SHOP
	msgbox Petipres_Houses_MilkBought_Text, MSGBOX_DEFAULT
	goto Petipres_Houses_LeaveShop

Petipres_Houses_NoMoney:
	msgbox Petipres_Houses_NoMoney_Text, MSGBOX_DEFAULT
	goto Petipres_Houses_LeaveShop

Petipres_Houses_BagFull:
	msgbox Petipres_Houses_BagFull_Text, MSGBOX_DEFAULT

Petipres_Houses_LeaveShop:
	hidemoneybox

Petipres_Houses_Release:
	release
	end

Petipres_Houses_EugeneNeighbor::
	msgbox Petipres_Houses_EugeneNeighbor_Text, MSGBOX_NPC
	end

Petipres_Houses_EugeneNeighbor_Text:
	.string "Hi! You should visit\nEUGENE he is very welcoming\ltowards new people.$"

Petipres_Houses_Farmer::
	msgbox Petipres_Houses_Farmer_Text, MSGBOX_NPC
	end

Petipres_Houses_Farmer_Text:
	.string "You know, there's not much\nto do here. It's really just a\lsmall farming town.$"

Petipres_Houses_Egipolis::
	msgbox Petipres_Houses_Egipolis_Text, MSGBOX_NPC
	end

Petipres_Houses_Egipolis_Text:
	.string "I moved here to get away from\neverything after leaving\lEGIPOLIS.$"

Petipres_Houses_NewFace::
	msgbox Petipres_Houses_NewFace_Text, MSGBOX_NPC
	end

Petipres_Houses_NewFace_Text:
	.string "Well, well. A new face.\nLately, a lot of people\lhave been stopping in PETIPRES.$"

Petipres_Houses_Journal::
	msgbox Petipres_Houses_Journal_Text, MSGBOX_SIGN
	end

Petipres_Houses_Journal_Text:
	.string "Vous examinez le journal!\pPremière page…\pLes POKé BALLS peuvent capturer\nles POKéMON.\pJusqu'à 6 POKéMON sont\ntransportables.\pLes personnes s'occupant des\nPOKéMON sont des DRESSEURS\lde POKéMON.$"

Petipres_Houses_StatusBoard::
	msgbox Petipres_Houses_StatusBoard_Text, MSGBOX_SIGN
	end

Petipres_Houses_StatusBoard_Text:
	.string "Le tableau décrit les changements\nde STATUT des POKéMON pendant\lun combat.$"

Petipres_Houses_TrainerTips::
	msgbox Petipres_Houses_TrainerTips_Text, MSGBOX_SIGN
	end

Petipres_Houses_TrainerTips_Text:
	.string "The board describes Pokémon\nstatus conditions in battle.$"

Petipres_Houses_Eugene_Text:
	.string "EUGENE: allow me to introduce myself,\nI'm EUGENE, the greatest owner\lof cattle in the region, kid.\lYou look like a good guy to me, take\lsome MOO MOO MILK!$"

Petipres_Houses_EugeneAfter_Text:
	.string "EUGENE : Well kid, are you doing\n"
	.string "ok? You can buy some\l"
	.string "MOO MOO MILK from my wife.$"

Petipres_Houses_Marianne_Text:
	.string "MARIANNE : Hello, I'm EUGENE's\nwife, MARIANNE.\lI sell the milk we produce\lit's only 500 Pokédollars each.$"

Petipres_Houses_MilkOffer_Text:
	.string "Do you want some?$"

Petipres_Houses_MilkBought_Text:
	.string "MARIANNE : Thank you, see you soon.$"

Petipres_Houses_NoMoney_Text:
	.string "You don't have enough money.$"
Petipres_Houses_BagFull_Text:
	.string "There isn't any room in your BAG.$"
