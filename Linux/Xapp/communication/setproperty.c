/* 
   ウィンドウにはプロパティという領域があり,
   ここにデータを格納して,取り出すことがで
   きます。プロパティはデータ,データの名前
   とタイプ,フォーマットから構成されます
*/
/*
  XChangeProperty(Display *display,
                  Window window,
		  Atom property,
		  Atom type,
		  int format,
		  int mode,
		  unsigned char* data,
		  int nelements)
  windowには格納するウィンドウID
  propertyには変更するプロパティの名前の
  アトムを指定します。
  type,formatはデータの種類と格納する
  データの単位が何ビットであるかを指定
  一般によく使われるものとして
   ------------------------------------------------
  |アトム          |データタイプ      |フォーマット|
   ------------------------------------------------
  |XA_STRING       |文字列            |    8       |
   ------------------------------------------------
  |XA_COMPOUND_TEXT|日本語などの文字列|    8       |
   ------------------------------------------------
  |XA_INTEGER      |整数値            |    32      |
   ------------------------------------------------
  |XA_WINDOW       |ウィンドウID      |    32      |
   ------------------------------------------------
  |XA_PIXMAP       |ピックスマップID  |    32      |
   ------------------------------------------------

   modeはデータを置き換える場合にはPropModeReplace
   既存のデータの前に挿入するにはPropModePrepend
   後ろに追加するにはPropModeAppendを指定
   dataに格納するデータを指定
   nelementsにはdataの要素数。dataのバイト数は
   nelements*format/8になります。
*/
#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>

main()
{
  Display *display;
  Window root,window;
  Atom atom1,atom2,atom3;
  static char s1[] = "Body",s2[] = "Head ",s3[] = " Tail";
  int i,data[10];
  
  display = XOpenDisplay(NULL);
  root = RootWindow(display,0);

  atom1 = XInternAtom(display,"My String",False);
  atom2 = XInternAtom(display,"My Interger",False);
  atom3 = XInternAtom(display,"My Window",False);
  
  XChangeProperty(display,root,atom1,XA_STRING,8,
		  PropModeReplace,s1,4);
  XChangeProperty(display,root,atom1,XA_STRING,8,
		  PropModeReplace,s2,5);
  XChangeProperty(display,root,atom1,XA_STRING,8,
		  PropModeReplace,s3,5);
  
  for(i = 0 ; i < 10 ; i++) data[i] = i;
  XChangeProperty(display,root,atom2,XA_INTEGER,32,
		  PropModeReplace,data,10);

  window = XCreateSimpleWindow(display,root,
			       200,200,100,100,2,
			       0,1);
  XChangeProperty(display,root,atom3,XA_WINDOW,32,
		  PropModeReplace,&window,1);
  XFlush(display);
}
