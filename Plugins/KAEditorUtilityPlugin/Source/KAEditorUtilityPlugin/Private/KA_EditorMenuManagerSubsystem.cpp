/***************************************************
* Copyright 2018 - 2024 Kinnaji.All right reserved.
****************************************************/


#include "KA_EditorMenuManagerSubsystem.h"

#include "PackageTools.h"
#include "Components/Widget.h"
#include "Subsystems/UnrealEditorSubsystem.h"
#include "Blueprint/UserWidget.h"

void UKA_EditorMenuManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	EditorMenuData = LoadObject<UKA_EditorMenuData>(this, *FString("/KAEditorUtilityPlugin/KA_EditorMenuData.KA_EditorMenuData"), nullptr, LOAD_None, nullptr);
	UpdateAllRegisteredMenus();
}

bool UKA_EditorMenuManagerSubsystem::AddGeneratedMenu(UBlueprint* menuAsset)
{
	UpdateAllRegisteredMenus();

	if( !RegisterMenuAsset(menuAsset) )
	{
		return false;
	}

	RegisterManagementTargetAsset(menuAsset);
	SaveEditorMenuData();

	return true;
}

bool UKA_EditorMenuManagerSubsystem::AddWidgetMenu(const FKA_WidgetMenuEntryData& widgetEntryData)
{
	UpdateAllRegisteredMenus();

	if( !RegisterMenuWidget(widgetEntryData) )
	{
		return false;
	}

	RegisterManagementTargetWidget(widgetEntryData);
	
	SaveEditorMenuData();
	
	return true;
}

bool UKA_EditorMenuManagerSubsystem::RegisterMenuAsset(const UBlueprint* menuAsset)
{
	if(!IsValid(menuAsset))
	{
		return false;
	}

	UToolMenuEntryScript* toolMenuEntry = menuAsset->GeneratedClass->GetDefaultObject<UToolMenuEntryScript>();

	if (!IsValid(toolMenuEntry))
	{
		return false;
	}

	if(HasTargetMenu(GetMenuPath(*toolMenuEntry)))
	{
		return true;
	}

	if(!AddSubMenu(toolMenuEntry->Data.Menu))
	{
		return false;
	}

	return RegisterMenu(toolMenuEntry);
	
}

bool UKA_EditorMenuManagerSubsystem::RemoveInvalidRegisteredMenus()
{
	TArray<FName> removeTargetList;
	TArray<UBlueprint*> reRegistrationTargetList;
	for (auto&& kvp : EditorMenuData->UserCreatedMenus)
	{
		UBlueprint* menuAsset = kvp.Value.LoadSynchronous();

		if (!IsValid(menuAsset))
		{
			removeTargetList.Add(kvp.Key);
			continue;
		}

		const UToolMenuEntryScript* toolMenuEntry = menuAsset->GeneratedClass->GetDefaultObject<UToolMenuEntryScript>();
		if (GetMenuPath(*toolMenuEntry) != kvp.Key)
		{
			removeTargetList.Add(kvp.Key);
			reRegistrationTargetList.Add(menuAsset);
		}
	}

	TArray<FKA_WidgetMenuEntryData> reRegistrationMenuWidgetList;
	for(auto&& kvp : EditorMenuData->CreatedWidgetMenus)
	{
		if( !IsValid(kvp.Value.MenuWidgetType.Get()))
		{
			removeTargetList.Add(kvp.Key);
		}

		if(kvp.Key != kvp.Value.MenuName)
		{
			removeTargetList.Add(kvp.Key);
			reRegistrationMenuWidgetList.Add(kvp.Value);
		}
	}

	if(removeTargetList.Num() < 1)
	{
		return false;
	}

	for (const FName removeTarget : removeTargetList)
	{
		RemoveRegisteredMenuByName(removeTarget);
	}

	for (const UBlueprint* target : reRegistrationTargetList)
	{
		RegisterManagementTargetAsset(target);
	}

	for (auto&& target : reRegistrationMenuWidgetList)
	{
		RegisterManagementTargetWidget(target);
	}

	SaveEditorMenuData();


	return true;
}

