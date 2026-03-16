
Firefox 148.0
Gemini
Responding with 3 Pro Preview
Based on the provided context (specifically browser.css which confirms the use of subsurfaces) and the known architecture of Firefox's Wayland backend (located in widget/gtk and gfx), here is the list of Wayland protocols and extensions Firefox interacts with directly or relies upon for critical functionality.

Although Firefox uses GTK for the UI toolkit, it bypasses GTK to speak directly to the Wayland compositor for high-performance graphics (WebRender), video acceleration, and input handling.

Directly Used Wayland Protocols & Extensions
Firefox (via its internal MozWaylandDisplay, nsWindow, and DMABufSurface implementations) uses the following:

Protocol	Version	Usage in Firefox	Impact if Missing
wl_compositor	4	Core. Creates surfaces and regions.	Fatal. App won't start.
wl_shm	1	Core. Shared memory buffers (fallback rendering, cursors).	Fatal. App won't start.
wl_subcompositor	1	Confirmed in browser.css. Used to composite WebRender content (OpenGL/Vulkan) independently of the main GTK window frame.	Rendering artifacts or failure to display web content.
wl_seat	7	Input devices (keyboard, pointer, touch).	No input.
wl_output	4	Monitor information (DPI, refresh rate).	Incorrect scaling or frame pacing.
linux-dmabuf-unstable-v1	3/4	Critical. Zero-copy texture sharing. Allows the GPU to write directly to buffers Firefox sends to the compositor.	High Impact. Fallback to software copying (slow), high CPU usage, likely no WebGL/Video hardware acceleration.
wp_viewporter	1	Used with DMABUF to crop and scale video surfaces on the compositor side.	Videos may look incorrect, scaling will be inefficient (done by CPU/GPU shader instead of display hardware).
xdg_wm_base	2	Managed by GTK mostly, but Firefox tracks states (fullscreen, toplevel).	Window management breaks.
zxdg_decoration_manager_v1	1	Negotiates Server-Side Decorations (SSD) vs Client-Side Decorations (CSD).	Firefox may force CSD (drawing its own tabs/borders) even if you want the compositor to do it.
zwp_idle_inhibit_manager_v1	1	Prevents screen locking during video playback.	Screen turns off while watching YouTube/Netflix.
zwp_relative_pointer_v1	1	Mouse locking for FPS games (Web gaming).	Games relying on "mouselook" won't work.
zwp_pointer_constraints_v1	1	Confines pointer to window (Web gaming).	Cursor escapes window in games.
xdg_activation_v1	1	Token passing to focus windows (e.g., clicking a link in Thunderbird opens Firefox).	Clicking links in other apps won't bring Firefox to the foreground.
wp_fractional_scale_v1	1	Allows rendering at non-integer scales (e.g., 1.25x) without blur.	Firefox renders at 2x and downscales (blurry/expensive) or 1x (too small) on some monitors.
zwp_primary_selection_device_manager_v1	1	Middle-click paste (Clipboard).	Middle-click paste stops working.
Impact Analysis by Compositor
Since you are switching between Weston, Hyprland, Sway, and labwc, here is how they interact with the list above.

1. Weston (Reference Implementation)
Pros: It is the reference. dmabuf, viewporter, and core protocols are extremely stable.
Impact:
Decorations: Weston's reference shell often lacks zxdg_decoration_manager_v1 support, forcing Firefox to draw its own titlebars (CSD).
Gaming: Good support for pointer constraints/relative pointer.
Clipboard: Standard clipboard works, primary selection usually supported.
2. Sway & Hyprland (wlroots-based)
Both use wlroots, so they share very similar protocol support.

Pros: Best-in-class support for wlr-protocols and standard extensions Firefox uses.
Impact:
Decorations: Both support zxdg_decoration_manager_v1. You can configure Firefox to hide its CSD (title bar) and let Sway/Hyprland handle borders (MOZ_GTK_TITLEBAR_DECORATION=system or via compositor config).
Fractional Scaling: Hyprland and Sway have robust support for wp_fractional_scale_v1 (check your specific version). Firefox looks crisp at 1.5x scale.
DMABUF/Viewporter: Fully supported. Hardware acceleration works well.
Focus: xdg_activation_v1 is supported, so opening links from external apps works reliably.
3. labwc (wlroots-based, Openbox-like)
Pros: Lightweight, also uses wlroots.
Impact:
Decorations: Strong support for SSD (Server Side Decorations). Firefox will likely look most "native" here if you prefer window borders managed by the WM.
General: Since it is wlroots-based, it shares the high compatibility of Sway/Hyprland for DMABUF and input protocols.
Summary of Risks
If you move away from a full desktop environment (GNOME/KDE) to these compositors:

Missing Portals: Ensure you have xdg-desktop-portal-wlr (or xdg-desktop-portal-hyprland) installed. Firefox uses this (via DBus, not Wayland directly) for file pickers and screen sharing. Without it, you cannot share your screen in Google Meet/Zoom.
Fractional Scaling: If you use a high-DPI monitor, Hyprland/Sway are generally better than Weston for sharp text rendering due to newer protocol implementation.
Video Acceleration: Ensure your mesa drivers are up to date. The linux-dmabuf protocol is sensitive to driver/compositor mismatches, though wlroots compositors are generally very compliant