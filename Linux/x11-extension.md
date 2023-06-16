As of June 2023, actually mutter has sort of mechanism to disable certain x11 extension.
However, that is only two. why not try to disable all.

I modified weston/src/compositor/xwayland.c (weston-xwayland.c), to test kgames.
So, there are some extensions if disabled in Xwayland, automatically crash due to depedency.
because of cario-graphics, COMPOSITE,DAMAGE,RENDER also trigger crash.
RANDER is sort of mandatory otherwise, Xwayland does not have code to do it.

Check weston-xwayland.c which extensions can be disabled in weston.

The latest X11 protocol defitions are unified into below
https://gitlab.freedesktop.org/xorg/proto/xorgproto
