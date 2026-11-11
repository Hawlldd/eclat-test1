Route2_RocketScene::
EclatRoute2_RocketScene::
	lockall
	setflag FLAG_SAFE_FOLLOWER_MOVEMENT
	goto_if_set FLAG_ECLAT_ROUTE2_ROCKETS_DONE, EclatRoute2_RocketEnd
	goto_if_defeated TRAINER_ECLAT_ROUTE2_ROCKET, EclatRoute2_RocketEnd
	applymovement LOCALID_ECLAT_ROUTE2_33, Common_Movement_FaceLeft
	applymovement LOCALID_ECLAT_ROUTE2_34, Common_Movement_FaceLeft
	waitmovement LOCALID_ECLAT_ROUTE2_33
	setspeaker SP_NAME_ROCKET
	msgbox EclatRoute2_RocketText0, MSGBOX_DEFAULT
	msgbox EclatRoute2_RocketText1, MSGBOX_DEFAULT
	setspeaker SP_NAME_PLAYER
	msgbox EclatRoute2_RocketText2, MSGBOX_DEFAULT
	setspeaker SP_NAME_ROCKET
	msgbox EclatRoute2_RocketText3, MSGBOX_DEFAULT
	setspeaker SP_NAME_PLAYER
	msgbox EclatRoute2_RocketText4, MSGBOX_DEFAULT
	setspeaker SP_NAME_ROCKET
	msgbox EclatRoute2_RocketText5, MSGBOX_DEFAULT
	closemessage
	playse SE_PIN
	applymovement LOCALID_ECLAT_ROUTE2_33, Common_Movement_ExclamationMark
	applymovement LOCALID_ECLAT_ROUTE2_34, Common_Movement_ExclamationMark
	waitmovement LOCALID_ECLAT_ROUTE2_33
	delay 20
	setspeaker SP_NAME_MYSTERIOUS
	msgbox EclatRoute2_RocketText6, MSGBOX_DEFAULT
	closemessage
	getplayerxy VAR_0x8004, VAR_0x8005
	goto_if_eq VAR_0x8005, 18, EclatRoute2_PlayerAtRow18

	@ Player is at row 19 (facing lower grunt). Myst takes row 18 (facing upper grunt).
	clearflag FLAG_ECLAT_ROUTE2_MYST_HIDDEN
	setobjectxyperm LOCALID_ECLAT_ROUTE2_35, 38, 18
	addobject LOCALID_ECLAT_ROUTE2_35
	setobjectxy LOCALID_ECLAT_ROUTE2_35, 38, 18
	applymovement LOCALID_ECLAT_ROUTE2_35, EclatRoute2_MystStepsIn
	waitmovement LOCALID_ECLAT_ROUTE2_35
	setobjectxyperm LOCALID_ECLAT_ROUTE2_35, 42, 18
	goto EclatRoute2_MystConfronts

EclatRoute2_PlayerAtRow18:
	@ Player is at row 18 (facing upper grunt). Myst takes row 19 (facing lower grunt).
	clearflag FLAG_ECLAT_ROUTE2_MYST_HIDDEN
	setobjectxyperm LOCALID_ECLAT_ROUTE2_35, 38, 19
	addobject LOCALID_ECLAT_ROUTE2_35
	setobjectxy LOCALID_ECLAT_ROUTE2_35, 38, 19
	applymovement LOCALID_ECLAT_ROUTE2_35, EclatRoute2_MystStepsIn
	waitmovement LOCALID_ECLAT_ROUTE2_35
	setobjectxyperm LOCALID_ECLAT_ROUTE2_35, 42, 19
	goto EclatRoute2_MystConfronts

EclatRoute2_MystConfronts:
	applymovement LOCALID_ECLAT_ROUTE2_33, Common_Movement_FaceLeft
	applymovement LOCALID_ECLAT_ROUTE2_34, Common_Movement_FaceLeft
	waitmovement LOCALID_ECLAT_ROUTE2_33
	setspeaker SP_NAME_MYST
	msgbox EclatRoute2_RocketText7, MSGBOX_DEFAULT
	setspeaker SP_NAME_NONE
	closemessage
	trainerbattle_no_intro TRAINER_ECLAT_ROUTE2_ROCKET, EclatRoute2_RocketBattleDefeat
	goto EclatRoute2_RocketVictory

