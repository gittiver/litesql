
set CMAKE_BUILDDIR=build\cmake\vc
set STARTDIR=%CD%
@if "%VS71COMNTOOLS%"=="" call "%VS80COMNTOOLS%\vsvars32.bat"
@if "%VS80COMNTOOLS%"=="" call "%VS71COMNTOOLS%\vsvars32.bat"

mkdir %CMAKE_BUILDDIR%

cd %CMAKE_BUILDDIR%
cmake -D LITESQL_WITH_DOCS:bool=ON -D LITESQL_WITH_MYSQL:bool=ON -D LITESQL_WITH_SQLITE:bool=ON -D LITESQL_WITH_TESTS:bool=ON  -D LITESQL_WITH_UI:bool=ON -D LITESQL_MSVC_MT:BOOL=OFF %STARTDIR%
devenv /rebuild Debug litesql.sln /project "ALL_BUILD.vcproj" /projectconfig Debug
devenv /rebuild Release litesql.sln /project "ALL_BUILD.vcproj" /projectconfig Debug

devenv /build Debug litesql.sln /project "RUN_TESTS.vcproj" /projectconfig Debug

devenv /build Release litesql.sln /project "PACKAGE.vcproj" /projectconfig Release

cpack --config CPackSourceConfig.cmake

cd %STARTDIR%

pause
