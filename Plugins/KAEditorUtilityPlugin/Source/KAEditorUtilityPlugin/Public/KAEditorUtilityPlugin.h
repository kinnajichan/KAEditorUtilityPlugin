/***************************************************
* Copyright 2018 - 2024 Kinnaji.All right reserved.
****************************************************/

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FToolBarBuilder;
class FMenuBuilder;

class FKAEditorUtilityPluginModule : public IModuleInterface
{
public:

	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	void OpenToolMenuGeneratorButtonClicked();
	void OpenEditorIconListButtonClicked();
	
private:

	void RegisterMenus();

private:
	TSharedPtr<class FUICommandList> KAEditorUtilityCommands;
};
