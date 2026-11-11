Petipres_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Petipres_OnTransition
	.byte 0

Petipres_OnTransition:
	goto_if_unset FLAG_HIDE_PETIPRES_STRANGER, Petipres_Ready
	setvar VAR_PETIPRES_STRANGER, 1
Petipres_Ready:
	end

Petipres_Potion::
	finditem ITEM_POTION
	end

Petipres_Miltank::
	lock
	faceplayer
	playmoncry SPECIES_MILTANK, CRY_MODE_NORMAL
	msgbox Petipres_Miltank_Text, MSGBOX_DEFAULT
	waitmoncry
	release
	end

Petipres_Pidgey::
	lock
	faceplayer
	playmoncry SPECIES_PIDGEY, CRY_MODE_NORMAL
	msgbox Petipres_Pidgey_Text, MSGBOX_DEFAULT
	waitmoncry
	release
	end

Petipres_Radar::
	lock
	faceplayer
	goto_if_set FLAG_GOT_PETIPRES_RADAR, Petipres_RadarDone
	msgbox Petipres_Radar_Text, MSGBOX_DEFAULT
	giveitem ITEM_POKE_RADAR
	goto_if_eq VAR_RESULT, FALSE, Petipres_Release
	setflag FLAG_GOT_PETIPRES_RADAR
Petipres_RadarDone:
	msgbox Petipres_RadarAfter_Text, MSGBOX_DEFAULT
Petipres_Release:
	release
	end

Petipres_TreasureClue::
	lockall
	msgbox Petipres_Clue_Text, MSGBOX_DEFAULT
	goto_if_set FLAG_PETIPRES_TREASURE_CLUE, Petipres_ClueDone
	setflag FLAG_PETIPRES_TREASURE_CLUE
	playfanfare MUS_OBTAIN_ITEM
	msgbox Petipres_ClueUnlocked_Text, MSGBOX_DEFAULT
	waitfanfare
Petipres_ClueDone:
	releaseall
	end

Petipres_Stranger::
	goto Petipres_StrangerScene

Petipres_StrangerScene::
	lockall
	setvar VAR_PETIPRES_STRANGER, 1
	applymovement OBJ_EVENT_ID_PLAYER, Petipres_LookRight
	waitmovement 0
	setspeaker SP_NAME_PLAYER
	msgbox Petipres_ExcuseMe_Text, MSGBOX_DEFAULT
	closemessage
	special SpawnCameraObject
	applymovement LOCALID_CAMERA, Petipres_CameraRight
	waitmovement 0
	playse SE_PIN
	applymovement LOCALID_PETIPRES_23, Petipres_StrangerRun
	waitmovement 0
	removeobject LOCALID_PETIPRES_23
	setflag FLAG_HIDE_PETIPRES_STRANGER
	applymovement LOCALID_CAMERA, Petipres_CameraLeft
	waitmovement 0
	special RemoveCameraObject
	setspeaker SP_NAME_PLAYER
	msgbox Petipres_Wait_Text, MSGBOX_DEFAULT
	msgbox Petipres_LetterFound_Text, MSGBOX_DEFAULT
	setspeaker SP_NAME_NONE
	msgbox Petipres_Letter_Text, MSGBOX_SIGN
	setspeaker SP_NAME_PLAYER
	msgbox Petipres_LetterReaction_Text, MSGBOX_DEFAULT
	setspeaker SP_NAME_NONE
	releaseall
	end

Petipres_LookRight:
	face_right
	step_end

Petipres_CameraRight:
	walk_slow_up
	walk_slow_right
	walk_slow_right
	walk_slow_right
	step_end

Petipres_CameraLeft:
	walk_slow_down
	walk_slow_left
	walk_slow_left
	walk_slow_left
	step_end

Petipres_StrangerRun:
	emote_exclamation_mark
	delay_16
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	step_end

Petipres_Boy::
	msgbox Petipres_Boy_Text, MSGBOX_NPC
	end

Petipres_Boy_Text:
	.string "You should buy some\n"
	.string "MOO-MOO MILK!\l"
	.string "It's super profitable!$"