void UKA_EditorMenuManagerSubsystem::UpdateAllRegisteredMenus()
{
	RemoveInvalidRegisteredMenus();

	for (auto&& kvp : EditorMenuData->UserCreatedMenus)
	{
		RegisterMenuAsset(kvp.Value.Get());
	}

	for (auto&& widgetMenuPair : EditorMenuData->CreatedWidgetMenus)
	{
		RegisterMenuWidget(widgetMenuPair.Value);
	}
}

void UKA_EditorMenuManagerSubsystem::RemoveRegisteredMenuByName(FName menuName)
{
	if (EditorMenuData->UserCreatedMenus.Contains(menuName))
	{
		EditorMenuData->UserCreatedMenus.Remove(menuName);
	}
	
	if(EditorMenuData->CreatedWidgetMenus.Contains(menuName))
	{
		EditorMenuData->CreatedWidgetMenus.Remove(menuName);
	}

	UToolMenus& toolMenus = *GetToolMenus();
	FName parentName, section, currentName;
	
	if( RegisteredMenuDataMap.Contains(menuName))
	{
		FToolMenuEntryScriptData entryData;
		RegisteredMenuDataMap.RemoveAndCopyValue(menuName,entryData);
		parentName = entryData.Menu;
		section = entryData.Section;
		currentName = entryData.Name;
	}

	if( RegisteredWidgetMenuMap.Contains(menuName) )
	{
		FKA_WidgetMenuEntryData entryData;
		RegisteredWidgetMenuMap.RemoveAndCopyValue(menuName, entryData);
		UToolMenus::SplitMenuPath(menuName,parentName,currentName);
		section = entryData.MenuSection;
	}
	
	toolMenus.RemoveEntry(parentName, section, currentName);
	
	RemoveEmptySubMenuTree(parentName);
}

void UKA_EditorMenuManagerSubsystem::FindSubMenuNames(FName parentName, TArray<FKA_UserSelectableMenuData>& result)
{
	UToolMenu* toolMenu = GetToolMenus()->FindMenu(parentName);
	if (!IsValid(toolMenu))
	{
		return;
	}
	for(FToolMenuSection& section : toolMenu->Sections)
	{
		for (FToolMenuEntry& menu:section.Blocks)
		{
			if (!menu.IsSubMenu())
			{
				continue;
			}

			result.Add(FKA_UserSelectableMenuData(menu.Name, menu.Label.Get()));
		}
	}
}

bool UKA_EditorMenuManagerSubsystem::AddSubMenu(FName menuName)
{
	if (HasTargetMenu(menuName))
	{
		return true;
	}

	UToolMenus* toolMenus = GetToolMenus();

	if (IsRootMenu(menuName))
	{
		toolMenus->ExtendMenu(menuName);
		return true;
	}

	TArray<FName> menuNames;
	UToolMenu* parentMenu = nullptr;
	FName baseName = menuName;
	FName parentName;
	FName itemName;

	while (toolMenus->SplitMenuPath(baseName, parentName, itemName))
	{
		baseName = parentName;
		menuNames.Insert(itemName, 0);
		parentMenu = toolMenus->FindMenu(parentName);
		if (IsValid(parentMenu))
		{
			break;
		}

		if ( IsRootMenu(parentName) )
		{

			parentMenu = toolMenus->ExtendMenu(parentName);
			break;
		}
	}

	for (FName name : menuNames)
	{
		if (!IsValid(parentMenu))
		{
			return false;
		}
		UToolMenu* generatedMenu = parentMenu->AddSubMenu(parentMenu, FName(), name, FText::FromName(name));
		parentMenu = generatedMenu;
	}

	return true;
}

