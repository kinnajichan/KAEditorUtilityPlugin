// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "KA_EditorMenuFunctionLibrary.generated.h"

struct FSlateBrush;

/**
 * キンアジ EditorUtilityFunctionLibrary
 */
UCLASS()
class KAEDITORUTILITYPLUGIN_API UKA_EditorMenuFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	/**
	 * @brief EditorUtilityBlueprint作成関数
	 * @param AssetPath アセットのパス
	 * @param TargetClass 対象クラス
	 * @return 作成したBP
	 */
	UFUNCTION(BlueprintCallable,Category="KAEditorUtility")
	static UBlueprint* KA_CreateEUBAsset(FString AssetPath, UClass* TargetClass);
	
	/**
	 * MenuEntryScriptDataの更新を試みる
	 * @param MenuAsset 
	 * @param EntryData 
	 * @return データの設定に成功したか
	 */
	UFUNCTION(BlueprintCallable, Category = "KAEditorUtility|Menu")
	static bool KA_TrySetMenuEntryData(UBlueprint* MenuAsset, FToolMenuEntryScriptData EntryData);

	/**
	 * BPAssetを作成する
	 * @param AssetPath アセット保存先のパス
	 * @param TargetClass 対象クラス
	 * @param BlueprintType BPのタイプ
	 * @param BlueprintClassType BPのClass(UBlueprintの派生クラス)
	 * @param BlueprintGeneratedClassType BPのGeneratedClass(UBlueprintGeneratedClassの派生クラス)
	 * @return 作成したBPのポインタ
	 */
	UFUNCTION(BlueprintCallable, Category = "KAEditorUtility")
	static UBlueprint* KA_CreateBlueprintAsset(FString AssetPath, UClass* TargetClass, EBlueprintType BlueprintType, TSubclassOf<UBlueprint> BlueprintClassType, TSubclassOf <UBlueprintGeneratedClass> BlueprintGeneratedClassType);

	
	/**
	 * BPAssetから指定クラスのDefaultObjectを取得する
	 * @param targetBP 対象のBPアセット
	 * @param targetClass 取得するクラス
	 * @param defaultObject BP取得したDefaultObject
	 * @return 取得に成功したか
	 */
	UFUNCTION(BlueprintPure, Category = "KAEditorUtility", meta = (DeterminesOutputType = "targetClass", DynamicOutputParam = "defaultObject"))
	static bool KA_TryGetDefaultObject(const UBlueprint* targetBP, TSubclassOf<UObject> targetClass, UObject*& defaultObject);

	
	/**
	 * フォルダ選択ダイアログを表示
	 * @return 選択されたフォルダ
	 */
	UFUNCTION(BlueprintCallable, Category = "KAEditorUtility")
	static FString KA_OpenFolderSelectDialog();

	
	/**
	 * 対象の装飾キーが押されているか判定する
	 * @return ShiftKeyが押されているか
	 */
	UFUNCTION(BlueprintPure, Category = "KAEditorUtility|Key")
	static bool KA_IsShiftKeyDown();

	/**
	 * 対象の装飾キーが押されているか判定する
	 * @return AltKeyが押されているか
	 */
	UFUNCTION(BlueprintPure, Category = "KAEditorUtility|Key")
	static bool KA_IsAltKeyDown();

	/**
	 * 対象の装飾キーが押されているか判定する
	 * @return CtrlKeyが押されているか
	 */
	UFUNCTION(BlueprintPure, Category = "KAEditorUtility|Key")
	static bool KA_IsCtrlKeyDown();

	/**
	 * 対象の装飾キーが押されているか判定する
	 * @return CommandKeyが押されているか
	 */
	UFUNCTION(BlueprintPure, Category = "KAEditorUtility|Key")
	static bool KA_IsCommandKeyDown();
	
	/**
	 * 装飾キーが押されているか判定する
	 * @param isShiftDown ShiftKeyが押されているか
	 * @param isCtrlDown CtrlKeyが押されているか
	 * @param isAltDown AltKeyが押されているか
	 * @param isCommandDown CommandKeyが押されているか
	 * @return いずれかの装飾キーが押されているか
	 */
	UFUNCTION(BlueprintPure, Category = "KAEditorUtility|Key")
	static bool KA_IsModifierKeyDown(bool& isShiftDown,bool& isCtrlDown,bool& isAltDown, bool& isCommandDown);

	/**
	 * 指定アセットのAssetEditorを開く
	 * @param target 対象アセット
	 */
	UFUNCTION(BlueprintCallable, Category = "KAEditorUtility")
	static void KA_OpenAsset(const UObject* target);

	
	/**
	 * ShiftKeyが押されていた場合に指定AssetEditorを開く
	 * @param target 対象アセット
	 * @return アセットを開いたか
	 */
	UFUNCTION(BlueprintCallable, Category = "KAEditorUtility")
	static bool KA_OpenAssetIfShiftKeyDown(const UObject* target);
	
	/**
	 * ToolMenuContextからCursorWorldLocationを取得する
	 * @param menuContext 取得元のMenuContext
	 * @return 取得したLocation
	 */
	UFUNCTION(BlueprintCallable, Category = "KAEditorUtility")
	static FVector KA_GetCursorWorldLocation(const FToolMenuContext& menuContext);

	/**
	 * Editorで選択中のコンポーネント一覧を取得する
	 * @param selectedComponents 選択中のコンポーネント配列
	 */
	UFUNCTION(BlueprintCallable, Category = "KAEditorUtiltiy")
	static void KA_GetSelectedComponentsInEditor(TArray<UActorComponent*>& selectedComponents);

	/**
	 * Editorで使用されている画像を使用したSlateBrushを取得する
	 * @param resultArray SlateBrush配列
	 */
	UFUNCTION(BlueprintCallable, Category = "KAEditorUtility|Slate")
	static void KA_GetSlateBrushList(TArray<FSlateBrush>& resultArray);

	/**
	 * 対象のSlateBrushを使用しているStyle一覧を取得する
	 * 取得したStyleのフォーマット : StyleSetName.StyleName
	 * @param targetBrush 対象のSlateBrush
	 * @param brushUsingStyles Brushを使用しているStyle配列
	 * @return Styleの取得に成功したか
	 */
	UFUNCTION(BlueprintCallable, Category = "KAEditorUtility|Slate")
	static bool KA_GetStyleNames(const FSlateBrush& targetBrush, TArray<FName>& brushUsingStyles);

	/**
	 * 開いている右クリック等のメニューを全て閉じる
	 */
	UFUNCTION(BlueprintCallable, Category = "KAEditorUtiltiy|Menu")
	static void KA_DismissAllMenus();

};
