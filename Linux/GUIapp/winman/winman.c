#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/Xos.h>
#include <X11/cursorfont.h>

#include <stdio.h>
#include <signal.h>

/* #include "bitmaps/focus_fram_bi" */ /* Name must be <= 14 chars
                                        * for sys V compatibility */

#define MAX_CHOICE 10
#define DRAW 1
#define ERASE 0
#define RAISE 1
#define LOWER 0
#define MOVE 1
#define RESIZE 0
#define NONE 100
#define NOTDEFINED 0
#define BLACK 1
#define WHITE 0

Window focus_window;
Window inverted_pane = NONE;

static char *menu_label[] = 
{
  "Raise",
  "Lower",
  "Move",
  "Resize",
  "CirculateDn",
  "CirculateUp",
  "(De)Iconify",
  "Kybrd Focus",
  "New Xterm",
  "Exit",
};

Display *display;
int screen_num;
XFontStruct *font_info;
char icon_name[50];

main()
{
  Window menuwin;
  Window panes[MAX_CHOICE];
  int menu_width, menu_height, x = 0, y = 0, border_width = 4;
  int winindex;
  int cursor_shape;
  Cursor cursor, hand_cursor;
  char *font_name = "9x15";
  int direction,ascent, descent;
  int char_count;
  char *string;
  XCharStruct overall;
  Bool owner_events;
  int pointer_mode;
  int keyboard_mode;
  Window confine_to;
  GC gc,rgc;
  int pane_height;
  Window assoc_win;
  XEvent event;
  unsigned int button;

  if((display=XOpenDisplay(NULL)) == NULL ){
    (void) fprintf(stderr, "winman: cannot connect to Xserver %s\n", XDisplayName(NULL));
    exit(-1);
  }
    
  screen_num = DefaultScreen(display);
  
  /* Access font */
  font_info = XLoadQueryFont(display,font_name);
  if(font_info == NULL){
    (void)fprintf(stderr,"winman: Cannot open font %s\n", font_name);
    exit(-1);
  }

  string = menu_label[6];
  char_count = strlen(string);

  /* Determine for the extent of each menu pane based on
   * the font size */

  XTextExtents(font_info, string, char_count, &direction, &ascent,
	       &descent, &overall);
  
  menu_width = overall.width + 4;
  pane_height = overall.ascent + overall.descent + 4;
  menu_height = pane_height * MAX_CHOICE;
  
  /* Place the window in upper-right corner */
  x = DisplayWidth(display,screen_num) - menu_width - (2*border_width);
  y = 0; /* Appears at loop */

  menuwin = XCreateSimpleWindow(display,RootWindow(display,
             screen_num), x, y, menu_width, menu_height,
	     border_width, BlackPixel(display,screen_num),
	     WhitePixel(display,screen_num));

  /* Create the choice windows for the text */
  for(winindex = 0; winindex < MAX_CHOICE; winindex++){
    panes[winindex] = XCreateSimpleWindow(display, menuwin, 0,
	   menu_height/MAX_CHOICE*winindex, menu_width,
	   pane_height, border_width = 1,
	   BlackPixel(display,screen_num),
	   WhitePixel(display,screen_num));
    XSelectInput(display, panes[winindex], ButtonPressMask
		 | ButtonReleaseMask | ExposureMask);
  }

  XSelectInput(display, RootWindow(display, screen_num),
          SubstructureNotifyMask);

  /* These do not appear until parent (menuwin) is mapped */
  XMapSubwindows(display,menuwin);

  /* Create the cursor for the menu */
  cursor = XCreateFontCursor(display, XC_left_ptr);
  hand_cursor = XCreateFontCursor(display, XC_hand2);

  XDefineCursor(display, menuwin, cursor);

  focus_window = RootWindow(display, screen_num);

  /* Create two graphics contexts for inverting panes (white
   * and black). We invert the panes by changing the background
   * pixel, clearing the window, and using the GC with the
   * contrasting color to redraw the text. Another way is using
   * XCopyArea. The default is to generate GraphicsExpose and
   * NoExpose events to indicate whether the source area was
   * obscured. Since the logical function is GXinvert, the
   * destination is also the source. Therefore, if other
   * windows are obscuring parts of the exposed pane, the
   * wrong area will be inverted. Therefore, we would need
   * to handle GraphicsExpose and NoExpose events. We'll do
   * it the eaier way. */

  gc = XCreateGC(display, RootWindow(display, screen_num), 0,
	  NULL);
  XSetForeground(display, gc, BlackPixel(display,screen_num));
  rgc = XCreateGC(display,RootWindow(display, screen_num), 0,
	  NULL);
  XSetForeground(display, rgc, WhitePixel(display, screen_num));

  /* Map the menu window (and its subwindows) to the screen_num */
  XMapWindow(display, menuwin);
  XFlush(display);
  /* Force child processes to disinherit the TCP file description;
   * this helps the shell command (creating new xterm) forked and
   * executed from the menu to work properly */
  if((fcntl(ConnectionNumber(display), F_SETFD, 1)) == -1)
     fprintf(stderr, "winman: child cannot disinherit TCP fd");

  /* Loop getting events on the menu window and icons */
  while (1){
    /* Wait for an event */
    XNextEvent(display,&event);
    
    /* If expose, draw text in pane if it is pane */
    switch(event.type) {
    case Expose:
      /*  if(isIcon(event.xexpose.window, event.xexpose.x,
		event.xexpose.y, &assoc_win, icon_name, False))
	XDrawString(display, event.xexpose.window, gc, 2,
		    ascent + 2, icon_name, strlen(icon_name));
      else */ { /* It's a pane, might be inverted */
	if(inverted_pane == event.xexpose.window)
	  paint_pane(event.xexpose.window, panes, gc, rgc,
		     BLACK);
	else
	  paint_pane(event.xexpose.window, panes, gc, rgc,
		     WHITE);
      }
      break;
    case ButtonPress:
      paint_pane(event.xbutton.window, panes, gc, rgc, BLACK);

      button = event.xbutton.button;
      inverted_pane = event.xbutton.window;

      /* Get the matching ButtonRelease on same button */
      while(XCheckTypedEvent(display, ButtonPress,
			     &event));
      /* Wait for release; if on correct button, exit */
      XMaskEvent(display, ButtonReleaseMask, &event);
      if(event.xbutton.button == button)
	break;
    }
    /* All events are sent to the grabbing window
     * regrardless of whether this is True or False;
     * owner_events only affects the distribution
     * of events when pointer is within this
     * application's windows */
    owner_events = True;

    /* We don't want pointer or keyboard events
     * frozen in the server */
    pointer_mode = GrabModeAsync;
    keyboard_mode = GrabModeAsync;

    /* We don't want to confine the cursor */
    confine_to = None;
    XGrabPointer(display, menuwin, owner_events,
		 ButtonPressMask | ButtonReleaseMask,
		 pointer_mode, keyboard_mode,
		 confine_to, hand_cursor, CurrentTime);

    /* If press and release occurred in same window,
     * do command; if not, do nothing */
    if(inverted_pane == event.xbutton.window){
      /* Convert window ID to window array index */
      for(winindex = 0; inverted_pane !=
	    panes[winindex]; winindex++)
	;
      switch(winindex){
      case 0:
	raise_lower(menuwin, RAISE);
	break;
      case 1:
	raise_lower(menuwin, LOWER);
	break;
      case 4:
	circup(menuwin);
	break;
      case 5:
	circdn(menuwin);
	break;
      case 9: /* Exit */
	XSetInputFocus(display,
		       RootWindow(display,screen_num),
		       RevertToPointerRoot,
		       CurrentTime);
	/* Turn all icons back into windows */
	/* Must clear focus highlights */
	XClearWindow(display,RootWindow(display,
	       screen_num));
	/* Need to change focus border width back here */
	XFlush(display);
	XCloseDisplay(display);
	exit(1);
	break;
      }
    }
  }
}
    
