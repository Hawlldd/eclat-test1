Iviere_FilaosLab_MapScripts::
	.byte 0

Iviere_FilaosLab_EventScript_Filao::
	lock
	faceplayer
	goto_if_unset FLAG_RECEIVED_POKE_BALLS_FROM_FILAO, Iviere_FilaosLab_EventScript_FilaoGivePokeBalls
	setspeaker SP_NAME_FILAO
	msgbox Iviere_FilaosLab_Text_OfferHeal, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, Iviere_FilaosLab_EventScript_DeclineHeal
	closemessage
	special HealPlayerParty
	playfanfare MUS_RG_HEAL
	waitfanfare
	setspeaker SP_NAME_FILAO
	msgbox Iviere_FilaosLab_Text_Healed, MSGBOX_DEFAULT
	release
	end

Iviere_FilaosLab_EventScript_FilaoGivePokeBalls:
	setspeaker SP_NAME_FILAO
	msgbox Iviere_FilaosLab_Text_FilaoGivePokeBalls, MSGBOX_DEFAULT
	giveitem ITEM_POKE_BALL, 5
	setflag FLAG_RECEIVED_POKE_BALLS_FROM_FILAO
	setspeaker SP_NAME_FILAO
	msgbox Iviere_FilaosLab_Text_FilaoAfterPokeBalls, MSGBOX_DEFAULT
	release
	end

Iviere_FilaosLab_EventScript_DeclineHeal:
	setspeaker SP_NAME_FILAO
	msgbox Iviere_FilaosLab_Text_DeclineHeal, MSGBOX_DEFAULT
	release
	end

Iviere_FilaosLab_EventScript_Woman::
	msgbox Iviere_FilaosLab_Text_AskFilao, MSGBOX_NPC
	end

Iviere_FilaosLab_EventScript_Aide::
	msgbox Iviere_FilaosLab_Text_AskFilao, MSGBOX_NPC
	end

Iviere_FilaosLab_EventScript_GoodLuck::
	msgbox Iviere_FilaosLab_Text_GoodLuck, MSGBOX_NPC
	end

Iviere_FilaosLab_EventScript_Diploma::
	msgbox Iviere_FilaosLab_Text_Diploma, MSGBOX_SIGN
	end

Iviere_FilaosLab_EventScript_SaveTip::
	msgbox Iviere_FilaosLab_Text_SaveTip, MSGBOX_SIGN
	end

Iviere_FilaosLab_EventScript_MenuTip::
	msgbox IviereTown_Text_MenuSign, MSGBOX_SIGN
	end

Iviere_FilaosLab_Text_OfferHeal:
	.string "{PLAYER}, I'm sorry about\n"
	.string "what happened with those thieves.\p"
	.string "I can at least heal your POKéMON.\n"
	.string "Would you like me to do that?$"

Iviere_FilaosLab_Text_DeclineHeal:
	.string "Very well, I remain at your disposal.$"

Iviere_FilaosLab_Text_Healed:
	.string "Your POKéMON are feeling better.$"

Iviere_FilaosLab_Text_AskFilao:
	.string "You know, if you ask him, I'm sure\n"
	.string "the PROF. will heal your POKéMON.$"

Iviere_FilaosLab_Text_GoodLuck:
	.string "Good luck for the future.\n"
	.string "The life of a trainer is not\l"
	.string "always easy.$"

Iviere_FilaosLab_Text_Diploma:
	.string "PROF. FILAO's Doctorate.$"

Iviere_FilaosLab_Text_SaveTip:
	.string "The SAVE option is in the MENU.$"

Iviere_FilaosLab_Text_FilaoGivePokeBalls:
	.string "{PLAYER}! Before you head out,\n"
	.string "take these with you.\p"
	.string "You will need loyal companions\n"
	.string "by your side to face the trials\l"
	.string "that lie ahead.$"

Iviere_FilaosLab_Text_FilaoAfterPokeBalls:
	.string "If your POKéMON ever grow tired,\n"
	.string "speak to me and I will gladly\l"
	.string "heal them.$"
