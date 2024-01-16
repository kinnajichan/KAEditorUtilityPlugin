# Editor上でアセットを開いた際のイベントを受け取る方法
下記Eventに対して関数をバインドすることでアセットを開いた際の動作を設定可能です。  

|型|設定する関数の形式|取得関数|
|-|-|-|
|[FAssetEditorOpenEvent](https://docs.unrealengine.com/5.2/en-US/API/Editor/UnrealEd/Subsystems/UAssetEditorSubsystem/FAssetEditorOpenEvent/)|void XXX(UObject* object)|[UAssetEditorSubsystem](https://docs.unrealengine.com/5.2/en-US/API/Editor/UnrealEd/Subsystems/UAssetEditorSubsystem/) :: [FAssetEditorOpenEvent](https://docs.unrealengine.com/5.2/en-US/API/Editor/UnrealEd/Subsystems/UAssetEditorSubsystem/FAssetEditorOpenEvent/)|
|[FOnAssetOpenedInEditorEvent](https://docs.unrealengine.com/5.2/en-US/API/Editor/UnrealEd/Subsystems/UAssetEditorSubsystem/FOnAssetOpenedInEditorEvent/)|void XXX(UObject* object,<br>IAssetEditorInstance* editorInstance)|[UAssetEditorSubsystem](https://docs.unrealengine.com/5.2/en-US/API/Editor/UnrealEd/Subsystems/UAssetEditorSubsystem/) :: [OnAssetOpenedInEditor](https://docs.unrealengine.com/5.2/en-US/API/Editor/UnrealEd/Subsystems/UAssetEditorSubsystem/OnAssetOpenedInEditor/)|


## イベント登録方法サンプル
``` C++
// イベント登録元のサブシステム取得
UAssetEditorSubsystem* AssetEditorSubsystem = GEditor->GetEditorSubsystem<UAssetEditorSubsystem>();

// UObjectを継承したクラスの関数をイベントへ登録する場合
// サンプルで登録している関数の形式については上記設定する関数の形式を参照
AssetEditorSubsystem->OnAssetEditorOpened().AddUObject(this, &UTestClass::OnOpenedAsset);
```
