# CGL
CGL is a 3D rendering engine using OpenGL that is written entirely in C with 
as few dependencies as possible (within reason). The only dependencies are GLFW
and glad. 

Vector and matrix math is handwritten and can be found either in this project
or at https://github.com/JesseFryer/clm

# Build

Windows

Make sure you are in a Developer Command Prompt for Visual Studio so you have 
access to the cl compiler and check it's the x64 version.

```
> git clone https://github.com/JesseFryer/CGL.git
> cd CGL
> mkdir bin
> cd build
> build.bat
```

You can can run the application from the build directory with either run.bat
or br.bat to build and run.

```
> run.bat
> br.bat
```

Or navigate to the bin directory to find CGL.exe

# Features
PHONG lighting model


![](https://github.com/JesseFryer/CGL/blob/main/gifs/showcase1.gif)