paint_pane(window, panes, ngc, rgc, mode)
Window window;
Window panes[];
GC ngc, rgc;
int mode;
{
  int win;
  int x = 2, y;
  GC gc;

  if(mode == BLACK) {
    XSetWindowBackground(display, window, BlackPixel(display,
	     screen_num));
    gc = rgc;
  }
  else {
    XSetWindowBackground(display, window, WhitePixel(display,
	      screen_num));
    gc = ngc;
  }
  /* Clearing repaints the background */
  XClearWindow(display, window);

  /* Findo out index of window for label text */
  for(win = 0; window != panes[win]; win++)
    ;

  y = font_info->max_bounds.ascent;

  /* The string length is necessary because strings
   * for XDrawString may not be null terminated */
  XDrawString(display, window, gc, x, y, menu_label[win],
	      strlen(menu_label[win]));
}

circup(menuwin)
Window menuwin;
{
  XCirculateSubwindowsUp(display, RootWindow(display,screen_num));
  XRaiseWindow(display, menuwin);
}

circdn(menuwin)
Window menuwin;
{
  XCirculateSubwindowsDown(display, RootWindow(display,screen_num));
  XRaiseWindow(display, menuwin);
}

raise_lower(menuwin, raise_or_lower)
Window menuwin;
Bool raise_or_lower;
{
  XEvent report;
  int root_x,root_y;
  Window child, root;
  int win_x, win_y;
  unsigned int mask;
  unsigned int button;

  /* Wait for ButtonPress, find out which subwindow of root */
  XMaskEvent(display, ButtonPressMask, &report);
  button = report.xbutton.button;
  XQueryPointer(display, RootWindow(display,screen_num),&root,
		&child, &root_x, &root_y, &win_x, &win_y,
		&mask);

  /* If not RootWindow, raise */
  if(child != NULL) {
    if(raise_or_lower == RAISE)
      XRaiseWindow(display, child);
    else
      XLowerWindow(display,child);
    /* Make sure window manager can never be obscured */
    XRaiseWindow(display,menuwin);
  }

  /* Get the matching ButtonRelease on same button */
  while(1){
    XMaskEvent(display, ButtonReleaseMask, &report);
    if(report.xbutton.button == button) break;
  }
  /* Throw out any remaining events so we start fresh */
  while(XCheckMaskEvent(display, ButtonReleaseMask |
			ButtonPressMask, &report))
    ;
}

