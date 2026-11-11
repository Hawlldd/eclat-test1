PetalburgCity_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, PetalburgCity_OnTransition
	.byte 0

PetalburgCity_OnTransition:
	setflag FLAG_VISITED_PETALBURG_CITY
	end

PetalburgCity_EventScript_VertigoGirl::
	msgbox PetalburgCity_Text_VertigoGirl, MSGBOX_NPC
	end

PetalburgCity_EventScript_BerryBoy::
	msgbox PetalburgCity_Text_BerryBoy, MSGBOX_NPC
	end

PetalburgCity_EventScript_HoennBoy::
	msgbox PetalburgCity_Text_HoennBoy, MSGBOX_NPC
	end

PetalburgCity_EventScript_PeacefulWoodsGirl::
	msgbox PetalburgCity_Text_PeacefulWoodsGirl, MSGBOX_NPC
	end

PetalburgCity_EventScript_GymSign::
	msgbox PetalburgCity_Text_GymSign, MSGBOX_SIGN
	end

PetalburgCity_EventScript_Bellossom::
	lock
	faceplayer
	playmoncry SPECIES_BELLOSSOM, CRY_MODE_NORMAL
	msgbox PetalburgCity_Text_Bellossom, MSGBOX_DEFAULT
	waitmoncry
	release
	end

PetalburgCity_EventScript_WildPokemonIgnoring::
	msgbox PetalburgCity_Text_WildPokemonIgnoring, MSGBOX_NPC
	end

PetalburgCity_EventScript_Vileplume::
	lock
	faceplayer
	playmoncry SPECIES_VILEPLUME, CRY_MODE_NORMAL
	msgbox PetalburgCity_Text_Vileplume, MSGBOX_DEFAULT
	waitmoncry
	release
	end

PetalburgCity_EventScript_Route4Guard::
	lock
	faceplayer
	goto_if_set FLAG_BADGE01_GET, PetalburgCity_EventScript_Route4Guard_HasBadge
	msgbox PetalburgCity_Text_Route4Guard_NoBadge, MSGBOX_NPC
	release
	end

PetalburgCity_EventScript_Route4Guard_HasBadge::
	msgbox PetalburgCity_Text_Route4Guard_HasBadge, MSGBOX_NPC
	release
	end

PetalburgCity_EventScript_LottoGuy::
	msgbox PetalburgCity_Text_LottoGuy, MSGBOX_NPC
	end

PetalburgCity_EventScript_EvolutionStoneGuy::
	msgbox PetalburgCity_Text_EvolutionStoneGuy, MSGBOX_NPC
	end

PetalburgCity_EventScript_SecretTicketGuy::
	lock
	faceplayer
	goto_if_set FLAG_PERCHABAE_TICKET, PetalburgCity_EventScript_SecretTicketGuy_AlreadyGot
	msgbox PetalburgCity_Text_SecretTicketGuy_Found, MSGBOX_DEFAULT
	checkitemspace ITEM_MYSTIC_TICKET, 1
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	giveitem ITEM_MYSTIC_TICKET
	setflag FLAG_PERCHABAE_TICKET
	msgbox PetalburgCity_Text_SecretTicketGuy_TicketExplain, MSGBOX_DEFAULT
	closemessage
	fadescreen FADE_TO_BLACK
	removeobject VAR_LAST_TALKED
	fadescreen FADE_FROM_BLACK
	release
	end

PetalburgCity_EventScript_SecretTicketGuy_AlreadyGot::
	msgbox PetalburgCity_Text_SecretTicketGuy_TicketExplain, MSGBOX_NPC
	release
	end

PetalburgCity_EventScript_Route3Sign::
	msgbox PetalburgCity_Text_Route3Sign, MSGBOX_SIGN
	end

PetalburgCity_EventScript_Route4Sign::
	msgbox PetalburgCity_Text_Route4Sign, MSGBOX_SIGN
	end

PetalburgCity_EventScript_CitySign::
	msgbox PetalburgCity_Text_CitySign, MSGBOX_SIGN
	end

PetalburgCity_Text_VertigoGirl:
	.string "I would love to visit\n"
	.string "PERCHABAE city, but I have\n"
	.string "vertigo.$"

PetalburgCity_Text_BerryBoy:
	.string "Hi!\n"
	.string "Did you know that you can buy\n"
	.string "Berries at the POKéMON center?\n"
	.string "You should think about buying some.\n"
	.string "It can be decisive in a match.$"

PetalburgCity_Text_HoennBoy:
	.string "How do you like this town?\n"
	.string "It's nestled in the trees, which\n"
	.string "reminds me of a town in HOENN, but\n"
	.string "I can't remember which.$"

PetalburgCity_Text_PeacefulWoodsGirl:
	.string "I chose to live in this city\n"
	.string "because the PEACEFUL WOODS are\n"
	.string "close by.$"

PetalburgCity_Text_GymSign:
	.string "PERCHABAE POKéMON GYM\n"
	.string "LEADER: ROSELINE, the flower of\n"
	.string "PERCHABAE.$"

PetalburgCity_Text_Bellossom:
	.string "Bellossom: Bellooosom!$"

PetalburgCity_Text_WildPokemonIgnoring:
	.string "This wild POKéMON seems to be\n"
	.string "ignoring you. It's best to leave\n"
	.string "it alone.$"

PetalburgCity_Text_Vileplume:
	.string "VILEPLUME: Rafleeeesia !$"

PetalburgCity_Text_Route4Guard_NoBadge:
	.string "Hi! I'm trying to become the best\n"
	.string "trainer I can be, which is why I'm\n"
	.string "challenging all those who have the\n"
	.string "PERCHABAE BADGE.\p"
	.string "Weak trainers don't interest me.$"

PetalburgCity_Text_Route4Guard_HasBadge:
	.string "{PLAYER} flashes the LYS BADGE.\p"
	.string "You didn't get your BADGE on a\n"
	.string "stroke of luck it seems.$"

PetalburgCity_Text_LottoGuy:
	.string "Hi there!\n"
	.string "You're probably thinking that my\n"
	.string "outfit is strange.\p"
	.string "And it's true. I'm organizing a\n"
	.string "big game for POKéMON TRAINERS.\p"
	.string "All you have to do is find some\n"
	.string "TICKETS to play my great lotto!\p"
	.string "My friends will give you some if you\n"
	.string "find them.\p"
	.string "You can try to win a POKéMON egg\n"
	.string "from over 300 different species.$"

PetalburgCity_Text_EvolutionStoneGuy:
	.string "Evolution stones are quite rare.\p"
	.string "However, you can find MOON STONE\n"
	.string "on Clefairy and SUNSTONES on\n"
	.string "Sunkern, even if it takes luck.$"

PetalburgCity_Text_SecretTicketGuy_Found:
	.string "Yo-oh!\n"
	.string "Well done, you found me!\n"
	.string "Take this!$"

PetalburgCity_Text_SecretTicketGuy_TicketExplain:
	.string "With this TICKET, you can try\n"
	.string "your luck in the lottery in\n"
	.string "PERCHABAE.\p"
	.string "Alright, with that, I'm going to\n"
	.string "hide somewhere else!$"

PetalburgCity_Text_Route3Sign:
	.string "Head west on ROUTE 3.$"

PetalburgCity_Text_Route4Sign:
	.string "Head east on ROUTE 4.$"

PetalburgCity_Text_CitySign:
	.string "PERCHABAE\n"
	.string "Elevated above the lush greenery.$"
