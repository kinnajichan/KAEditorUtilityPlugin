// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "KAEditorUtilityPluginStyle.h"

class FKAEditorUtilityPluginCommands : public TCommands<FKAEditorUtilityPluginCommands>
{
public:

	FKAEditorUtilityPluginCommands()
		: TCommands<FKAEditorUtilityPluginCommands>(TEXT("KAEditorUtilityPlugin"), NSLOCTEXT("Contexts", "KAEditorUtilityPlugin", "KAEditorUtilityPlugin Plugin"), NAME_None, FKAEditorUtilityPluginStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > PluginAction;
};
