@echo off
call bin\sh.exe version_script.sh > version_out.txt
set /p VAR=<version_out.txt
echo %VAR%
call usr\bin\zip.exe -j ../%VAR%.zip ../FLASH.bin
del version_out.txt
del version