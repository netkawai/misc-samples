#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>

#include <stdio.h>

iconify(menuwin)
Window menuwin;
{
  XEvent report;
  extern Window focus_window;
  Window assoc_win;
  int press_x,press_y;
  Window child;
  Window root;
  int win_x,win_y;
  unsigned int mask;
  unsigned int button;

  /* Wait for ButtonPress, any win */
  XMaskEvent(display, ButtonPressMask, &report);
  button = report.xbutton.button;

  /* Find out which subwindow the mouse was in */
  XQueryPointer(display,RootWindow(display,screen_num),&root,
      &child, &press_x, &press_y, &win_x, &win_y, &mask);

  /* Can't iconify rootwindow or menu window */
  if((child == NULL) || (child == menuwin))
  {
    /* Wait for ButtonRelease before exiting */
    while(1)
    {
      XMaskEvent(display,ButtonReleaseMask, &report);
      if(report.xbutton.button == button) break;
    }
    return;
  }

  /* Retuned value of isIcon not used here,but
   * it is elsewhere in the code */
  isIcon(child, press_x, press_y, &assoc_win, icon_name, True);
  /* Window selected is unmapped, whether it is icon
   * or main window; the other is then mapped */
  XUnmapWindow(display,child);
  XMapWindow(display,*assoc_win);

  /* Wait for ButtonRelease before exiting */
  /* Get the matching ButtonRelease on same button */
  while(1)
  {
    XMaskEvent(display,ButtonReleaseMask, &report);
    if(report.xbutton.button == button) break;
  }

  /* Throw out any remaining events so we start fresh
   * for next op */
  while(XCheckMaskEvent(display,ButtonReleaseMask |
	  ButtonPressMask, &report))
    ;
}
      
extern Display *display;
extern int screen_num;

/* For Linked list containing window ID, icon ID, and icon_name;
 * own indicates whether winman created the icon window (True)
 * or was passed it through the WMHints (False) */
typedef struct _windowList{
  struct _windowList *next;
  Window window;
  Window icon;
  Bool own;
  char *icon_name;
} WindowListRec, *WindowList;

WindowList Icons = NULL;

Bool isIcon(win, x, y, assoc, icon_name, makeicon)
Window win;
int x,y;
char* icon_name;
Bool makeicon;
{
  WindowList win_list;
  Window makeIcon();

  /* Go through linked list of window-icon structure */
  for(win_list = Icons; win_list; win_list = win_list->next)
  {
    if(win == win_list->icon) /* Win is icon */
    {
      *assoc = win_list->window;
      strcpy(icon_name, win_list->icon_name);
      return (True);
    }
    if(win == win_list->window) /* Win is main window */
    {
      *assoc = win_list->icon;
      strcpy(icon_name, win_list->icon_name);
      return (False);
    }
  }
  /* Window not in list means icon not created yet; create icon
   * and add main window to save-set in case window manager dies */
  if(makeicon)
  {
    *assoc = makeIcon(win,x,y,icon_name);
    XAddToSaveSet(display,win);
  }
  return (False);
}

Window makeIcon(window, x, y, icon_name_return)
Window window;       /* Associated window */
int x,y;             /* Current mouse position */
char *icon_name_return;
{
  int icon_x,icon_y;      /* Icon U. L. X and Y
			   * coordinates */
  int icon_w,icon_h;      /* Icon width and height */
  int icon_bdr;           /* Icon border width */
  int depth;              /* For XGetGeometry */
  Window root;            /* For XGetGeometry */
  



