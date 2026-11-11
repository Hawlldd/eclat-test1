Route3_MapScripts::
	.byte 0

Route3_Woman::
EclatRoute3_Woman::
	lock
	faceplayer
	msgbox Route3_Woman_Text, MSGBOX_DEFAULT
	release
	end

Route3_Woman_Text:
	.string "You know, CLEFAIRY can sometimes\n"
	.string "hold MOON STONEs.$"

Route3_ItemSuperPotion::
EclatRoute3_ItemSuperPotion::
	finditem ITEM_SUPER_POTION
	end

Route3_ItemAwakening::
EclatRoute3_ItemAwakening::
	finditem ITEM_AWAKENING
	end

Route3_ItemRareCandy::
EclatRoute3_ItemRareCandy::
	finditem ITEM_RARE_CANDY
	end

Route3_TrainerCalvin::
EclatRoute3_TrainerCalvin::
	trainerbattle_single TRAINER_ROUTE3_CALVIN, Route3_TrainerCalvin_Intro, Route3_TrainerCalvin_Defeat
	msgbox Route3_TrainerCalvin_After, MSGBOX_AUTOCLOSE
	end

Route3_TrainerCalvin_Intro:
	.string "Watch out for my new\n"
	.string "POKéMON!$"

Route3_TrainerCalvin_Defeat:
	.string "Well, I need new ones!$"

Route3_TrainerCalvin_After:
	.string "Trained POKéMON are no longer\n"
	.string "stronger than wild ones!$"

Route3_TrainerMeg::
EclatRoute3_TrainerMeg::
	trainerbattle_single TRAINER_ROUTE3_MEG, Route3_TrainerMeg_Intro, Route3_TrainerMeg_Defeat
	msgbox Route3_TrainerMeg_After, MSGBOX_AUTOCLOSE
	end

Route3_TrainerMeg_Intro:
	.string "Hey! Are you going to PETIPRES?\n"
	.string "Not without having a battle!$"

Route3_TrainerMeg_Defeat:
	.string "You're not nice!$"

Route3_TrainerMeg_After:
	.string "This is the only road that\n"
	.string "connects to the next town.$"

Route3_TrainerElijah::
EclatRoute3_TrainerElijah::
	trainerbattle_single TRAINER_ROUTE3_ELIJAH, Route3_TrainerElijah_Intro, Route3_TrainerElijah_Defeat
	msgbox Route3_TrainerElijah_After, MSGBOX_AUTOCLOSE
	end

Route3_TrainerElijah_Intro:
	.string "I captured my POKéMON in the\n"
	.string "PEACEFUL WOODS.$"

Route3_TrainerElijah_Defeat:
	.string "Maybe I should have captured\n"
	.string "more POKéMON.$"

Route3_TrainerElijah_After:
	.string "Sometimes it's worth it to\n"
	.string "spend hours searching for rare\n"
	.string "POKéMON.$"

Route3_TrainerColton::
EclatRoute3_TrainerColton::
	trainerbattle_single TRAINER_ROUTE3_COLTON, Route3_TrainerColton_Intro, Route3_TrainerColton_Defeat
	msgbox Route3_TrainerColton_After, MSGBOX_AUTOCLOSE
	end

Route3_TrainerColton_Intro:
	.string "Hey! You don't have any shorts!\n"
	.string "You suck!$"

Route3_TrainerColton_Defeat:
	.string "Lost! Lost, I lost!$"

Route3_TrainerColton_After:
	.string "I always wear shorts, even\n"
	.string "in the winter!$"

Route3_TrainerLana::
EclatRoute3_TrainerLana::
	trainerbattle_single TRAINER_ROUTE3_LANA, Route3_TrainerLana_Intro, Route3_TrainerLana_Defeat
	msgbox Route3_TrainerLana_After, MSGBOX_AUTOCLOSE
	end

Route3_TrainerLana_Intro:
	.string "Do you want my picture?$"

Route3_TrainerLana_Defeat:
	.string "Hey, that's my picture!$"

Route3_TrainerLana_After:
	.string "Don't make eye contact if you\n"
	.string "don't wanna battle!$"

Route3_TrainerCedric::
EclatRoute3_TrainerCedric::
	trainerbattle_single TRAINER_ROUTE3_CEDRIC, Route3_TrainerCedric_Intro, Route3_TrainerCedric_Defeat
	msgbox Route3_TrainerCedric_After, MSGBOX_AUTOCLOSE
	end

Route3_TrainerCedric_Intro:
	.string "I'm sick of these shorts, the\n"
	.string "tall grass is always scratching!$"

Route3_TrainerCedric_Defeat:
	.string "Anyway, I still prefer shorts.$"

Route3_TrainerCedric_After:
	.string "It's so dumb that I have to\n"
	.string "wear shorts even in winter!$"

Route3_SecretNpc::
EclatRoute3_SecretNpc::
	lock
	faceplayer
	goto_if_set FLAG_ROUTE_3_LOTTERY_TICKET, Route3_SecretNpc_After
	msgbox Route3_SecretNpc_Found, MSGBOX_DEFAULT
	checkitemspace ITEM_EON_TICKET, 1
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	giveitem ITEM_EON_TICKET
	setflag FLAG_ROUTE_3_LOTTERY_TICKET
	msgbox Route3_SecretNpc_TicketDesc, MSGBOX_DEFAULT
	release
	end

Route3_SecretNpc_After:
	msgbox Route3_SecretNpc_TicketDesc, MSGBOX_DEFAULT
	release
	end

Route3_SecretNpc_Found:
	.string "Yo-oh!\n"
	.string "Well done, you found me!\p"
	.string "Take this!$"

Route3_SecretNpc_TicketDesc:
	.string "With this TICKET, you can try\n"
	.string "your luck in the lottery!$"

Route3_Rattata::
EclatRoute3_Rattata::
	lock
	faceplayer
	playmoncry SPECIES_RATTATA, CRY_MODE_NORMAL
	msgbox Route3_Rattata_Text, MSGBOX_DEFAULT
	waitmoncry
	release
	end

Route3_Rattata_Text:
	.string "RATTATA : Crr crr crr.$"

Route3_Goldeen::
EclatRoute3_Goldeen::
	lock
	faceplayer
	playmoncry SPECIES_GOLDEEN, CRY_MODE_NORMAL
	msgbox Route3_Goldeen_Text, MSGBOX_DEFAULT
	waitmoncry
	release
	end

Route3_Goldeen_Text:
	.string "GOLDEEN : Poissi poissirene!$"

Route3_Sign1::
EclatRoute3_Sign1::
	msgbox Route3_Sign_Text, MSGBOX_SIGN
	end

Route3_Sign2::
EclatRoute3_Sign2::
	msgbox Route3_Sign_Text, MSGBOX_SIGN
	end

Route3_Sign_Text:
	.string "ROUTE 3\n"
	.string "PEACEFUL WOODS - PERCHARBRE$"

Route3_HiddenItemOranBerry::
EclatRoute3_HiddenItemOranBerry::
	finditem ITEM_ORAN_BERRY
	end
