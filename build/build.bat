@echo off

set progName=CGL
set binDir=..\..\bin\
set srcDir=..\..\src\
set libDir=..\..\deps\lib\
set incDir=..\..\deps\include\

set src=%srcDir%main.c^
 %srcDir%glad.c^
 %srcDir%input.c^
 %srcDir%shader.c^
 %srcDir%vao.c^
 %srcDir%clm.c^
 %srcDir%camera.c^
 %srcDir%voxel_renderer.c

set libs=glfw3.lib opengl32.lib user32.lib gdi32.lib shell32.lib ws2_32.lib

set compFlags=/I %incDir% /Zl
set linkFlags=/LIBPATH:%libDir%

mkdir buildfiles

pushd buildfiles
cl /Fe%binDir%%progName% %compFlags% %src% %libs% /link %linkFlags%
popd
