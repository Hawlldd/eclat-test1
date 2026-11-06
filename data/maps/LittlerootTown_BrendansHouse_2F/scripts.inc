LittlerootTown_BrendansHouse_2F_MapScripts::
	map_script MAP_SCRIPT_ON_FRAME_TABLE, LittlerootTown_BrendansHouse_2F_EclatIntro
	.byte 0

LittlerootTown_BrendansHouse_2F_EclatIntro:
	map_script_2 VAR_LITTLEROOT_INTRO_STATE, 0, LittlerootTown_BrendansHouse_2F_EventScript_EclatStarter
	.2byte 0

LittlerootTown_BrendansHouse_2F_EventScript_EclatStarter:
	lockall
	setrespawn HEAL_LOCATION_LITTLEROOT_TOWN_BRENDANS_HOUSE_2F
	setflag FLAG_HIDE_LITTLEROOT_TOWN_BRENDANS_HOUSE_TRUCK
	setflag FLAG_HIDE_LITTLEROOT_TOWN_MAYS_HOUSE_TRUCK
	msgbox EclatIntro_Text_ShinyRates, MSGBOX_DEFAULT
	msgbox EclatIntro_Text_ChooseShinyRate, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, EclatIntro_SetBoostedShinyRate
	clearflag FLAG_ECLAT_BOOSTED_SHINY_RATE
	goto EclatIntro_DescribeBedroom

EclatIntro_SetBoostedShinyRate:
	setflag FLAG_ECLAT_BOOSTED_SHINY_RATE

EclatIntro_DescribeBedroom:
	msgbox EclatIntro_Text_Bedroom, MSGBOX_DEFAULT
EclatIntro_ChooseType:
	message EclatIntro_Text_ChooseType
	waitmessage
	dynmultichoice 20, 1, TRUE, 3, 0, DYN_MULTICHOICE_CB_NONE, EclatIntro_Text_Fire, EclatIntro_Text_Water, EclatIntro_Text_Grass
	closemessage
	goto_if_eq VAR_RESULT, 1, EclatIntro_WaterStarters
	goto_if_eq VAR_RESULT, 2, EclatIntro_GrassStarters

	dynmultipush EclatIntro_Text_Charmander, SPECIES_CHARMANDER
	dynmultipush EclatIntro_Text_Cyndaquil, SPECIES_CYNDAQUIL
	dynmultipush EclatIntro_Text_Torchic, SPECIES_TORCHIC
	dynmultipush EclatIntro_Text_Chimchar, SPECIES_CHIMCHAR
	dynmultipush EclatIntro_Text_Tepig, SPECIES_TEPIG
	dynmultipush EclatIntro_Text_Fennekin, SPECIES_FENNEKIN
	dynmultipush EclatIntro_Text_Litten, SPECIES_LITTEN
	dynmultipush EclatIntro_Text_Scorbunny, SPECIES_SCORBUNNY
	dynmultipush EclatIntro_Text_Fuecoco, SPECIES_FUECOCO
	goto EclatIntro_ChooseStarter

EclatIntro_WaterStarters:
	dynmultipush EclatIntro_Text_Squirtle, SPECIES_SQUIRTLE
	dynmultipush EclatIntro_Text_Totodile, SPECIES_TOTODILE
	dynmultipush EclatIntro_Text_Mudkip, SPECIES_MUDKIP
	dynmultipush EclatIntro_Text_Piplup, SPECIES_PIPLUP
	dynmultipush EclatIntro_Text_Oshawott, SPECIES_OSHAWOTT
	dynmultipush EclatIntro_Text_Froakie, SPECIES_FROAKIE
	dynmultipush EclatIntro_Text_Popplio, SPECIES_POPPLIO
	dynmultipush EclatIntro_Text_Sobble, SPECIES_SOBBLE
	dynmultipush EclatIntro_Text_Quaxly, SPECIES_QUAXLY
	goto EclatIntro_ChooseStarter

EclatIntro_GrassStarters:
	dynmultipush EclatIntro_Text_Bulbasaur, SPECIES_BULBASAUR
	dynmultipush EclatIntro_Text_Chikorita, SPECIES_CHIKORITA
	dynmultipush EclatIntro_Text_Treecko, SPECIES_TREECKO
	dynmultipush EclatIntro_Text_Turtwig, SPECIES_TURTWIG
	dynmultipush EclatIntro_Text_Snivy, SPECIES_SNIVY
	dynmultipush EclatIntro_Text_Chespin, SPECIES_CHESPIN
	dynmultipush EclatIntro_Text_Rowlet, SPECIES_ROWLET
	dynmultipush EclatIntro_Text_Grookey, SPECIES_GROOKEY
	dynmultipush EclatIntro_Text_Sprigatito, SPECIES_SPRIGATITO

