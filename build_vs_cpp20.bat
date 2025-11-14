@echo off
echo ========================================
echo Visual Studio C++20 Beacon 编译助手
echo ========================================
echo.

REM 检查 Visual Studio 安装
set "VS2022_PATH="
for %%p in ("%ProgramFiles%\Microsoft Visual Studio\2022" "%ProgramFiles(x86)%\Microsoft Visual Studio\2022") do (
    if exist "%%p\Community\MSBuild\Current\Bin\MSBuild.exe" (
        set "VS2022_PATH=%%p\Community"
        goto :found_vs
    )
    if exist "%%p\Professional\MSBuild\Current\Bin\MSBuild.exe" (
        set "VS2022_PATH=%%p\Professional"
        goto :found_vs
    )
    if exist "%%p\Enterprise\MSBuild\Current\Bin\MSBuild.exe" (
        set "VS2022_PATH=%%p\Enterprise"
        goto :found_vs
    )
)

echo 错误: 未找到 Visual Studio 2022 安装
echo 请先安装 Visual Studio 2022 (Community/Professional/Enterprise)
pause
exit /b 1

:found_vs
echo 找到 Visual Studio 2022: %VS2022_PATH%
echo.

REM 设置环境变量
call "%VS2022_PATH%\VC\Auxiliary\Build\vcvars64.bat" >nul 2>&1

echo 可用的编译选项:
echo 1. Debug EXE (x64)     - 调试版本 HTTP/WebSocket Beacon
echo 2. Release EXE (x64)   - 发布版本 HTTP/WebSocket Beacon  
echo 3. Debug SMB (x64)     - 调试版本 SMB Beacon
echo 4. Debug TCP (x64)     - 调试版本 TCP Beacon
echo 5. Debug DLL (x64)     - 调试版本 DLL
echo 6. Release DLL (x64)   - 发布版本 DLL
echo 7. Release SVC (x64)   - 发布版本服务
echo 8. x86 版本编译选项
echo 9. 清理编译文件
echo 0. 退出
echo.

set /p choice="请选择编译选项 (0-9): "

if "%choice%"=="1" goto :compile_debug_exe_x64
if "%choice%"=="2" goto :compile_release_exe_x64
if "%choice%"=="3" goto :compile_debug_smb_x64
if "%choice%"=="4" goto :compile_debug_tcp_x64
if "%choice%"=="5" goto :compile_debug_dll_x64
if "%choice%"=="6" goto :compile_release_dll_x64
if "%choice%"=="7" goto :compile_release_svc_x64
if "%choice%"=="8" goto :x86_options
if "%choice%"=="9" goto :clean
if "%choice%"=="0" goto :exit

echo 无效选择，请重新运行脚本
pause
exit /b 1

:compile_debug_exe_x64
echo.
echo 正在编译 Debug EXE (x64) C++20 Beacon...
MSBuild beacon_cpp20\beacon_cpp20.vcxproj /p:Configuration="Debug EXE" /p:Platform=x64 /p:LanguageStandard=stdcpp20 /v:minimal
goto :result

:compile_release_exe_x64
echo.
echo 正在编译 Release EXE (x64) C++20 Beacon...
MSBuild beacon_cpp20\beacon_cpp20.vcxproj /p:Configuration="Release EXE" /p:Platform=x64 /p:LanguageStandard=stdcpp20 /v:minimal
goto :result

:compile_debug_smb_x64
echo.
echo 正在编译 Debug SMB (x64) C++20 Beacon...
MSBuild beacon_cpp20\beacon_cpp20.vcxproj /p:Configuration="Debug_SMB" /p:Platform=x64 /p:LanguageStandard=stdcpp20 /v:minimal
goto :result

:compile_debug_tcp_x64
echo.
echo 正在编译 Debug TCP (x64) C++20 Beacon...
MSBuild beacon_cpp20\beacon_cpp20.vcxproj /p:Configuration="Debug_TCP" /p:Platform=x64 /p:LanguageStandard=stdcpp20 /v:minimal
goto :result

:compile_debug_dll_x64
echo.
echo 正在编译 Debug DLL (x64) C++20 Beacon...
MSBuild beacon_cpp20\beacon_cpp20.vcxproj /p:Configuration="DebugDLL" /p:Platform=x64 /p:LanguageStandard=stdcpp20 /v:minimal
goto :result

