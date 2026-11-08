Route2_RocketScene::
EclatRoute2_RocketScene::
	lockall
	faceplayer
	goto_if_set FLAG_ECLAT_ROUTE2_ROCKETS_DONE, EclatRoute2_RocketEnd
	msgbox EclatRoute2_RocketText0, MSGBOX_DEFAULT
	msgbox EclatRoute2_RocketText1, MSGBOX_DEFAULT
	msgbox EclatRoute2_RocketText2, MSGBOX_DEFAULT
	msgbox EclatRoute2_RocketText3, MSGBOX_DEFAULT
	msgbox EclatRoute2_RocketText4, MSGBOX_DEFAULT
	msgbox EclatRoute2_RocketText5, MSGBOX_DEFAULT
	closemessage
	clearflag FLAG_ECLAT_ROUTE2_MYST_HIDDEN
	setobjectxyperm LOCALID_ECLAT_ROUTE2_35, 41, 19
	addobject LOCALID_ECLAT_ROUTE2_35
	applymovement LOCALID_ECLAT_ROUTE2_35, EclatRoute2_MystArrives
	waitmovement 0
	msgbox EclatRoute2_RocketText6, MSGBOX_DEFAULT
	trainerbattle_single TRAINER_ECLAT_ROUTE2_ROCKET, EclatRoute2_RocketBattleIntro, EclatRoute2_RocketBattleDefeat, EclatRoute2_RocketVictory
	end

Route2_RocketVictory::
EclatRoute2_RocketVictory::
	lockall
	msgbox EclatRoute2_RocketText7, MSGBOX_DEFAULT
	msgbox EclatRoute2_RocketText8, MSGBOX_DEFAULT
	msgbox EclatRoute2_RocketText9, MSGBOX_DEFAULT
	msgbox EclatRoute2_RocketText10, MSGBOX_DEFAULT
	msgbox EclatRoute2_RocketText11, MSGBOX_DEFAULT
	msgbox EclatRoute2_RocketText12, MSGBOX_DEFAULT
	msgbox EclatRoute2_RocketText13, MSGBOX_DEFAULT
	msgbox EclatRoute2_RocketText14, MSGBOX_DEFAULT
	msgbox EclatRoute2_RocketText15, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_ECLAT_ROUTE2_33, EclatRoute2_GruntLeaves
	waitmovement 0
	removeobject LOCALID_ECLAT_ROUTE2_33
	applymovement LOCALID_ECLAT_ROUTE2_34, EclatRoute2_GruntLeaves
	waitmovement 0
	removeobject LOCALID_ECLAT_ROUTE2_34
	applymovement LOCALID_ECLAT_ROUTE2_35, EclatRoute2_MystLeaves
	waitmovement 0
	removeobject LOCALID_ECLAT_ROUTE2_35
	setflag FLAG_ECLAT_ROUTE2_ROCKETS_DONE
	setflag FLAG_ECLAT_ROUTE2_MYST_HIDDEN
EclatRoute2_RocketEnd:
	releaseall
	end

EclatRoute2_MystArrives:
	walk_up
	face_right
	step_end

EclatRoute2_GruntLeaves:
	walk_right
	walk_right
	walk_right
	step_end

EclatRoute2_MystLeaves:
	walk_right
	walk_right
	walk_right
	walk_right
	step_end

EclatRoute2_RocketText0:
	.string "GRUNT: Alright, we need to get busy\nand join the other squad.\lThey are waiting for us inside\lthe forest!$"

EclatRoute2_RocketText1:
	.string "GRUNT: Who are you? Go away!$"

EclatRoute2_RocketText2:
	.string "{PLAYER} : YOU, your members of\nTEAM ROCKET?!\lTell me, where are the Grunts\lWho raided my house and\lIVIERE TOWN!$"

EclatRoute2_RocketText3:
	.string "GRUNT: Who do you think you're\nimpressing with your wild dog attitude?\lGo on, get out of here\lbefore I clobber you!$"

EclatRoute2_RocketText4:
	.string "{PLAYER} : We'll see about that!$"

EclatRoute2_RocketText5:
	.string "??? : STOP!$"

EclatRoute2_RocketText6:
	.string "MYST : There you are,\nyou hothead!\lYou need to be more careful.\lYou're a novice trainer.\lRemember.\lDon't jump into a fight\lagainst two henchmen without thinking\lor you'll get beaten to a bloody\l pulp..\p...\p(sighs) All right, I can't\lchange your mind.\lI'll give you a hand, and...\las soon as we knock them out\lI'll tie them up and show you my\lstyle of interrogation, they\lshould provide us with the answers\lwe're looking for, at least if they\lknow anything.$"

EclatRoute2_RocketText7:
	.string "Grunt: Damn! We need to\nlook for reinforcements!$"

EclatRoute2_RocketText8:
	.string "GRUNT: Hey! Wait for me!$"

EclatRoute2_RocketText9:
	.string "MYST: No, you're not going anywhere!\nYou will kindly answer our\lquestions. First, what do\lyou know about a stolen POKéMON\lfrom IVIERE TOWN?$"

EclatRoute2_RocketText10:
	.string "GRUNT: Ok, ok! But I don't\nknow anything about that, thats\lnot my department..$"

EclatRoute2_RocketText11:
	.string "He doesnt look\nconvinced!\lI don't think he got enough\lof a beating! I am an avenger!\lOne POKéMON battle is not enough!\lI'll make him talk even if I have to...\lCHIDORI!(Raizen: Naruto joke get it?\pNo? I'll stop talking now)$"

EclatRoute2_RocketText12:
	.string "MYST: It's Possible, but what he's\nsaying sounds plausible. Leave\lI'll be hunting down his friends\lto question them.$"

EclatRoute2_RocketText13:
	.string "{PLAYER} :Alright, as soon as you\nget answers, come find me.$"

EclatRoute2_RocketText14:
	.string "GRUNT: Huh?! I don't know anything,\nlet me go!$"

EclatRoute2_RocketText15:
	.string "MYST : Alright, stop moaning!\nHere you go!\lWe keep in touch {PLAYER}.$"

EclatRoute2_RocketBattleIntro:
	.string "GRUNT: No way this guy's\nlooking down on us! I think after\lwe knock in your teeth,\lyou troublemakers will\lkeep quiet!$"
EclatRoute2_RocketBattleDefeat:
	.string "!$"
