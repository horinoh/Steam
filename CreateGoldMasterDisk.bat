@echo off

rem インストーラのビルド
call BuildInstaller.bat

rem インストーラの設定ファイルをビルド先へコピー
xcopy Disk_1 GoldMasterDisk\Disk_1 /E /Y

rem ISOを作成
call CreateISO.bat

pause