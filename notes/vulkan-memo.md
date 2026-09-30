VK_KHR_display      3
VK_KHR_xlib_surface 5
VK_KHR_xcb_surface 6
VK_KHR_wayland_surface 7
-?                     8
VK_KHR_android_surface 9
VK_KHR_win32_surface 10
VK_EXT_directfb_surface 347


Nvidia

Vulkan loader library

libGLX_nvidia.so (for Xlib or Xcb surface)
NVIDIA Linux driver support only xlib surface.

Vulkan and OpenGL/(ES) on X11
X11 client libray(libGLX_nvidia.so)<--> Xorg(glserver_nvidia.so) <--> Linux (NVIDIA Kernel Binary) : X11

In WAYLAND (Mutter)
X11 - OpenGL/(ES) on Wayland
Xwayland  <--> Mesa (GL Software Render) <--> Mutter <--> Linux (NVIDIA Kernel Binary: Do Nothing) : WAYLAND


OpenGL/(ES) - WAYLAND
Mutter(WAYLAND) <--> COGL(EGLStream) <--> Linux (NVIDIA Kernel Binary: EGL Render) : WAYLAND

WAYLAND: Cons
No dependency on X11 session
Copy/paste do work,  X11 apps do not work but, if Gtk(GNOME) is used, some of them work -> most desktop type apps need (terminal, editor, browser etc)
X11: Cons
Provide dynamic backend loading architecture on top of linux (mesa and mutter do not have dynamic backend loading)

Non-X surface swapchain (VK_KHR_display)
App -> libEGL_nvidia.so <--> Linux (NVIDIA Kernel Binary) : Full Screen

