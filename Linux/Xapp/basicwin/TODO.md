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

1. When I have read this article https://keithp.com/blogs/kgames/. I was curious that whether I can run v1.0 or not. I knew v2.3 with cairo and fontconfig, it will work (ya it worked) in modern desktop environment (Xwayland). Behold, but v1.0 did not. When I run kreversi without command line options.
```
$ ./kreversi
Error: Shell widget kreversi has zero width and/or height
```
It seems the main window size becomes undefined. so I specified with -g option. It appears but, no texts on the window
I tried with kspider
```
$ ./kspider -g 400x400
Error: Widget menuBar has zero width and/or height
```
This indicates that the menu of Xt/Athena Widget of height becomes zero. It means, they could not load font or they could not retrieve Text Height or Width in my guess.
I am curious that this is Xwayland issue or the client library issue.
I know since American universities had(still have?) million of X Terminals and/or PC based xorg-server, they will not throw away x11 anytime soon. but it is annoying.
In modern sense, X11 handles only two aspects which are window(drawsurface) management and user input, that's it.
  