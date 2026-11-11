Petipres_PokemonCenter_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Petipres_PokemonCenter_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, CableClub_OnResume
	.byte 0

Petipres_PokemonCenter_OnTransition:
	setrespawn HEAL_LOCATION_PETIPRES
	end

Petipres_PokemonCenter_Nurse::
	setvar VAR_0x800B, LOCALID_PETIPRES_POKEMON_CENTER_1
	goto_if_set FLAG_PETIPRES_NURSE_INTRO, Petipres_PokemonCenter_Nurse_Regular
	lock
	faceplayer
	setflag FLAG_PETIPRES_NURSE_INTRO
	setspeaker SP_NAME_JOY
	msgbox Petipres_PokemonCenter_Nurse_Intro1, MSGBOX_DEFAULT
	setspeaker SP_NAME_NONE
	giveitem ITEM_POTION
	goto_if_eq VAR_RESULT, FALSE, Petipres_PokemonCenter_Nurse_NoRoom
	setspeaker SP_NAME_JOY
	msgbox Petipres_PokemonCenter_Nurse_Intro2, MSGBOX_DEFAULT
	goto Petipres_PokemonCenter_Nurse_DoFirstHeal

Petipres_PokemonCenter_Nurse_NoRoom:
	setspeaker SP_NAME_JOY
	msgbox Petipres_PokemonCenter_Nurse_IntroNoRoom, MSGBOX_DEFAULT
	goto Petipres_PokemonCenter_Nurse_DoFirstHeal

Petipres_PokemonCenter_Nurse_DoFirstHeal:
	setspeaker SP_NAME_NONE
	incrementgamestat GAME_STAT_USED_POKECENTER
	call EventScript_PkmnCenterNurse_TakeAndHealPkmn
	setspeaker SP_NAME_JOY
	msgbox Petipres_PokemonCenter_Nurse_HealOutro, MSGBOX_DEFAULT
	setspeaker SP_NAME_NONE
	release
	end

Petipres_PokemonCenter_Nurse_Regular:
	call Common_EventScript_PkmnCenterNurse
	waitmessage
	waitbuttonpress
	release
	end

Petipres_PokemonCenter_Chansey::
	lock
	faceplayer
	playmoncry SPECIES_CHANSEY, CRY_MODE_NORMAL
	applymovement LOCALID_PETIPRES_POKEMON_CENTER_5, Petipres_ChanseyHappy
	waitmovement 0
	msgbox Petipres_PokemonCenter_Chansey_Text, MSGBOX_DEFAULT
	waitmoncry
	release
	end

Petipres_ChanseyHappy:
	emote_heart
	jump_in_place_down
	step_end

Petipres_PokemonCenter_BerrySeller::
	lock
	faceplayer
	msgbox Petipres_PokemonCenter_BerrySeller_Text, MSGBOX_DEFAULT
	pokemart Petipres_PokemonCenter_Berries
	msgbox Petipres_PokemonCenter_BerryThanks_Text, MSGBOX_DEFAULT
	release
	end

	.align 2
Petipres_PokemonCenter_Berries:
	.2byte ITEM_ORAN_BERRY
	.2byte ITEM_PECHA_BERRY
	.2byte ITEM_CHERI_BERRY
	pokemartlistend

Petipres_PokemonCenter_MilkTip::
	msgbox Petipres_PokemonCenter_MilkTip_Text, MSGBOX_NPC
	end

Petipres_PokemonCenter_HealingTip::
	msgbox Petipres_PokemonCenter_HealingTip_Text, MSGBOX_NPC
	end

Petipres_PokemonCenter_CenterTip::
	msgbox Petipres_PokemonCenter_CenterTip_Text, MSGBOX_NPC
	end

Petipres_PokemonCenter_Nurse_Intro1:
	.string "Hello there! You're new around\n"
	.string "here, aren't you?\p"
	.string "Why such a tense look?\p"
	.string "I know things out there can feel\n"
	.string "overwhelming right now...\p"
	.string "...but in here, this POKéMON\n"
	.string "CENTER is a safe haven for you\l"
	.string "and your partners.\p"
	.string "Since this is your first visit,\n"
	.string "take this with you!$"

Petipres_PokemonCenter_Nurse_Intro2:
	.string "Not every Joy in my family\n"
	.string "will spoil you like this...\p"
	.string "...but I want you to feel right\n"
	.string "at home.\p"
	.string "Now, allow me to restore your\n"
	.string "team to full health!$"

Petipres_PokemonCenter_Nurse_IntroNoRoom:
	.string "Oh dear, your BAG is full!\p"
	.string "I can't give you this POTION,\n"
	.string "but allow me to at least heal\l"
	.string "your team to full health!$"

Petipres_PokemonCenter_Nurse_HealOutro:
	.string "Thank you for waiting!\n"
	.string "Your POKéMON are fully healed.\p"
	.string "Stay safe out there, and keep\n"
	.string "your chin up!$"

Petipres_PokemonCenter_MilkTip_Text:
	.string "You should consider buying some\n"
	.string "MOO MOO MILK, it's extremely\l"
	.string "profitable.$"

Petipres_PokemonCenter_HealingTip_Text:
	.string "POKéMON CENTERS can heal\n"
	.string "injured and fainted POKéMON!$"

Petipres_PokemonCenter_CenterTip_Text:
	.string "There's a POKéMON CENTER in\n"
	.string "each city.\l"
	.string "They'll take care of your\l"
	.string "POKéMON for free!$"

Petipres_PokemonCenter_Chansey_Text:
	.string "Chansey: Sey... Chaaan!$"

Petipres_PokemonCenter_BerrySeller_Text:
	.string "Hello, I grow and sell\n"
	.string "berries. Do you want to buy some?$"

Petipres_PokemonCenter_BerryThanks_Text:
	.string "Thank you. It was my pleasure!$"
