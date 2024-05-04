# EditorUtilityWidgetを閉じる際の確認処理追加の方法
## 概要
任意のEditorUtilityWidgetに対して、EditorUtilityWidgetが開かれたタブを閉じる際に確認画面を出してキャンセルをできるようにする等の処理を追加する方法のドキュメント

### サンプルアセット一覧  
|アセットパス|
|-|
|/KAEditorUtilityPlugin/Sample/EUW_CanCloseEUWSample|



## EUWを閉じる際の確認処理の追加方法

任意のEditorUtilityWidgetを開き、ClassSettingsからImplementedInterfaceに`KA_EUWTabManagerInterface`を追加します。

![alt text](images/CanCloseEUW_AddInterface.png)

`GetCanCloseEUWTab`関数をオーバーライドし、条件を記載します。
※`ShowMessageDialog`等と組み合わせることで、閉じる際にメッセージウィンドウを表示させることができます。

![alt text](image-3.png)

`EventConstruct`ノードから`AsyncBindCanCloseEUWTab`を接続させます。
そして、`CanCloseTabDelegate`に`CreateEvent`ノードを接続し、プルダウンから`GetCanCloseEUWTab`を選択します。
※引数のEUWBPを自動的に使用しているEditorUtilityWidgetにするには、`KA_GetEUWBPFromEUW`ノードを使用してみてください。

![alt text](image-4.png)

これでEditorUtilityWidgetを起動すると、EditorUtilityWidgetのタブを閉じる際に`GetCanCloseEUWTab`の処理が走ります。

![alt text](image-5.png)

以上