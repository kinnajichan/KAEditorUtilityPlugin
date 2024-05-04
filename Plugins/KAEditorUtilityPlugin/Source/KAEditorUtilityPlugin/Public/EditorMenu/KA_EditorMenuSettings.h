/***************************************************
* Copyright 2018 - 2024 Kinnaji.All right reserved.
****************************************************/

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "EditorMenu/KA_EditorMenuData.h"
#include "KA_EditorMenuSettings.generated.h"

/**
 * EditorMenu用Configクラス
 */
UCLASS(config = KAEditorUtilityPlugin, defaultconfig)
class KAEDITORUTILITYPLUGIN_API UKA_EditorMenuSettings : public UDeveloperSettings
{
	GENERATED_BODY()
public:

	UKA_EditorMenuSettings(const FObjectInitializer& ObjectInitializer);

	virtual FName GetCategoryName() const override	{return TEXT("Plugins");}

#if WITH_EDITOR
	virtual FText GetSectionText() const override	{return NSLOCTEXT("KAEditorMenu", "KAEditorMenu", "KA Editor Menu");}

	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

public:

	UPROPERTY(EditDefaultsOnly, Config, NoClear, Category = "KAEditorMenu", meta = (AllowedClasses = "/Script/KAEditorUtilityPlugin.KA_EditorMenuData", ConfigRestartRequired = true))
		FSoftObjectPath MenuData;
	
};
