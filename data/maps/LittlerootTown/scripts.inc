LittlerootTown_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, IviereTown_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, IviereTown_OnResume
	map_script MAP_SCRIPT_ON_FRAME_TABLE, IviereTown_OnFrame
	.byte 0

IviereTown_OnFrame:
	map_script_2 VAR_LITTLEROOT_INTRO_STATE, 9, IviereTown_EventScript_FilaoStarterScene
	.2byte 0

	@ State descriptions for VAR_LITTLEROOT_TOWN_STATE
	@ 1: Met Rival
	@ 2: Saving Birch
	@ 3: Received Pokedex
	@ 4: Received Running Shoes

	@ State descriptions for VAR_LITTLEROOT_INTRO_STATE
	@ 1: In truck male
	@ 2: In truck female
	@ 3: Entered house
	@ 4: Told to go set clock
	@ 5: Entered 2F to set clock
	@ 6: Set clock
	@ 7: Told to go meet rival

	@ State descriptions for VAR_LITTLEROOT_HOUSES_STATE_MAY
	@                    and VAR_LITTLEROOT_HOUSES_STATE_BRENDAN
	@ 1: On Truck (and is corresponding gender)
	@ 2: Met Rival's Mom (and is corresponding gender)
	@ 3: Entered Hall of Fame
	@ 4: Completed Lati TV / SS Ticket event
	@ 5: Exited house after above event

	@ State descriptions for VAR_LITTLEROOT_RIVAL_STATE
	@ 1: Never occurs
	@ 2: Entered Rival's bedroom
	@ 3: Met Rival
	@ 4: Received Pokedex


LittlerootTown_OnTransition:
	setflag FLAG_VISITED_LITTLEROOT_TOWN
	call Common_EventScript_SetupRivalGfxId
	call_if_eq VAR_LITTLEROOT_INTRO_STATE, 2, LittlerootTown_EventScript_MoveMomToMaysDoor
	call_if_unset FLAG_RESCUED_BIRCH, LittlerootTown_EventScript_SetTwinPos
	call_if_eq VAR_LITTLEROOT_TOWN_STATE, 3, LittlerootTown_EventScript_SetMomStandingInFrontOfDoorPos
	call_if_eq VAR_LITTLEROOT_HOUSES_STATE_MAY, 4, LittlerootTown_EventScript_SetExitedHouseAfterLatiSSTicketEvent
	call_if_eq VAR_LITTLEROOT_HOUSES_STATE_BRENDAN, 4, LittlerootTown_EventScript_SetExitedHouseAfterLatiSSTicketEvent
	call_if_eq VAR_OLDALE_RIVAL_STATE, 1, LittlerootTown_EventScript_MoveRivalFromOldale
	call_if_eq VAR_LITTLEROOT_RIVAL_STATE, 3, LittlerootTown_EventScript_SetRivalLeftForRoute103
	call_if_eq VAR_DEX_UPGRADE_JOHTO_STARTER_STATE, 1, LittlerootTown_EventScript_HideMapNamePopup
	call_if_eq VAR_DEX_UPGRADE_JOHTO_STARTER_STATE, 2, LittlerootTown_EventScript_LeftLabAfterDexUpgrade
	end

LittlerootTown_EventScript_LeftLabAfterDexUpgrade::
	setvar VAR_DEX_UPGRADE_JOHTO_STARTER_STATE, 3
	return

LittlerootTown_EventScript_HideMapNamePopup::
	setflag FLAG_HIDE_MAP_NAME_POPUP
	return

LittlerootTown_EventScript_SetRivalLeftForRoute103::
	setflag FLAG_RIVAL_LEFT_FOR_ROUTE103
	return

LittlerootTown_EventScript_MoveRivalFromOldale::
	setvar VAR_OLDALE_RIVAL_STATE, 2
	setflag FLAG_HIDE_OLDALE_TOWN_RIVAL
	return

LittlerootTown_EventScript_SetExitedHouseAfterLatiSSTicketEvent::
	setvar VAR_LITTLEROOT_HOUSES_STATE_MAY, 5
	setvar VAR_LITTLEROOT_HOUSES_STATE_BRENDAN, 5
	return

LittlerootTown_EventScript_MoveMomToMaysDoor::
	setobjectxyperm LOCALID_LITTLEROOT_MOM, 14, 8
	return

LittlerootTown_EventScript_SetTwinPos::
	goto_if_eq VAR_LITTLEROOT_TOWN_STATE, 0, LittlerootTown_EventScript_SetTwinGuardingRoutePos
	setobjectxyperm LOCALID_LITTLEROOT_TWIN, 10, 1
	setobjectmovementtype LOCALID_LITTLEROOT_TWIN, MOVEMENT_TYPE_FACE_UP
	return

LittlerootTown_EventScript_SetTwinGuardingRoutePos::
	setobjectxyperm LOCALID_LITTLEROOT_TWIN, 7, 2
	setobjectmovementtype LOCALID_LITTLEROOT_TWIN, MOVEMENT_TYPE_FACE_DOWN
	return

LittlerootTown_EventScript_SetMomStandingInFrontOfDoorPos::
	clearflag FLAG_HIDE_LITTLEROOT_TOWN_MOM_OUTSIDE
	setobjectmovementtype LOCALID_LITTLEROOT_MOM, MOVEMENT_TYPE_FACE_DOWN
	checkplayergender
	call_if_eq VAR_RESULT, MALE, LittlerootTown_EventScript_SetMomInFrontOfDoorMale
	call_if_eq VAR_RESULT, FEMALE, LittlerootTown_EventScript_SetMomInFrontOfDoorFemale
	return

LittlerootTown_EventScript_SetMomInFrontOfDoorMale::
	setobjectxyperm LOCALID_LITTLEROOT_MOM, 5, 9
	return

LittlerootTown_EventScript_SetMomInFrontOfDoorFemale::
	setobjectxyperm LOCALID_LITTLEROOT_MOM, 14, 9
	return

LittlerootTown_OnFrame:
	map_script_2 VAR_LITTLEROOT_INTRO_STATE, 1, LittlerootTown_EventScript_StepOffTruckMale
	map_script_2 VAR_LITTLEROOT_INTRO_STATE, 2, LittlerootTown_EventScript_StepOffTruckFemale
	map_script_2 VAR_DEX_UPGRADE_JOHTO_STARTER_STATE, 1, LittlerootTown_EventScript_BeginDexUpgradeScene
	.2byte 0

LittlerootTown_EventScript_StepOffTruckMale::
	lockall
	setvar VAR_0x8004, 5
	setvar VAR_0x8005, 8
	call LittlerootTown_EventScript_GoInsideWithMom
	setflag FLAG_HIDE_LITTLEROOT_TOWN_BRENDANS_HOUSE_TRUCK
	warpsilent MAP_LITTLEROOT_TOWN_BRENDANS_HOUSE_1F, 8, 8
	waitstate
	releaseall
	end

LittlerootTown_EventScript_StepOffTruckFemale::
	lockall
	setvar VAR_0x8004, 14
	setvar VAR_0x8005, 8
	call LittlerootTown_EventScript_GoInsideWithMom
	setflag FLAG_HIDE_LITTLEROOT_TOWN_MAYS_HOUSE_TRUCK
	warpsilent MAP_LITTLEROOT_TOWN_MAYS_HOUSE_1F, 2, 8
	waitstate
	releaseall
	end

