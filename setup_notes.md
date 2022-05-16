glfw
------

GLFW stands for Graphics Library Framework. It provides programmers with the ability to create and manage windows and OpenGL contexts, as well as handle joystick, keyboard and mouse input.

 apt-get install libxrandr-dev
  594  apt-get install libxinerama-dev
  595  apt-get install libxcursor-dev

cmake -DBUILD_SHARED_LIBS=ON -DCMAKE_INSTALL_PREFIX=../install ..

opengl
---------

ii  libopengl-dev:amd64                                   1.4.0-1                                    amd64        Vendor neutral GL dispatch library -- OpenGL development files

cat /usr/lib/x86_64-linux-gnu/pkgconfig/opengl.pc
prefix=/usr
libdir=${prefix}/lib/x86_64-linux-gnu
includedir=${prefix}/include

Name: OpenGL
Description: OpenGL (without GLX) library and headers.
Version: 4.5
Libs: -L${libdir} -lOpenGL
Cflags: -I${includedir}


glad
------

From the learnopengl:

Go to the GLAD web service, make sure the language is set to C++, and in the API section select an OpenGL version of at least 3.3 (which is what we'll be using; higher versions are fine as well). Also make sure the profile is set to Core and that the Generate a loader option is ticked. Ignore the extensions (for now) and click Generate to produce the resulting library files. 


https://glad.dav1d.de/#language=c&specification=gl&api=gl%3D3.3&api=gles1%3Dnone&api=gles2%3Dnone&api=glsc2%3Dnone&profile=compatibility&loader=on

VSCode
------

Issue with LD_LIBRARY_PATH

https://github.com/microsoft/vscode-cpptools/issues/616

Solved: it was in fact with gdb, which seems to no interpret the `~`




