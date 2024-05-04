/***************************************************
* Copyright 2018 - 2024 Kinnaji.All right reserved.
****************************************************/

#pragma once

#include "CoreMinimal.h"
#include "EditorSubsystem.h"
#include "EditorUtilityWidgetBlueprint.h"
#include "EditorUtilityAssist/EUW/KA_EUWTabManagerProxy.h"
#include "EditorUtilityLibrary.h"
#include "KA_EditorUtilityAssistSubsystem.generated.h"

/**
 * EditorUtilityをアシストするためのSubsystemクラス
 */

UCLASS()
class KAEDITORUTILITYPLUGIN_API UKA_EditorUtilityAssistSubsystem : public UEditorSubsystem
{
	GENERATED_BODY()


public:

	UPROPERTY()
	TMap<FName, UKA_EUWTabManagerProxy*> TabManagerProxyList;

private:
	TSharedPtr<SDockTab> GetEUWDockTab(class UEditorUtilityWidgetBlueprint* EUWBP, FName TabID);

	FName GenerateEUWTabID(class UEditorUtilityWidgetBlueprint* EUWBP,FName BaseTabID);

public:

	UFUNCTION()
	bool BindCanCloseEUWTab(class UEditorUtilityWidgetBlueprint* EUWBP, FName TabID, FOnGetCanCloseEUWTab CanCloseTabDelegate);

	/**
	* CanCloseTabをアンバインドする
	* @param EUWBP EUWのクラス
	* @param TabID TabID（基本的にNone）
	*/
	UFUNCTION(BlueprintCallable, Category = "KAEditorUtility")
	void UnbindCanCloseEUWTab(class UEditorUtilityWidgetBlueprint* EUWBP, FName TabID);
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBindCanCloseEUWTabComplete);

/**
 * EUWTabのCanCloseTabをBindするための非同期Blueprintノードクラス
 */

UCLASS()
class KAEDITORUTILITYPLUGIN_API UAsyncBindCanCloseEUWTab : public UEditorUtilityBlueprintAsyncActionBase
{
	GENERATED_UCLASS_BODY()

public:
#if WITH_EDITOR
	/**
	* CanCloseTabをバインドする
	* @param EUWBP EUWのクラス
	* @param TabID TabID（基本的にNone）
	* @param CanCloseTabDelegate 閉じられかどうかを決定する関数のデリゲート
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"))
		static UAsyncBindCanCloseEUWTab* AsyncBindCanCloseEUWTab(UEditorUtilityWidgetBlueprint* EUWBP, FName TabID, FOnGetCanCloseEUWTab CanCloseTabDelegate);

#endif

public:

	UPROPERTY(BlueprintAssignable)
		FBindCanCloseEUWTabComplete Complete;

public:

	void Start(UEditorUtilityWidgetBlueprint* EUWBP, FName TabID, FOnGetCanCloseEUWTab CanCloseTabDelegate);

private:

	bool HandleComplete(float DeltaTime);

	uint64 EndFrame = 0;
	double EndTime = 0;
	int32 ExecCount = 0;
	int32 MaxExecCount = 10;

	class UEditorUtilityWidgetBlueprint* EUWBP;
	FName TabID;
	FOnGetCanCloseEUWTab CanCloseTabDelegate;

};