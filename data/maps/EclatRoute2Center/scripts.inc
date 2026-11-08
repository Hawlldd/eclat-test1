EclatRoute2Center_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, EclatRoute2Center_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, CableClub_OnResume
	.byte 0

EclatRoute2Center_OnTransition:
	setrespawn HEAL_LOCATION_ECLAT_ROUTE2
	end

EclatRoute2Center_Nurse::
	setvar VAR_0x800B, LOCALID_ECLAT_ROUTE2_CENTER_1
	call Common_EventScript_PkmnCenterNurse
	waitmessage
	waitbuttonpress
	release
	end

EclatRoute2Center_Npc3::
	msgbox EclatRoute2Center_Npc3_Text, MSGBOX_NPC
	end

EclatRoute2Center_Npc3_Text:
	.string "It would seem that some guys are\nrunning into TEAM ROCKET,\lthey have been seen in the vicinity,\lBe careful if you\lgo to the forest.$"

EclatRoute2Center_Npc4::
	msgbox EclatRoute2Center_Npc4_Text, MSGBOX_NPC
	end

EclatRoute2Center_Npc4_Text:
	.string "I have 6 POKé BALLS on my belt.\pWe can only carry 6\nPOKéMON.$"

EclatRoute2Center_Npc5::
	msgbox EclatRoute2Center_Npc5_Text, MSGBOX_NPC
	end

EclatRoute2Center_Npc5_Text:
	.string "You should store your POKéMON in\nthe PC!$"
