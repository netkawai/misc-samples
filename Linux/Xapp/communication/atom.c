/* 
   Xでは通信するデータやタイプに名前を付けて区別します
   実際にはこの名前に対応する整数値を使います。この
   整数値を「アトム」と呼びます 
*/
/*
  Atom XInternAtom(Display *display,char *atom_name,Bool only_if_exists);
  既にアトムが作られていて,atom_nameに対応するアトムが知りたい時には
  only_if_existsにTrueを新しくアトムを生成する時にはFalseを指定

  char *XGetAtomName(Display *display,Atom atom)
  atomに対応する文字列を得ます
*/
#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>

main()
{
  Display *display;
  Atom atom;
  display = XOpenDisplay(NULL);

  /* XA_CUT_BUFFER0は既に定義済みのアトム */
  printf("Cut buffer 0: %s\n", XGetAtomName(display,XA_CUT_BUFFER0));
  /* Trueなので新しいアトムは生成されない */
  atom = XInternAtom(display,"My String", True);
  printf("Atom for custom string: %d\n",atom);
  /* Falseなので新しいアトムが生成される */
  atom = XInternAtom(display,"My String",False);
  printf("Atom for custom string: %d\n",atom);

  printf("Custom string: %s\n", XGetAtomName(display,atom));
}
