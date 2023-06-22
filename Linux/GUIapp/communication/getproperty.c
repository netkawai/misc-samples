/* 
   プロパティについてはsetproperty.c
   を参照
*/
/*
  int XGetWindowProperty(Display *display,
                         Window window,
			 Atom property,
			 long long_offset,
			 long long_length,
			 Bool delete,
			 Atom req_type,
			 Atom *actual_type_return,
			 int  *actual_format_return,
			 unsigned long *nitems_return,
			 unsigned long *bytes_after_return,
			 unsigned char **prop_return)
   windowはデータを取り出すウィンドウID
   propertyは取り出したいプロパティのアトム
   データをどこからどれだけ取り出すかを
   long_offsetとlong_lengthに32ビットで指定
   データを取り出した後,プロパティを削除したい
   ならTrueそうでなければ,False
   req_type取り出したいデータタイプ
   actual_type_returnは取り出せたデータタイプ
   actual_format_returnは取り出せたデータの
   単位ビット数が8,16,32で返ります。
   nitems_returnは取り出せたデータの要素数
   bytes_after_returnはデータの一部だけ取り出した
   時,残りのバイト数
   prop_returnは取り出せたデータへのポインタ
   この関数は成功するとSuccessを返す
*/
#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>

main()
{
  Display *display;
  Window root,window;
  Atom atom1,atom2,atom3,type;
  int format,*data,i;
  unsigned long nitems,left;
  char *text;

  display = XOpenDisplay(NULL);
  root = RootWindow(display,0);

  atom1 = XInternAtom(display,"My String",False);
  atom2 = XInternAtom(display,"My Interger",False);
  atom3 = XInternAtom(display,"My Window",False);

  XGetWindowProperty(display,root,atom1,0,20,False,
		     XA_STRING,&type,&format,&nitems,
		     &left,&text);
  printf("My String: %s\n",text);

  XGetWindowProperty(display,root,atom2,3,2,False,
		     XA_INTEGER,&type,&format,&nitems,
		     &left,&data);
  for(i = 0 ; i < nitems ; i++)
    printf("My Integer [%d] = %d\n",i,data[i]);

  XGetWindowProperty(display,root,atom3,0,1,False,
		     XA_WINDOW,&type,&format,&nitems,
		     &left,&window);
  printf("My Window: %d\n",window);
}

