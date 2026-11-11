EclatRoute1_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, EclatRoute1_OnTransition
	map_script MAP_SCRIPT_ON_FRAME_TABLE, EclatRoute1_OnFrame
	map_script MAP_SCRIPT_ON_RESUME, EclatRoute1_OnResume
	.byte 0

EclatRoute1_OnTransition:
	setflag FLAG_ECLAT_ROUTE1_HIDE_MEGA_NPC
	special StartEclatRoute1Flybys
	goto_if_unset FLAG_ECLAT_ROUTE1_GOT_TICKET, EclatRoute1_EndTransition
	goto_if_unset FLAG_BADGE02_GET, EclatRoute1_EndTransition
	clearflag FLAG_ECLAT_ROUTE1_HIDE_MEGA_NPC
EclatRoute1_EndTransition:
	end

EclatRoute1_OnFrame:
	map_script_2 VAR_ECLAT_ROUTE1_STATE, 0, EclatRoute1_MeetMyst
	.2byte 0

EclatRoute1_OnResume:
	special StartEclatRoute1Flybys
	end

EclatRoute1_MeetMyst:
	lockall
	applymovement LOCALID_PLAYER, EclatRoute1_Movement_NoticedMyst
	waitmovement 0
	setspeaker SP_NAME_MYSTERIOUS
	msgbox EclatRoute1_Text_MystIntro, MSGBOX_DEFAULT
	setspeaker SP_NAME_PLAYER
	msgbox EclatRoute1_Text_PlayerSuspicious, MSGBOX_DEFAULT
	setspeaker SP_NAME_MYSTERIOUS
	msgbox EclatRoute1_Text_MystReply, MSGBOX_DEFAULT
	setspeaker SP_NAME_PLAYER
	msgbox EclatRoute1_Text_PlayerQuestion, MSGBOX_DEFAULT
	setspeaker SP_NAME_MYST
	msgbox EclatRoute1_Text_MystRocket, MSGBOX_DEFAULT
	setspeaker SP_NAME_PLAYER
	msgbox EclatRoute1_Text_PlayerClefable, MSGBOX_DEFAULT
	setspeaker SP_NAME_MYST
	msgbox EclatRoute1_Text_MystLeave, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_ECLAT_ROUTE1_MYST, EclatRoute1_Movement_MystLeaves
	waitmovement 0
	removeobject LOCALID_ECLAT_ROUTE1_MYST
	setflag FLAG_ECLAT_ROUTE1_MET_MYST
	setvar VAR_ECLAT_ROUTE1_STATE, 1
	releaseall
	end

EclatRoute1_Movement_NoticedMyst:
	walk_right
	emote_exclamation_mark
	step_end

EclatRoute1_Movement_MystLeaves:
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	step_end

EclatRoute1_Sally::
	trainerbattle_single TRAINER_ECLAT_ROUTE1_SALLY, EclatRoute1_Text_SallyIntro, EclatRoute1_Text_SallyDefeat
	msgbox EclatRoute1_Text_SallyAfter, MSGBOX_AUTOCLOSE
	end

EclatRoute1_Ben::
	trainerbattle_single TRAINER_ECLAT_ROUTE1_BEN, EclatRoute1_Text_BenIntro, EclatRoute1_Text_BenDefeat
	msgbox EclatRoute1_Text_BenAfter, MSGBOX_AUTOCLOSE
	end

EclatRoute1_Fisher::
	lock
	faceplayer
	goto_if_set FLAG_ECLAT_ROUTE1_OLD_ROD, EclatRoute1_FisherAfter
	msgbox EclatRoute1_Text_Fisher, MSGBOX_DEFAULT
	giveitem ITEM_OLD_ROD
	goto_if_eq VAR_RESULT, FALSE, EclatRoute1_FisherDone
	setflag FLAG_ECLAT_ROUTE1_OLD_ROD
EclatRoute1_FisherDone:
	release
	end

EclatRoute1_FisherAfter:
	msgbox EclatRoute1_Text_FisherAfter, MSGBOX_DEFAULT
	release
	end

	.macro eclat_route1_rattata flag:req
	call EclatRoute1_RattataBattle
	setflag \flag
	goto EclatRoute1_RattataRemove
	.endm

EclatRoute1_Rattata1::
	eclat_route1_rattata FLAG_TEMP_5
EclatRoute1_Rattata2::
	eclat_route1_rattata FLAG_TEMP_6
EclatRoute1_Rattata3::
	eclat_route1_rattata FLAG_TEMP_7
EclatRoute1_Rattata4::
	eclat_route1_rattata FLAG_TEMP_8
EclatRoute1_Rattata5::
	eclat_route1_rattata FLAG_TEMP_9
EclatRoute1_Rattata6::
	eclat_route1_rattata FLAG_TEMP_A
	.purgem eclat_route1_rattata

EclatRoute1_RattataBattle:
	lock
	faceplayer
	playmoncry SPECIES_RATTATA, CRY_MODE_ENCOUNTER
	waitmoncry
	setwildbattle SPECIES_RATTATA, 2
	dowildbattle
	return