LittlerootTown_EventScript_GoInsideWithMom::
	delay 15
	playse SE_LEDGE
	applymovement LOCALID_PLAYER, LittlerootTown_Movement_PlayerStepOffTruck
	waitmovement 0
	opendoor VAR_0x8004, VAR_0x8005
	waitdooranim
	addobject LOCALID_LITTLEROOT_MOM
	applymovement LOCALID_LITTLEROOT_MOM, LittlerootTown_Movement_MomExitHouse
	waitmovement 0
	closedoor VAR_0x8004, VAR_0x8005
	waitdooranim
	delay 10
	applymovement LOCALID_LITTLEROOT_MOM, LittlerootTown_Movement_MomApproachPlayerAtTruck
	waitmovement 0
	msgbox LittlerootTown_Text_OurNewHomeLetsGoInside, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_LITTLEROOT_MOM, LittlerootTown_Movement_MomApproachDoor
	applymovement LOCALID_PLAYER, LittlerootTown_Movement_PlayerApproachDoor
	waitmovement 0
	opendoor VAR_0x8004, VAR_0x8005
	waitdooranim
	applymovement LOCALID_LITTLEROOT_MOM, LittlerootTown_Movement_MomEnterHouse
	applymovement LOCALID_PLAYER, LittlerootTown_Movement_PlayerEnterHouse
	waitmovement 0
	setflag FLAG_HIDE_LITTLEROOT_TOWN_MOM_OUTSIDE
	setvar VAR_LITTLEROOT_INTRO_STATE, 3
	hideplayer
	closedoor VAR_0x8004, VAR_0x8005
	waitdooranim
	clearflag FLAG_HIDE_LITTLEROOT_TOWN_FAT_MAN
	clearflag FLAG_HIDE_MAP_NAME_POPUP
	return

LittlerootTown_Movement_MomExitHouse:
	walk_down
	step_end

LittlerootTown_Movement_MomApproachPlayerAtTruck:
	walk_down
	walk_in_place_faster_left
	step_end

LittlerootTown_Movement_MomApproachDoor:
	delay_16
	delay_8
	walk_up
	step_end

LittlerootTown_Movement_MomEnterHouse:
	walk_up
	set_invisible
	step_end

LittlerootTown_Movement_PlayerApproachDoor:
	delay_16
	delay_8
	walk_right
	walk_in_place_faster_up
	step_end

LittlerootTown_Movement_PlayerEnterHouse:
	walk_up
	walk_up
	step_end

LittlerootTown_Movement_PlayerStepOffTruck:
	jump_right
	delay_16
	delay_16
	delay_16
	step_end

LittlerootTown_EventScript_BeginDexUpgradeScene::
	lockall
	playse SE_PIN
	applymovement LOCALID_LITTLEROOT_BIRCH, Common_Movement_ExclamationMark
	waitmovement 0
	delay 80
	msgbox LittlerootTown_Text_BirchSomethingToShowYouAtLab, MSGBOX_DEFAULT
	closemessage
	clearflag FLAG_HIDE_LITTLEROOT_TOWN_RIVAL
	clearflag FLAG_HIDE_LITTLEROOT_TOWN_BIRCH
	delay 20
	clearflag FLAG_HIDE_MAP_NAME_POPUP
	warp MAP_LITTLEROOT_TOWN_PROFESSOR_BIRCHS_LAB, 6, 5
	waitstate
	releaseall
	end

LittlerootTown_OnWarp:
	map_script_2 VAR_DEX_UPGRADE_JOHTO_STARTER_STATE, 1, LittlerootTown_EventScript_SetRivalBirchPosForDexUpgrade
	.2byte 0

LittlerootTown_EventScript_SetRivalBirchPosForDexUpgrade::
	addobject LOCALID_LITTLEROOT_BIRCH
	addobject LOCALID_LITTLEROOT_RIVAL
	checkplayergender
	goto_if_eq VAR_RESULT, MALE, LittlerootTown_EventScript_SetRivalBirchPosForDexUpgradeMale
	goto LittlerootTown_EventScript_SetRivalBirchPosForDexUpgradeFemale
	end

LittlerootTown_EventScript_SetRivalBirchPosForDexUpgradeMale::
	setobjectxy LOCALID_LITTLEROOT_RIVAL, 6, 10
	setobjectxy LOCALID_LITTLEROOT_BIRCH, 5, 10
	end

LittlerootTown_EventScript_SetRivalBirchPosForDexUpgradeFemale::
	setobjectxy LOCALID_LITTLEROOT_RIVAL, 13, 10
	setobjectxy LOCALID_LITTLEROOT_BIRCH, 14, 10
	end

LittlerootTown_EventScript_FatMan::
	msgbox LittlerootTown_Text_CanUsePCToStoreItems, MSGBOX_NPC
	end

LittlerootTown_EventScript_Boy::
	msgbox LittlerootTown_Text_BirchSpendsDaysInLab, MSGBOX_NPC
	end

LittlerootTown_EventScript_Twin::
	lock
	faceplayer
	goto_if_set FLAG_ADVENTURE_STARTED, LittlerootTown_EventScript_GoodLuck
	goto_if_set FLAG_RESCUED_BIRCH, LittlerootTown_EventScript_YouSavedBirch
	goto_if_ne VAR_LITTLEROOT_TOWN_STATE, 0, LittlerootTown_EventScript_GoSaveBirch
	msgbox LittlerootTown_Text_IfYouGoInGrassPokemonWillJumpOut, MSGBOX_DEFAULT
	release
	end

LittlerootTown_EventScript_GoSaveBirch::
	special GetPlayerBigGuyGirlString
	msgbox LittlerootTown_Text_CanYouGoSeeWhatsHappening, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_LITTLEROOT_TWIN, Common_Movement_FaceOriginalDirection
	waitmovement 0
	setvar VAR_LITTLEROOT_TOWN_STATE, 2
	release
	end

LittlerootTown_EventScript_YouSavedBirch::
	special GetPlayerBigGuyGirlString
	msgbox LittlerootTown_Text_YouSavedBirch, MSGBOX_DEFAULT
	release
	end

LittlerootTown_EventScript_GoodLuck::
	msgbox LittlerootTown_Text_GoodLuckCatchingPokemon, MSGBOX_DEFAULT
	release
	end

LittlerootTown_EventScript_NeedPokemonTriggerLeft::
	lockall
	applymovement LOCALID_LITTLEROOT_TWIN, LittlerootTown_Movement_TwinApproachPlayerLeft
	waitmovement 0
	call LittlerootTown_EventScript_DangerousWithoutPokemon
	applymovement LOCALID_LITTLEROOT_TWIN, LittlerootTown_Movement_TwinReturnLeft
	waitmovement 0
	releaseall
	end

LittlerootTown_EventScript_DangerousWithoutPokemon::
	msgbox LittlerootTown_Text_IfYouGoInGrassPokemonWillJumpOut, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_LITTLEROOT_TWIN, LittlerootTown_Movement_TwinPushPlayerFromRoute
	applymovement LOCALID_PLAYER, LittlerootTown_Movement_PushPlayerBackFromRoute
	waitmovement 0
	msgbox LittlerootTown_Text_DangerousIfYouDontHavePokemon, MSGBOX_DEFAULT
	closemessage
	return

LittlerootTown_Movement_TwinApproachPlayerLeft:
	face_right
	delay_8
	disable_jump_landing_ground_effect
	jump_in_place_right
	delay_8
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_up
	walk_fast_up
	walk_fast_left
	face_down
	step_end

LittlerootTown_Movement_TwinPushPlayerFromRoute:
	walk_down
	step_end

