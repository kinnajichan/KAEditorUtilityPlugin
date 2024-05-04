/***************************************************
* Copyright 2018 - 2024 Kinnaji.All right reserved.
****************************************************/

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "KA_EUWTabManagerProxy.generated.h"

/**
 * CanCloseTabDelegateをBindするための関数用Object
 */

DECLARE_DYNAMIC_DELEGATE_RetVal(bool, FOnGetCanCloseEUWTab);

UCLASS()
class KAEDITORUTILITYPLUGIN_API UKA_EUWTabManagerProxy : public UObject
{
	GENERATED_BODY()

public:

	//Blueprint側のデリゲート
	UPROPERTY()
		FOnGetCanCloseEUWTab GetCanCloseEUWTabDelegate;
	
		FName TabID;
	
		FSoftObjectPath EUWBPPath;

		bool GetCanCloseScriptEditorTab();

		void OnAssetRemoved(const FAssetData& InAssetData);


};
