// Copyright Epic Games, Inc. All Rights Reserved.

#include "KAEditorUtilityPlugin.h"
#include "KAEditorUtilityPluginStyle.h"
#include "KAEditorUtilityPluginCommands.h"
#include "Misc/MessageDialog.h"
#include "ToolMenus.h"

static const FName KAEditorUtilityPluginTabName("KAEditorUtilityPlugin");

#define LOCTEXT_NAMESPACE "FKAEditorUtilityPluginModule"

void FKAEditorUtilityPluginModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FKAEditorUtilityPluginStyle::Initialize();
	FKAEditorUtilityPluginStyle::ReloadTextures();

	FKAEditorUtilityPluginCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FKAEditorUtilityPluginCommands::Get().PluginAction,
		FExecuteAction::CreateRaw(this, &FKAEditorUtilityPluginModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FKAEditorUtilityPluginModule::RegisterMenus));
}

void FKAEditorUtilityPluginModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FKAEditorUtilityPluginStyle::Shutdown();

	FKAEditorUtilityPluginCommands::Unregister();
}

void FKAEditorUtilityPluginModule::PluginButtonClicked()
{
	// Put your "OnButtonClicked" stuff here
	FText DialogText = FText::Format(
							LOCTEXT("PluginButtonDialogText", "Add code to {0} in {1} to override this button's actions"),
							FText::FromString(TEXT("FKAEditorUtilityPluginModule::PluginButtonClicked()")),
							FText::FromString(TEXT("KAEditorUtilityPlugin.cpp"))
					   );
	FMessageDialog::Open(EAppMsgType::Ok, DialogText);
}

void FKAEditorUtilityPluginModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FKAEditorUtilityPluginCommands::Get().PluginAction, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("PluginTools");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FKAEditorUtilityPluginCommands::Get().PluginAction));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FKAEditorUtilityPluginModule, KAEditorUtilityPlugin)