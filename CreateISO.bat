@echo off

rem https://cdrtfe.sourceforge.io/cdrtfe/index_en.html
rem インストールして、以下をコピーしてくる
rem cdrtfe\tools\cdrtools\mkisofs.exe
rem cdrtfe\tools\cygwin\cygwin1.dll

for /d %%i in (GoldMasterDisk/*) do (
    mkisofs -r -J -o GoldMasterDisk/%%i.iso GoldMasterDisk/%%i
)