Route2_RocketVictory::
EclatRoute2_RocketVictory::
	lockall
	setflag FLAG_SAFE_FOLLOWER_MOVEMENT
	applymovement LOCALID_ECLAT_ROUTE2_35, Common_Movement_FaceRight
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_FaceRight
	applymovement LOCALID_ECLAT_ROUTE2_33, Common_Movement_FaceLeft
	applymovement LOCALID_ECLAT_ROUTE2_34, Common_Movement_FaceLeft
	waitmovement LOCALID_ECLAT_ROUTE2_33
	setspeaker SP_NAME_ROCKET
	msgbox EclatRoute2_RocketVictoryText1, MSGBOX_DEFAULT
	msgbox EclatRoute2_RocketVictoryText2, MSGBOX_DEFAULT
	setspeaker SP_NAME_MYST
	msgbox EclatRoute2_RocketVictoryText3, MSGBOX_DEFAULT
	setspeaker SP_NAME_ROCKET
	msgbox EclatRoute2_RocketVictoryText4, MSGBOX_DEFAULT
	setspeaker SP_NAME_PLAYER
	msgbox EclatRoute2_RocketVictoryText5, MSGBOX_DEFAULT
	setspeaker SP_NAME_ROCKET
	msgbox EclatRoute2_RocketVictoryText6, MSGBOX_DEFAULT
	setspeaker SP_NAME_MYST
	msgbox EclatRoute2_RocketVictoryText7, MSGBOX_DEFAULT
	setspeaker SP_NAME_PLAYER
	msgbox EclatRoute2_RocketVictoryText8, MSGBOX_DEFAULT
	setspeaker SP_NAME_MYST
	msgbox EclatRoute2_RocketVictoryText9, MSGBOX_DEFAULT
	setspeaker SP_NAME_NONE
	closemessage
	applymovement LOCALID_ECLAT_ROUTE2_33, EclatRoute2_GruntLeaves
	applymovement LOCALID_ECLAT_ROUTE2_34, EclatRoute2_GruntLeaves
	waitmovement LOCALID_ECLAT_ROUTE2_33
	removeobject LOCALID_ECLAT_ROUTE2_33
	removeobject LOCALID_ECLAT_ROUTE2_34
	applymovement LOCALID_ECLAT_ROUTE2_35, EclatRoute2_MystLeaves
	waitmovement LOCALID_ECLAT_ROUTE2_35
	removeobject LOCALID_ECLAT_ROUTE2_35
	setflag FLAG_ECLAT_ROUTE2_ROCKETS_DONE
	setflag FLAG_ECLAT_ROUTE2_MYST_HIDDEN
	settrainerflag TRAINER_ECLAT_ROUTE2_ROCKET
EclatRoute2_RocketEnd:
	clearflag FLAG_SAFE_FOLLOWER_MOVEMENT
	releaseall
	end

EclatRoute2_MystStepsIn:
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	step_end

EclatRoute2_GruntLeaves:
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	step_end

EclatRoute2_MystLeaves:
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	step_end

EclatRoute2_RocketText0:
	.string "Alright, we need to wrap this up\n"
	.string "and join the main squad.\p"
	.string "They're waiting for us inside\n"
	.string "PEACEFUL WOODS!$"

EclatRoute2_RocketText1:
	.string "Hm? Who are you supposed to be?\n"
	.string "Scram, kid! We're busy!$"

EclatRoute2_RocketText2:
	.string "Hey, that 'R' on your chests...\n"
	.string "You're with TEAM ROCKET?!\p"
	.string "Tell me right now: where are the\n"
	.string "scumbags who raided IVIERE TOWN\l"
	.string "and broke into my house?!$"

