## Implementation of multi-bytes string in X11 core protocol.

Retrieve the code of jpwin but non-ASCII characters should be UTF-8 in source code instead of UCS2 or SJIS or EUC or JIS  sake of Japanese
1.  Install JIS(Japanese Industry Standard) x11 original kanji bitmap fonts.
* Debian/Ubuntu(deb) apt-get install xfonts-jisx0213
* Redhat/Fedora(rpm) rpm -i xorg-x11-fonts-misc
* Archlinux(pacman) pacman -S xorg-fonts-misc
2. Load jiskan16 or  JISX0213 encoding fonts with XLoadQueryFont.
3. Somehow covert from UTF-8 (unsigned char) to JISX0213 (wchar_t) and ISO8859(unsinged char). This is a lookup process, there is no formula need to use table mapping.
   probably I can steal from original Mule-UCS with updated Unicode version.
4. Set w_char value to XChar2b structure.
5. To switch between ISO8859 and JISx0213, I need to create at least two GCs and set each font and switch between DrawString and DrawString16, on the purpose,
I need a flag in struct to keep a specific character which functions I should call

IMPORTANT: The X11 core or xorg never had the sort of automatic font fallback mechanism (in encoding). if the height of both half-width (ASCII/ISO8859/JISx0201) characters and full-width(multi-byte/JISx0213) characters needed to be matched, unless that, it was really hard to draw them in the same line with a correct(appropriate) baseline in X11 client side since those are bitmap fonts(not scalable). Because of that, X11 was not practical in non-Latin countries. Mule-UCS was one of applications to try to implement it to use the combination of jiskan16 and 8x16 fonts or k14 and 7x14 fonts. However after dissolution of X Consortium in late 1990s, drawing functions shifted away from x11 core because of scalable fonts. That makes the necessity to have a scalable font, unlike alphabet, Japanese needs thousands character for the practical use, due to economic boom in 1980s in Japan, jiskan16 was created by an university professor, k14 by a student around late 1980s [https://ja.wikipedia.org/wiki/Jiskan]. But, the open license high quality scalable font of Japanese only appeared at 2014 by Google as Noto Sans CJK. So the scalable Japanese font was not available almost 25 years!!.

More over until 201x, it was still common to use ISO2022 or EUC-JP or Shift-JIS, non UTF-8 encoding in LC_TYPE or PO files. That makes much more difficult to adopt it. because in Japan, at that time, already popularized 3 different encodings (thanks to big cooperations and universities). Shift-JIS in business(consumer) by Microsoft/ASCII cooperation, EUC-JP by IBM Japan and universities, E-mail by ISO-2022-JP for required 7bit clean. That is the reason, https://en.wikipedia.org/wiki/Mojibake became so common and we needed to have an option to switch encoding.

## Investigation fox Xt

1. When I have read this article https://keithp.com/blogs/kgames/. I ran v2.3 with cairo and fontconfig, It worked, but the layout of xmille had been broken to fit FHD(1920x1080) monitor. I think he would be using a 4k monitor. I tried spider with 2.3. It is ok. So I tried 1.0 and 1.0 ~~did not~~ worked. When I run kspider without loading X resources.
```
$ ./kspider -g 400x400
Error: Widget menuBar has zero width and/or height
```
This indicates that the menu of Xt/Athena Widget of height becomes zero. ~~It means, they could not load font or they could not retrieve Text Height or Width in my guess. I am curious that this is Xwayland issue or the client library issue.~~ Because I did not load X resources (.ad files) before starting programs. I don't know what represent ad? Anyway, the Xkw/layout widget has y(bison/yacc) and l(lex) file to parse the X resources to specify the layout instead of hard coding in the source code. I realized that I needed to load those files with xrdb. I tried xmille with 1.0. The layout of xmille(1.0) was perfect.
after 2000s, xorg went completely away from xorg server without making obsolete, it is annoying.
In modern sense, X11 handles only two aspects which are window(drawsurface) management and user input, that's it. Those X resources had been obsolete, since X resources are stored in X server, and cairo is the client-side library, they are not related each other. It is quite confusing. I don't check the reason of xmille layout issue. Most likely, he did not implement scaling down cards. All cards are clipped out in xmille to fit FHD resolution.

  