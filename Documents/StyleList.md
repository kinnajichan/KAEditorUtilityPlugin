# WidgetStyle一覧
Editorで表示されている各種アイコンを使用したWidgetStyleを一覧取得する機能説明  

## アイコン一覧Widget使用方法
1. KAEditorUtilityPlugin\Content\IconList\EUW_EditorIconList.uassetのEUWを実行
1. 使用しているStyleが知りたいアイコンをクリック
1. ポップアップに該当画像を使用したStyle一覧が表示される
    * フォーマットは StyleSet名.Style名 

## 各種Style取得関数説明
全て **UKA_EditorMenuFunctionLibrary** に記載されているstatic関数  

|関数|説明|
|-|-|
|void KA_GetSlateBrushList(TArray<FSlateBrush\>& resultArray);|画像が設定されているBrush一覧を取得する|
|bool KA_GetStyleNames(const FSlateBrush& targetBrush, TArray<FName\>& brushUsingStyles);|対象のBrushを使用しているStyle名一覧を取得する<br>brushUsingStylesで取得するStyleのフォーマットは<br> StyleSet名.Style名|
