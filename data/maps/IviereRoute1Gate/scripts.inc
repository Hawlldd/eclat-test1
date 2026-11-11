.set LOCALID_GUARD, 1

IviereRoute1Gate_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, IviereRoute1Gate_OnTransition
	.byte 0

IviereRoute1Gate_OnTransition:
	goto_if_unset FLAG_SYS_DEXNAV_GET, IviereRoute1Gate_EndTransition
	setvar VAR_TEMP_0, 1
IviereRoute1Gate_EndTransition:
	end

IviereRoute1Gate_Trigger_DexNav_Top::
	lockall
	goto_if_unset FLAG_TALKED_TO_MOM_AFTER_STARTER, IviereRoute1Gate_Event_StopPlayerNoMom
	goto_if_set FLAG_SYS_DEXNAV_GET, IviereRoute1Gate_DexNavAlreadyGot
	playse SE_PIN
	applymovement LOCALID_GUARD, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement LOCALID_GUARD, Common_Movement_FaceLeft
	waitmovement 0
	delay 10
	applymovement LOCALID_PLAYER, Common_Movement_FaceRight
	waitmovement 0
	setspeaker SP_NAME_GUARD
	msgbox IviereRoute1Gate_Text_GuardCallOut, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_PLAYER, IviereRoute1Gate_Movement_PlayerApproachTop
	waitmovement 0
	applymovement LOCALID_GUARD, Common_Movement_FaceDown
	waitmovement 0
	goto IviereRoute1Gate_Event_GiveDexNav

IviereRoute1Gate_Trigger_DexNav_Bottom::
	lockall
	goto_if_unset FLAG_TALKED_TO_MOM_AFTER_STARTER, IviereRoute1Gate_Event_StopPlayerNoMom
	goto_if_set FLAG_SYS_DEXNAV_GET, IviereRoute1Gate_DexNavAlreadyGot
	playse SE_PIN
	applymovement LOCALID_GUARD, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement LOCALID_GUARD, Common_Movement_FaceLeft
	waitmovement 0
	delay 10
	applymovement LOCALID_PLAYER, Common_Movement_FaceRight
	waitmovement 0
	setspeaker SP_NAME_GUARD
	msgbox IviereRoute1Gate_Text_GuardCallOut, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_PLAYER, IviereRoute1Gate_Movement_PlayerApproachBottom
	waitmovement 0
	applymovement LOCALID_GUARD, Common_Movement_FaceDown
	waitmovement 0
	goto IviereRoute1Gate_Event_GiveDexNav

IviereRoute1Gate_Event_StopPlayerNoMom:
	playse SE_PIN
	applymovement LOCALID_GUARD, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement LOCALID_GUARD, Common_Movement_FaceLeft
	waitmovement 0
	delay 10
	applymovement LOCALID_PLAYER, Common_Movement_FaceRight
	waitmovement 0
	setspeaker SP_NAME_GUARD
	msgbox IviereRoute1Gate_Text_GuardStopNoMom, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_PLAYER, Common_Movement_WalkLeft
	waitmovement 0
	applymovement LOCALID_GUARD, Common_Movement_FaceDown
	waitmovement 0
	releaseall
	end

IviereRoute1Gate_Event_GiveDexNav::
	setspeaker SP_NAME_GUARD
	msgbox IviereRoute1Gate_Text_GuardNotice, MSGBOX_DEFAULT
	playfanfare MUS_OBTAIN_ITEM
	message IviereRoute1Gate_Text_ReceivedDexNav
	delay 10
	showdexnav
	waitfanfare
	waitbuttonpress
	hideitemdescription
	setflag FLAG_SYS_DEXNAV_GET
	setvar VAR_TEMP_0, 1
	setspeaker SP_NAME_GUARD
	msgbox IviereRoute1Gate_Text_GuardExplainDexNav, MSGBOX_DEFAULT
	closemessage
	releaseall
	end

IviereRoute1Gate_DexNavAlreadyGot:
	setvar VAR_TEMP_0, 1
	releaseall
	end