LittlerootTown_Movement_TwinReturnLeft:
	walk_right
	walk_down
	walk_down
	walk_left
	walk_left
	walk_left
	walk_left
	walk_up
	walk_in_place_faster_down
	step_end

LittlerootTown_Movement_PushPlayerBackFromRoute:
	lock_facing_direction
	walk_down
	unlock_facing_direction
	step_end

LittlerootTown_EventScript_NeedPokemonTriggerRight::
	lockall
	applymovement LOCALID_LITTLEROOT_TWIN, LittlerootTown_Movement_TwinApproachPlayerRight
	waitmovement 0
	call LittlerootTown_EventScript_DangerousWithoutPokemon
	applymovement LOCALID_LITTLEROOT_TWIN, LittlerootTown_Movement_TwinReturnRight
	waitmovement 0
	releaseall
	end

LittlerootTown_Movement_TwinApproachPlayerRight:
	face_right
	delay_8
	disable_jump_landing_ground_effect
	jump_in_place_right
	delay_8
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_up
	walk_fast_up
	walk_fast_right
	face_down
	step_end

LittlerootTown_Movement_TwinReturnRight:
	walk_left
	walk_down
	walk_left
	walk_left
	walk_left
	walk_in_place_faster_down
	step_end

LittlerootTown_EventScript_GoSaveBirchTrigger::
	lockall
	applymovement LOCALID_LITTLEROOT_TWIN, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	special GetPlayerBigGuyGirlString
	msgbox LittlerootTown_Text_CanYouGoSeeWhatsHappening, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_LITTLEROOT_TWIN, Common_Movement_FaceOriginalDirection
	waitmovement 0
	setvar VAR_LITTLEROOT_TOWN_STATE, 2
	releaseall
	end

LittlerootTown_EventScript_TownSign::
	msgbox LittlerootTown_Text_TownSign, MSGBOX_SIGN
	end

LittlerootTown_EventScript_BirchsLabSign::
	msgbox LittlerootTown_Text_ProfBirchsLab, MSGBOX_SIGN
	end

LittlerootTown_EventScript_BrendansHouseSign::
	lockall
	checkplayergender
	call_if_eq VAR_RESULT, MALE, LittlerootTown_EventScript_PlayersHouseSignMale
	call_if_eq VAR_RESULT, FEMALE, LittlerootTown_EventScript_BirchsHouseSignFemale
	releaseall
	end

LittlerootTown_EventScript_PlayersHouseSignMale::
	msgbox LittlerootTown_Text_PlayersHouse, MSGBOX_DEFAULT
	return

LittlerootTown_EventScript_BirchsHouseSignFemale::
	msgbox LittlerootTown_Text_ProfBirchsHouse, MSGBOX_DEFAULT
	return

LittlerootTown_EventScript_MaysHouseSign::
	lockall
	checkplayergender
	call_if_eq VAR_RESULT, MALE, LittlerootTown_EventScript_BirchsHouseSignMale
	call_if_eq VAR_RESULT, FEMALE, LittlerootTown_EventScript_PlayersHouseSignFemale
	releaseall
	end

LittlerootTown_EventScript_BirchsHouseSignMale::
	msgbox LittlerootTown_Text_ProfBirchsHouse, MSGBOX_DEFAULT
	return

LittlerootTown_EventScript_PlayersHouseSignFemale::
	msgbox LittlerootTown_Text_PlayersHouse, MSGBOX_DEFAULT
	return

LittlerootTown_EventScript_GiveRunningShoesTrigger0::
	lockall
	setvar VAR_0x8008, 0
	setobjectxy LOCALID_LITTLEROOT_MOM, 10, 9
	goto LittlerootTown_EventScript_GiveRunningShoesTrigger
	end

LittlerootTown_EventScript_GiveRunningShoesTrigger1::
	lockall
	setvar VAR_0x8008, 1
	setobjectxy LOCALID_LITTLEROOT_MOM, 11, 9
	goto LittlerootTown_EventScript_GiveRunningShoesTrigger
	end

LittlerootTown_EventScript_GiveRunningShoesTrigger2::
	lockall
	setvar VAR_0x8008, 2
	goto LittlerootTown_EventScript_GiveRunningShoesTrigger
	end

LittlerootTown_EventScript_GiveRunningShoesTrigger3::
	lockall
	setvar VAR_0x8008, 3
	goto LittlerootTown_EventScript_GiveRunningShoesTrigger
	end

LittlerootTown_EventScript_GiveRunningShoesTrigger4::
	lockall
	setvar VAR_0x8008, 4
	goto LittlerootTown_EventScript_GiveRunningShoesTrigger
	end

LittlerootTown_EventScript_GiveRunningShoesTrigger5::
	lockall
	setvar VAR_0x8008, 5
	goto LittlerootTown_EventScript_GiveRunningShoesTrigger
	end

LittlerootTown_EventScript_GiveRunningShoesTrigger::
	checkplayergender
	call_if_eq VAR_RESULT, MALE, LittlerootTown_EventScript_MomNoticePlayerMale
	call_if_eq VAR_RESULT, FEMALE, LittlerootTown_EventScript_MomNoticePlayerFemale
	checkplayergender
	call_if_eq VAR_RESULT, MALE, LittlerootTown_EventScript_SetHomeDoorCoordsMale
	call_if_eq VAR_RESULT, FEMALE, LittlerootTown_EventScript_SetHomeDoorCoordsFemale
	msgbox LittlerootTown_Text_WaitPlayer, MSGBOX_DEFAULT
	closemessage
	checkplayergender
	call_if_eq VAR_RESULT, MALE, LittlerootTown_EventScript_MomApproachPlayerMale
	call_if_eq VAR_RESULT, FEMALE, LittlerootTown_EventScript_MomApproachPlayerFemale
	call LittlerootTown_EventScript_GiveRunningShoes
	checkplayergender
	call_if_eq VAR_RESULT, MALE, LittlerootTown_EventScript_MomReturnHomeMale
	call_if_eq VAR_RESULT, FEMALE, LittlerootTown_EventScript_MomReturnHomeFemale
	goto LittlerootTown_EventScript_SetReceivedRunningShoes
	end

LittlerootTown_EventScript_SetHomeDoorCoordsMale::
	setvar VAR_0x8009, 5
	setvar VAR_0x800A, 8
	return

LittlerootTown_EventScript_SetHomeDoorCoordsFemale::
	setvar VAR_0x8009, 14
	setvar VAR_0x800A, 8
	return

LittlerootTown_EventScript_MomNoticePlayerMale::
	applymovement LOCALID_LITTLEROOT_MOM, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	return

LittlerootTown_EventScript_MomNoticePlayerFemale::
	applymovement LOCALID_LITTLEROOT_MOM, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	return

LittlerootTown_EventScript_MomApproachPlayerMale::
	call_if_eq VAR_0x8008, 0, LittlerootTown_EventScript_MomApproachPlayer0
	call_if_eq VAR_0x8008, 1, LittlerootTown_EventScript_MomApproachPlayer1
	call_if_eq VAR_0x8008, 2, LittlerootTown_EventScript_MomApproachPlayerMale2
	call_if_eq VAR_0x8008, 3, LittlerootTown_EventScript_MomApproachPlayerMale3
	call_if_eq VAR_0x8008, 4, LittlerootTown_EventScript_MomApproachPlayerMale4
	call_if_eq VAR_0x8008, 5, LittlerootTown_EventScript_MomApproachPlayerMale5
	return

