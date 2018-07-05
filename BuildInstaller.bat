@echo off

set STEAMCMD="C:\steamworks_sdk_142\sdk\tools\ContentBuilder\builder\steamcmd.exe"
set USER=
set USER_PASS=
set PROJECT_FILE="D:\GitHub\Steamworks\sku_project_example.txt"
set OUTPUT_DIR="D:\GitHub\Steamworks\GoldMasterDisk"
set BRANCH=
rem set BRANCH=baseline
set BRANCH_PASS=

 %STEAMCMD% +login %USER% %USER_PASS% +build_installer %PROJECT_FILE% %OUTPUT_DIR% %BRANCH% %BRANCH_PASS% +exit
