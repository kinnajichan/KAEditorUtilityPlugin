/***************************************************
* Copyright 2018 - 2024 Kinnaji.All right reserved.
****************************************************/


#include "EditorMenu/KA_EditorMenuFunctionLibrary.h"
#include "DesktopPlatformModule.h"
#include "KismetCompilerModule.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "FileHelpers.h"
#include "EditorUtilityBlueprint.h"
#include "LevelEditorMenuContext.h"
#include "EditorAssetLibrary.h"
#include "Selection.h"
#include "Styling/SlateStyleRegistry.h"


UBlueprint* UKA_EditorMenuFunctionLibrary::KA_CreateEUBAsset(FString AssetPath, UClass* TargetClass)
{
	UClass* BlueprintClass = nullptr;
	UClass* BlueprintGeneratedClass = nullptr;
	IKismetCompilerInterface& KismetCompilerModule = FModuleManager::LoadModuleChecked<IKismetCompilerInterface>("KismetCompiler");
	KismetCompilerModule.GetBlueprintTypesForClass(TargetClass, BlueprintClass, BlueprintGeneratedClass);
	return KA_CreateBlueprintAsset(AssetPath, TargetClass, BPTYPE_Normal, UEditorUtilityBlueprint::StaticClass(), BlueprintGeneratedClass);
}

bool UKA_EditorMenuFunctionLibrary::KA_TrySetMenuEntryData(UBlueprint* MenuAsset, FToolMenuEntryScriptData EntryData)
{
	if (!IsValid(MenuAsset)) {
		return false;
	}

	UClass* Class = MenuAsset->GeneratedClass;
	UToolMenuEntryScript* CDO = Class->GetDefaultObject<UToolMenuEntryScript>();
	if (!IsValid(CDO)) {
		return false;
	}
	CDO->Data = EntryData;
	return true;
}

UBlueprint* UKA_EditorMenuFunctionLibrary::KA_CreateBlueprintAsset(FString AssetPath, UClass* TargetClass, EBlueprintType BlueprintType, TSubclassOf<UBlueprint> BlueprintClassType, TSubclassOf<UBlueprintGeneratedClass> BlueprintGeneratedClassType)
{
	if (UEditorAssetLibrary::DoesAssetExist(AssetPath))
	{
		return nullptr;
	}
	
	UPackage* OuterForAsset = CreatePackage(*AssetPath);
	if(!IsValid(OuterForAsset))
	{
		return nullptr;
	}
	
	UBlueprint* newBlueprint = FKismetEditorUtilities::CreateBlueprint(TargetClass, OuterForAsset, FName(FPaths::GetBaseFilename(AssetPath)), BlueprintType, BlueprintClassType, BlueprintGeneratedClassType, FName("KA_EditorUtilityPlugin"));
	if (!IsValid(newBlueprint))
	{
		return nullptr;
	}
	
	FAssetRegistryModule::AssetCreated(newBlueprint);
	OuterForAsset->SetDirtyFlag(true);
	TArray<UPackage*> packagesToSave;
	packagesToSave.Add(OuterForAsset);
	FEditorFileUtils::PromptForCheckoutAndSave(packagesToSave, false, false);
	return newBlueprint;
}

bool UKA_EditorMenuFunctionLibrary::KA_TryGetDefaultObject(const UBlueprint* targetBP, TSubclassOf<UObject> targetClass,
	UObject*& defaultObject)
{
	if( !IsValid(targetBP) )
	{
		return false;
	}

	if( !targetBP->GeneratedClass->IsChildOf(targetClass) )
	{
		return false;
	}

	defaultObject = targetBP->GeneratedClass.GetDefaultObject();
	return true;
}

FString UKA_EditorMenuFunctionLibrary::KA_OpenFolderSelectDialog()
{
	void* parentWindowPtr = FSlateApplication::Get().GetActiveTopLevelWindow()->GetNativeWindow()->GetOSWindowHandle();

	FString FolderPath;
	IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
	if (DesktopPlatform)
	{
		DesktopPlatform->OpenDirectoryDialog(parentWindowPtr, TEXT("Select Folder"), FPaths::ProjectContentDir(), FolderPath);
	}

	return FolderPath;
}