LittlerootTown_EventScript_MomApproachPlayerFemale::
	call_if_eq VAR_0x8008, 0, LittlerootTown_EventScript_MomApproachPlayer0
	call_if_eq VAR_0x8008, 1, LittlerootTown_EventScript_MomApproachPlayer1
	call_if_eq VAR_0x8008, 2, LittlerootTown_EventScript_MomApproachPlayerFemale2
	call_if_eq VAR_0x8008, 3, LittlerootTown_EventScript_MomApproachPlayerFemale3
	call_if_eq VAR_0x8008, 4, LittlerootTown_EventScript_MomApproachPlayerFemale4
	call_if_eq VAR_0x8008, 5, LittlerootTown_EventScript_MomApproachPlayerFemale5
	return

LittlerootTown_EventScript_MomApproachPlayer0::
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	applymovement LOCALID_LITTLEROOT_MOM, LittlerootTown_Movement_MomApproachPlayer0
	waitmovement 0
	return

LittlerootTown_EventScript_MomApproachPlayer1::
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	applymovement LOCALID_LITTLEROOT_MOM, LittlerootTown_Movement_MomApproachPlayer1
	waitmovement 0
	return

LittlerootTown_EventScript_MomApproachPlayerMale2::
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	applymovement LOCALID_LITTLEROOT_MOM, LittlerootTown_Movement_MomApproachPlayerMale2
	waitmovement 0
	return

LittlerootTown_EventScript_MomApproachPlayerMale3::
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	applymovement LOCALID_LITTLEROOT_MOM, LittlerootTown_Movement_MomApproachPlayerMale3
	waitmovement 0
	return

LittlerootTown_EventScript_MomApproachPlayerMale4::
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	applymovement LOCALID_LITTLEROOT_MOM, LittlerootTown_Movement_MomApproachPlayerMale4
	waitmovement 0
	return

LittlerootTown_EventScript_MomApproachPlayerMale5::
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	applymovement LOCALID_LITTLEROOT_MOM, LittlerootTown_Movement_MomApproachPlayerMale5
	waitmovement 0
	return

LittlerootTown_EventScript_MomApproachPlayerFemale2::
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	applymovement LOCALID_LITTLEROOT_MOM, LittlerootTown_Movement_MomApproachPlayerFemale2
	waitmovement 0
	return

LittlerootTown_EventScript_MomApproachPlayerFemale3::
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	applymovement LOCALID_LITTLEROOT_MOM, LittlerootTown_Movement_MomApproachPlayerFemale3
	waitmovement 0
	return

LittlerootTown_EventScript_MomApproachPlayerFemale4::
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	applymovement LOCALID_LITTLEROOT_MOM, LittlerootTown_Movement_MomApproachPlayerFemale4
	waitmovement 0
	return

LittlerootTown_EventScript_MomApproachPlayerFemale5::
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	applymovement LOCALID_LITTLEROOT_MOM, LittlerootTown_Movement_MomApproachPlayerFemale5
	waitmovement 0
	return

LittlerootTown_EventScript_MomReturnHomeMale::
	call_if_eq VAR_0x8008, 0, LittlerootTown_EventScript_MomReturnHome0
	call_if_eq VAR_0x8008, 1, LittlerootTown_EventScript_MomReturnHome1
	call_if_eq VAR_0x8008, 2, LittlerootTown_EventScript_MomReturnHomeMale2
	call_if_eq VAR_0x8008, 3, LittlerootTown_EventScript_MomReturnHomeMale3
	call_if_eq VAR_0x8008, 4, LittlerootTown_EventScript_MomReturnHomeMale4
	call_if_eq VAR_0x8008, 5, LittlerootTown_EventScript_MomReturnHomeMale5
	return

LittlerootTown_EventScript_MomReturnHomeFemale::
	call_if_eq VAR_0x8008, 0, LittlerootTown_EventScript_MomReturnHome0
	call_if_eq VAR_0x8008, 1, LittlerootTown_EventScript_MomReturnHome1
	call_if_eq VAR_0x8008, 2, LittlerootTown_EventScript_MomReturnHomeFemale2
	call_if_eq VAR_0x8008, 3, LittlerootTown_EventScript_MomReturnHomeFemale3
	call_if_eq VAR_0x8008, 4, LittlerootTown_EventScript_MomReturnHomeFemale4
	call_if_eq VAR_0x8008, 5, LittlerootTown_EventScript_MomReturnHomeFemale5
	return

LittlerootTown_EventScript_MomReturnHome0::
	applymovement LOCALID_LITTLEROOT_MOM, LittlerootTown_Movement_MomReturnHome0
	waitmovement 0
	return

LittlerootTown_EventScript_MomReturnHome1::
	applymovement LOCALID_LITTLEROOT_MOM, LittlerootTown_Movement_MomReturnHome1
	waitmovement 0
	return

LittlerootTown_EventScript_MomReturnHomeMale2::
	applymovement LOCALID_LITTLEROOT_MOM, LittlerootTown_Movement_MomReturnHomeMale2
	waitmovement 0
	opendoor VAR_0x8009, VAR_0x800A
	waitdooranim
	applymovement LOCALID_LITTLEROOT_MOM, LittlerootTown_Movement_MomExitThroughDoor
	waitmovement 0
	hideobjectat LOCALID_LITTLEROOT_MOM, MAP_LITTLEROOT_TOWN
	closedoor VAR_0x8009, VAR_0x800A
	waitdooranim
	return

LittlerootTown_EventScript_MomReturnHomeMale3::
	applymovement LOCALID_LITTLEROOT_MOM, LittlerootTown_Movement_MomReturnHomeMale3
	waitmovement 0
	opendoor VAR_0x8009, VAR_0x800A
	waitdooranim
	applymovement LOCALID_LITTLEROOT_MOM, LittlerootTown_Movement_MomExitThroughDoor
	waitmovement 0
	hideobjectat LOCALID_LITTLEROOT_MOM, MAP_LITTLEROOT_TOWN
	closedoor VAR_0x8009, VAR_0x800A
	waitdooranim
	return

LittlerootTown_EventScript_MomReturnHomeMale4::
	applymovement LOCALID_LITTLEROOT_MOM, LittlerootTown_Movement_MomReturnHomeMale4
	waitmovement 0
	opendoor VAR_0x8009, VAR_0x800A
	waitdooranim
	applymovement LOCALID_LITTLEROOT_MOM, LittlerootTown_Movement_MomExitThroughDoor
	waitmovement 0
	hideobjectat LOCALID_LITTLEROOT_MOM, MAP_LITTLEROOT_TOWN
	closedoor VAR_0x8009, VAR_0x800A
	waitdooranim
	return

LittlerootTown_EventScript_MomReturnHomeMale5::
	applymovement LOCALID_LITTLEROOT_MOM, LittlerootTown_Movement_MomReturnHomeMale5
	waitmovement 0
	opendoor VAR_0x8009, VAR_0x800A
	waitdooranim
	applymovement LOCALID_LITTLEROOT_MOM, LittlerootTown_Movement_MomExitThroughDoor
	waitmovement 0
	hideobjectat LOCALID_LITTLEROOT_MOM, MAP_LITTLEROOT_TOWN
	closedoor VAR_0x8009, VAR_0x800A
	waitdooranim
	return

