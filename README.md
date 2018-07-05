# Steamworks

## Steamworks SDK 
* ここでは環境変数を作成(Env.bat) ``setx STEAM_SDK_PATH "C:\steamworks_sdk_139\sdk"``
* 環境変数 Path に dll へのパスを通しておく ``%STEAM_SDK_PATH%\redistributable_bin\win64``
* 実行ファイルの場所(Steam\Debug, Steam\Release)へ steam_appid.txt を配置し ``480`` と書いておく
* GenerateProjectFiles.bat でプロジェクトファイルを生成する

## インストーラ
### 設定ファイル
* ``%STEAM_SDK_PATH%\tools\goldmaster`` を使う
* 出力先フォルダを作成 (ここでは Disk_1 とする)
    * Disk_1 以下へ goldmaster\disk_assets の内容をコピー
    * 必要なら Disk_1\setup.ini を書き換える
        * "Game", "AppID", "RequiredSize", "SupportURL", "WebLinkURL" 等
    * 必要ならリソースを置き換える
        * icon.ico, splash.tga
        * resources 以下
            * click.wav, hover.wav, launch.wav, setup.bmp 等
* 必要なら sku_project_example.txt を書き換える
    * "name", "appID", "included_depots" 等
* (念の為)一旦Steamクライアント終了してから、Steamクライアントをコンソール付きで起動する
    ~~~
    Steam.exe -console
    ~~~

## インストーラのビルド
* 予め(default, baseline等の)デポビルドを行っておく
    * ブランチ指定する場合は **baseline という名前で**ブランチを予め作っておく

### コンソールからの場合
* build_installer <プロジェクトファイル> <出力先> <ブランチ> <ブランチパスワード>
    ~~~
    build_installer "D:\GitHub\Steamworks\sku_project_example.txt" "D:\GitHub\Steamworks\output"
    build_installer "D:\GitHub\Steamworks\sku_project_example.txt" "D:\GitHub\Steamworks\output" baseline XXXX
    ~~~
    * ログは C:\Program Files (x86)\Steam\logs\backup_log.txt

### コマンドラインからの場合
* steamcmd.exe +login <ユーザ名> <ユーザパスワード> +build_installer <プロジェクトファイル> <出力先> <ブランチ> <ブランチパスワード> +exit
    ~~~
    C:\steamworks_sdk_XXX\sdk\tools\ContentBuilder\builder\steamcmd.exe +login USER YYY +build_installer "D:\GitHub\Steamworks\sku_project_example.txt" "D:\GitHub\Steamworks\output" baseline ZZZ +exit
    ~~~

* output 以下に Disk_1,... フォルダが作成され、中に .csd, .csm, .sis ファイルが出力されていれば成功
    * 進捗があまりなく動いているか心配になるが、Steamで他のDLなどはせずに待つ
* 上記で設定ファイルを置いた Disk_1 を outout 以下へコピー(output/Disk_1と統合)する
* (何か適当なツールで)Disk_1, Disk_2,... をisoにする 
* (何か適当なツールで)iso を仮想ディスクとしてマウントするか、右クリック - ディスクイメージの書き込みで物理ディスクへ書き込む