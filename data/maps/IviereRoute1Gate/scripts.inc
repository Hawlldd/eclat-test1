IviereRoute1Gate_MapScripts::
	.byte 0

IviereRoute1Gate_Guard::
	msgbox IviereRoute1Gate_Text_Hello, MSGBOX_NPC
	end

IviereRoute1Gate_SouthHall::
	lockall
	goto_if_set FLAG_ECLAT_ROUTE1_GATE_TEA, IviereRoute1Gate_SouthHallDone
	checkitem ITEM_TEA, 1
	goto_if_eq VAR_RESULT, TRUE, IviereRoute1Gate_AcceptTea
	msgbox IviereRoute1Gate_Text_Closed, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_PLAYER, Common_Movement_WalkUp
	waitmovement 0
	goto IviereRoute1Gate_SouthHallDone

IviereRoute1Gate_AcceptTea:
	msgbox IviereRoute1Gate_Text_Tea, MSGBOX_DEFAULT
	removeitem ITEM_TEA, 1
	setflag FLAG_ECLAT_ROUTE1_GATE_TEA
IviereRoute1Gate_SouthHallDone:
	releaseall
	end

IviereRoute1Gate_Text_Hello:
	.string "Hello.$"

IviereRoute1Gate_Text_Closed:
	.string "I'm on guard duty here.\n"
	.string "I'm dying of thirst!\p"
	.string "The way is closed.\n"
	.string "You can't pass!$"

IviereRoute1Gate_Text_Tea:
	.string "Oh, that TEA looks delicious!\p"
	.string "You'd let me have some?\n"
	.string "Great, thanks!$"
