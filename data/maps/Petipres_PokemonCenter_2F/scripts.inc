Petipres_PokemonCenter_2F_MapScripts::
	map_script MAP_SCRIPT_ON_FRAME_TABLE, CableClub_OnFrame
	map_script MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE, CableClub_OnWarp
	map_script MAP_SCRIPT_ON_LOAD, CableClub_OnLoad
	.byte 0

Petipres_UpstairsVisitor::
	msgbox Petipres_UpstairsVisitor_Text, MSGBOX_NPC
	end

Petipres_PokemonCenter_PCGuide::
	msgbox Petipres_PokemonCenter_PCGuide_Text, MSGBOX_NPC
	end

Petipres_UpstairsVisitor_Text:
	.string "You can connect with your friends\n"
	.string "from here.$"

Petipres_PokemonCenter_PCGuide_Text:
	.string "You can log in with TRAINERS\n"
	.string "from around the world.$"