EclatIntro_ChooseStarter:
	dynmultistack 18, 0, FALSE, 6, FALSE, 0, DYN_MULTICHOICE_CB_NONE
	goto_if_eq VAR_RESULT, MULTI_B_PRESSED, EclatIntro_ChooseType
	copyvar VAR_TEMP_1, VAR_RESULT
	bufferspeciesname STR_VAR_1, VAR_TEMP_1
	showmonpic VAR_TEMP_1, 10, 3
	msgbox EclatIntro_Text_ConfirmStarter, MSGBOX_YESNO
	hidemonpic
	goto_if_eq VAR_RESULT, NO, EclatIntro_ChooseType
	givemon VAR_TEMP_1, 5, ITEM_NONE
	setseenmon VAR_TEMP_1
	setcaughtmon VAR_TEMP_1
	setflag FLAG_SYS_POKEMON_GET
	setflag FLAG_SYS_POKEDEX_GET
	special EnableNationalPokedex
	setvar VAR_LITTLEROOT_INTRO_STATE, 8
	playse SE_PIN
	applymovement LOCALID_PLAYER, Common_Movement_ExclamationMark
	waitmovement 0
	msgbox EclatIntro_Text_Mom, MSGBOX_DEFAULT
	releaseall
	end

EclatIntro_Text_ShinyRates:
	.string "This game features two rates\n"
	.string "of shiny POKéMON.\p"
	.string "You must choose between a rate\n"
	.string "of 1/400 or 1/8192.\p"
	.string "This choice cannot be\n"
	.string "changed later.$"

EclatIntro_Text_ChooseShinyRate:
	.string "Do you want to use the\n"
	.string "1/400 shiny rate?$"

EclatIntro_Text_Scorbunny:
	.string "Scorbunny$"

EclatIntro_Text_Fuecoco:
	.string "Fuecoco$"

EclatIntro_Text_Sobble:
	.string "Sobble$"

EclatIntro_Text_Quaxly:
	.string "Quaxly$"

EclatIntro_Text_Grookey:
	.string "Grookey$"

EclatIntro_Text_Sprigatito:
	.string "Sprigatito$"

EclatIntro_Text_Bedroom:
	.string "You're in your room,\n"
	.string "contemplating the POKéBALL\l"
	.string "and the new POKéMON,\l"
	.string "that the professor of IVIERE TOWN\l"
	.string "gave you.$"

EclatIntro_Text_ChooseType:
	.string "Which type of POKéMON did you choose?$"

EclatIntro_Text_ConfirmStarter:
	.string "Did you choose {STR_VAR_1}?$"

EclatIntro_Text_Mom:
	.string "Mom: Help! Get out you\n"
	.string "filthy vultures!$"

EclatIntro_Text_Fire:
	.string "Fire$"

EclatIntro_Text_Water:
	.string "Water$"

EclatIntro_Text_Grass:
	.string "Grass$"

EclatIntro_Text_Charmander:
	.string "Charmander$"

EclatIntro_Text_Cyndaquil:
	.string "Cyndaquil$"

EclatIntro_Text_Torchic:
	.string "Torchic$"

EclatIntro_Text_Chimchar:
	.string "Chimchar$"

EclatIntro_Text_Tepig:
	.string "Tepig$"

EclatIntro_Text_Fennekin:
	.string "Fennekin$"

EclatIntro_Text_Litten:
	.string "Litten$"

EclatIntro_Text_Squirtle:
	.string "Squirtle$"

EclatIntro_Text_Totodile:
	.string "Totodile$"

EclatIntro_Text_Mudkip:
	.string "Mudkip$"

EclatIntro_Text_Piplup:
	.string "Piplup$"

EclatIntro_Text_Oshawott:
	.string "Oshawott$"

EclatIntro_Text_Froakie:
	.string "Froakie$"

EclatIntro_Text_Popplio:
	.string "Popplio$"

EclatIntro_Text_Bulbasaur:
	.string "Bulbasaur$"

EclatIntro_Text_Chikorita:
	.string "Chikorita$"

EclatIntro_Text_Treecko:
	.string "Treecko$"

EclatIntro_Text_Turtwig:
	.string "Turtwig$"

