@echo off

set STEAM_SDK=%STEAM_SDK_PATH%
rem set STEAM_SDK="C:\steamworks_sdk_142\sdk"
set STEAM_CMD=%STEAM_SDK%"\tools\ContentBuilder\builder\steamcmd"

set USER=anonymous
set USER_PASS=

set PROJECT_FILE="D:\GitHub\Steamworks\sku_project_example.txt"
set OUTPUT_DIR="D:\GitHub\Steamworks\GoldMasterDisk"
set BRANCH=
rem set BRANCH=baseline
set BRANCH_PASS=

 %STEAM_CMD% +login %USER% %USER_PASS% +build_installer %PROJECT_FILE% %OUTPUT_DIR% %BRANCH% %BRANCH_PASS% +exit
