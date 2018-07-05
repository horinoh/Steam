@echo off

rem https://cdrtfe.sourceforge.io/cdrtfe/index_en.html インストールしておく
set CDRTOOLS="C:\Program Files (x86)\cdrtfe\tools"

xcopy %CDRTOOLS%\cygwin\cygwin1.dll . /Y

for /d %%i in (GoldMasterDisk/*) do (
    %CDRTOOLS%\cdrtools\mkisofs -r -J -o GoldMasterDisk/%%i.iso GoldMasterDisk/%%i
)

del cygwin1.dll