EclatRoute1_RattataRemove:
	removeobject VAR_LAST_TALKED
	release
	end

EclatRoute1_LotteryNpc::
	lock
	faceplayer
	applymovement VAR_LAST_TALKED, Common_Movement_ExclamationMark
	waitmovement 0
	msgbox EclatRoute1_Text_LotteryFound, MSGBOX_DEFAULT
	giveitem ITEM_ECLAT_LOTTERY_TICKET
	goto_if_eq VAR_RESULT, FALSE, EclatRoute1_LotteryDone
	setflag FLAG_ECLAT_ROUTE1_GOT_TICKET
	msgbox EclatRoute1_Text_LotteryUse, MSGBOX_DEFAULT
	closemessage
	removeobject VAR_LAST_TALKED
EclatRoute1_LotteryDone:
	release
	end

EclatRoute1_Text_LotteryFound:
	.string "Yo-oh!\nWell done you found me!\lTake this!$"

EclatRoute1_Text_LotteryUse:
	.string "With this TICKET, you can try\n"
	.string "your luck in the lottery\l"
	.string "in PERCHARBRE.\p"
	.string "Alright, with that, I'm going to\n"
	.string "hide somewhere else!$"

EclatRoute1_LeaveAlone::
	msgbox EclatRoute1_Text_LeaveAlone, MSGBOX_NPC
	end

EclatRoute1_TM42Ball::
	finditem ITEM_TM42
	end

EclatRoute1_PidgeotiteBall::
	finditem ITEM_PIDGEOTITE
	end

EclatRoute1_RodFan::
	msgbox EclatRoute1_Text_RodFan, MSGBOX_NPC
	end

EclatRoute1_Beginner::
	msgbox EclatRoute1_Text_Beginner, MSGBOX_NPC
	end

EclatRoute1_TeaNpc::
	msgbox EclatRoute1_Text_TeaNpc, MSGBOX_NPC
	end

EclatRoute1_RouteSign::
	msgbox EclatRoute1_Text_RouteSign, MSGBOX_SIGN
	end

EclatRoute1_MightyenaSign::
	msgbox EclatRoute1_Text_MightyenaSign, MSGBOX_SIGN
	end

EclatRoute1_Text_TeaNpc:
	.string "Hi. I saw a strange POKéMON nearby.\p"
	.string "It was green and had something like\n"
	.string "a musical note on its head.\p"
	.string "I'd like to lure it here, but I\n"
	.string "don't know how.$"

EclatRoute1_Text_MystIntro:
	.string "Huh? What in the world\n"
	.string "is a kid doing out here\l"
	.string "in the wild all alone?\p"
	.string "This isn't a playground.\n"
	.string "Wild MIGHTYENA and dangerous\l"
	.string "beasts prowl in the grass.\p"
	.string "Wait… you just stepped out\n"
	.string "of that gate, didn't you?\p"
	.string "You wouldn't happen to have\n"
	.string "seen two suspicious guys\l"
	.string "dressed in black, would you?\p"
	.string "I was hot on their trail,\n"
	.string "but I lost them in the brush.$"

EclatRoute1_Text_PlayerSuspicious:
	.string "First of all: who are you?\p"
	.string "My house was just raided\n"
	.string "by thugs like that, and now\l"
	.string "you appear asking questions?\p"
	.string "It seems a little convenient.\n"
	.string "How do I know you're not\l"
	.string "one of them?$"

EclatRoute1_Text_MystReply:
	.string "Haha! Easy there, kid.\p"
	.string "You're sharp and cautious--\n"
	.string "I respect that.\p"
	.string "Take a look at me, though:\n"
	.string "I'm not wearing that uniform.\p"
	.string "If I were an accomplice, why\n"
	.string "would I ask a stranger\l"
	.string "which way my partners fled?$"

EclatRoute1_Text_PlayerQuestion:
	.string "Fair point… I believe you.\n"
	.string "Why track them down, then?$"

EclatRoute1_Text_MystRocket:
	.string "Where are my manners?\n"
	.string "Allow me to introduce myself.\p"
	.string "My name is MYST.\n"
	.string "I am an operative with the\l"
	.string "G-Men--the same elite unit\l"
	.string "as LANCE, the Dragon Master.\p"
	.string "Those two men are grunts\n"
	.string "from a criminal syndicate\l"
	.string "known as TEAM ROCKET.\p"
	.string "Lately, they have been causing\n"
	.string "trouble across this region,\l"
	.string "stealing POKéMON from trainers.\p"
	.string "You said your home was raided…\n"
	.string "What did they do?$"

EclatRoute1_Text_PlayerClefable:
	.string "They broke in and took the\n"
	.string "only keepsake of my father…\l"
	.string "his beloved CLEFABLE.\p"
	.string "I just got my first partner,\n"
	.string "and I swore to my mother:\p"
	.string "I will cross the whole region\n"
	.string "to get his CLEFABLE back,\l"
	.string "no matter the cost!$"