IviereRoute1Gate_Movement_PlayerApproachTop:
	walk_right
	walk_right
	walk_right
	walk_in_place_faster_up
	step_end

IviereRoute1Gate_Movement_PlayerApproachBottom:
	walk_right
	walk_right
	walk_right
	walk_up
	walk_in_place_faster_up
	step_end

IviereRoute1Gate_Guard::
	lock
	faceplayer
	goto_if_unset FLAG_TALKED_TO_MOM_AFTER_STARTER, IviereRoute1Gate_Guard_NeedTalkMom
	goto_if_set FLAG_SYS_DEXNAV_GET, IviereRoute1Gate_GuardAfter
	setspeaker SP_NAME_GUARD
	msgbox IviereRoute1Gate_Text_GuardNotice, MSGBOX_DEFAULT
	playfanfare MUS_OBTAIN_ITEM
	message IviereRoute1Gate_Text_ReceivedDexNav
	delay 10
	showdexnav
	waitfanfare
	waitbuttonpress
	hideitemdescription
	setflag FLAG_SYS_DEXNAV_GET
	setvar VAR_TEMP_0, 1
	setspeaker SP_NAME_GUARD
	msgbox IviereRoute1Gate_Text_GuardExplainDexNav, MSGBOX_DEFAULT
	closemessage
	release
	end

IviereRoute1Gate_Guard_NeedTalkMom:
	setspeaker SP_NAME_GUARD
	msgbox IviereRoute1Gate_Text_GuardStopNoMom, MSGBOX_DEFAULT
	release
	end

IviereRoute1Gate_GuardAfter:
	setspeaker SP_NAME_GUARD
	msgbox IviereRoute1Gate_Text_GuardAfter, MSGBOX_NPC
	end

IviereRoute1Gate_Text_GuardStopNoMom:
	.string "Hold it right there, kid!\p"
	.string "Setting off on a journey\n"
	.string "without even speaking to\l"
	.string "your mother first?\p"
	.string "That is not something\n"
	.string "I can stand by and watch.\p"
	.string "She was terrified, and she\n"
	.string "needs to know you're safe.\p"
	.string "Go home and talk to her first!$"

IviereRoute1Gate_Text_GuardCallOut:
	.string "Wait a second, young trainer!\p"
	.string "Come over to the counter\n"
	.string "for a moment!$"

IviereRoute1Gate_Text_GuardNotice:
	.string "Hmm… Very good, kid.\p"
	.string "I see you went back home\n"
	.string "to speak with your mother.\p"
	.string "You're a good kid, {PLAYER}.\p"
	.string "Looking at you standing tall\n"
	.string "takes me right back to when\l"
	.string "your father and I battled\l"
	.string "together across this region.\p"
	.string "He was an amazing trainer\n"
	.string "and an honorable man.\p"
	.string "I heard what happened… but\n"
	.string "seeing the fire in your eyes,\l"
	.string "I know you won't rest until\l"
	.string "you recover his POKéMON.\p"
	.string "I cannot leave my post,\n"
	.string "but I won't let my friend's\l"
	.string "son go out unprepared.\p"
	.string "Here, take this.$"

IviereRoute1Gate_Text_ReceivedDexNav:
	.string "{PLAYER} received the DEXNAV!$"

IviereRoute1Gate_Text_GuardExplainDexNav:
	.string "That device is a DEXNAV!\p"
	.string "With it, you can detect\n"
	.string "wild POKéMON in the grass,\l"
	.string "track down rare moves,\l"
	.string "and check their potential\l"
	.string "right from your Start Menu!\p"
	.string "Use it to build a team as\n"
	.string "strong as your father's was.\p"
	.string "Go make him proud, kid!$"

IviereRoute1Gate_Text_GuardAfter:
	.string "How is that DEXNAV treating\n"
	.string "you, kid?\p"
	.string "Wherever your father is now,\n"
	.string "I know he watches over you\l"
	.string "with great pride.\p"
	.string "Give it everything you've got!$"
