/***************************************************
* Copyright 2018 - 2024 Kinnaji.All right reserved.
****************************************************/


#include "EditorUtilityAssist/KA_EditorUtilityAssistLibrary.h"

UEditorUtilityWidgetBlueprint* UKA_EditorUtilityAssistLibrary::KA_GetEUWBPFromEUW(UEditorUtilityWidget* InEUW)
{
	if (InEUW)
	{
		return Cast<UEditorUtilityWidgetBlueprint>(InEUW->GetClass()->ClassGeneratedBy);
	}
	return nullptr;
}