EclatRoute1_Text_MystLeave:
	.string "Your late father's POKéMON…\n"
	.string "Damn it. Those scoundrels have\l"
	.string "no shame.\p"
	.string "Listen to me, {PLAYER}…\n"
	.string "I won't sugarcoat it: TEAM\l"
	.string "ROCKET is dangerous, and saving\l"
	.string "a stolen POKéMON isn't easy.\p"
	.string "Yet, looking in your eyes,\n"
	.string "I see you won't back down.\p"
	.string "I'll keep pursuing their trail\n"
	.string "ahead. If I find their hideout,\l"
	.string "I'll do whatever I can to help.\p"
	.string "Train hard and stay alert.\n"
	.string "I'll keep an eye on you, kid!$"

EclatRoute1_Text_RodFan:
	.string "There's a fisherman over there,\ngiving out free RODS.\lYou should go talk to him.$"

EclatRoute1_Text_Beginner:
	.string "There aren't strong POKéMON\naround here. It's the ideal\lplace to mark your debut!$"

EclatRoute1_Text_LeaveAlone:
	.string "This wild POKéMON seems to be\nignoring you. It's best to leave\lit alone.$"

EclatRoute1_Text_SallyIntro:
	.string "Hey! Are you going to PETIPRES?\nNot without having a\lbattle against me!$"

EclatRoute1_Text_SallyDefeat:
	.string "You're not nice!$"

EclatRoute1_Text_SallyAfter:
	.string "This is the only road that connects\nIVIERE TOWN and PETIPRES.$"

EclatRoute1_Text_BenIntro:
	.string "I'm sick of these shorts, the\ntall grass is always giving me\lhives, I'm going to let off\lsome steam on you!$"

EclatRoute1_Text_BenDefeat:
	.string "Anyway, I still prefer\nshorts.$"

EclatRoute1_Text_BenAfter:
	.string "It's so dumb that I have\nto wear shorts even in winter,\lIt's always freezing!\lI don't know why, but I can't\lremove them.$"

EclatRoute1_Text_Fisher:
	.string "Splosh splosh...\pSplosh splosh...\pThis is the sound of the attack\nSPLASH! Here, with this\lROD, you can fish for\llittle fish!$"

EclatRoute1_Text_FisherAfter:
	.string "Splosh splosh...\pSo, did you get any fish?$"

EclatRoute1_Text_RouteSign:
	.string "ROUTE 1\nIVIERE TOWN - PETIPRES$"

EclatRoute1_Text_MightyenaSign:
	.string "Beware of MIGHTYENA\nin the tall grass!$"

EclatRoute1_MegaNpc::
	lock
	faceplayer
	checkitem ITEM_MEGA_RING, 1
	goto_if_eq VAR_RESULT, TRUE, EclatRoute1_MegaNpcHeracronite
	msgbox EclatRoute1_Text_MegaIntro, MSGBOX_DEFAULT
	giveitem ITEM_MEGA_RING
	goto_if_eq VAR_RESULT, FALSE, EclatRoute1_MegaNpcDone

EclatRoute1_MegaNpcHeracronite:
	goto_if_set FLAG_ECLAT_ROUTE1_GOT_HERACRONITE, EclatRoute1_MegaNpcSalamencite
	giveitem ITEM_HERACRONITE
	goto_if_eq VAR_RESULT, FALSE, EclatRoute1_MegaNpcDone
	setflag FLAG_ECLAT_ROUTE1_GOT_HERACRONITE

EclatRoute1_MegaNpcSalamencite:
	goto_if_set FLAG_ECLAT_ROUTE1_GOT_SALAMENCITE, EclatRoute1_MegaNpcHoundoominite
	giveitem ITEM_SALAMENCITE
	goto_if_eq VAR_RESULT, FALSE, EclatRoute1_MegaNpcDone
	setflag FLAG_ECLAT_ROUTE1_GOT_SALAMENCITE

EclatRoute1_MegaNpcHoundoominite:
	goto_if_set FLAG_ECLAT_ROUTE1_GOT_HOUNDOOMINITE, EclatRoute1_MegaNpcAfter
	giveitem ITEM_HOUNDOOMINITE
	goto_if_eq VAR_RESULT, FALSE, EclatRoute1_MegaNpcDone
	setflag FLAG_ECLAT_ROUTE1_GOT_HOUNDOOMINITE
	goto EclatRoute1_MegaNpcDone

EclatRoute1_MegaNpcAfter:
	msgbox EclatRoute1_Text_MegaAfter, MSGBOX_DEFAULT
EclatRoute1_MegaNpcDone:
	release
	end

EclatRoute1_Text_MegaIntro:
	.string "Hello, trainer. Are you familiar\n"
	.string "with mega-evolution?\l"
	.string "You have badges, so you must\l"
	.string "be a good trainer.\l"
	.string "Take this and see for yourself.\l"
	.string "And in return, work hard\l"
	.string "on your POKéDEX, it'll help\l"
	.string "PROF. OAK.$"

EclatRoute1_Text_MegaAfter:
	.string "So were you able to observe\n"
	.string "interesting mega-evolutions?$"
