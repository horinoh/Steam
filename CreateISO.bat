@echo off

rem https://cdrtfe.sourceforge.io/cdrtfe/index_en.html �C���X�g�[�����Ă���
rem %CDRTOOLS%\cygwin �����ϐ� Path �ɒʂ��Ă���
set CDRTOOLS="C:\Program Files (x86)\cdrtfe\tools"

for /d %%i in (GoldMasterDisk/*) do (
    %CDRTOOLS%\cdrtools\mkisofs -r -J -o GoldMasterDisk/%%i.iso GoldMasterDisk/%%i
)