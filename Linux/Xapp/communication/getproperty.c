/* 
   �ץ��ѥƥ��ˤĤ��Ƥ�setproperty.c
   �򻲾�
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
   window�ϥǡ�������Ф�������ɥ�ID
   property�ϼ��Ф������ץ��ѥƥ��Υ��ȥ�
   �ǡ�����ɤ�����ɤ�������Ф�����
   long_offset��long_length��32�ӥåȤǻ���
   �ǡ�������Ф�����,�ץ��ѥƥ�����������
   �ʤ�True�����Ǥʤ����,False
   req_type���Ф������ǡ���������
   actual_type_return�ϼ��Ф����ǡ���������
   actual_format_return�ϼ��Ф����ǡ�����
   ñ�̥ӥåȿ���8,16,32���֤�ޤ���
   nitems_return�ϼ��Ф����ǡ��������ǿ�
   bytes_after_return�ϥǡ����ΰ����������Ф���
   ��,�Ĥ�ΥХ��ȿ�
   prop_return�ϼ��Ф����ǡ����ؤΥݥ���
   ���δؿ������������Success���֤�
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