:compile_release_dll_x64
echo.
echo 正在编译 Release DLL (x64) C++20 Beacon...
MSBuild beacon_cpp20\beacon_cpp20.vcxproj /p:Configuration="Release DLL" /p:Platform=x64 /p:LanguageStandard=stdcpp20 /v:minimal
goto :result

:compile_release_svc_x64
echo.
echo 正在编译 Release SVC (x64) C++20 Beacon...
MSBuild beacon_cpp20\beacon_cpp20.vcxproj /p:Configuration="Release SVC" /p:Platform=x64 /p:LanguageStandard=stdcpp20 /v:minimal
goto :result

:x86_options
echo.
echo x86 编译选项:
echo 1. Debug EXE (x86)
echo 2. Release EXE (x86)
echo 3. Debug SMB (x86)
echo 4. Debug TCP (x86)
echo 5. Debug DLL (x86)
echo 6. Release DLL (x86)
echo 7. Release SVC (x86)
echo 0. 返回主菜单
echo.

set /p x86_choice="请选择 x86 编译选项 (0-7): "

if "%x86_choice%"=="1" goto :compile_debug_exe_x86
if "%x86_choice%"=="2" goto :compile_release_exe_x86
if "%x86_choice%"=="3" goto :compile_debug_smb_x86
if "%x86_choice%"=="4" goto :compile_debug_tcp_x86
if "%x86_choice%"=="5" goto :compile_debug_dll_x86
if "%x86_choice%"=="6" goto :compile_release_dll_x86
if "%x86_choice%"=="7" goto :compile_release_svc_x86
if "%x86_choice%"=="0" goto :start

echo 无效选择
goto :x86_options

:compile_debug_exe_x86
echo.
echo 正在编译 Debug EXE (x86) C++20 Beacon...
MSBuild beacon_cpp20\beacon_cpp20.vcxproj /p:Configuration="Debug EXE" /p:Platform=Win32 /p:LanguageStandard=stdcpp20 /v:minimal
goto :result

:compile_release_exe_x86
echo.
echo 正在编译 Release EXE (x86) C++20 Beacon...
MSBuild beacon_cpp20\beacon_cpp20.vcxproj /p:Configuration="Release EXE" /p:Platform=Win32 /p:LanguageStandard=stdcpp20 /v:minimal
goto :result

:compile_debug_smb_x86
echo.
echo 正在编译 Debug SMB (x86) C++20 Beacon...
MSBuild beacon_cpp20\beacon_cpp20.vcxproj /p:Configuration="Debug_SMB" /p:Platform=Win32 /p:LanguageStandard=stdcpp20 /v:minimal
goto :result

:compile_debug_tcp_x86
echo.
echo 正在编译 Debug TCP (x86) C++20 Beacon...
MSBuild beacon_cpp20\beacon_cpp20.vcxproj /p:Configuration="Debug_TCP" /p:Platform=Win32 /p:LanguageStandard=stdcpp20 /v:minimal
goto :result

:compile_debug_dll_x86
echo.
echo 正在编译 Debug DLL (x86) C++20 Beacon...
MSBuild beacon_cpp20\beacon_cpp20.vcxproj /p:Configuration="DebugDLL" /p:Platform=Win32 /p:LanguageStandard=stdcpp20 /v:minimal
goto :result

:compile_release_dll_x86
echo.
echo 正在编译 Release DLL (x86) C++20 Beacon...
MSBuild beacon_cpp20\beacon_cpp20.vcxproj /p:Configuration="Release DLL" /p:Platform=Win32 /p:LanguageStandard=stdcpp20 /v:minimal
goto :result

:compile_release_svc_x86
echo.
echo 正在编译 Release SVC (x86) C++20 Beacon...
MSBuild beacon_cpp20\beacon_cpp20.vcxproj /p:Configuration="Release SVC" /p:Platform=Win32 /p:LanguageStandard=stdcpp20 /v:minimal
goto :result

:clean
echo.
echo 正在清理编译文件...
if exist "beacon_cpp20\x64" rmdir /s /q "beacon_cpp20\x64"
if exist "beacon_cpp20\Win32" rmdir /s /q "beacon_cpp20\Win32"
echo 清理完成
pause
exit /b 0

:result
echo.
if %ERRORLEVEL% EQU 0 (
    echo 编译成功！
    echo 输出文件位于: beacon_cpp20\{Platform}\{Configuration}\
) else (
    echo 编译失败，请检查错误信息
)
echo.
pause
exit /b %ERRORLEVEL%

:start
cls
goto :eof

:exit
echo 退出编译助手
pause
exit /b 0