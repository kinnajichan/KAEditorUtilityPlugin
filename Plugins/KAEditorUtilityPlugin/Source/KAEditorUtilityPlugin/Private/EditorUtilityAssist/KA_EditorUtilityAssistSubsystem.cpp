/***************************************************
* Copyright 2018 - 2024 Kinnaji.All right reserved.
****************************************************/

#include "EditorUtilityAssist/KA_EditorUtilityAssistSubsystem.h"

#include "LevelEditor.h"
#include "IBlutilityModule.h"
#include "AssetRegistry/AssetRegistryModule.h"

#define LOCTEXT_NAMESPACE "KA_EditorUtilityAssistSubsystem"

TSharedPtr<SDockTab> UKA_EditorUtilityAssistSubsystem::GetEUWDockTab(class UEditorUtilityWidgetBlueprint* EUWBP,FName TabID)
{
	if (!EUWBP)
	{
		return nullptr;
	}
	FName RegistrationName = GenerateEUWTabID(EUWBP,TabID);

	FText DisplayName = FText::FromString(FName::NameToDisplayString(EUWBP->GetName(), false));
	FLevelEditorModule& LevelEditorModule = FModuleManager::GetModuleChecked<FLevelEditorModule>(TEXT("LevelEditor"));
	TSharedPtr<FTabManager> LevelEditorTabManager = LevelEditorModule.GetLevelEditorTabManager();
	
	if (LevelEditorTabManager->HasTabSpawner(RegistrationName))
	{
		IBlutilityModule* BlutilityModule = FModuleManager::GetModulePtr<IBlutilityModule>("Blutility");
		TSharedPtr<SDockTab> DockTab = LevelEditorTabManager->FindExistingLiveTab(RegistrationName);
		if (DockTab.IsValid())
		{
			return DockTab;
		}
	}
	return nullptr;
}

FName UKA_EditorUtilityAssistSubsystem::GenerateEUWTabID(class UEditorUtilityWidgetBlueprint* EUWBP, FName BaseTabID)
{
	if (EUWBP)
	{
		//UEditorUtilitySubsystem::RegisterTabAndGetIDと同じ方法でIDを生成
		return BaseTabID.IsNone() ? FName(*(EUWBP->GetPathName() + LOCTEXT("ActiveTabSuffix", "_ActiveTab").ToString())) : FName(*(EUWBP->GetPathName() + BaseTabID.ToString()));
	}
	return NAME_None;
}

bool UKA_EditorUtilityAssistSubsystem::BindCanCloseEUWTab(class UEditorUtilityWidgetBlueprint* EUWBP, FName TabID, FOnGetCanCloseEUWTab CanCloseTabDelegate)
{
	FName GeneratedTabID = GenerateEUWTabID(EUWBP, TabID);
	if (TSharedPtr<SDockTab> DockTab = GetEUWDockTab(EUWBP, TabID))
	{
		UKA_EUWTabManagerProxy* TabManagerProxy = nullptr;
		if (TabManagerProxyList.Contains(GeneratedTabID))
		{
			TabManagerProxy = TabManagerProxyList[GeneratedTabID];
		}
		if(!TabManagerProxy)
		{
			TabManagerProxy = NewObject<UKA_EUWTabManagerProxy>(this);
			TabManagerProxyList.Add(GeneratedTabID, TabManagerProxy);
		}
		if (FAssetRegistryModule* AssetRegistryModule = FModuleManager::GetModulePtr<FAssetRegistryModule>(TEXT("AssetRegistry")))
		{
			AssetRegistryModule->Get().OnAssetRemoved().AddUObject(TabManagerProxy, &UKA_EUWTabManagerProxy::OnAssetRemoved);
		}
		TabManagerProxy->GetCanCloseEUWTabDelegate = CanCloseTabDelegate;
		TabManagerProxy->TabID = GeneratedTabID;
		TabManagerProxy->EUWBPPath = FSoftObjectPath(EUWBP);
		DockTab.Get()->SetCanCloseTab(SDockTab::FCanCloseTab::CreateUObject(TabManagerProxy, &UKA_EUWTabManagerProxy::GetCanCloseScriptEditorTab));
		return true;
	}
	return false;
}

void UKA_EditorUtilityAssistSubsystem::UnbindCanCloseEUWTab(class UEditorUtilityWidgetBlueprint* EUWBP, FName TabID)
{
	if (TabManagerProxyList.Contains(TabID))
	{
		if (UKA_EUWTabManagerProxy* TabManagerProxy = TabManagerProxyList[TabID])
		{
			TabManagerProxy->GetCanCloseEUWTabDelegate.Clear();
		}
		TabManagerProxyList.Remove(TabID);
	}
}

UAsyncBindCanCloseEUWTab::UAsyncBindCanCloseEUWTab(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UAsyncBindCanCloseEUWTab* UAsyncBindCanCloseEUWTab::AsyncBindCanCloseEUWTab(class UEditorUtilityWidgetBlueprint* InEUWBP, FName InTabID, FOnGetCanCloseEUWTab InCanCloseTabDelegate)
{
	UAsyncBindCanCloseEUWTab* NewTask = NewObject<UAsyncBindCanCloseEUWTab>();
	NewTask->Start(InEUWBP, InTabID, InCanCloseTabDelegate);
	return NewTask;
}

void UAsyncBindCanCloseEUWTab::Start(class UEditorUtilityWidgetBlueprint* InEUWBP, FName InTabID, FOnGetCanCloseEUWTab InCanCloseTabDelegate)
{
	EUWBP = InEUWBP;
	TabID = InTabID;
	CanCloseTabDelegate = InCanCloseTabDelegate;
	EndFrame = GFrameCounter + 1;
	EndTime = FApp::GetCurrentTime();
	ExecCount = 0;
	FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateUObject(this, &UAsyncBindCanCloseEUWTab::HandleComplete), 0);
}

bool UAsyncBindCanCloseEUWTab::HandleComplete(float DeltaTime)
{
	ExecCount++;
	if (GEditor->GetEditorSubsystem<UKA_EditorUtilityAssistSubsystem>()->BindCanCloseEUWTab(EUWBP, TabID, CanCloseTabDelegate))
	{
		Complete.Broadcast();
		SetReadyToDestroy();
		return false;
	}
	//最大回数試してBindできない場合は終了する
	if (MaxExecCount <= ExecCount)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to Bind Can Close EUW Tab"));
		Complete.Broadcast();
		SetReadyToDestroy();
		return false;
	}
	return true;
}

#undef LOCTEXT_NAMESPACE