@echo off

rem �C���X�g�[���̃r���h
call BuildInstaller.bat

rem �C���X�g�[���̐ݒ�t�@�C�����r���h��փR�s�[
xcopy Disk_1 GoldMasterDisk\Disk_1 /E /Y

rem ISO���쐬
call CreateISO.bat

pause