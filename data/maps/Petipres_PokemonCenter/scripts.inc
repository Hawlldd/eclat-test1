Petipres_PokemonCenter_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Petipres_PokemonCenter_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, CableClub_OnResume
	.byte 0

Petipres_PokemonCenter_OnTransition:
	setrespawn HEAL_LOCATION_PETIPRES
	end

Petipres_PokemonCenter_Nurse::
	setvar VAR_0x800B, LOCALID_PETIPRES_POKEMON_CENTER_1
	call Common_EventScript_PkmnCenterNurse
	waitmessage
	waitbuttonpress
	release
	end

Petipres_PokemonCenter_Chansey::
	lock
	faceplayer
	playmoncry SPECIES_CHANSEY, CRY_MODE_NORMAL
	msgbox Petipres_PokemonCenter_Chansey_Text, MSGBOX_DEFAULT
	waitmoncry
	release
	end

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

Petipres_PokemonCenter_MilkTip_Text:
	.string "You should consider buying some\nMOO MOO MILK, it's extremely\lprofitable.$"

Petipres_PokemonCenter_HealingTip::
	msgbox Petipres_PokemonCenter_HealingTip_Text, MSGBOX_NPC
	end

Petipres_PokemonCenter_HealingTip_Text:
	.string "POKéMON CENTERS can heal\ninjured and fainted POKéMON!$"

Petipres_PokemonCenter_CenterTip::
	msgbox Petipres_PokemonCenter_CenterTip_Text, MSGBOX_NPC
	end

Petipres_PokemonCenter_CenterTip_Text:
	.string "There's a POKéMON CENTER in\neach city.\lThey'll take care of your\lPOKEMON for free!$"

Petipres_PokemonCenter_Chansey_Text:
	.string "Chansey: Leveinard !$"

Petipres_PokemonCenter_BerrySeller_Text:
	.string "Hello, I grow and sell\nberries. Do you want to buy some?$"

Petipres_PokemonCenter_BerryThanks_Text:
	.string "Thank you. It was my pleasure!$"