void UKA_EditorMenuManagerSubsystem::RemoveEmptySubMenuTree(FName menuName)
{
	UToolMenus& toolMenus = *GetToolMenus();
	UToolMenu* toolMenu = toolMenus.FindMenu(menuName);

	if (!IsValid(toolMenu))
	{
		return;
	}

	for (FToolMenuSection& section : toolMenu->Sections)
	{	
		if(section.Blocks.Num() != 0)
		{
			return;
		}
	}

	if (RemoveSubMenu(menuName))
	{
		FName current;
		FName parent;
		toolMenus.SplitMenuPath(menuName, parent, current);
		RemoveEmptySubMenuTree(parent);
	};
}

bool UKA_EditorMenuManagerSubsystem::IsMenuAssetRegistered(const FAssetData& targetAssetData)
{
	for (auto&& menuTuple : EditorMenuData->UserCreatedMenus )
	{
		if(menuTuple.Value->GetPathName() == targetAssetData.GetObjectPathString())
		{
			return true;
		}
	}
	return false;
}

bool UKA_EditorMenuManagerSubsystem::RemoveSubMenu(FName menuName)
{

	UToolMenus& toolMenus = *GetToolMenus();

	if (!HasTargetMenu(menuName))
	{
		return false;
	}

	FName current;
	FName parent;
	toolMenus.SplitMenuPath(menuName, parent, current);
	UToolMenu* parentMenu = toolMenus.FindMenu(parent);

	if (!IsValid(parentMenu))
	{
		return false;
	}
	toolMenus.RemoveEntry(parent, parentMenu->GetSectionName(current), current);
	toolMenus.RefreshAllWidgets();
	return true;
}

void UKA_EditorMenuManagerSubsystem::GetRootMenuDataList(TArray<FKA_UserSelectableMenuData>& result)
{
	result.Append(EditorMenuData->UserSelectableMenuCategories);
}

FName UKA_EditorMenuManagerSubsystem::JoinPath(const FName Base, const FName Child)
{
	return UToolMenus::JoinMenuPaths(Base, Child);
}

bool UKA_EditorMenuManagerSubsystem::RegisterMenu(UToolMenuEntryScript* target)
{
	UToolMenus* toolMenus = GetToolMenus();
	if( !toolMenus->AddMenuEntryObject(target))
	{
		return false;
	}

	const FName key = GetMenuPath(*target);
	if (!RegisteredMenuDataMap.Contains(key))
	{
		RegisteredMenuDataMap.Add(key);
	}
	RegisteredMenuDataMap[key] = target->Data;

	if (!target->Data.Section.IsNone())
	{
		toolMenus->SetSectionLabel(target->Data.Menu, target->Data.Section, FText::FromName(target->Data.Section));
	}

	toolMenus->RefreshAllWidgets();
	return true;
}

bool UKA_EditorMenuManagerSubsystem::RegisterMenuWidget(const FKA_WidgetMenuEntryData& widgetEntryData)
{
	UToolMenus& toolMenus = *GetToolMenus();

	const FName menuName = widgetEntryData.MenuName;
	FName parentName, currentName;
	UToolMenus::SplitMenuPath(menuName, parentName, currentName);
	
	if(!AddSubMenu(parentName))
	{
		return false;
	}
	
	FToolMenuEntry* entry;
	
	UToolMenu* parentMenu = toolMenus.FindMenu(parentName);
	if(parentMenu == nullptr)
	{
		return false;
	}
	
	if(!HasTargetMenu(widgetEntryData.MenuName))
	{
		FToolMenuEntry newEntry;
		newEntry.Name = currentName;
		parentMenu->AddMenuEntry(widgetEntryData.MenuSection,newEntry);
		entry = parentMenu->FindSection(widgetEntryData.MenuSection)->FindEntry(currentName);
	}
	else
	{
		FToolMenuSection* section = parentMenu->FindSection(parentMenu->GetSectionName(currentName));
		if(section == nullptr)
		{
			return false;
		}
		entry = section->FindEntry(currentName);
	}

	if(entry == nullptr)
	{
		return false;
	}

	entry->Label = widgetEntryData.IsDisplayMenuLabel ? widgetEntryData.MenuLabel : FText(); 
	entry->MakeCustomWidget.BindUObject(this, &UKA_EditorMenuManagerSubsystem::GetCustomMenuWidget, widgetEntryData.MenuName);
	entry->Type = EMultiBlockType::Widget;
	entry->WidgetData.bNoIndent = true;
	entry->WidgetData.bNoPadding = true;
	
	if(!RegisteredWidgetMenuMap.Contains(menuName))
	{
		RegisteredWidgetMenuMap.Add(menuName);
	}
	RegisteredWidgetMenuMap[menuName] = widgetEntryData;

	if(!widgetEntryData.MenuSection.IsNone())
	{
		toolMenus.SetSectionLabel(parentName, widgetEntryData.MenuSection, FText::FromName(widgetEntryData.MenuSection));
	}
	
	return true;
}