LittlerootTown_EventScript_MomReturnHomeFemale2::
	applymovement LOCALID_LITTLEROOT_MOM, LittlerootTown_Movement_MomReturnHomeFemale2
	waitmovement 0
	opendoor VAR_0x8009, VAR_0x800A
	waitdooranim
	applymovement LOCALID_LITTLEROOT_MOM, LittlerootTown_Movement_MomExitThroughDoor
	waitmovement 0
	hideobjectat LOCALID_LITTLEROOT_MOM, MAP_LITTLEROOT_TOWN
	closedoor VAR_0x8009, VAR_0x800A
	waitdooranim
	return

LittlerootTown_EventScript_MomReturnHomeFemale3::
	applymovement LOCALID_LITTLEROOT_MOM, LittlerootTown_Movement_MomReturnHomeFemale3
	waitmovement 0
	opendoor VAR_0x8009, VAR_0x800A
	waitdooranim
	applymovement LOCALID_LITTLEROOT_MOM, LittlerootTown_Movement_MomExitThroughDoor
	waitmovement 0
	hideobjectat LOCALID_LITTLEROOT_MOM, MAP_LITTLEROOT_TOWN
	closedoor VAR_0x8009, VAR_0x800A
	waitdooranim
	return

LittlerootTown_EventScript_MomReturnHomeFemale4::
	applymovement LOCALID_LITTLEROOT_MOM, LittlerootTown_Movement_MomReturnHomeFemale4
	waitmovement 0
	opendoor VAR_0x8009, VAR_0x800A
	waitdooranim
	applymovement LOCALID_LITTLEROOT_MOM, LittlerootTown_Movement_MomExitThroughDoor
	waitmovement 0
	hideobjectat LOCALID_LITTLEROOT_MOM, MAP_LITTLEROOT_TOWN
	closedoor VAR_0x8009, VAR_0x800A
	waitdooranim
	return

LittlerootTown_EventScript_MomReturnHomeFemale5::
	applymovement LOCALID_LITTLEROOT_MOM, LittlerootTown_Movement_MomReturnHomeFemale5
	waitmovement 0
	opendoor VAR_0x8009, VAR_0x800A
	waitdooranim
	applymovement LOCALID_LITTLEROOT_MOM, LittlerootTown_Movement_MomExitThroughDoor
	waitmovement 0
	hideobjectat LOCALID_LITTLEROOT_MOM, MAP_LITTLEROOT_TOWN
	closedoor VAR_0x8009, VAR_0x800A
	waitdooranim
	return

LittlerootTown_Movement_MomApproachPlayer0:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

LittlerootTown_Movement_MomApproachPlayer1:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

LittlerootTown_Movement_MomApproachPlayerMale2:
	walk_right
	walk_right
	walk_right
	walk_right
	step_end

LittlerootTown_Movement_MomApproachPlayerMale3:
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	step_end

LittlerootTown_Movement_MomApproachPlayerMale4:
	walk_right
	walk_right
	step_end

LittlerootTown_Movement_MomApproachPlayerMale5:
	walk_right
	walk_right
	walk_right
	step_end

LittlerootTown_Movement_MomApproachPlayerFemale2:
	walk_left
	walk_left
	walk_left
	step_end

LittlerootTown_Movement_MomApproachPlayerFemale3:
	walk_left
	walk_left
	step_end

LittlerootTown_Movement_MomApproachPlayerFemale4:
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	step_end

LittlerootTown_Movement_MomApproachPlayerFemale5:
	walk_left
	walk_left
	walk_left
	walk_left
	step_end

LittlerootTown_Movement_MomReturnHome0:
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

LittlerootTown_Movement_MomReturnHome1:
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

LittlerootTown_Movement_MomReturnHomeMale2:
	walk_left
	walk_left
	walk_left
	walk_left
	walk_in_place_faster_up
	step_end

LittlerootTown_Movement_MomReturnHomeMale3:
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_in_place_faster_up
	step_end

LittlerootTown_Movement_MomReturnHomeMale4:
	walk_left
	walk_left
	walk_in_place_faster_up
	step_end

LittlerootTown_Movement_MomReturnHomeMale5:
	walk_left
	walk_left
	walk_left
	walk_in_place_faster_up
	step_end

LittlerootTown_Movement_MomReturnHomeFemale2:
	walk_right
	walk_right
	walk_right
	walk_in_place_faster_up
	step_end

LittlerootTown_Movement_MomReturnHomeFemale3:
	walk_right
	walk_right
	walk_in_place_faster_up
	step_end

LittlerootTown_Movement_MomReturnHomeFemale4:
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_in_place_faster_up
	step_end

LittlerootTown_Movement_MomReturnHomeFemale5:
	walk_right
	walk_right
	walk_right
	walk_right
	walk_in_place_faster_up
	step_end

LittlerootTown_Movement_MomExitThroughDoor:
	walk_up
	step_end

LittlerootTown_EventScript_Mom::
	lock
	faceplayer
	checkplayergender
	call_if_eq VAR_RESULT, MALE, LittlerootTown_EventScript_SetHomeDoorCoordsMale
	call_if_eq VAR_RESULT, FEMALE, LittlerootTown_EventScript_SetHomeDoorCoordsFemale
	call LittlerootTown_EventScript_GiveRunningShoes
	applymovement LOCALID_LITTLEROOT_MOM, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	opendoor VAR_0x8009, VAR_0x800A
	waitdooranim
	applymovement LOCALID_LITTLEROOT_MOM, LittlerootTown_Movement_MomExitThroughDoor
	waitmovement 0
	hideobjectat LOCALID_LITTLEROOT_MOM, MAP_LITTLEROOT_TOWN
	closedoor VAR_0x8009, VAR_0x800A
	waitdooranim
	goto LittlerootTown_EventScript_SetReceivedRunningShoes
	end

LittlerootTown_EventScript_SetReceivedRunningShoes::
	removeobject LOCALID_LITTLEROOT_MOM
	setflag FLAG_SYS_B_DASH
	setvar VAR_LITTLEROOT_TOWN_STATE, 4
	release
	end

LittlerootTown_EventScript_GiveRunningShoes::
	msgbox LittlerootTown_Text_WearTheseRunningShoes, MSGBOX_DEFAULT
	playfanfare MUS_OBTAIN_ITEM
	message LittlerootTown_Text_SwitchShoesWithRunningShoes
	waitfanfare
	setflag FLAG_RECEIVED_RUNNING_SHOES
	msgbox LittlerootTown_Text_ExplainRunningShoes, MSGBOX_DEFAULT
	msgbox LittlerootTown_Text_ComeHomeIfAnythingHappens, MSGBOX_DEFAULT
	closemessage
	delay 30
	return

LittlerootTown_Text_OurNewHomeLetsGoInside:
	.string "MOM: {PLAYER}, we're here, honey!\p"
	.string "It must be tiring riding with our things\n"
	.string "in the moving truck.\p"
	.string "Well, this is LITTLEROOT TOWN.\p"
	.string "How do you like it?\n"
	.string "This is our new home!\p"
	.string "It has a quaint feel, but it seems to be\n"
	.string "an easy place to live, don't you think?\p"
	.string "And, you get your own room, {PLAYER}!\n"
	.string "Let's go inside.$"

LittlerootTown_Text_WaitPlayer:
	.string "MOM: Wait, {PLAYER}!$"

LittlerootTown_Text_WearTheseRunningShoes:
	.string "MOM: {PLAYER}! {PLAYER}! Did you\n"
	.string "introduce yourself to PROF. BIRCH?\p"
	.string "Oh! What an adorable POKéMON!\n"
	.string "You got it from PROF. BIRCH. How nice!\p"
	.string "You're your father's child, all right.\n"
	.string "You look good together with POKéMON!\p"
	.string "Here, honey! If you're going out on an\n"
	.string "adventure, wear these RUNNING SHOES.\p"
	.string "They'll put a zip in your step!$"

