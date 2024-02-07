@echo off

set progName=CGL
set binDir=..\..\bin\
set srcDir=..\..\src\
set depDir=..\..\deps\
set libDir=%depDir%lib\
set incDir=%depDir%include\

set src=%srcDir%main.c^
 %depDir%src\glad.c^
 %depDir%src\stbi_image.c^
 %srcDir%input.c^
 %srcDir%shader.c^
 %srcDir%vao.c^
 %srcDir%clm.c^
 %srcDir%camera.c^
 %srcDir%voxel_renderer.c^
 %srcDir%texture.c^
 %srcDir%cgl.c^
 %srcDir%perlin.c

set libs=glfw3.lib opengl32.lib user32.lib gdi32.lib shell32.lib ws2_32.lib

set compFlags=/I %incDir% /Zl
set linkFlags=/LIBPATH:%libDir%

mkdir buildfiles

pushd buildfiles
cl /Fe%binDir%%progName% %compFlags% %src% %libs% /link %linkFlags%
popd