EclatIntro_Text_Snivy:
	.string "Snivy$"

EclatIntro_Text_Chespin:
	.string "Chespin$"

EclatIntro_Text_Rowlet:
	.string "Rowlet$"

LittlerootTown_BrendansHouse_2F_OnTransition:
	call_if_lt VAR_LITTLEROOT_RIVAL_STATE, 2, LittlerootTown_BrendansHouse_2F_EventScript_CheckSetReadyToMeetBrendan
	call_if_ge VAR_LITTLEROOT_RIVAL_STATE, 3, LittlerootTown_BrendansHouse_2F_EventScript_CheckShouldUpdateBrendanPos
	call_if_eq VAR_LITTLEROOT_INTRO_STATE, 4, PlayersHouse_2F_EventScript_BlockStairsUntilClockIsSet
	call SecretBase_EventScript_SetDecorationFlags
	setvar VAR_SECRET_BASE_INITIALIZED, 0
	end

LittlerootTown_BrendansHouse_2F_EventScript_CheckShouldUpdateBrendanPos::
	goto_if_set FLAG_MET_RIVAL_LILYCOVE, LittlerootTown_BrendansHouse_2F_EventScript_TryUpdateBrendanPos
	goto_if_ge VAR_BIRCH_LAB_STATE, 2, LittlerootTown_BrendansHouse_2F_EventScript_Ret
	goto LittlerootTown_BrendansHouse_2F_EventScript_TryUpdateBrendanPos

LittlerootTown_BrendansHouse_2F_EventScript_TryUpdateBrendanPos::
	checkplayergender
	goto_if_eq VAR_RESULT, MALE, LittlerootTown_BrendansHouse_2F_EventScript_Ret
	@ Odd that the MaysHouse equivalent was used below instead
	goto_if_ge VAR_DEX_UPGRADE_JOHTO_STARTER_STATE, 2, LittlerootTown_MaysHouse_2F_EventScript_Ret
	setobjectxyperm LOCALID_RIVALS_HOUSE_2F_RIVAL, 0, 2
	setobjectmovementtype LOCALID_RIVALS_HOUSE_2F_RIVAL, MOVEMENT_TYPE_FACE_UP
	return

LittlerootTown_BrendansHouse_2F_EventScript_Ret::
	return

LittlerootTown_BrendansHouse_2F_EventScript_CheckSetReadyToMeetBrendan::
	checkplayergender
	goto_if_eq VAR_RESULT, FEMALE, LittlerootTown_BrendansHouse_2F_EventScript_SetReadyToMeetBrendan
	return

LittlerootTown_BrendansHouse_2F_EventScript_SetReadyToMeetBrendan::
	setvar VAR_LITTLEROOT_RIVAL_STATE, 2
	return

LittlerootTown_BrendansHouse_2F_OnWarp:
	map_script_2 VAR_SECRET_BASE_INITIALIZED, 0, LittlerootTown_BrendansHouse_2F_EventScript_CheckInitDecor
	.2byte 0

LittlerootTown_BrendansHouse_2F_EventScript_CheckInitDecor::
	checkplayergender
	goto_if_eq VAR_RESULT, MALE, SecretBase_EventScript_InitDecorations
	end

LittlerootTown_BrendansHouse_2F_EventScript_RivalsPokeBall::
	lockall
	goto_if_eq VAR_LITTLEROOT_RIVAL_STATE, 2, LittlerootTown_BrendansHouse_2F_EventScript_MeetBrendan
	msgbox RivalsHouse_2F_Text_ItsRivalsPokeBall, MSGBOX_DEFAULT
	releaseall
	end

