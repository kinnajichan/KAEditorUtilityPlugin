/***************************************************
* Copyright 2018 - 2024 Kinnaji.All right reserved.
****************************************************/

#pragma once

#include "CoreMinimal.h"
#include "EditorSubsystem.h"
#include "EditorMenu/KA_EditorMenuData.h"
#include "KA_EditorMenuManagerSubsystem.generated.h"

/**
 * EditorMenuを管理するSubsystem
 */
UCLASS()
class KAEDITORUTILITYPLUGIN_API UKA_EditorMenuManagerSubsystem : public UEditorSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

public:
	/**
	 * 作成したメニューを管理対象に追加する
	 * @param menuAsset 追加するメニューのBPAsset
	 * @return 追加に成功したか
	 */
	UFUNCTION(BlueprintCallable, Category = "KAEditorUtility|Menu")
	bool AddGeneratedMenu(UBlueprint* menuAsset);

	
	/**
	 * 作成したWidgetを使用したメニューを管理対象に追加する
	 * @param widgetEntryData Widgetメニュー登録情報
	 * @return 追加に成功したか
	 */
	UFUNCTION(BlueprintCallable, Category = "KAEditorUtility|Menu")
	bool AddWidgetMenu(const FKA_WidgetMenuEntryData& widgetEntryData);

	
	/**
	 * 無効な管理対象のメニューを管理対象から削除する
	 * @return 削除したメニューが存在したか
	 */
	UFUNCTION(BlueprintCallable, Category = "KAEditorUtility|Menu")
	bool RemoveInvalidRegisteredMenus();

	
	/**
	 * 登録済み全メニュー更新
	 */
	UFUNCTION(BlueprintCallable, Category = "KAEditorUtility|Menu")
	void UpdateAllRegisteredMenus();

	
	/**
	 * 登録済みメニューを管理対象から除外する
	 * @param menuName メニュー名
	 */
	UFUNCTION(BlueprintCallable, Category = "KAEditorUtility|Menu")
	void RemoveRegisteredMenuByName(FName menuName);

	
	/**
	 * 指定のメニューに紐づくサブメニュー一覧取得
	 * @param parentName 親メニュー名
	 * @param result SubMenu情報配列
	 */
	UFUNCTION(BlueprintCallable, Category = "KAEditorUtility|Menu")
	void FindSubMenuNames(FName parentName, TArray<FKA_UserSelectableMenuData>& result);

	/**
	 * サブメニューを追加する
	 * @param menuName メニュー名
	 * @return 追加に成功したか
	 */
	UFUNCTION(BlueprintCallable, Category = "KAEditorUtility|Menu")
	bool AddSubMenu(FName menuName);

	/**
	 * サブメニューを削除する
	 * @param menuName メニュー名
	 * @return 削除に成功したか
	 */
	UFUNCTION(BlueprintCallable, Category = "KAEditorUtility|Menu")
	bool RemoveSubMenu(FName menuName);

	
	/**
	 * ユーザーが追加可能な親メニュー情報一覧取得
	 * @param result 親メニュー情報一覧
	 */
	UFUNCTION(BlueprintCallable, Category = "KAEditorUtility|Menu")
	void GetRootMenuDataList(TArray<FKA_UserSelectableMenuData>& result);

	/**
	 * メニュー名を結合する
	 * @param Base 親メニュー名
	 * @param Child 子メニュー名
	 * @return 結合結果メニュー名
	 */
	UFUNCTION(BlueprintCallable, Category = "KAEditorUtility|Menu")
	FName JoinPath(const FName Base, const FName Child);

	/**
	 * 指定のメニューから親をたどり、空のサブメニューを全て削除する
	 * @param menuName 削除し始めるメニュー名
	 */
	UFUNCTION(BlueprintCallable, Category = "KAEditorUtility|Menu")
	void RemoveEmptySubMenuTree(FName menuName);

	/**
	 * 対象のAssetが既に管理対象に登録されているかを調査する
	 * @param targetAssetData 対象のAssetData
	 * @return 管理対象に存在するか
	 */
	UFUNCTION(BlueprintCallable, Category = "KAEditorUtility|Menu")
	bool IsMenuAssetRegistered(const FAssetData& targetAssetData);
	
private:
	bool RegisterMenuAsset(const UBlueprint* menuAsset);
	bool RegisterMenu(UToolMenuEntryScript* target);
	bool RegisterMenuWidget(const FKA_WidgetMenuEntryData& widgetEntryData);
	static UToolMenus* GetToolMenus();
	static bool HasTargetMenu(FName menuName);
	static FName GetMenuPath(const UToolMenuEntryScript& entryScript);
	void SaveEditorMenuData();
	void RegisterManagementTargetAsset(const UBlueprint* target);
	void RegisterManagementTargetWidget(const FKA_WidgetMenuEntryData& widgetEntryData);
	bool IsRootMenu(FName menuName) const;
	
	TSharedRef<SWidget> GetCustomMenuWidget(const FToolMenuContext& toolMenuContext, const FToolMenuCustomWidgetContext& customWidgetContext, FName menuName);

	TArray<FName> AutoCreatedSubMenuNames;
	TMap<FName, FKA_WidgetMenuEntryData> RegisteredWidgetMenuMap;
	TMap<FName, FToolMenuEntryScriptData> RegisteredMenuDataMap;
public:
	UPROPERTY()
	TObjectPtr<UKA_EditorMenuData> EditorMenuData;
};
