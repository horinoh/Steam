@echo off

set STEAM_SDK=%STEAM_SDK_PATH%
rem set STEAM_SDK="C:\steamworks_sdk_142\sdk"
set STEAM_CMD=%STEAM_SDK%"\tools\ContentBuilder\builder\steamcmd"

set OUTPUT_DIR="D:\GitHub\Steamworks\spacewar"

set USER=anonymous
set USER_PASS=

set APPID=480
set BRANCH=default
set BRANCH_PASS=

rem %STEAM_CMD% +login %USER% %USER_PASS% +force_install_dir %OUTPUT_DIR% +app_update %APPID% -beta %BRANCH% -betapassword %BRANCH_PASS% +quit
%STEAM_CMD% +login %USER% %USER_PASS% +force_install_dir %OUTPUT_DIR% +app_update %APPID% +quit
