/***************************************************
* Copyright 2018 - 2024 Kinnaji.All right reserved.
****************************************************/

#include "KAEditorUtilityPlugin.h"
#include "KAEditorUtilityPluginStyle.h"
#include "KAEditorUtilityPluginCommands.h"
#include "Misc/MessageDialog.h"
#include "ToolMenus.h"
#include "EditorUtilitySubsystem.h"
#include "EditorUtilityWidgetBlueprint.h"

static const FName KAEditorUtilityPluginTabName("KAEditorUtilityPlugin");

#define LOCTEXT_NAMESPACE "FKAEditorUtilityPluginModule"

void FKAEditorUtilityPluginModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FKAEditorUtilityPluginStyle::Initialize();
	FKAEditorUtilityPluginStyle::ReloadTextures();

	FKAEditorUtilityPluginCommands::Register();
	
	KAEditorUtilityCommands = MakeShareable(new FUICommandList);

	KAEditorUtilityCommands->MapAction(
		FKAEditorUtilityPluginCommands::Get().OpenToolMenuGenerator,
		FExecuteAction::CreateRaw(this, &FKAEditorUtilityPluginModule::OpenToolMenuGeneratorButtonClicked),
		FCanExecuteAction());
	KAEditorUtilityCommands->MapAction(
		FKAEditorUtilityPluginCommands::Get().OpenEditorIconList,
		FExecuteAction::CreateRaw(this, &FKAEditorUtilityPluginModule::OpenEditorIconListButtonClicked),
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

void FKAEditorUtilityPluginModule::OpenToolMenuGeneratorButtonClicked()
{
	if (auto* EUS = GEditor->GetEditorSubsystem<UEditorUtilitySubsystem>())
	{
		if (auto* EUW = (UEditorUtilityWidgetBlueprint*)StaticLoadObject(UEditorUtilityWidgetBlueprint::StaticClass(), EUS, *FString("/KAEditorUtilityPlugin/EUW_ToolMenuGenerator"), nullptr, LOAD_None, nullptr))
		{
			EUS->SpawnAndRegisterTab(EUW);
		}
	}
}

void FKAEditorUtilityPluginModule::OpenEditorIconListButtonClicked()
{
	if (auto* EUS = GEditor->GetEditorSubsystem<UEditorUtilitySubsystem>())
	{
		if (auto* EUW = (UEditorUtilityWidgetBlueprint*)StaticLoadObject(UEditorUtilityWidgetBlueprint::StaticClass(), EUS, *FString("/KAEditorUtilityPlugin/IconList/EUW_EditorIconList"), nullptr, LOAD_None, nullptr))
		{
			EUS->SpawnAndRegisterTab(EUW);
		}
	}
}

void FKAEditorUtilityPluginModule::RegisterMenus()
{
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("KAEditorUtility");
			Section.Label = LOCTEXT("KAEditorUtilitySection", "KAEditorUtility");
			Section.AddMenuEntryWithCommandList(FKAEditorUtilityPluginCommands::Get().OpenToolMenuGenerator, KAEditorUtilityCommands);
			Section.AddMenuEntryWithCommandList(FKAEditorUtilityPluginCommands::Get().OpenEditorIconList, KAEditorUtilityCommands);
			UToolMenus::Get()->RefreshAllWidgets();
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FKAEditorUtilityPluginModule, KAEditorUtilityPlugin)