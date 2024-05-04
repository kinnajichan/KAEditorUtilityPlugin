/***************************************************
* Copyright 2018 - 2024 Kinnaji.All right reserved.
****************************************************/

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "EditorUtilityWidgetBlueprint.h"
#include "EditorUtilityWidget.h"
#include "KA_EditorUtilityAssistLibrary.generated.h"

/**
 * * EditorUtilityをアシストするためのLibraryクラス
 */
UCLASS()
class KAEDITORUTILITYPLUGIN_API UKA_EditorUtilityAssistLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	/**
	 * @brief EUWのインスタンスからEUWBPクラスを取得する
	 * @param EUWインスタンス
	 * @return EUWBPのクラス
	*/
	UFUNCTION(BlueprintCallable, Category = "KAEditorUtility")
		static UEditorUtilityWidgetBlueprint* KA_GetEUWBPFromEUW(UEditorUtilityWidget* InEUW);
	
};
