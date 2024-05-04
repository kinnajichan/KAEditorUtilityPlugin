/***************************************************
* Copyright 2018 - 2024 Kinnaji.All right reserved.
****************************************************/

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Engine/Blueprint.h"
#include "KA_EditorMenuData.generated.h"


/**
 * ユーザーが選択可能なメニュー情報
 */
USTRUCT(BlueprintType)
struct FKA_UserSelectableMenuData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "KAEditorUtility")
	FName MenuName; // メニュー名

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "KAEditorUtility")
	FText DisplayName; // 表示名

	FKA_UserSelectableMenuData() 
		: MenuName(FName()), DisplayName(FText())
	{
	}

	FKA_UserSelectableMenuData(const FName& MenuName, const FText& DisplayName)
		: MenuName(MenuName), DisplayName(DisplayName)
	{
	}
};

/**
 * Widgetを使用したメニューの登録情報
 */
USTRUCT(BlueprintType)
struct FKA_WidgetMenuEntryData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="KAEditorUtility")
	FName MenuName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="KAEditorUtility")
	FName MenuSection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="KAEditorUtility")
	FText MenuLabel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="KAEditorUtility")
	bool IsDisplayMenuLabel;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="KAEditorUtility")
	TSubclassOf<UUserWidget> MenuWidgetType;
};

/**
 * メニュー管理データ
 */
UCLASS(BlueprintType)
class KAEDITORUTILITYPLUGIN_API UKA_EditorMenuData : public UPrimaryDataAsset
{
public:

	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "KAEditorUtility")
	TMap<FName, TSoftObjectPtr<UBlueprint>> UserCreatedMenus;

	UPROPERTY(EditAnywhere, Category = "KAEditorUtility")
	TMap<FName, FKA_WidgetMenuEntryData> CreatedWidgetMenus;

	UPROPERTY(EditAnywhere, Category="KAEditorUtility")
	TArray<FKA_UserSelectableMenuData> UserSelectableMenuCategories;

	UPROPERTY(EditAnywhere, Category = "KAEditorUtility")
	TSet<FName> VerifiedRootMenuNameSet;
};

