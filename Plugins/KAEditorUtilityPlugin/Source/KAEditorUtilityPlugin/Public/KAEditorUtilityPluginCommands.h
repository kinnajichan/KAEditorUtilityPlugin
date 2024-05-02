/***************************************************
* Copyright 2018 - 2024 Kinnaji.All right reserved.
****************************************************/

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
	TSharedPtr< FUICommandInfo > OpenToolMenuGenerator;
	TSharedPtr< FUICommandInfo > OpenEditorIconList;
};