EclatRoute2_RocketText3:
	.string "Bahaha! What's this, a little\n"
	.string "brat playing detective?\p"
	.string "Get lost, kid. We don't have\n"
	.string "time to babysit toddlers.\p"
	.string "Turn around and run home to\n"
	.string "mommy before you get hurt.$"

EclatRoute2_RocketText4:
	.string "I'm not going anywhere!\p"
	.string "If you won't talk politely,\n"
	.string "then we'll settle this in a\l"
	.string "POKéMON BATTLE!\p"
	.string "I'll make you answer for what\n"
	.string "you did to my family!$"

EclatRoute2_RocketText5:
	.string "What did you just say?!\p"
	.string "A pint-sized pip-squeak like you\n"
	.string "thinks he can look down on us\l"
	.string "and boss TEAM ROCKET around?!\p"
	.string "You just crossed the line!\n"
	.string "We're gonna knock your teeth in\l"
	.string "so you learn your place!$"

EclatRoute2_RocketText6:
	.string "HOLD IT RIGHT THERE!$"

EclatRoute2_RocketText7:
	.string "There you are, you hothead!\p"
	.string "I told you to be careful! You're\n"
	.string "still a novice trainer.\p"
	.string "Jumping into a fight against two\n"
	.string "grown criminals without thinking\l"
	.string "is a recipe for disaster!\p"
	.string "...\p"
	.string "(sighs) ...Though I see there's\n"
	.string "no talking you out of it.\p"
	.string "Fine! Since you took this fight,\n"
	.string "I'll cover your back.\p"
	.string "Take him down in battle!\p"
	.string "Once you knock him out, I'll tie\n"
	.string "them up and interrogate them to\l"
	.string "get the answers we need!$"

EclatRoute2_RocketBattleIntro:
	.string "Don't blink, you little brat!\n"
	.string "TEAM ROCKET is about to show\l"
	.string "you how the real world works!$"

EclatRoute2_RocketBattleDefeat:
	.string "What the--?! Beaten by a kid?!$"

EclatRoute2_RocketVictoryText1:
	.string "Damn it! How could a rookie kid\n"
	.string "take down my team?!\p"
	.string "We need reinforcements, now!$"

EclatRoute2_RocketVictoryText2:
	.string "Hey! Don't leave me behind!$"

EclatRoute2_RocketVictoryText3:
	.string "Hold on. You're not going\n"
	.string "anywhere!\p"
	.string "Now, you're going to answer our\n"
	.string "questions very clearly.\p"
	.string "What do you know about the\n"
	.string "POKéMON stolen in IVIERE TOWN?$"

EclatRoute2_RocketVictoryText4:
	.string "A-all right, all right! I swear\n"
	.string "I know nothing about that!\p"
	.string "That raid was handled by an\n"
	.string "entirely different unit!\p"
	.string "We're just frontline sentries\n"
	.string "posted here on the road!$"

EclatRoute2_RocketVictoryText5:
	.string "Don't play dumb with us!\n"
	.string "Where did your unit go?!$"

EclatRoute2_RocketVictoryText6:
	.string "I'm telling the truth, I swear!\p"
	.string "The main squad is gathering\n"
	.string "deep inside PEACEFUL WOODS!\p"
	.string "That's all I know, please let\n"
	.string "me go!$"

EclatRoute2_RocketVictoryText7:
	.string "Sounds like he's actually\n"
	.string "telling the truth this time.\p"
	.string "Leave these two to me, {PLAYER}.\p"
	.string "I'll secure them and squeeze out\n"
	.string "every last drop of information.$"

EclatRoute2_RocketVictoryText8:
	.string "Understood. As soon as you find\n"
	.string "out anything else, let me know.$"

EclatRoute2_RocketVictoryText9:
	.string "Count on it.\p"
	.string "Head into PEACEFUL WOODS, but\n"
	.string "keep your guard up.\p"
	.string "Those thugs in the woods won't\n"
	.string "be playing games.\p"
	.string "Stay safe, {PLAYER}!$"
