
@echo off

set progName=clmTest
set binDir=..\..\bin\
set srcDir=..\..\src\
set incDir=..\..\deps\include\

set src=%srcDir%clmtest.c^
 %srcDir%clm.c

set libs=user32.lib gdi32.lib shell32.lib ws2_32.lib

set compFlags=/I %incDir%

pushd buildfiles
cl /Fe%binDir%%progName% %compFlags% %src% %libs%
popd
