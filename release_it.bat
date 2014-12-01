set PATH=%PATH%;D:\Program Files\CMake 2.8\bin

set CMAKE_BUILDDIR=build\cmake\vc
set STARTDIR=%CD%
set WXDIR=%STARTDIR%\..\wxWidgets\3.0.2


@if NOT "%VS100COMNTOOLS%"=="" (call "%VS100COMNTOOLS%\vsvars32.bat" & goto VS_END)
@if NOT "%VS90COMNTOOLS%"=="" (call "%VS90COMNTOOLS%\vsvars32.bat" & goto VS_END)
@if NOT "%VS80COMNTOOLS%"=="" (call "%VS80COMNTOOLS%\vsvars32.bat" & goto VS_END)
@if NOT "%VS71COMNTOOLS%"=="" (call "%VS71COMNTOOLS%\vsvars32.bat" & goto VS_END)

:VS_END

mkdir %CMAKE_BUILDDIR%

cd %CMAKE_BUILDDIR%

del CMakeCache.txt

cmake -D LITESQL_WITH_DOCS:bool=ON -D LITESQL_WITH_MYSQL:bool=ON -D LITESQL_WITH_SQLITE:bool=ON -D LITESQL_WITH_TESTS:bool=ON -D LITESQL_WITH_UI:bool=ON -D LITESQL_MSVC_MT:BOOL=ON -D wxWidgets_ROOT_DIR=%WXDIR% -D wxWidgets_LIB_DIR=%WXDIR%\lib\vc_lib %STARTDIR%

devenv litesql.sln /Rebuild Debug /Project ALL_BUILD /ProjectConfig Debug
devenv /Rebuild Release litesql.sln /Project ALL_BUILD /ProjectConfig Debug

devenv /Build Debug litesql.sln /Project RUN_TESTS /ProjectConfig Debug

devenv /Build Release litesql.sln /Project PACKAGE /ProjectConfig Release

cpack --config CPackSourceConfig.cmake

cd %STARTDIR%

pause