LittlerootTown_Text_SwitchShoesWithRunningShoes:
	.string "{PLAYER} switched shoes with the\n"
	.string "RUNNING SHOES.$"

LittlerootTown_Text_ExplainRunningShoes:
	.string "MOM: {PLAYER}, those shoes came with\n"
	.string "instructions.\p"
	.string "“Press the B Button while wearing these\n"
	.string "RUNNING SHOES to run extra-fast!\p"
	.string "“Slip on these RUNNING SHOES and race\n"
	.string "in the great outdoors!”$"

LittlerootTown_Text_ComeHomeIfAnythingHappens:
	.string "… … … … … … … …\n"
	.string "… … … … … … … …\p"
	.string "To think that you have your very own\n"
	.string "POKéMON now…\l"
	.string "Your father will be overjoyed.\p"
	.string "…But please be careful.\n"
	.string "If anything happens, you can come home.\p"
	.string "Go on, go get them, honey!$"

LittlerootTown_Text_CanUsePCToStoreItems:
	.string "If you use a PC, you can store items\n"
	.string "and POKéMON.\p"
	.string "The power of science is staggering!$"

LittlerootTown_Text_BirchSpendsDaysInLab:
	.string "PROF. BIRCH spends days in his LAB\n"
	.string "studying, then he'll suddenly go out in\l"
	.string "the wild to do more research…\p"
	.string "When does PROF. BIRCH spend time\n"
	.string "at home?$"

LittlerootTown_Text_IfYouGoInGrassPokemonWillJumpOut:
	.string "Um, um, um!\p"
	.string "If you go outside and go in the grass,\n"
	.string "wild POKéMON will jump out!$"

LittlerootTown_Text_DangerousIfYouDontHavePokemon:
	.string "It's dangerous if you don't have\n"
	.string "your own POKéMON.$"

LittlerootTown_Text_CanYouGoSeeWhatsHappening:
	.string "Um, hi!\p"
	.string "There are scary POKéMON outside!\n"
	.string "I can hear their cries!\p"
	.string "I want to go see what's going on,\n"
	.string "but I don't have any POKéMON…\p"
	.string "Can you go see what's happening\n"
	.string "for me?$"

LittlerootTown_Text_YouSavedBirch:
	.string "You saved PROF. BIRCH!\n"
	.string "I'm so glad!$"

LittlerootTown_Text_GoodLuckCatchingPokemon:
	.string "Are you going to catch POKéMON?\n"
	.string "Good luck!$"

LittlerootTown_Text_TownSign:
	.string "LITTLEROOT TOWN\n"
	.string "“A town that can't be shaded any hue.”$"

LittlerootTown_Text_ProfBirchsLab:
	.string "PROF. BIRCH'S POKéMON LAB$"

LittlerootTown_Text_PlayersHouse:
	.string "{PLAYER}'s HOUSE$"

LittlerootTown_Text_ProfBirchsHouse:
	.string "PROF. BIRCH'S HOUSE$"

LittlerootTown_Text_BirchSomethingToShowYouAtLab:
	.string "PROF. BIRCH: Well, well, {PLAYER}{KUN}!\n"
	.string "That was good work out there!\p"
	.string "I knew there was something special\n"
	.string "about you when I first saw you,\l"
	.string "but I never expected this.\p"
	.string "Oh, yes. Do you still have the POKéDEX\n"
	.string "I gave you?\p"
	.string "I have something to show you.\n"
	.string "Let's go to my LAB.$"


IviereTown_OnTransition:
	setflag FLAG_VISITED_LITTLEROOT_TOWN
	special StartIviereFlyby
	end

IviereTown_OnResume:
	special StartIviereFlyby
	end

IviereTown_EventScript_TownPokemon::
	lock
	faceplayer
	playmoncry SPECIES_PIDGEY, CRY_MODE_NORMAL
	msgbox IviereTown_Text_TownPokemon, MSGBOX_DEFAULT
	release
	end

IviereTown_Text_TownPokemon:
	.string "Pidgey : Rrrrouuu rrrrouuu !$"

IviereTown_EventScript_NameRater::
	lock
	faceplayer
	msgbox IviereTown_Text_NameRater, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, SlateportCity_NameRatersHouse_EventScript_DeclineNameRate
	closemessage
	dynmultichoice 18, 2, FALSE, 3, 0, DYN_MULTICHOICE_CB_NONE, IviereTown_Text_Party, IviereTown_Text_Box, IviereTown_Text_Cancel
	goto_if_eq VAR_RESULT, 0, IviereTown_EventScript_RatePartyMon
	goto_if_eq VAR_RESULT, 1, SlateportCity_NameRatersHouse_EventScript_ChooseMonToRate
	goto SlateportCity_NameRatersHouse_EventScript_DeclineNameRate

IviereTown_EventScript_RatePartyMon:
	special ChoosePartyMon
	waitstate
	goto_if_eq VAR_0x8004, PARTY_NOTHING_CHOSEN, SlateportCity_NameRatersHouse_EventScript_DeclineNameRate
	goto SlateportCity_NameRatersHouse_EventScript_RateMonNickname

IviereTown_Text_Party:
	.string "Party$"

IviereTown_Text_Box:
	.string "Box$"

IviereTown_Text_Cancel:
	.string "Cancel$"


IviereTown_EventScript_Child::
	msgbox IviereTown_Text_Child, MSGBOX_NPC
	end

IviereTown_EventScript_RiverNpc::
	lock
	faceplayer
	goto_if_set FLAG_GOT_EXP_SHARE_FROM_ALDA, IviereTown_EventScript_AldaAfterGift
	setspeaker SP_NAME_ALDA
	msgbox IviereTown_Text_AldaStoryGift, MSGBOX_DEFAULT
	giveitem ITEM_EXP_SHARE
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	setflag FLAG_GOT_EXP_SHARE_FROM_ALDA
	setflag FLAG_SYS_EXP_SHARE
	setspeaker SP_NAME_ALDA
	msgbox IviereTown_Text_AldaExplainExpShare, MSGBOX_DEFAULT
	release
	end

IviereTown_EventScript_AldaAfterGift::
	goto_if_set FLAG_SYS_EXP_SHARE, IviereTown_EventScript_AldaAfterGiftNormal
	checkitem ITEM_EXP_SHARE
	goto_if_eq VAR_RESULT, TRUE, IviereTown_EventScript_AldaTurnOnExpShare
	giveitem ITEM_EXP_SHARE
IviereTown_EventScript_AldaTurnOnExpShare:
	setflag FLAG_SYS_EXP_SHARE
IviereTown_EventScript_AldaAfterGiftNormal:
	setspeaker SP_NAME_ALDA
	msgbox IviereTown_Text_AldaAfter, MSGBOX_NPC
	end

IviereTown_EventScript_PlayerSign::
	msgbox IviereTown_Text_PlayerSign, MSGBOX_SIGN
	end

IviereTown_EventScript_FilaoHouseSign::
	msgbox IviereTown_Text_FilaoHouseSign, MSGBOX_SIGN
	end

IviereTown_EventScript_TownSign::
	msgbox IviereTown_Text_TownSign, MSGBOX_SIGN
	end

IviereTown_EventScript_MenuSign::
	msgbox IviereTown_Text_MenuSign, MSGBOX_SIGN
	end

