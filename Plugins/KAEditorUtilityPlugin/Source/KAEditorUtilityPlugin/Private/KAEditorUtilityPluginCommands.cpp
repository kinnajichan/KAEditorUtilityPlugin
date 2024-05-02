/***************************************************
* Copyright 2018 - 2024 Kinnaji.All right reserved.
****************************************************/

#include "KAEditorUtilityPluginCommands.h"

#define LOCTEXT_NAMESPACE "FKAEditorUtilityPluginModule"

void FKAEditorUtilityPluginCommands::RegisterCommands()
{
	UI_COMMAND(OpenToolMenuGenerator, "Open EUW_ToolMenuGenerator", "Open EUW_ToolMenuGenerator action", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(OpenEditorIconList, "Open EUW_EditorIconList", "Open EUW_EditorIconList action", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
