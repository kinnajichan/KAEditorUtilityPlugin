// Copyright Epic Games, Inc. All Rights Reserved.

#include "KAEditorUtilityPluginCommands.h"

#define LOCTEXT_NAMESPACE "FKAEditorUtilityPluginModule"

void FKAEditorUtilityPluginCommands::RegisterCommands()
{
	UI_COMMAND(PluginAction, "KAEditorUtilityPlugin", "Execute KAEditorUtilityPlugin action", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
