/***************************************************
* Copyright 2018 - 2024 Kinnaji.All right reserved.
****************************************************/


#include "EditorUtilityAssist/EUW/KA_EUWTabManagerProxy.h"
#include "EditorUtilityAssist/KA_EditorUtilityAssistSubsystem.h"
#include "AssetRegistry/AssetRegistryModule.h"

bool UKA_EUWTabManagerProxy::GetCanCloseScriptEditorTab()
{
	//右クリックメニューからの閉じるボタン用にメニュー表示されているときと右クリックされた時は閉じれるようにする
	if (FSlateApplication::Get().AnyMenusVisible())
	{
		return true;
	}
	if (FSlateApplication::Get().GetPressedMouseButtons().Contains(EKeys::RightMouseButton) == true)
	{
		return true;
	}
	//一度閉じれる判定がtrueになったら以降は常にtrue
	if (GetCanCloseEUWTabDelegate.IsBound())
	{
		if (GetCanCloseEUWTabDelegate.Execute())
		{
			GetCanCloseEUWTabDelegate.Clear();
			GEditor->GetEditorSubsystem<UKA_EditorUtilityAssistSubsystem>()->TabManagerProxyList.Remove(TabID);
			return true;
		}
		else
		{
			return false;
		}
	}
	return true;
}

void UKA_EUWTabManagerProxy::OnAssetRemoved(const FAssetData& InAssetData)
{
	//Bind中のEUWAssetが削除されたりRenameされた場合はProxyを作り直すので古いものを破棄する
	if (InAssetData.GetSoftObjectPath().ToString() == EUWBPPath.ToString())
	{
		if (GetCanCloseEUWTabDelegate.IsBound())
		{
			GetCanCloseEUWTabDelegate.Clear();
			GEditor->GetEditorSubsystem<UKA_EditorUtilityAssistSubsystem>()->TabManagerProxyList.Remove(TabID);
		}
		if (FAssetRegistryModule* AssetRegistryModule = FModuleManager::GetModulePtr<FAssetRegistryModule>(TEXT("AssetRegistry")))
		{
			AssetRegistryModule->Get().OnAssetRemoved().RemoveAll(this);
		}
	}
}
