/* 
   セレクションとはある決められた通信方式である。
   セレクションには次の3種類があります。
    -------------------------
   |セレクション|アトム      |
    -------------------------
   |PRIMARY     |XA_PRIMARY  |
    -------------------------
   |SECONDARY   |XA_SECONDARY|
    -------------------------
*/
/* 
   セレクションを使ったデータ通信でデータの送り主に
   なるにはセレクションを所有しなければならない。
   セレクションの所有者は一人しかいないので,送り主
   を変えるにはセレクションの所有者を移動する
*/
/*
  XSetSelectionOwner(Display *display,
                     Atom selection,
		     Window owner,
		     Time time);
  selectionに所有したいセレクションを指定します。
  一般にXA_PRIMARY,XA_SECONDARYを指定します。

  Window XGetSelectionOwner(display,selection);
  現在セレクションを所有しているウィンドウを得ます。
*/

#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>

Display *display;
Window window;
GC gc;
XFontStruct *font;
Atom property;
Bool selected = False;
char txt[512];
int len = 0;

main()
{
  XEvent event;

  display =  XOpenDisplay(NULL);
  window = XCreateSimpleWindow(display,
			       RootWindow(display,0),
			       400,400,400,50,2,
			       BlackPixel(display,0),
			       WhitePixel(display,0));
  XSelectInput(display,window,
	       KeyPressMask | ButtonPressMask);

  XMapWindow(display,window);
  
  font = XLoadQueryFont(display,"7x14");
  gc = XCreateGC(display,window,0,0);
  XSetFont(display,gc,font->fid);
  XSetForeground(display,gc,BlackPixel(display,0));

  property = XInternAtom(display,"__COPY__TEXT",False);

  while(1)
    {
      XNextEvent(display,&event);
      switch(event.type)
	{
	case KeyPress:
	  DrawText(event);break;
	case ButtonPress:
	  if(selected == False)
	    {
	      if(event.xbutton.button == Button1)
		OwnSelection(event);
	      else if(event.xbutton.button == Button3)
		RequestSelectionData(event);
	    }
	  break;
	case SelectionClear:
	  LoseSelection(event); break;
	case SelectionRequest:
	  StoreSelectionData(event); break;
	case SelectionNotify:
	  RetrieveSelectionData(event); break;
	}
    }
}

DrawText(event)
XEvent event;
{
  KeySym key;
  char string[512];

  if(XLookupString(&event,string,20,&key,NULL) == 1)
    {
      txt[len++] = string[0];
      XClearWindow(display,window);
      XDrawString(display,window,gc,30,30,txt,len);
    }
}

OwnSelection(event)
XEvent event;
{
  XSetSelectionOwner(display,XA_PRIMARY,window,
		     event.xbutton.time);
  if(XGetSelectionOwner(display,XA_PRIMARY)
     == window)
    {
      XSetWindowBackground(display,
			   window,
			   BlackPixel(display,0));
      XSetForeground(display,gc,
		     WhitePixel(display,0));
      XClearWindow(display,window);
      XDrawString(display,window,gc,
		  30,30,txt,len);
      selected = True;
    }
}

LoseSelection(event)
XEvent event;
{
  if(event.xselectionclear.window == window
     && event.xselection.selection == XA_PRIMARY)
    {
      XSetWindowBackground(display,window,
			   WhitePixel(display,0));
      XSetForeground(display,gc,
		     BlackPixel(display,0));
      XClearWindow(display,window);
      XDrawString(display,window,gc,
		  30,30,txt,len);
      selected = False;
    }
}

RequestSelectionData(event)
XEvent event;
{
  XConvertSelection(display,XA_PRIMARY,XA_STRING,
		    property,window,event.xbutton.time);
}

StoreSelectionData(event)
XEvent event;
{
  XSelectionEvent sect_event;
  sect_event.type = SelectionNotify;
  sect_event.requestor = event.xselectionrequest.requestor;
  sect_event.selection = event.xselectionrequest.selection;
  sect_event.target = event.xselectionrequest.target;
  sect_event.time = event.xselectionrequest.time;
  sect_event.property = event.xselectionrequest.property;

  if(event.xselectionrequest.selection == XA_PRIMARY
     && event.xselectionrequest.target == XA_STRING)
    XChangeProperty(display,sect_event.requestor,sect_event.property,
		    sect_event.target,8,PropModeReplace,txt,len);
  else
    sect_event.property = None;

  XSendEvent(display,sect_event.requestor,False,NULL,&sect_event);
}

RetrieveSelectionData(event)
XEvent event;
{
  Atom type;
  char *buff;
  int fmt;
  unsigned long nitems,left;

  if(event.xselection.selection == XA_PRIMARY
     && event.xselection.property == property)
    if(XGetWindowProperty(display,window,property,
			  0,1024,False,XA_STRING,
			  &type,&fmt,&nitems,&left,&buff)
       == Success && type == XA_STRING && nitems > 0)
      {
	strncpy(txt,buff,nitems);
	len = nitems;
	XClearWindow(display,window);
	XDrawString(display,window,gc,30,30,txt,len);
	XFree(buff);
	XDeleteProperty(display,window,property);
      }
}
