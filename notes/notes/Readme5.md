License Open source community.

GPL(FSF/{Redhat}) vs Apache/BSD/MIT(IBM/Google)

FSF Stance:
End users (Non copy right holder) MUST retain the right to free to access 
the source code of products for modifying and changing the product. 
To maintain the usage of software, the binary format is not enough, end users need 
source code for their purpose.
Pros:
If the products have problems for generating other products or for processing date,
end users can fix problems instantly, if end users have certain programming skill.
The products conduct gaunrantee to inspect mechanism publically.
Cons:
You can not sell the products with high price tag under this license, 
because the source code is the complete design set of product.
You need to enforce some restrictions to users, but the restrictions can not coexit with free.
In economicial view, if people have free right to pay or not pay, majority will not pay, because
money is not free to everyone.
You can not keep the confidential or security processing. Again, this is economicial reason.
Business model: NPO, Service provider, advertisement, selling certain device with the product
GPL declines proprietary

Apache/BSD/MIT Stance:
Users (Non copy right holder) mainly are software developers. 
Users (Non copy right holder) can retain the free right 
to develop open source either propriety software for their purpose.
The end users should rely on their software developer. 
Pros:
Software provider can make proprietary software to obtain limited resource(money).
Licencess can promote the proprietary software to users.
Cons:
End users can not modify it, if the software provider choose their product in non open source.
End users have to pay for their products.
Business model: selling software, service provider, advertisement, selling certain device with the product
Permissive licensee sometime encourage proprietary , or advertisement

In this case, GPL can utilize permissive software but for their purpose, they don't want to encourage the license.
because users don't share their effort to original authors. Authors can not share benefit.
Permissive license can not use GPL, because GPL have restrictions for free of access rather than free of choice.
Voluntarily, users can share their effort.

GNOME(GTK) basically don't want proprietary, support is limited because economical limitation.
Qt have commercial license for economical reason. FSF does not encourage dual license.


SVR4 and BSD, Emacs and Vi, CDE(X11) and Others, WebKit and Gecko, GTK and Qt, GNOME and KDE

Enlightment is BSD-2(simplified BSD license)

Window Manager will not be linked to library throught wayland protocol.

GTK3 is LGPL, however, if I wrap with wxWidgets, if I make the availability of the source code of GTK3+wxWidgets,
the application portion is not required to open the source code.


From:                             |---------------------------------------------|
Xorg Server <- X11 protocol -> Xlib(xcb) -> Xt-> application(xterm)             +
                                  |-> weston <- wayland protocol -> webkit

To:
compositor/mutter/weston<- wayland protocol -> webkit
                  |----> xwayland(xorg)<- X11 protocol -> Xlib(xcb)/Xt -> application(xterm)


gtk+,(gnome) sponsored by Redhat who does not support fully X Window System(MIT/Open Group)
want to obsolate or simplify X11.
