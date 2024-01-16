# KAEditorUtility EditorMenu拡張機能使用方法
## 概要
UnrealEditor新規メニュー追加サポート機能やEditor起動時に作成したメニューの自動登録を行う機能等の、KAEditorUtility内のメニューに関する機能群についてのドキュメント

## 追加メニュー一覧
KAEditorUtilityで追加したEditorMenu一覧  

|追加場所|メニュー名|説明|
|-|-|-|
|ヘッダーメニュー|UpdateToolMenus|管理対象に登録されているメニュー情報を更新する|
|ヘッダーメニュー|新規メニュー追加|メニュー作成Windowを開く|
|EUB右クリックメニュー|メニュー登録|新規メニュー追加を通さず作成したメニューを管理対象へ追加する|

### サンプルメニュー一覧  
|追加場所|メニュー名|説明|
|-|-|-|
|ヘッダーメニュー|GetComponentsInEditorSample|Editor上で選択中のコンポーネントを取得するサンプル|
|ヘッダーメニュー|WidgetMenuTest|UserWidgetをメニューに表示するサンプル|
|レベルエディタ上のActor<br>右クリックメニュー|GetCursorLocationSample|Tool Menu Contextを使用するサンプル|


## 新規メニュー追加サポート機能
UnrealEditorに独自メニューを追加するアセットを作成する機能  
EditorUtilityToolMenuEntryを作成し、各種基本設定を行う

### 新規メニュー作成方法  
1. ヘッダーメニュー/KAEditorUtility/新規メニュー作成 を実行  
![GenerateMenu](./images/Header.png)
1. メニュー作成windowが開くので、各種項目を設定する
    * 各種設定項目については後述のメニュー作成Window詳細を参照
1. メニュー作成ボタンを押す
1. 登録完了後ポップアップが表示される
1. 意図した場所にメニューが追加されているか確認する
1. 設定したパスにメニューアセットが追加されているか確認する

<div style="page-break-before: always;"></div>

### メニュー作成Window詳細
メニュー追加時の設定項目で必須や利用頻度が高い項目の入力サポートを行い、入力されたデータを設定した状態でメニューを作成するWindow
![MenuWindow](./images/MenuGenerateWindow.png)

**項目説明**  

|項目名|説明|
|-|-|
|メニュー階層|メニューを追加したい大本のメニュー場所|
|サブメニュー階層|メニュー階層で指定した個所以下にある階層<br>未選択を指定時はメニュー階層で指定した場所へ直接新規メニューを登録する<br>詳細は下記サブメニュー設定についてを参照|
|新規サブメニュー名|メニュー階層以下に追加する新規サブメニュー名|
|メニュー名|メニュー名<br>メニューの識別に使用される他、このメニューのアセット名やラベル名のデフォルト値としても使用される|
|セクション名|メニュー追加時のセクション名<br>セクションの表示名としても使用される|
|ラベル名|メニューの表示名<br>未入力時はメニュー名が自動設定される|
|アセット保存フォルダ|アセットの保存先フォルダ<br>選択ボタンを押すとOSのフォルダ選択ポップアップが表示される|
|アセット名|作成するメニューアセットの名前<br> 未入力時は EUW_{アセット名}Menu が自動設定される|
|サブメニューとして作成|作成するメニューアセットをサブメニューとして作成するかのチェックボックス|
|メニュー作成|メニュー作成を実行する|

**サブメニュー作成について**  

* ヘッダーメニューのサブメニュー階層がEditor側の仕様により1階層しか追加できないため、サブメニューは1階層分しか設定できないように制限をつけている  
* サブメニューとして作成にチェックが入っている場合はサブメニュー関連の項目を設定不可にしている
* サブメニュー階層のドロップダウンで未設定を指定時はメインメニュー階層直下にメニューを追加する
* サブメニュー階層のドロップダウンで新規作成を指定時は新規サブメニュー名をメニュー階層以下に追加する
* サブメニュー階層のドロップダウンで新規作成以外を指定している場合はサブメニュー名を入力できない

### メニュー追加対象メニュー階層の追加方法
1. AEditorUtilityPlugin\Content\KA_EditorMenuDataを開く
1. Verified Menu Name Setへ新たに登録可能にしたいメニューの親メニュー名を追加
1. メニュー作成Widgetでも選択可能にする場合はUser Selectable Menu Categoriesにも追加

## 作成済みメニューをEditor起動時に自動登録機能
EditorUtilityToolMenuEntryは作成するだけではEditorのメニューに登録されないので、Editor起動時にPluginの管理対象になっているメニューを自動登録する  
上記新規メニュー作成から作成したメニューは自動で管理対象になっている

### メニュー作成Windowを通さずに作成したメニューの管理対象登録方法
1. EditorUtilityToolMenuEntryを選択したEditorUtilityBlueprintアセットをコンテンツブラウザ上で選択する
1. アセットを選択した状態で右クリックメニューを開く
1. メニューの末尾にメニュー登録項目があるので、実行する  
![RegisterMenu1](./images/RegisterMenu1.png)

**備考**  

* 登録済みメニューの場合グレーアウトで表示される  
![RegisterMenu2](./images/RegisterMenu2.png)
* 複数選択可で、選択アセットで一つでも登録していないアセットがある場合実行できる  
![RegisterMenu3](./images/RegisterMenu3.png)

## 自作UserWidgetのメニュー表示機能
ユーザーが作成したUserWidgetを指定のメニューとして登録する機能

### 自作UserWidget登録方法
UserWidget作成後、下記関数を実行する事でメニューへ登録が可能
``` c++
void UKA_EditorMenuManagerSubsystem::AddWidgetMenu(const FKA_WidgetMenuEntryData& widgetEntryData);
```

**FKA_WidgetMenuEntryData**の各設定項目説明  

|変数名|用途|
|-|-|
|Menu Name|メニューの登録先<br>例) MainFrame.MainMenu.KAEditorUtility.MyMenu|
|Menu Section|追加先セクション（任意）|
|Menu Label|メニューの表示名（任意）|
|Is Display Menu Label|メニュー名をWidgetの横に表示するか|
|Menu Widget Type|登録したいメニューのクラス<br>UUserWidgetの子クラスが指定可能|

### 自作メニュー登録サンプルEUW使用方法
上記自作Widget登録関数を呼び出すサンプルのEUWを用意しております  
パス : KAEditorUtilityPlugin\Content\SampleEUW_MenuWidgetRegisterSample
FKA_WidgetMenuEntryDataの各項目が設定できるようになっておりますので、   
各種設定後登録ボタンを押すことで自作メニュー登録が可能です

### 確認手順
1. UserWidgetを継承したWBPを作成（EUW可）
1. Widgetの中身を作成
1. KAEditorUtilityPlugin\Content\SampleEUW_MenuWidgetRegisterSample の右クリックメニューからRun Editor Utility Widget を実行
1. 各種情報設定後登録ボタンを実行
1. 指定の階層にメニューが追加される事を確認

## 登録したメニューの削除方法
下記のいずれかを行った後UpdateToolMenusを実行する事で管理対象から削除が可能  

* 登録したメニューアセットやWidgetを削除
* 下記削除関数を実行  
    ``` c++
    void UKA_EditorMenuManagerSubsystem::RemoveRegisteredMenuByName(FName menuName);
    ```
* KAEditorUtilityPlugin\Content\KA_EditorMenuDataを開き各変数を編集する
    * User Created MenusのValueを空にする
    * Created Widget MenusのMenu Widget TypeにNoneを設定する
    * 行を削除した場合はEditor再起動時に反映されます

