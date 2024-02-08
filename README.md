# CGL
CGL is a 3D voxel engine using OpenGL written in C with as few dependencies as possible (within reason). 

Dependencies:
  - glfw3
  - glad
  - stbi_image

The vector and matrix math library used is handmade and can be found either in this project or at https://github.com/JesseFryer/clm
    
# Build

Windows

To compile, make sure you are in a Developer Command Prompt for Visual Studio so you have 
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
