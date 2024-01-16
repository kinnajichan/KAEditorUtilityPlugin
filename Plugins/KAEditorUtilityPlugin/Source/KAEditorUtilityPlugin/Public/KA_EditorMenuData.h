// Fill out your copyright notice in the Description page of Project Settings.

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "KA_EditorUtility")
	FName MenuName; // メニュー名

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "KA_EditorUtility")
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="KA_EditorUtiltiy")
	FName MenuName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="KA_EditorUtiltiy")
	FName MenuSection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="KA_EditorUtiltiy")
	FText MenuLabel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="KA_EditorUtiltiy")
	bool IsDisplayMenuLabel;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="KA_EditorUtiltiy")
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
	
	UPROPERTY(EditAnywhere, Category = "KA_EditorUtility")
	TMap<FName, TSoftObjectPtr<UBlueprint>> UserCreatedMenus;

	UPROPERTY(EditAnywhere, Category = "KA_EditorUtility")
	TMap<FName, FKA_WidgetMenuEntryData> CreatedWidgetMenus;

	UPROPERTY(EditAnywhere, Category="KA_EditorUtility")
	TArray<FKA_UserSelectableMenuData> UserSelectableMenuCategories;

	UPROPERTY(EditAnywhere, Category = "KA_EditorUtility")
	TSet<FName> VerifiedRootMenuNameSet;
};

