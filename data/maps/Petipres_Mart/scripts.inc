Petipres_Mart_MapScripts::
	.byte 0

Petipres_Mart_Clerk::
	lock
	faceplayer
	message gText_HowMayIServeYou
	waitmessage
	pokemart Petipres_Mart_TMs
	msgbox gText_PleaseComeAgain, MSGBOX_DEFAULT
	release
	end

	.align 2
Petipres_Mart_TMs:
	.2byte ITEM_TM_HIDDEN_POWER
	.2byte ITEM_TM_PROTECT
	pokemartlistend

Petipres_Mart_SecondClerk::
	lock
	faceplayer
	message gText_HowMayIServeYou
	waitmessage
	pokemart Petipres_Mart_Stock
	msgbox gText_PleaseComeAgain, MSGBOX_DEFAULT
	release
	end

	.align 2
Petipres_Mart_Stock:
	.2byte ITEM_POKE_BALL
	.2byte ITEM_POTION
	.2byte ITEM_ANTIDOTE
	.2byte ITEM_PARALYZE_HEAL
	.2byte ITEM_AWAKENING
	pokemartlistend

Petipres_Mart_PotionCustomer::
	msgbox Petipres_Mart_PotionCustomer_Text, MSGBOX_NPC
	end

Petipres_Mart_PotionCustomer_Text:
	.string "I have to buy POTIONS.\pYou never know when a\nPOKéMON may need it!$"

Petipres_Mart_Antidotes::
	msgbox Petipres_Mart_Antidotes_Text, MSGBOX_NPC
	end

Petipres_Mart_Antidotes_Text:
	.string "We sell ANTIDOTES in this\nshop.$"