move_resize(menuwin, hand_cursor, move_or_resize)
Window menuwin;
Cursor hand_cursor;
Bool move_or_resize;
{
  XEvent report;
  XWindowAttributes win_attr;
  int press_x, press_y, release_x, release_y, move_x, move_y;
  static int box_drawn = False;
  int left, right, top, bottom;
  Window root, child;
  Window win_to_configure;
  int win_x, win_y;
  unsigned int mask;
  unsigned int pressed_button;
  XSizeHints size_hints;
  Bool min_size, increment;
  unsigned int width, height;
  int temp_size;
  static GC gc;
  static int first_time = True;
  long user_supplied_mask;

  if(first_time){
    gc = XCreateGC(display, RootWindow(display,screen_num), 0,
		   NULL);
    XSetSubwindowMode(display, gc, IncludeInferiors);
    XSetForeground(display, gc, BlackPixel(display, screen_num));
    XSetFunction(display, gc, GXxor);
    first_time = False;
  }

  /* Wait for ButtonPress choosing window to configure */
  XMaskEvent(display, ButtonPressMask, &report);
  pressed_button = report.xbutton.button;

  /* Which child of root was press in? */
  XQueryPointer(display, RootWindow(display,screen_num), &root,
		&child, &press_x, &press_y, &win_x,
		&win_y, &mask);
  win_to_configure = child;
  if((win_to_configure == NULL) ||
     ((win_to_configure == menuwin)
      && (move_or_resize == RESIZE))){
    /* If in RootWindow or resizing menuwin,
     * get release event and get out */
    while(XCheckMaskEvent(display,ButtonReleaseMask |
			  ButtonPressMask,&report))
      ;
    return;
  }
  
  /* Button press was in a valid subwindow of root */
  /* Get original position and size of window */
  XGetWindowAttributes(display, win_to_configure,
		       &win_attr);
  /* Get size hints for the window */
  XGetWMNormalHints(display, win_to_configure, &size_hints,
		    &user_supplied_mask);
  
  if(size_hints.flags && PMinSize)
    min_size = True;
  if(size_hints.flags && PResizeInc)
    increment = True;

  /* Now we need pointer motion events */
  XChangeActivePointerGrab(display,PointerMotionHintMask |
	 ButtonMotionMask | ButtonReleaseMask |
	 OwnerGrabButtonMask, hand_cursor, CurrentTime);
  /* Don't allow other display operations during move
   * because the moving outline drawn with Xor won't
   * work properly otherwise */
  XGrabServer(display);

  /* Move outline of window until button release */
  while(1){
    XNextEvent(display, &report);
    switch(report.type){
    case ButtonRelease:
      if(report.xbutton.button == pressed_button){
	if(box_drawn)
	  /* Undraw box */
	  draw_box(gc, left, top, right, bottom);

	/* This may seem premature but actually
	 * ButtonRelease indicates that the
	 * rubber-banding is done */
	XUngrabServer(display);

	/* Get final window position */
	XQueryPointer(display, RootWindow(display,
	       screen_num), &root, &child,
	       &release_x, &release_y, &win_x,
	       &win_y, &mask);

	/* Move or resize window */
	if(move_or_resize == MOVE)
	  XMoveWindow(display, win_to_configure,
	      win_attr.x + (release_x -
	      press_x), win_attr.y +
	      (release_y - press_y));
	else
	  XResizeWindow(display, win_to_configure,
	    win_attr.width + (release_x - press_x),
	    win_attr.height + (release_y - press_y));

	XRaiseWindow(display, win_to_configure);
	XFlush(display);
	box_drawn = False;
	while(XCheckMaskEvent(display,
	      ButtonReleaseMask
	      | ButtonPressMask,
	      &report))
	  ;
	return;
      }
      break;

    case MotionNotify:
      if(box_drawn == True)
	/* Undrawn box */
	draw_box(gc, left, top, right, bottom);

      /* Can get rid of all MotionNotify events in
       * queue, since otherwise the round-trip delays
       * caused by XQueryPointer may cause a backlog
       * of MotionNotify events, which will cause
       * additional wasted XQueryPointer calls */
      while(XCheckTypedEvent(display, MotionNotify,
	    &report));

      /* Get current mouse position */
      XQueryPointer(display, RootWindow(display,
	    screen_num), &root, &child, &move_x,
	    &move_y, &win_x, &win_y, &mask);

      if(move_or_resize == MOVE){
	left = move_x - press_x + win_attr.x;
	right = move_y - press_y + win_attr.y;
	bottom = top + win_attr.height;
      }
      else
	{
	  if(move_x < win_attr.x)
	    move_x = 0;
	  if(move_y < win_attr.y)
	    move_y = 0;
	  left = win_attr.x;
	  top = win_attr.y;
	  right = left + win_attr.width + move_x
	        - press_x;
	  bottom = top + win_attr.height + move_y
	         - press_y;
	  /* Must adjust size according to size hints */
	  /* Enforce minimum dimensions */
	  width = right - left;
	  height = bottom - top;
	  /* Make sure dimension are increment of
	   * width_inc and height_inc and at least
	   * min_width and min_height */
	  for(temp_size = size_hints.min_width;
	      temp_size < width;
	      temp_size += size_hints.width_inc)
	    ;

	  for(temp_size = size_hints.min_height;
	      temp_size < height;
	      temp_size += size_hints.height_inc)
	    ;
	  /* Most applications (xterm
	   * included) pad their right
	   * and bottom dimesions by
	   * 2 pixels */
	  bottom = top + temp_size + 2;
	  right = left + temp_size + 2;
	}
      
      draw_box(gc, left, top, right, bottom);
      box_drawn = True;
      break;
    default:
      ;
    }
  }
}

draw_box(gc, x, y, width, height)
GC gc;
int x,y;
unsigned int width, height;
{
  /* Set foreground pixel value -- default may be white on white */
  XSetForeground(display, gc, BlackPixel(display,screen_num));

  /* Drawing on root window -- through all windows */
  XSetSubwindowMode(display, gc, IncludeInferiors);

  /* Logical function is XOR, so that double drawing erase box
   * on both color and monochrome screens */
  XSetFunction(display, gc, GXxor);

  XDrawRectangle(display, RootWindow(display,screen_num), gc, x, y,
		 width, height);
}

