# DepotDownload

- steamcmd.exe はSDKの以下のパスにある
~~~
steamworks_sdk_XXX\sdk\\tools\ContentBuilder\builder\steamcmd.exe
~~~
- 以下のようにしてダウンロード
~~~
steamcmd +login <ユーザ名> <パスワード> +force_install_dir <出力先> +app_update <アプリID> -beta <ブランチ名> -betapassword <パスワード> +quit
~~~
- 例
~~~
steamcmd +login anonymous +force_install_dir "D:\spacewar" +app_update 480 +quit
~~~