UToolMenus* UKA_EditorMenuManagerSubsystem::GetToolMenus()
{
	return UToolMenus::Get();
}

bool UKA_EditorMenuManagerSubsystem::HasTargetMenu(FName menuName)
{
	return IsValid(GetToolMenus()->FindMenu(menuName));
}

FName UKA_EditorMenuManagerSubsystem::GetMenuPath(const UToolMenuEntryScript& entryScript)
{
	return UToolMenus::JoinMenuPaths(entryScript.Data.Menu, entryScript.Data.Name);
}

void UKA_EditorMenuManagerSubsystem::SaveEditorMenuData()
{
	EditorMenuData->Modify();
	UPackageTools::SavePackagesForObjects({ EditorMenuData });
}

void UKA_EditorMenuManagerSubsystem::RegisterManagementTargetAsset(const UBlueprint* target)
{
	const UToolMenuEntryScript* toolMenuEntry = target->GeneratedClass->GetDefaultObject<UToolMenuEntryScript>();

	const FName key = GetMenuPath(*toolMenuEntry);
	if (!EditorMenuData->UserCreatedMenus.Contains(key))
	{
		EditorMenuData->UserCreatedMenus.Add(key, target);
	}
}

void UKA_EditorMenuManagerSubsystem::RegisterManagementTargetWidget(const FKA_WidgetMenuEntryData& widgetEntryData)
{
	if(!EditorMenuData->CreatedWidgetMenus.Contains(widgetEntryData.MenuName))
	{
		EditorMenuData->CreatedWidgetMenus.Add(widgetEntryData.MenuName, widgetEntryData);
	}
}

bool UKA_EditorMenuManagerSubsystem::IsRootMenu(FName menuName) const
{
	return EditorMenuData->VerifiedRootMenuNameSet.Contains(menuName);
}

TSharedRef<SWidget> UKA_EditorMenuManagerSubsystem::GetCustomMenuWidget(const FToolMenuContext& toolMenuContext,
                                                                        const FToolMenuCustomWidgetContext& customWidgetContext, FName menuName)
{
	if(!RegisteredWidgetMenuMap.Contains(menuName))
	{
		return SNew(STextBlock).Text(FText::FromString("Invalid Menu Name"));
	}
	TSubclassOf<UUserWidget> targetClass = RegisteredWidgetMenuMap[menuName].MenuWidgetType;
	if(!IsValid(targetClass))
	{
		return SNew(STextBlock).Text(FText::FromString("Invalid Menu Class"));
	}
	UUserWidget* makeWidget = CreateWidget<UUserWidget>(GEditor->GetEditorSubsystem<UUnrealEditorSubsystem>()->GetEditorWorld(), targetClass);

	if( !IsValid(makeWidget ))
	{
		return SNew(STextBlock).Text(FText::FromString("Menu Widget Creation Failed"));
	}
	return makeWidget->TakeWidget();
}