IviereTown_Text_NameRater:
	.string "Hello! I am the NAME RATER!\p"
	.string "Would you like to give a new name\n"
	.string "to one of your POKéMON?$"

IviereTown_Text_Child:
	.string "I'm totally bored! I have no friends\n"
	.string "to play with. If only I had a POKéMON.$"

IviereTown_Text_AldaStoryGift:
	.string "Oh, hello, child…\p"
	.string "I heard that terrible commotion\n"
	.string "from your home earlier.\p"
	.string "At my age, seeing such cruelty\n"
	.string "truly breaks my heart.\p"
	.string "I knew your father so well…\n"
	.string "He was a kind, honorable man.\p"
	.string "Seeing the fiery determination\n"
	.string "in your eyes moves me deeply.\p"
	.string "Even when things do not turn out\n"
	.string "the way we wish, we can still\l"
	.string "stand tall and do what's right.\p"
	.string "Please, take this with you on\n"
	.string "your journey.$"

IviereTown_Text_AldaExplainExpShare:
	.string "That EXP. SHARE will allow all\n"
	.string "the POKéMON in your party to\l"
	.string "grow together as one.\p"
	.string "May your steps be guided by\n"
	.string "wisdom and strength, dear.$"

IviereTown_Text_AldaAfter:
	.string "May the road treat you kindly.\p"
	.string "Whenever doubts arise, remember:\n"
	.string "your father's legacy lives on\l"
	.string "within your heart.$"

IviereTown_Text_PlayerSign:
	.string "{PLAYER}'s House$"

IviereTown_Text_FilaoHouseSign:
	.string "Prof. FILAO's House$"

IviereTown_Text_TownSign:
	.string "IVIERE TOWN\n"
	.string "A peaceful city lit by its\l"
	.string "clear water reflections.$"

IviereTown_Text_MenuSign:
	.string "TIP\p"
	.string "Use START to open the menu.$"

IviereTown_EventScript_FilaoStarterScene::
	lockall
	delay 20
	applymovement LOCALID_PLAYER, Common_Movement_FaceDown
	waitmovement 0
	delay 10
	setspeaker SP_NAME_PLAYER
	msgbox IviereTown_Text_PlayerFrustrated, MSGBOX_DEFAULT
	closemessage
	playse SE_PIN
	applymovement LOCALID_PLAYER, Common_Movement_ExclamationMark
	waitmovement 0
	delay 10
	setspeaker SP_NAME_FILAO
	msgbox IviereTown_Text_FilaoCallOut, MSGBOX_DEFAULT
	closemessage
	clearflag FLAG_HIDE_IVIERE_FILAO_OUTSIDE
	addobject LOCALID_IVIERE_FILAO
	applymovement LOCALID_IVIERE_FILAO, IviereTown_Movement_FilaoApproachPlayer
	applymovement LOCALID_PLAYER, Common_Movement_FaceLeft
	waitmovement LOCALID_IVIERE_FILAO
	applymovement LOCALID_IVIERE_FILAO, Common_Movement_FaceRight
	waitmovement LOCALID_IVIERE_FILAO
	setspeaker SP_NAME_FILAO
	msgbox IviereTown_Text_FilaoAsks, MSGBOX_DEFAULT
	closemessage
	setspeaker SP_NAME_PLAYER
	msgbox IviereTown_Text_PlayerExplains, MSGBOX_DEFAULT
	closemessage
	setspeaker SP_NAME_FILAO
	msgbox IviereTown_Text_FilaoComforts, MSGBOX_DEFAULT
	closemessage
	goto IviereTown_ChooseType

IviereTown_Movement_FilaoApproachPlayer:
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	step_end

IviereTown_Movement_FilaoLeaves:
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	step_end

IviereTown_ChooseType:
	message IviereTown_Text_ChooseType
	waitmessage
	dynmultichoice 20, 1, TRUE, 3, 0, DYN_MULTICHOICE_CB_NONE, IviereTown_Text_Fire, IviereTown_Text_Water, IviereTown_Text_Grass
	closemessage
	goto_if_eq VAR_RESULT, 1, IviereTown_WaterStarters
	goto_if_eq VAR_RESULT, 2, IviereTown_GrassStarters

	dynmultipush IviereTown_Text_Charmander, SPECIES_CHARMANDER
	dynmultipush IviereTown_Text_Cyndaquil, SPECIES_CYNDAQUIL
	dynmultipush IviereTown_Text_Torchic, SPECIES_TORCHIC
	dynmultipush IviereTown_Text_Chimchar, SPECIES_CHIMCHAR
	dynmultipush IviereTown_Text_Tepig, SPECIES_TEPIG
	dynmultipush IviereTown_Text_Fennekin, SPECIES_FENNEKIN
	dynmultipush IviereTown_Text_Litten, SPECIES_LITTEN
	dynmultipush IviereTown_Text_Scorbunny, SPECIES_SCORBUNNY
	dynmultipush IviereTown_Text_Fuecoco, SPECIES_FUECOCO
	goto IviereTown_ChooseStarter

IviereTown_WaterStarters:
	dynmultipush IviereTown_Text_Squirtle, SPECIES_SQUIRTLE
	dynmultipush IviereTown_Text_Totodile, SPECIES_TOTODILE
	dynmultipush IviereTown_Text_Mudkip, SPECIES_MUDKIP
	dynmultipush IviereTown_Text_Piplup, SPECIES_PIPLUP
	dynmultipush IviereTown_Text_Oshawott, SPECIES_OSHAWOTT
	dynmultipush IviereTown_Text_Froakie, SPECIES_FROAKIE
	dynmultipush IviereTown_Text_Popplio, SPECIES_POPPLIO
	dynmultipush IviereTown_Text_Sobble, SPECIES_SOBBLE
	dynmultipush IviereTown_Text_Quaxly, SPECIES_QUAXLY
	goto IviereTown_ChooseStarter

IviereTown_GrassStarters:
	dynmultipush IviereTown_Text_Bulbasaur, SPECIES_BULBASAUR
	dynmultipush IviereTown_Text_Chikorita, SPECIES_CHIKORITA
	dynmultipush IviereTown_Text_Treecko, SPECIES_TREECKO
	dynmultipush IviereTown_Text_Turtwig, SPECIES_TURTWIG
	dynmultipush IviereTown_Text_Snivy, SPECIES_SNIVY
	dynmultipush IviereTown_Text_Chespin, SPECIES_CHESPIN
	dynmultipush IviereTown_Text_Rowlet, SPECIES_ROWLET
	dynmultipush IviereTown_Text_Grookey, SPECIES_GROOKEY
	dynmultipush IviereTown_Text_Sprigatito, SPECIES_SPRIGATITO

IviereTown_ChooseStarter:
	dynmultistack 18, 0, FALSE, 6, FALSE, 0, DYN_MULTICHOICE_CB_NONE
	goto_if_eq VAR_RESULT, MULTI_B_PRESSED, IviereTown_ChooseType
	copyvar VAR_TEMP_1, VAR_RESULT
	bufferspeciesname STR_VAR_1, VAR_TEMP_1
	showmonpic VAR_TEMP_1, 10, 3
	msgbox IviereTown_Text_ConfirmStarter, MSGBOX_YESNO
	hidemonpic
	goto_if_eq VAR_RESULT, NO, IviereTown_ChooseType
	givemon VAR_TEMP_1, 5, ITEM_NONE
	setseenmon VAR_TEMP_1
	setcaughtmon VAR_TEMP_1
	setflag FLAG_SYS_POKEMON_GET
	setflag FLAG_SYS_POKEDEX_GET
	special EnableNationalPokedex
	playfanfare MUS_OBTAIN_ITEM
	message IviereTown_Text_ReceivedStarter
	waitfanfare
	waitmessage
	msgbox gText_NicknameThisPokemon, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, IviereTown_EventScript_FilaoInspire
	call Common_EventScript_GetGiftMonPartySlot
	call Common_EventScript_NameReceivedPartyMon

