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
	waitmovement OBJ_EVENT_ID_PLAYER
	msgbox Petipres_ExcuseMe_Text, MSGBOX_DEFAULT
	closemessage
	special SpawnCameraObject
	applymovement LOCALID_CAMERA, Petipres_CameraRight
	waitmovement LOCALID_CAMERA
	playse SE_PIN
	applymovement LOCALID_PETIPRES_23, Petipres_StrangerRun
	waitmovement LOCALID_PETIPRES_23
	removeobject LOCALID_PETIPRES_23
	setflag FLAG_HIDE_PETIPRES_STRANGER
	applymovement LOCALID_CAMERA, Petipres_CameraLeft
	waitmovement LOCALID_CAMERA
	special RemoveCameraObject
	msgbox Petipres_Wait_Text, MSGBOX_DEFAULT
	msgbox Petipres_LetterFound_Text, MSGBOX_DEFAULT
	msgbox Petipres_Letter_Text, MSGBOX_SIGN
	msgbox Petipres_LetterReaction_Text, MSGBOX_DEFAULT
	releaseall
	end

Petipres_LookRight:
	face_right
	step_end

Petipres_CameraRight:
	walk_slow_right
	walk_slow_right
	walk_slow_right
	step_end

Petipres_CameraLeft:
	walk_slow_left
	walk_slow_left
	walk_slow_left
	step_end

Petipres_StrangerRun:
	emote_exclamation_mark
	delay_8
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	step_end

Petipres_Boy::
	msgbox Petipres_Boy_Text, MSGBOX_NPC
	end

Petipres_Boy_Text:
	.string "You should buy lots of MOO-MOO MILK,\nit's super profitable!$"

Petipres_GymBoy::
	msgbox Petipres_GymBoy_Text, MSGBOX_NPC
	end

Petipres_GymBoy_Text:
	.string "Is that a POKé BALL I see!\nSo you also have POKéMON?\pIt's cool to be able to carry\nthem wherever you want, huh?$"

Petipres_Visitor::
	msgbox Petipres_Visitor_Text, MSGBOX_NPC
	end

Petipres_Visitor_Text:
	.string "I would go to PERCHARBRE, but I\nam too old to cross the\lPEACEFUL WOODS.$"

Petipres_TrainerPc::
	msgbox Petipres_TrainerPc_Text, MSGBOX_NPC
	end

Petipres_TrainerPc_Text:
	.string "I just got back from the store,\nthey have amazing items!$"

Petipres_RocketGrunt::
	msgbox Petipres_RocketGrunt_Text, MSGBOX_NPC
	end

Petipres_RocketGrunt_Text:
	.string "You should take a look at the CENTER\nPOKéMON before venturing to\lroute 2.$"

Petipres_TownSign::
	msgbox Petipres_TownSign_Text, MSGBOX_SIGN
	end

Petipres_TownSign_Text:
	.string "PETIPRES\nA peaceful town with rustic\lcharm.$"

Petipres_Miltank_Text:
	.string "Miltank: Meuuuuh!$"

Petipres_Pidgey_Text:
	.string "Pidgey : Rrrrouuu rrrrouuu !$"

Petipres_Radar_Text:
	.string "Salut ! Tu te demandes peut-\nêtre ce que je fais ? Hé bien, je\lchasse les POKéMON shineys ! Ce\lsont des POKéMON extrêmement\lrares reconnaissables grâce à\lleur couleur particulière. Essaie\ld'en trouver, tu verras.\pJe te laisse ce radard, à mon\nniveau, je n'en ai plus besoin.$"

Petipres_RadarAfter_Text:
	.string "Bonne chasse !$"

Petipres_Clue_Text:
	.string "Il y a quelque chose de gravé\nici.\lDeuxième indice : Vous trouverez\lle prochain incice au pied d'un\lpetit arbre à l'ouest du\lpensionnat se trouvant au milieu\lde la vase.$"

Petipres_ClueUnlocked_Text:
	.string "You have unlocked the next clue.$"

Petipres_ExcuseMe_Text:
	.string "{PLAYER} : Excuse me, sir!$"

Petipres_Wait_Text:
	.string "{PLAYER} : Hey, wait!$"

Petipres_LetterFound_Text:
	.string "{PLAYER} : Damn, why did that\nguy run away?… Wait, It\llooks like he left a letter.$"

Petipres_Letter_Text:
	.string "I know what you're trying\nto do, give up, they are too\lstrong for you. I can assure you\lit's for your good.$"

Petipres_LetterReaction_Text:
	.string "{PLAYER} : If this guy thinks\nI'll follow directions\lscribbled on some old paper…\lNevertheless, he wasnt\lwrong on one point, I must be\lvigilant, these TEAM\lROCKET guys look tough.$"
