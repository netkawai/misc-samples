## kgames

1. When I have read this article https://keithp.com/blogs/kgames/. I downloaded from [https://salsa.debian.org/debian/kgames]. I ran v2.3 with cairo and fontconfig, It worked, but the layout of xmille had been broken to fit FHD(1920x1080) monitor. I think he must be using a 4k monitor. I tried spider with 2.3. It is ok. So I tried 1.0 and 1.0 ~~did not~~ worked. When I run kspider without loading X resources.
```
$ ./kspider -g 400x400
Error: Widget menuBar has zero width and/or height
```
This indicates that the menu of Xt/Athena Widget of height becomes zero. ~~It means, they could not load font or they could not retrieve Text Height or Width in my guess. I am curious that this is Xwayland issue or the client library issue.~~ Because I did not load X resources (.ad files) before starting programs. I don't know what represent ad? Anyway, the Xkw/layout widget has y(bison/yacc) and l(lex) file to parse the X resources to specify the layout instead of hard coding in the source code. I realized that I needed to load those files with xrdb. I tried xmille with 1.0. The layout of xmille(1.0) was perfect.
after 2000s, xorg went completely away from xorg server without making obsolete, it is annoying.
In modern sense, X11 handles only two aspects which are window(drawsurface) management and user input, that's it. Those X resources had been obsolete, since X resources are stored in X server, and cairo is the client-side library, they are not related each other. It is quite confusing. I don't check the reason of xmille layout issue. Most likely, he did not implement scaling down cards. All cards are clipped out in xmille to fit FHD resolution.

Since the xrender was the breaker of the original principal of X protocol. I think I forked from the debian to github. THen I will try to replace X11/Xt part to wayland/weston-desktop library. Kiss is more important than compatibility (see the gtk)

Low priority: I wonder how difficult it is to fix xmille in FHD? simply scaling down cards? or I need to have a mechanism to choose proper resources depends on DPI?


  