LittlerootTown_BrendansHouse_2F_EventScript_MeetBrendan::
	delay 10
	addobject LOCALID_RIVALS_HOUSE_2F_RIVAL
	applymovement LOCALID_RIVALS_HOUSE_2F_RIVAL, LittlerootTown_BrendansHouse_2F_Movement_BrendanEnters
	waitmovement 0
	playse SE_PIN
	applymovement LOCALID_RIVALS_HOUSE_2F_RIVAL, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement LOCALID_RIVALS_HOUSE_2F_RIVAL, Common_Movement_Delay48
	waitmovement 0
	delay 10
	playbgm MUS_ENCOUNTER_BRENDAN, TRUE
	call_if_eq VAR_FACING, DIR_NORTH, LittlerootTown_BrendansHouse_2F_EventScript_MeetBrendanNorth
	call_if_eq VAR_FACING, DIR_SOUTH, LittlerootTown_BrendansHouse_2F_EventScript_MeetBrendanSouth
	call_if_eq VAR_FACING, DIR_WEST, LittlerootTown_BrendansHouse_2F_EventScript_MeetBrendanWest
	call_if_eq VAR_FACING, DIR_EAST, LittlerootTown_BrendansHouse_2F_EventScript_MeetBrendanEast
	setvar VAR_LITTLEROOT_RIVAL_STATE, 3
	setflag FLAG_HIDE_LITTLEROOT_TOWN_BRENDANS_HOUSE_2F_POKE_BALL
	clearflag FLAG_HIDE_LITTLEROOT_TOWN_BRENDANS_HOUSE_RIVAL_BEDROOM
	setvar VAR_LITTLEROOT_TOWN_STATE, 1
	savebgm MUS_DUMMY
	fadedefaultbgm
	releaseall
	end

LittlerootTown_BrendansHouse_2F_EventScript_MeetBrendanNorth::
	applymovement LOCALID_RIVALS_HOUSE_2F_RIVAL, LittlerootTown_BrendansHouse_2F_Movement_BrendanApproachPlayerNorth
	waitmovement 0
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	msgbox RivalsHouse_2F_Text_BrendanWhoAreYou, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_PLAYER, LittlerootTown_BrendansHouse_2F_Movement_PlayerWatchBrendanNorth
	applymovement LOCALID_RIVALS_HOUSE_2F_RIVAL, LittlerootTown_BrendansHouse_2F_Movement_BrendanWalkToPCNorth
	waitmovement 0
	return

LittlerootTown_BrendansHouse_2F_EventScript_MeetBrendanSouth::
	applymovement LOCALID_RIVALS_HOUSE_2F_RIVAL, LittlerootTown_BrendansHouse_2F_Movement_BrendanApproachPlayerSouth
	waitmovement 0
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	msgbox RivalsHouse_2F_Text_BrendanWhoAreYou, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_PLAYER, LittlerootTown_BrendansHouse_2F_Movement_PlayerWatchBrendanSouth
	applymovement LOCALID_RIVALS_HOUSE_2F_RIVAL, LittlerootTown_BrendansHouse_2F_Movement_BrendanWalkToPCSouth
	waitmovement 0
	return

LittlerootTown_BrendansHouse_2F_EventScript_MeetBrendanWest::
	applymovement LOCALID_RIVALS_HOUSE_2F_RIVAL, LittlerootTown_BrendansHouse_2F_Movement_BrendanApproachPlayerWest
	waitmovement 0
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	msgbox RivalsHouse_2F_Text_BrendanWhoAreYou, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_PLAYER, LittlerootTown_BrendansHouse_2F_Movement_PlayerWatchBrendanWest
	applymovement LOCALID_RIVALS_HOUSE_2F_RIVAL, LittlerootTown_BrendansHouse_2F_Movement_BrendanWalkToPCWest
	waitmovement 0
	return

LittlerootTown_BrendansHouse_2F_EventScript_MeetBrendanEast::
	applymovement LOCALID_RIVALS_HOUSE_2F_RIVAL, LittlerootTown_BrendansHouse_2F_Movement_BrendanApproachPlayerEast
	waitmovement 0
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	msgbox RivalsHouse_2F_Text_BrendanWhoAreYou, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_RIVALS_HOUSE_2F_RIVAL, LittlerootTown_BrendansHouse_2F_Movement_BrendanWalkToPCEast
	waitmovement 0
	return

LittlerootTown_BrendansHouse_2F_Movement_BrendanEnters:
	walk_down
	walk_down
	walk_in_place_faster_left
	step_end

LittlerootTown_BrendansHouse_2F_Movement_BrendanApproachPlayerNorth:
	walk_left
	walk_left
	walk_down
	walk_down
	walk_left
	step_end

LittlerootTown_BrendansHouse_2F_Movement_BrendanWalkToPCNorth:
	walk_up
	walk_up
	walk_up
	walk_left
	walk_left
	walk_left
	walk_left
	walk_in_place_faster_up
	step_end

LittlerootTown_BrendansHouse_2F_Movement_PlayerWatchBrendanNorth:
	delay_16
	walk_in_place_faster_up
	delay_16
	delay_16
	delay_16
	delay_16
	walk_in_place_faster_left
	step_end

LittlerootTown_BrendansHouse_2F_Movement_BrendanApproachPlayerSouth:
	walk_left
	walk_left
	walk_left
	step_end

