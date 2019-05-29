
set PATH=%PATH%;C:\Program Files\CMake\bin

set CMAKE_BUILDDIR=build\cmake\vc
set STARTDIR=%CD%
set WXDIR=%STARTDIR%\..\wxWidgets\3.1.0

@if NOT "%VS150COMNTOOLS%"=="" (call "%VS150COMNTOOLS%\VsDevCmd.bat" & goto VS_END)
@if NOT "%VS140COMNTOOLS%"=="" (call "%VS140COMNTOOLS%\vsvars32.bat" & goto VS_END)
@if NOT "%VS110COMNTOOLS%"=="" (call "%VS110COMNTOOLS%\vsvars32.bat" & goto VS_END)
@if NOT "%VS100COMNTOOLS%"=="" (call "%VS100COMNTOOLS%\vsvars32.bat" & goto VS_END)
@if NOT "%VS90COMNTOOLS%"=="" (call "%VS90COMNTOOLS%\vsvars32.bat" & goto VS_END)
@if NOT "%VS80COMNTOOLS%"=="" (call "%VS80COMNTOOLS%\vsvars32.bat" & goto VS_END)
@if NOT "%VS71COMNTOOLS%"=="" (call "%VS71COMNTOOLS%\vsvars32.bat" & goto VS_END)

:VS_END

mkdir %CMAKE_BUILDDIR%

cd %CMAKE_BUILDDIR%

del CMakeCache.txt

cmake -D LITESQL_USE_SYSTEM_EXPAT:bool=OFF -D LITESQL_USE_SYSTEM_SQLITE:bool=OFF -D LITESQL_WITH_DOCS:bool=ON -D LITESQL_WITH_MYSQL:bool=ON -D LITESQL_WITH_SQLITE:bool=ON -D LITESQL_WITH_TESTS:bool=ON -D LITESQL_WITH_UI:bool=OFF -D LITESQL_MSVC_MT:BOOL=ON -D wxWidgets_ROOT_DIR=%WXDIR% -D wxWidgets_LIB_DIR=%WXDIR%\lib\vc_lib %STARTDIR%

cmake --build . --target ALL_BUILD --config Release
cmake --build . --target ALL_BUILD --config Debug
cmake --build . --target RUN_TESTS 
cmake --build . --target package 

cpack --config CPackSourceConfig.cmake

cd %STARTDIR%

pause
