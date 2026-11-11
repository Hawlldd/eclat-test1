PeacefulWoods_MapScripts::
	.byte 0

PeacefulWoods_Trainer3::
	trainerbattle_single TRAINER_PEACEFUL_WOODS_OMAR, PeacefulWoods_Trainer3_Intro, PeacefulWoods_Trainer3_Defeat
	msgbox PeacefulWoods_Trainer3_After, MSGBOX_AUTOCLOSE
	end

PeacefulWoods_Trainer3_Intro:
	.string "Hey! You have POKéMON!\nYah! Let's battle!$"

PeacefulWoods_Trainer3_Defeat:
	.string "No! I thought my insects\nwould tear your eyes out!$"

PeacefulWoods_Trainer3_After:
	.string "Hush!\nYour scaring the bugs…$"

PeacefulWoods_Trainer4::
	trainerbattle_single TRAINER_PEACEFUL_WOODS_ALFRED, PeacefulWoods_Trainer4_Intro, PeacefulWoods_Trainer4_Defeat
	msgbox PeacefulWoods_Trainer4_After, MSGBOX_AUTOCLOSE
	end

PeacefulWoods_Trainer4_Intro:
	.string "Haha!\nA POKéMON TRAINER,\la real one wll never back down!!!$"

PeacefulWoods_Trainer4_Defeat:
	.string "Oops!\nI'm out of POKéMON!$"

PeacefulWoods_Trainer4_After:
	.string "This totally sucks.\nI'm gonna get some stronger ones!$"

PeacefulWoods_Trainer5::
	trainerbattle_single TRAINER_PEACEFUL_WOODS_SAMMY, PeacefulWoods_Trainer5_Intro, PeacefulWoods_Trainer5_Defeat
	msgbox PeacefulWoods_Trainer5_After, MSGBOX_AUTOCLOSE
	end

PeacefulWoods_Trainer5_Intro:
	.string "Hop, hop! Wait!\nWe are in no rush!$"

PeacefulWoods_Trainer5_Defeat:
	.string "Oh!\nI'm giving up! You are too strong!$"

PeacefulWoods_Trainer5_After:
	.string "You may find items\non the floor!\pI'm looking for any items I might\nhave missed…$"

PeacefulWoods_TrainerAnthony::
	trainerbattle_single TRAINER_PEACEFUL_WOODS_ANTHONY, PeacefulWoods_TrainerAnthony_Intro, PeacefulWoods_TrainerAnthony_Defeat
	msgbox PeacefulWoods_TrainerAnthony_After, MSGBOX_AUTOCLOSE
	end

PeacefulWoods_TrainerAnthony_Intro:
	.string "I may be small, but\ndon't underestimate me.$"

PeacefulWoods_TrainerAnthony_Defeat:
	.string "Oh no.\nEverything is going wrong.$"

PeacefulWoods_TrainerAnthony_After:
	.string "I won't forget this defeat.$"

PeacefulWoods_TrainerCharles::
	trainerbattle_single TRAINER_PEACEFUL_WOODS_CHARLES, PeacefulWoods_TrainerCharles_Intro, PeacefulWoods_TrainerCharles_Defeat
	msgbox PeacefulWoods_TrainerCharles_After, MSGBOX_AUTOCLOSE
	end

PeacefulWoods_TrainerCharles_Intro:
	.string "You know POKéMON\ncan evolve right?$"

PeacefulWoods_TrainerCharles_Defeat:
	.string "Oh!\nI Lost!$"

PeacefulWoods_TrainerCharles_After:
	.string "BUG POKéMON Evolve\nreally fast. I love them!$"

PeacefulWoods_ItemPokeBall::
	finditem ITEM_POKE_BALL
	end

PeacefulWoods_ItemBeedrillite::
	finditem ITEM_BEEDRILLITE
	end

PeacefulWoods_ItemPotion1::
	finditem ITEM_POTION
	end

PeacefulWoods_ItemPotion2::
	finditem ITEM_POTION
	end

PeacefulWoods_AmbientIgnored::
	msgbox PeacefulWoods_AmbientIgnored_Text, MSGBOX_NPC
	end

PeacefulWoods_AmbientIgnored_Text:
	.string "This wild POKéMON seems to be\nignoring you. it's best to leave\lit alone.$"

PeacefulWoods_AmbientFearful::
	msgbox PeacefulWoods_AmbientFearful_Text, MSGBOX_NPC
	end

PeacefulWoods_AmbientFearful_Text:
	.string "This Caterpie seems rather\nfearful, there is no point in\lfighting it.$"

PeacefulWoods_AmbientCocoon::
	msgbox PeacefulWoods_AmbientCocoon_Text, MSGBOX_NPC
	end

PeacefulWoods_AmbientCocoon_Text:
	.string "COCONFORT : ...$"

PeacefulWoods_LotteryGuy::
	lock
	faceplayer
	msgbox EclatRoute1_Text_LotteryFound, MSGBOX_DEFAULT
	giveitem ITEM_ECLAT_LOTTERY_TICKET
	goto_if_eq VAR_RESULT, FALSE, PeacefulWoods_LotteryGuy_Done
	setflag FLAG_PEACEFUL_WOODS_TICKET
	msgbox EclatRoute1_Text_LotteryUse, MSGBOX_DEFAULT
	removeobject VAR_LAST_TALKED
PeacefulWoods_LotteryGuy_Done:
	release
	end

PeacefulWoods_Sign0::
	msgbox PeacefulWoods_Sign0_Text, MSGBOX_SIGN
	end

PeacefulWoods_Sign0_Text:
	.string "TIP\pTo avoid fights,\ndon't walk in the tall\lgrass!$"

PeacefulWoods_Sign1::
	msgbox PeacefulWoods_Sign1_Text, MSGBOX_SIGN
	end

PeacefulWoods_Sign1_Text:
	.string "ROUTE 3\nPEACEFUL WOODS - PERCHARBER$"

PeacefulWoods_Sign2::
	msgbox PeacefulWoods_Sign2_Text, MSGBOX_SIGN
	end

PeacefulWoods_Sign2_Text:
	.string "ASTUCE\pInterdiction de capturer les\nPOKéMON d'un autre DRESSEUR!\pLa capture se fait à l'état\nsauvage!$"

PeacefulWoods_Sign3::
	msgbox PeacefulWoods_Sign3_Text, MSGBOX_SIGN
	end

PeacefulWoods_Sign3_Text:
	.string "You are entering\nPEACEFUL WOODS.$"

PeacefulWoods_Sign4::
	msgbox PeacefulWoods_Sign4_Text, MSGBOX_SIGN
	end

PeacefulWoods_Sign4_Text:
	.string "ANTIDOTE's sold in the\nPOKé MART heals\lpoisoning.$"

PeacefulWoods_Sign5::
	msgbox PeacefulWoods_Sign5_Text, MSGBOX_SIGN
	end

PeacefulWoods_Sign5_Text:
	.string "TIP\pA weakened POKéMON is easier\nto capture!\pIt can run away at full health!$"

PeacefulWoods_Sign6::
	msgbox PeacefulWoods_Sign6_Text, MSGBOX_SIGN
	end

PeacefulWoods_Sign6_Text:
	.string "“This altar is in homage to\nthe guardian of the forest.”$"
