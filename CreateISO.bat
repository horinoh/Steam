@echo off

rem https://cdrtfe.sourceforge.io/cdrtfe/index_en.html インストールしておく
rem %CDRTOOLS%\cygwin を環境変数 Path に通しておく
set CDRTOOLS="C:\Program Files (x86)\cdrtfe\tools"

for /d %%i in (GoldMasterDisk/*) do (
    %CDRTOOLS%\cdrtools\mkisofs -r -J -o GoldMasterDisk/%%i.iso GoldMasterDisk/%%i
)