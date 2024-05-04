/***************************************************
* Copyright 2018 - 2024 Kinnaji.All right reserved.
****************************************************/

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "KA_EUWTabManagerInterface.generated.h"

/**
 * EUWを閉じるかどうかを決定する関数用のInterfaceクラス
 */

UINTERFACE(MinimalAPI)
class UKA_EUWTabManagerInterface : public UInterface
{
	GENERATED_BODY()
};

class IKA_EUWTabManagerInterface
{
	GENERATED_BODY()

public:
	//AsyncBindCanCloseEUWTabを使用してBlueprint側でBindするためのInterface関数
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "KAEditorUtility")
		bool GetCanCloseEUWTab();

};