const FModifierKeysState GetModifierKeysState()
{
	return FSlateApplication::Get().GetModifierKeys();
}

bool UKA_EditorMenuFunctionLibrary::KA_IsShiftKeyDown()
{
	return GetModifierKeysState().IsShiftDown();
}

bool UKA_EditorMenuFunctionLibrary::KA_IsAltKeyDown()
{
	return GetModifierKeysState().IsAltDown();
}

bool UKA_EditorMenuFunctionLibrary::KA_IsCtrlKeyDown()
{
	return GetModifierKeysState().IsControlDown();
}

bool UKA_EditorMenuFunctionLibrary::KA_IsCommandKeyDown()
{
	return GetModifierKeysState().IsCommandDown();
}

bool UKA_EditorMenuFunctionLibrary::KA_IsModifierKeyDown(bool& isShiftDown, bool& isCtrlDown, bool& isAltDown,
	bool& isCommandDown)
{
	const FModifierKeysState& keysState = GetModifierKeysState();

	isShiftDown = keysState.IsShiftDown();
	isCtrlDown = keysState.IsControlDown();
	isAltDown = keysState.IsAltDown();
	isCommandDown = keysState.IsCommandDown();

	return keysState.AnyModifiersDown();
}

void UKA_EditorMenuFunctionLibrary::KA_OpenAsset(const UObject* target)
{
	UAssetEditorSubsystem* assetLibrary = GEditor->GetEditorSubsystem<UAssetEditorSubsystem>();
	assetLibrary->OpenEditorForAsset(target);
}

bool UKA_EditorMenuFunctionLibrary::KA_OpenAssetIfShiftKeyDown(const UObject* target)
{
	if(!KA_IsShiftKeyDown())
	{
		return false;
	}
	KA_OpenAsset(target);
	return true;
}

FVector UKA_EditorMenuFunctionLibrary::KA_GetCursorWorldLocation(const FToolMenuContext& menuContext)
{
	ULevelEditorContextMenuContext* targetContext = menuContext.FindContext<ULevelEditorContextMenuContext>();

	if( targetContext == nullptr )
	{
		return FVector::ZeroVector;
	}

	return targetContext->CursorWorldLocation;
}

void UKA_EditorMenuFunctionLibrary::KA_GetSelectedComponentsInEditor(TArray<UActorComponent*>& selectedComponents)
{
	for (FSelectionIterator it(GEditor->GetSelectedComponentIterator()); it; ++it)
	{
		if( UActorComponent* component = Cast<UActorComponent>(*it))
		{
			selectedComponents.Add(component);
		}
	}
}

void UKA_EditorMenuFunctionLibrary::KA_GetSlateBrushList(TArray<FSlateBrush>& resultArray)
{
	TArray<const FSlateBrush*> allBrushes;
	FSlateStyleRegistry::GetAllResources(allBrushes);

	TMap<FName,const FSlateBrush*> brushMap;
	
	for (const FSlateBrush* brush : allBrushes)
	{
		FName key = brush->GetResourceName();
		if(key.IsNone() || brushMap.Contains(key))
		{
			continue;
		}
		brushMap.Add(key, brush);
	}

	for (auto&& brushPair : brushMap)
	{
		resultArray.Add(*brushPair.Value);
	}
}

bool UKA_EditorMenuFunctionLibrary::KA_GetStyleNames(const FSlateBrush& targetBrush, TArray<FName>& brushUsingStyles)
{
	FName brushName = targetBrush.GetResourceName();
	if(brushName.IsNone())
	{
		return false;
	}
	FSlateStyleRegistry::IterateAllStyles([&brushUsingStyles,brushName](const ISlateStyle& styleSet) -> bool
	{
		TArray<FName> styleNames = styleSet.GetEntriesUsingBrush(brushName);
		for (FName styleName : styleNames)
		{
			brushUsingStyles.Add(FName( FString::Format(TEXT("{0}.{1}"), {*styleSet.GetStyleSetName().ToString(), *styleName.ToString()})));
		}
		return true;
	});
	return true;
}

void UKA_EditorMenuFunctionLibrary::KA_DismissAllMenus()
{
	FSlateApplication::Get().DismissAllMenus();
}
