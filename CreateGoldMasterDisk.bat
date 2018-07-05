@echo off

rem call BuildInstaller.bat

xcopy Disk_1 GoldMasterDisk\Disk_1 /E /Y

call CreateISO.bat

pause