LittlerootTown_BrendansHouse_2F_Movement_BrendanWalkToPCSouth:
	walk_up
	walk_left
	walk_left
	walk_left
	walk_left
	walk_in_place_faster_up
	step_end

LittlerootTown_BrendansHouse_2F_Movement_PlayerWatchBrendanSouth:
	delay_16
	walk_in_place_faster_up
	delay_16
	delay_16
	walk_in_place_faster_left
	step_end

LittlerootTown_BrendansHouse_2F_Movement_BrendanApproachPlayerWest:
	walk_left
	walk_left
	walk_down
	walk_in_place_faster_left
	step_end

LittlerootTown_BrendansHouse_2F_Movement_BrendanWalkToPCWest:
	walk_up
	walk_up
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_in_place_faster_up
	step_end

LittlerootTown_BrendansHouse_2F_Movement_PlayerWatchBrendanWest:
	delay_8
	delay_16
	walk_in_place_faster_up
	delay_16
	delay_16
	walk_in_place_faster_left
	step_end

LittlerootTown_BrendansHouse_2F_Movement_BrendanApproachPlayerEast:
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_in_place_faster_down
	step_end

LittlerootTown_BrendansHouse_2F_Movement_BrendanWalkToPCEast:
	walk_up
	walk_left
	walk_left
	walk_in_place_faster_up
	step_end

@ Unused, the player is already facing this direction so its unneeded
LittlerootTown_BrendansHouse_2F_Movement_PlayerWatchBrendanEast:
	delay_16
	delay_16
	walk_in_place_faster_left
	step_end

LittlerootTown_BrendansHouse_2F_EventScript_PC::
	lockall
	checkplayergender
	goto_if_eq VAR_RESULT, MALE, LittlerootTown_BrendansHouse_2F_EventScript_CheckPlayersPC
	goto_if_eq VAR_RESULT, FEMALE, LittlerootTown_BrendansHouse_2F_EventScript_CheckRivalsPC
	end

LittlerootTown_BrendansHouse_2F_EventScript_CheckPlayersPC::
	setvar VAR_0x8004, PC_LOCATION_BRENDANS_HOUSE
	special DoPCTurnOnEffect
	playse SE_PC_ON
	msgbox gText_PlayerHouseBootPC, MSGBOX_DEFAULT
	special BedroomPC
	releaseall
	end

LittlerootTown_BrendansHouse_2F_EventScript_TurnOffPlayerPC::
	setvar VAR_0x8004, PC_LOCATION_BRENDANS_HOUSE
	playse SE_PC_OFF
	special DoPCTurnOffEffect
	releaseall
	end

LittlerootTown_BrendansHouse_2F_EventScript_CheckRivalsPC::
	msgbox gText_PokemonTrainerSchoolEmail, MSGBOX_DEFAULT
	releaseall
	end

PlayersHouse_2F_EventScript_Notebook::
	msgbox PlayersHouse_2F_Text_Notebook, MSGBOX_SIGN
	end

PlayersHouse_2F_EventScript_GameCube::
	msgbox PlayersHouse_2F_Text_ItsAGameCube, MSGBOX_SIGN
	end

PlayersHouse_2F_Text_ClockIsStopped:
	.string "The clock is stopped…\p"
	.string "Better set it and start it!$"

PlayersHouse_2F_Text_HowDoYouLikeYourRoom:
	.string "MOM: {PLAYER}, how do you like your\n"
	.string "new room?\p"
	.string "Good! Everything's put away neatly!\p"
	.string "They finished moving everything in\n"
	.string "downstairs, too.\p"
	.string "POKéMON movers are so convenient!\p"
	.string "Oh, you should make sure that\n"
	.string "everything's all there on your desk.$"

PlayersHouse_2F_Text_Notebook:
	.string "{PLAYER} flipped open the notebook.\p"
	.string "ADVENTURE RULE NO. 1\n"
	.string "Open the MENU with START.\p"
	.string "ADVENTURE RULE NO. 2\n"
	.string "Record your progress with SAVE.\p"
	.string "The remaining pages are blank…$"

Common_Text_LookCloserAtMap:
	.string "{PLAYER} took a closer look at the\n"
	.string "{REGION} region map.$"

PlayersHouse_2F_Text_ItsAGameCube:
	.string "It's a Nintendo GameCube.\p"
	.string "A Game Boy Advance is connected to\n"
	.string "serve as the Controller.$"
