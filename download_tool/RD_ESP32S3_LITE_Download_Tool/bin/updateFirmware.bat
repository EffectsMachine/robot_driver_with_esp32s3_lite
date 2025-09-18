@echo off
setlocal

REM GitHub raw link for firmware.bin
set URL=https://raw.githubusercontent.com/EffectsMachine/robot_driver_with_esp32s3_lite/main/download_tool/RD_ESP32S3_LITE_Download_Tool/bin/firmware.bin

REM Get the current script directory
set SCRIPT_DIR=%~dp0

REM Local firmware path (same folder as this .bat file)
set LOCAL_PATH=%SCRIPT_DIR%firmware.bin

echo.
echo ===============================
echo  Downloading the latest firmware...
echo ===============================

REM Use curl to download and overwrite the file
curl -L "%URL%" -o "%LOCAL_PATH%"

if %ERRORLEVEL% EQU 0 (
    echo.
    echo Download complete. Firmware saved to:
    echo %LOCAL_PATH%
) else (
    echo.
    echo Download failed. Please check your network or path settings.
)

echo.
pause
