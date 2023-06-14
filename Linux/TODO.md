TODO: Disable all extension and try to run electron

As of June 2023, actually mutter has sort of mechanism to disable certain x11 extension.
However, that is only two. why not try to disable all extension

In xserver source code, miinitext.c
Mandatory extensions are XInputExtension, BIG-REQUESTS, SyncExtension, XKEYBOARD, XC-MISC, NOTE: Present cannot disable dynamically, but, it looks,
can be disabled by build time