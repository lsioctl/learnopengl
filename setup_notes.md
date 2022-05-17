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

OpenGL issues
----------

```
libGL error: failed to open /dev/dri/card0: Permission denied
libGL error: failed to open /dev/dri/card0: Permission denied
libGL error: failed to load driver: iris
```

Same with glxinfo:

```
glxinfo | grep render
libGL error: failed to open /dev/dri/card0: Permission denied
libGL error: failed to open /dev/dri/card0: Permission denied
libGL error: failed to load driver: iris
direct rendering: Yes
    GLX_MESA_query_renderer, GLX_MESA_swap_control, GLX_NV_float_buffer, 
    GLX_MESA_query_renderer, GLX_OML_swap_method, GLX_SGIS_multisample, 
Extended renderer info (GLX_MESA_query_renderer):
OpenGL renderer string: llvmpipe (LLVM 13.0.1, 256 bits)
    GL_ARB_conditional_render_inverted, GL_ARB_conservative_depth, 
    GL_NV_conditional_render, GL_NV_copy_image, GL_NV_depth_clamp, 
    GL_ARB_conditional_render_inverted, GL_ARB_conservative_depth, 
    GL_NV_blend_square, GL_NV_conditional_render, GL_NV_copy_depth_to_color, 
    GL_EXT_read_format_bgra, GL_EXT_render_snorm, GL_EXT_robustness, 
    GL_NV_conditional_render, GL_NV_draw_buffers, GL_NV_fbo_color_attachments, 
    GL_OES_element_index_uint, GL_OES_fbo_render_mipmap,
```

1. it does not use nvidia (confirmed with nvidia settings)

When changing prime profile and forcing nvidia

```
 glxinfo | grep render
direct rendering: Yes
OpenGL renderer string: Quadro T1000/PCIe/SSE2
    GL_ARB_conditional_render_inverted, GL_ARB_conservative_depth, 
    GL_NVX_conditional_render, GL_NVX_gpu_memory_info, GL_NVX_nvenc_interop, 
    GL_NV_compute_shader_derivatives, GL_NV_conditional_render, 
    GL_NV_path_rendering, GL_NV_path_rendering_shared_edge, 
    GL_NV_stereo_view_rendering, GL_NV_texgen_reflection, 
    GL_ARB_compute_variable_group_size, GL_ARB_conditional_render_inverted, 
    GL_NVX_conditional_render, GL_NVX_gpu_memory_info, GL_NVX_nvenc_interop, 
    GL_NV_compute_shader_derivatives, GL_NV_conditional_render, 
    GL_NV_path_rendering, GL_NV_path_rendering_shared_edge, 
    GL_NV_stereo_view_rendering, GL_NV_texgen_reflection, 
    GL_EXT_multisample_compatibility, GL_EXT_multisampled_render_to_texture, 
    GL_EXT_multisampled_render_to_texture2, 
    GL_EXT_raster_multisample, GL_EXT_render_snorm, GL_EXT_robustness, 
    GL_NV_conditional_render, GL_NV_conservative_raster, 
    GL_NV_path_rendering, GL_NV_path_rendering_shared_edge, 
    GL_NV_shadow_samplers_cube, GL_NV_stereo_view_rendering, 
    GL_OES_fbo_render_mipmap, GL_OES_geometry_point_size, 
    GL_OVR_multiview_multisampled_render_to_texture

```

```
 nvidia-smi 
Tue May 17 14:24:01 2022       
+-----------------------------------------------------------------------------+
| NVIDIA-SMI 470.103.01   Driver Version: 470.103.01   CUDA Version: 11.4     |
|-------------------------------+----------------------+----------------------+
| GPU  Name        Persistence-M| Bus-Id        Disp.A | Volatile Uncorr. ECC |
| Fan  Temp  Perf  Pwr:Usage/Cap|         Memory-Usage | GPU-Util  Compute M. |
|                               |                      |               MIG M. |
|===============================+======================+======================|
|   0  Quadro T1000        Off  | 00000000:01:00.0 Off |                  N/A |
| N/A   45C    P8     2W /  N/A |    710MiB /  3914MiB |      5%      Default |
|                               |                      |                  N/A |
+-------------------------------+----------------------+----------------------+
                                                                               
+-----------------------------------------------------------------------------+
| Processes:                                                                  |
|  GPU   GI   CI        PID   Type   Process name                  GPU Memory |
|        ID   ID                                                   Usage      |
|=============================================================================|
|    0   N/A  N/A      2646      G   /usr/lib/xorg/Xorg                349MiB |
|    0   N/A  N/A      2862      G   /usr/bin/gnome-shell              138MiB |
|    0   N/A  N/A      3831      G   ...RendererForSitePerProcess       65MiB |
+-----------------------------------------------------------------------------+

```

We have two providers: intel and Nvidia

```
xrandr --listproviders
Providers: number : 2
Provider 0: id: 0x48 cap: 0x9, Source Output, Sink Offload crtcs: 3 outputs: 7 associated providers: 0 name:modesetting
Provider 1: id: 0x29b cap: 0x0 crtcs: 0 outputs: 0 associated providers: 0 name:NVIDIA-G0
```

Solution, *very specific for my hardware*: 

* keep nvidia on demand on Nvidia Config prime's profile (to avoid performance mode with
everything going through Nvidia Graphic Card)
* use proper environment variables to offload to Nvidia GPU in vscode

```
__NV_PRIME_RENDER_OFFLOAD=1 __GLX_VENDOR_LIBRARY_NAME=nvidia glxinfo | grep render
direct rendering: Yes
OpenGL renderer string: Quadro T1000/PCIe/SSE2
    GL_ARB_conditional_render_inverted, GL_ARB_conservative_depth, 
    GL_NVX_conditional_render, GL_NVX_gpu_memory_info, GL_NVX_nvenc_interop, 
...
```

Sources:

https://askubuntu.com/questions/1201072/how-nvidia-on-demand-option-works-in-nvidia-x-server-settings