EclatRockyPath_MapScripts::
	map_script MAP_SCRIPT_ON_RESUME, EclatRockyPath_OnResume
	.byte 0

EclatRockyPath_OnResume:
	special StartEclatRockyFlybys
	end

EclatRockyPath_Npc1::
	lock
	faceplayer
	playmoncry SPECIES_GEODUDE, CRY_MODE_NORMAL
	msgbox EclatRockyPath_Npc1_Text, MSGBOX_DEFAULT
	waitmoncry
	release
	end
EclatRockyPath_Npc1_Text:
	.string "GEODUDE: Raaaaaaaa !$"

EclatRockyPath_Npc5::
	lock
	faceplayer
	playmoncry SPECIES_GEODUDE, CRY_MODE_NORMAL
	msgbox EclatRockyPath_Npc5_Text, MSGBOX_DEFAULT
	waitmoncry
	release
	end
EclatRockyPath_Npc5_Text:
	.string "GEODUDE: Raaaaaaaa !$"

EclatRockyPath_Trainer6::
	trainerbattle_single TRAINER_ECLAT_ROCKY_PATH_CHRIS, EclatRockyPath_Trainer6_Intro, EclatRockyPath_Trainer6_Defeat
	msgbox EclatRockyPath_Trainer6_After, MSGBOX_AUTOCLOSE
	end

EclatRockyPath_Trainer6_Intro:
	.string "I trained to be able\nbear defeat…\pIn fact, I mostly hardened myself\nphysically. To attack!$"
EclatRockyPath_Trainer6_Defeat:
	.string "I must have done too much…$"
EclatRockyPath_Trainer6_After:
	.string "We may train to\nendure, but defeat,\lit always hurts.$"

EclatRockyPath_Npc10::
	lock
	faceplayer
	playmoncry SPECIES_GEODUDE, CRY_MODE_NORMAL
	msgbox EclatRockyPath_Npc10_Text, MSGBOX_DEFAULT
	waitmoncry
	release
	end
EclatRockyPath_Npc10_Text:
	.string "GEODUDE: Raaaaaaaa !$"

EclatRockyPath_Trainer11::
	trainerbattle_single TRAINER_ECLAT_ROCKY_PATH_BILL, EclatRockyPath_Trainer11_Intro, EclatRockyPath_Trainer11_Defeat
	msgbox EclatRockyPath_Trainer11_After, MSGBOX_AUTOCLOSE
	end

EclatRockyPath_Trainer11_Intro:
	.string "I know a lot of things\nAbout this cave, if you\lfight me, I'll give you a tip.$"
EclatRockyPath_Trainer11_Defeat:
	.string "You really want to know\nmy tips?$"
EclatRockyPath_Trainer11_After:
	.string "Ok listen, in this cave there\nsome PARAS who carry\lmushrooms.\lA guy on ROUTE 4\lwill teach attacks to your\lPOKéMON if you give them to him.$"

EclatRockyPath_Trainer12::
	trainerbattle_single TRAINER_ECLAT_ROCKY_PATH_BETTY, EclatRockyPath_Trainer12_Intro, EclatRockyPath_Trainer12_Defeat
	msgbox EclatRockyPath_Trainer12_After, MSGBOX_AUTOCLOSE
	end

EclatRockyPath_Trainer12_Intro:
	.string "I came to get some shrooms\nin this cave, what about you?$"
EclatRockyPath_Trainer12_Defeat:
	.string "Aaaah!\nIt's not normal to be so strong!$"
EclatRockyPath_Trainer12_After:
	.string "Mushrooms allow you to\nappeal to the master of abilities,\lwhich considerably strengthens your\lPOKéMON.$"

EclatRockyPath_Npc17::
	lock
	faceplayer
	playmoncry SPECIES_GEODUDE, CRY_MODE_NORMAL
	msgbox EclatRockyPath_Npc17_Text, MSGBOX_DEFAULT
	waitmoncry
	release
	end
EclatRockyPath_Npc17_Text:
	.string "GEODUDE: Raaaaaaaa !$"

EclatRockyPath_Npc20::
	lock
	faceplayer
	playmoncry SPECIES_GEODUDE, CRY_MODE_NORMAL
	msgbox EclatRockyPath_Npc20_Text, MSGBOX_DEFAULT
	waitmoncry
	release
	end
EclatRockyPath_Npc20_Text:
	.string "GEODUDE: Raaaaaaaa !$"

EclatRockyPath_Trainer22::
	trainerbattle_single TRAINER_ECLAT_ROCKY_PATH_ELIOT, EclatRockyPath_Trainer22_Intro, EclatRockyPath_Trainer22_Defeat
	msgbox EclatRockyPath_Trainer22_After, MSGBOX_AUTOCLOSE
	end

EclatRockyPath_Trainer22_Intro:
	.string "Hey! I got lost, where's the\nexit?$"
EclatRockyPath_Trainer22_Defeat:
	.string "Okay okay! I will find the\nexit alone.$"
EclatRockyPath_Trainer22_After:
	.string "I think I'll stay here\nto practice a little actually.$"

EclatRockyPath_Npc23::
	lock
	faceplayer
	playmoncry SPECIES_GEODUDE, CRY_MODE_NORMAL
	msgbox EclatRockyPath_Npc23_Text, MSGBOX_DEFAULT
	waitmoncry
	release
	end
EclatRockyPath_Npc23_Text:
	.string "GEODUDE: Raaaaaaaa !$"

EclatRockyPath_Npc29::
	finditem ITEM_EVERSTONE
	end

EclatRockyPath_Npc30::
	lock
	faceplayer
	playmoncry SPECIES_GEODUDE, CRY_MODE_NORMAL
	msgbox EclatRockyPath_Npc30_Text, MSGBOX_DEFAULT
	waitmoncry
	release
	end
EclatRockyPath_Npc30_Text:
	.string "GEODUDE: Raaaaaaaa !$"

EclatRockyPath_Sign0::
	msgbox EclatRockyPath_Sign0_Text, MSGBOX_NPC
	end

EclatRockyPath_Sign0_Text:
	.string "ROCKY PATH\nROUTE 2 - PEACEFUL WOODS entrance$"
