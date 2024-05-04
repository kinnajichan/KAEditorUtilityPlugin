/***************************************************
* Copyright 2018 - 2024 Kinnaji.All right reserved.
****************************************************/


#include "EditorMenu/KA_EditorMenuSettings.h"
#include "EditorMenu/KA_EditorMenuManagerSubsystem.h"

UKA_EditorMenuSettings::UKA_EditorMenuSettings(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}


#if WITH_EDITOR
void UKA_EditorMenuSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FName name = PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None;
	if (name == TEXT("MenuData"))
	{
		if (UKA_EditorMenuManagerSubsystem* editorMenuManager = GEditor->GetEditorSubsystem<UKA_EditorMenuManagerSubsystem>())
		{
			if (UKA_EditorMenuData* menuData = Cast<UKA_EditorMenuData>(MenuData.TryLoad()))
			{
				editorMenuManager->EditorMenuData = menuData;
				editorMenuManager->UpdateAllRegisteredMenus();
			}
			else
			{
				MenuData = FSoftObjectPath(FString("/KAEditorUtilityPlugin/KA_EditorMenuData.KA_EditorMenuData"));
			}
		}
	}

}
#endif