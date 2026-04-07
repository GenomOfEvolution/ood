@echo off
setlocal enabledelayedexpansion

:: ============================================
:: Code Coverage для C++ проекта (OpenCppCoverage)
:: Запускать из папки с .sln файлом
:: ============================================

:: Переходим в директорию, где лежит этот скрипт
cd /d "%~dp0"

:: Настройки путей (относительно .sln)
set "SRC_DIR=%~dp0ShapesProgram"
set "TEST_EXE=%~dp0x64\Debug\Tests.exe"
set "REPORT_DIR=%~dp0CoverageReport"

:: Проверка: собран ли тестовый exe
if not exist "%TEST_EXE%" (
    echo [ERROR] Not found test file: %TEST_EXE%
    echo [INFO] Ensure, Tests project build in Debug|x64
    pause
    exit /b 1
)

:: Проверка: установлен ли OpenCppCoverage
where OpenCppCoverage.exe >nul 2>&1
if %errorlevel% neq 0 (
    echo [ERROR] OpenCppCoverage.exe not found in PATH
    pause
    exit /b 1
)

echo [INFO] Запуск покрытия кода...
echo [INFO] Исходники: %SRC_DIR%
echo [INFO] Тесты: %TEST_EXE%
echo [INFO] Отчёт: %REPORT_DIR%
echo.

:: ============================================
:: Основная команда
:: ============================================
OpenCppCoverage.exe ^
  --sources "%SRC_DIR%\*" ^
  --export_type html:%REPORT_DIR% ^
  -- "%TEST_EXE%" ^

:: ============================================
:: Обработка результата
:: ============================================
if %errorlevel% equ 0 (
    echo.
    echo [SUCCESS] Coverage generated!
    echo [INFO] Отчёт: %REPORT_DIR%\index.html
    echo.
    
    :: Автоматически открыть отчёт в браузере
    if exist "%REPORT_DIR%\index.html" (
        start "" "%REPORT_DIR%\index.html"
    )
) else (
    echo.
    echo [ERROR] An error occured when generated coverage (code: %errorlevel%)
)

pause