Petipres_GymBoy::
	msgbox Petipres_GymBoy_Text, MSGBOX_NPC
	end

Petipres_GymBoy_Text:
	.string "Is that a POKé BALL I see!\n"
	.string "So you also have POKéMON?\p"
	.string "It's cool to be able to carry\n"
	.string "them wherever you want, huh?$"

Petipres_Visitor::
	msgbox Petipres_Visitor_Text, MSGBOX_NPC
	end

Petipres_Visitor_Text:
	.string "I would go to PERCHARBRE, but I\n"
	.string "am too old to cross the\l"
	.string "PEACEFUL WOODS.$"

Petipres_TrainerPc::
	msgbox Petipres_TrainerPc_Text, MSGBOX_NPC
	end

Petipres_TrainerPc_Text:
	.string "I just got back from the store,\n"
	.string "they have amazing items!$"

Petipres_RocketGrunt::
	msgbox Petipres_RocketGrunt_Text, MSGBOX_NPC
	end

Petipres_RocketGrunt_Text:
	.string "Take a look at the POKéMON\n"
	.string "CENTER before heading out to\l"
	.string "ROUTE 2.$"

Petipres_TownSign::
	msgbox Petipres_TownSign_Text, MSGBOX_SIGN
	end

Petipres_TownSign_Text:
	.string "PETIPRES\n"
	.string "A peaceful town with rustic\l"
	.string "charm.$"

Petipres_Miltank_Text:
	.string "Miltank: Meuuuuh!$"

Petipres_Pidgey_Text:
	.string "Pidgey: Rrrrouuu rrrrouuu!$"

Petipres_Radar_Text:
	.string "Hey there! Wondering what I'm\n"
	.string "doing? Well, I'm hunting for\l"
	.string "Shiny POKéMON!\p"
	.string "They are extremely rare POKéMON\n"
	.string "with unique coloration.\p"
	.string "Here, take this Poké Radar!\n"
	.string "At my level, I don't really\l"
	.string "need it anymore.$"

Petipres_RadarAfter_Text:
	.string "Good luck with your hunts!$"

Petipres_Clue_Text:
	.string "There is something engraved\n"
	.string "here:\l"
	.string "Second Clue: The next clue is\l"
	.string "hidden at the base of a small\l"
	.string "tree west of the daycare.$"

Petipres_ClueUnlocked_Text:
	.string "You have unlocked the next clue.$"

Petipres_ExcuseMe_Text:
	.string "Excuse me, sir!\p"
	.string "Did you happen to see two men\n"
	.string "dressed in black around here?$"

Petipres_Wait_Text:
	.string "Hey, wait!\p"
	.string "…Why in the world did he\n"
	.string "run away like that?$"

Petipres_LetterFound_Text:
	.string "Wait… it looks like he dropped\n"
	.string "a folded piece of paper.$"

Petipres_Letter_Text:
	.string "“I know what you are trying\n"
	.string "to do.\p"
	.string "Give up. They are far too\n"
	.string "strong for you.\p"
	.string "I assure you, it is for your\n"
	.string "own good.”$"

Petipres_LetterReaction_Text:
	.string "…\p"
	.string "That look in his eyes…\n"
	.string "It wasn't anger.\l"
	.string "It was pure terror.\p"
	.string "He must have been a victim,\n"
	.string "too… someone who stood up to\l"
	.string "them and had his spirit broken.\p"
	.string "I understand your pain, sir…\n"
	.string "and I thank you for your\l"
	.string "warning.\p"
	.string "You let fear take over, and I\n"
	.string "cannot blame you for that.\p"
	.string "…But I will not end up like\n"
	.string "you.\p"
	.string "I made a sacred vow to Mom and\n"
	.string "to my father's memory.\p"
	.string "My partner and I will train\n"
	.string "and grow stronger with every\l"
	.string "step.\p"
	.string "I WILL defeat TEAM ROCKET and\n"
	.string "bring Dad's CLEFABLE back home!$"
