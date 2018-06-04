# Steamworks

## Steamworks SDK 
* ここでは環境変数を作成(Env.bat) ``setx STEAM_SDK_PATH "C:\steamworks_sdk_139\sdk"``
* 環境変数 Path に dll へのパスを通しておく ``%STEAM_SDK_PATH%\redistributable_bin\win64``
* 実行ファイルの場所(Steam\Debug, Steam\Release)へ steam_appid.txt を配置し ``480`` と書いておく
* GenerateProjectFiles.bat でプロジェクトファイルを生成する

## ディスクイメージ
* steamworks_sdk_XXX\sdk\tools\goldmaster を使う
* 出力先フォルダを作成 (ここでは retail_disk とする)
    * retail_disk 以下へ goldmaster\disk_assets の内容をコピー
    * 必要なら retail_disk\setup.ini を書き換える
        * "Game", "AppID", "RequiredSize", "SupportURL", "WebLinkURL" 等
    * 必要ならリソースを置き換える
        * icon.ico, splash.tga
        * resources 以下
            * click.wav, hover.wav, launch.wav, setup.bmp 等
* 必要なら sku_project_example.txt を書き換える
* 一旦Steamクライアント終了してから、Steamクライアントをコンソール付きで起動する
    ~~~
    Steam.exe -console
    ~~~

* コンソールからバックアップコマンドを入力する
    ~~~
    build_installer "D:\GitHub\Steamworks\sku_project_example.txt" "D:\GitHub\Steamworks\retail_disk"
    ~~~
    * retail_disk 以下に Disk_1,... フォルダが作成され、中に .csd, .csm, .sis ファイルが出力されていれば成功