/* 
   ������ɥ��ˤϥץ��ѥƥ��Ȥ����ΰ褬����,
   �����˥ǡ������Ǽ����,���Ф����Ȥ���
   ���ޤ����ץ��ѥƥ��ϥǡ���,�ǡ�����̾��
   �ȥ�����,�ե����ޥåȤ��鹽������ޤ�
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
  window�ˤϳ�Ǽ���륦����ɥ�ID
  property�ˤ��ѹ�����ץ��ѥƥ���̾����
  ���ȥ����ꤷ�ޤ���
  type,format�ϥǡ����μ���ȳ�Ǽ����
  �ǡ�����ñ�̤����ӥåȤǤ��뤫�����
  ���̤ˤ褯�Ȥ����ΤȤ���
   ------------------------------------------------
  |���ȥ�          |�ǡ���������      |�ե����ޥå�|
   ------------------------------------------------
  |XA_STRING       |ʸ����            |    8       |
   ------------------------------------------------
  |XA_COMPOUND_TEXT|���ܸ�ʤɤ�ʸ����|    8       |
   ------------------------------------------------
  |XA_INTEGER      |������            |    32      |
   ------------------------------------------------
  |XA_WINDOW       |������ɥ�ID      |    32      |
   ------------------------------------------------
  |XA_PIXMAP       |�ԥå����ޥå�ID  |    32      |
   ------------------------------------------------

   mode�ϥǡ������֤���������ˤ�PropModeReplace
   ��¸�Υǡ�����������������ˤ�PropModePrepend
   ������ɲä���ˤ�PropModeAppend�����
   data�˳�Ǽ����ǡ��������
   nelements�ˤ�data�����ǿ���data�ΥХ��ȿ���
   nelements*format/8�ˤʤ�ޤ���
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