IviereTown_EventScript_FilaoInspire:
	setspeaker SP_NAME_FILAO
	msgbox IviereTown_Text_FilaoGivePokeBalls, MSGBOX_DEFAULT
	giveitem ITEM_POKE_BALL, 5
	setflag FLAG_RECEIVED_POKE_BALLS_FROM_FILAO
	setspeaker SP_NAME_FILAO
	msgbox IviereTown_Text_FilaoInspire, MSGBOX_DEFAULT
	closemessage
	setspeaker SP_NAME_PLAYER
	msgbox IviereTown_Text_PlayerResolve, MSGBOX_DEFAULT
	closemessage
	setspeaker SP_NAME_FILAO
	msgbox IviereTown_Text_FilaoFarewell, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_IVIERE_FILAO, IviereTown_Movement_FilaoLeaves
	waitmovement LOCALID_IVIERE_FILAO
	removeobject LOCALID_IVIERE_FILAO
	setflag FLAG_HIDE_IVIERE_FILAO_OUTSIDE
	setvar VAR_LITTLEROOT_INTRO_STATE, 7
	releaseall
	end

IviereTown_Text_PlayerFrustrated:
	.string "…Damn it!\p"
	.string "If only I had a POKéMON…\n"
	.string "I could have protected Mom…\l"
	.string "I could have stopped them!\p"
	.string "I was completely powerless…$"

IviereTown_Text_FilaoCallOut:
	.string "Hey! {PLAYER}!\n"
	.string "Are you alright?!$"

IviereTown_Text_FilaoAsks:
	.string "I was walking nearby and heard\n"
	.string "terrible screaming from your\l"
	.string "house!\p"
	.string "What in the world happened?!$"

IviereTown_Text_PlayerExplains:
	.string "PROF. FILAO… two masked thieves\n"
	.string "broke into our house and\l"
	.string "attacked Mom!\p"
	.string "They stole my father's POKéMON…\n"
	.string "the only keepsake we had left\l"
	.string "of him!\p"
	.string "I wanted to fight back, but…\n"
	.string "I didn't have a POKéMON to\l"
	.string "battle with…$"

IviereTown_Text_FilaoComforts:
	.string "Thieves in IVIERE TOWN?!\n"
	.string "And they took your father's\l"
	.string "POKéMON?!\p"
	.string "How despicable… Those cowards\n"
	.string "will pay for this!\p"
	.string "{PLAYER}, look at me.\p"
	.string "Do not blame yourself for what\n"
	.string "happened.\p"
	.string "You stood up to criminals\n"
	.string "without hesitation to defend\l"
	.string "your family.\p"
	.string "I see the fire and\n"
	.string "determination in your eyes.\l"
	.string "You have the true soul of a\l"
	.string "POKéMON trainer.\p"
	.string "As it happens, I was just\n"
	.string "transporting these POKéMON to\l"
	.string "my lab…\p"
	.string "I want you to choose one to be\n"
	.string "your partner on this journey!$"

IviereTown_Text_ChooseType:
	.string "Which type of POKéMON do\n"
	.string "you choose?$"

IviereTown_Text_ConfirmStarter:
	.string "Do you choose {STR_VAR_1}?$"

IviereTown_Text_ReceivedStarter:
	.string "{PLAYER} received {STR_VAR_1}\n"
	.string "from PROF. FILAO!$"

IviereTown_Text_FilaoGivePokeBalls:
	.string "Take these POKé BALLS with\n"
	.string "you as well.\p"
	.string "You will need loyal companions\n"
	.string "by your side to face the trials\l"
	.string "that lie ahead.$"

IviereTown_Text_FilaoInspire:
	.string "{PLAYER}, treat that POKéMON\n"
	.string "with love and trust, and it\l"
	.string "will stand by you through any\l"
	.string "trial.\p"
	.string "Listen to me carefully:\n"
	.string "This tragedy is not where your\l"
	.string "story ends.\l"
	.string "It is where your journey begins!\p"
	.string "Channel your sorrow and anger\n"
	.string "into strength!\p"
	.string "Challenge the Gym Leaders, grow\n"
	.string "stronger, and aim for the\l"
	.string "POKéMON LEAGUE!\p"
	.string "Become strong enough that no one\n"
	.string "can ever hurt your loved ones\l"
	.string "again.\p"
	.string "Track down those villains,\n"
	.string "defeat them, and bring your\l"
	.string "father's POKéMON back home!$"

IviereTown_Text_PlayerResolve:
	.string "Thank you, PROF. FILAO!\p"
	.string "We'll train together every\n"
	.string "single day…\p"
	.string "We will conquer the Gyms, reach\n"
	.string "the POKéMON LEAGUE… and I WILL\l"
	.string "get Dad's POKéMON back!$"

IviereTown_Text_FilaoFarewell:
	.string "That's the spirit! I have\n"
	.string "complete faith in you.\p"
	.string "Whenever your POKéMON need rest,\n"
	.string "come visit my lab on the west\l"
	.string "side of town.\p"
	.string "Now go, {PLAYER}!\n"
	.string "Your destiny begins!$"

IviereTown_Text_Fire:
	.string "Fire$"

IviereTown_Text_Water:
	.string "Water$"

IviereTown_Text_Grass:
	.string "Grass$"

IviereTown_Text_Charmander:
	.string "Charmander$"

IviereTown_Text_Cyndaquil:
	.string "Cyndaquil$"

IviereTown_Text_Torchic:
	.string "Torchic$"

IviereTown_Text_Chimchar:
	.string "Chimchar$"

IviereTown_Text_Tepig:
	.string "Tepig$"

IviereTown_Text_Fennekin:
	.string "Fennekin$"

IviereTown_Text_Litten:
	.string "Litten$"

IviereTown_Text_Scorbunny:
	.string "Scorbunny$"

IviereTown_Text_Fuecoco:
	.string "Fuecoco$"

IviereTown_Text_Squirtle:
	.string "Squirtle$"

IviereTown_Text_Totodile:
	.string "Totodile$"

IviereTown_Text_Mudkip:
	.string "Mudkip$"

IviereTown_Text_Piplup:
	.string "Piplup$"

IviereTown_Text_Oshawott:
	.string "Oshawott$"

IviereTown_Text_Froakie:
	.string "Froakie$"

IviereTown_Text_Popplio:
	.string "Popplio$"

IviereTown_Text_Sobble:
	.string "Sobble$"

IviereTown_Text_Quaxly:
	.string "Quaxly$"

IviereTown_Text_Bulbasaur:
	.string "Bulbasaur$"

IviereTown_Text_Chikorita:
	.string "Chikorita$"

IviereTown_Text_Treecko:
	.string "Treecko$"

IviereTown_Text_Turtwig:
	.string "Turtwig$"

IviereTown_Text_Snivy:
	.string "Snivy$"

IviereTown_Text_Chespin:
	.string "Chespin$"

IviereTown_Text_Rowlet:
	.string "Rowlet$"

IviereTown_Text_Grookey:
	.string "Grookey$"

IviereTown_Text_Sprigatito:
	.string "Sprigatito$"
