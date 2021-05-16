/* Xlib include files */
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>

/* Standard C include file */
#include <stdio.h>

/* Bitmap data for icon */
#include "basicwin_icon.xbm"

#define BITMAPDEPTH 1

/* Values for window_size in main -- is window big enough to be
 * useful? */
#define TOO_SMALL 0
#define BIG_ENOUGH 1

/* Display and screen_num are used as arguments toto nearly every
 * Xlib routine, so it simplifies routine calls to declare them
 * global; if there were additional source files, these variables
 * would be declared "extern" in them */
Display *display;
int screen_num;

/* Name this program was invoked by; this is global because
 * it is used in several places in application routines,not
 * just in main */
static char *progname;

void main(int argc,char **argv)
{
  Window win;
  unsigned int width,height; /* Window size */
  int x = 0, y = 0;          /* Window position */
  unsigned int border_width = 4; /* Border four pixels wide */
  unsigned int display_width,display_height;
  char *window_name = " Basic Window Program ";
  char *icon_name = "basicwin";
  Pixmap icon_pixmap;
  int count;
  XSizeHints size_hints;  /* Preferred sizes for window man */
  XEvent report;          /* Structure for event information */
  GC gc;                  /* ID of graphics context */
  XFontStruct *font_info; /* Structure containing font information */
  int window_size = 0;
  char *display_name = NULL; /* Server to connect to */
  
  progname = argv[0];

  /* Connect to X Server */
  if((display=XOpenDisplay(display_name)) == NULL )
  {
    (void) fprintf(stderr,"%s: cannot connect to \
           X Server %s\n",progname, XDisplayName(display_name));
    exit(-1);
  }

  screen_num = DefaultScreen(display);
  
  display_width = DisplayWidth(display,screen_num);
  display_height = DisplayHeight(display,screen_num);

  width = display_width/3, height = display_height/4;

  win = XCreateSimpleWindow(display,RootWindow(display,screen_num),
	    x, y, width, height, border_width, BlackPixel(display,
	    screen_num),WhitePixel(display,screen_num));


  /* Create pixmap of depth 1 (bitmap) for icon */
  icon_pixmap = XCreateBitmapFromData(display,win,
		    basicwin_icon_bits,basicwin_icon_width,
                    basicwin_icon_height);
  /* Before mapping, set size hints for window manager */
  /* Note that in a real application, if size or position were
   * set by the user, the flags would be USPotition asn USSize
   * for both R3 and R4, and these would override the window
   * manager's preferences for this window; this is true even
   * in R4 when the x, y, width, and height fields are obsolete */

  /* x,y,width, and height hints are now taken from the
   * actual settings of eht window when mapped; note that
   * PPosition and PSize must be spefified anyway */

  size_hints.flags = PPosition | PSize | PMinSize;
  size_hints.min_width = 300;
  size_hints.min_height = 200;

  {
  XWMHints wm_hints;
  XClassHint class_hints;

  /* Format of the window name and icon name arguments has
   * changed in R4 */
  XTextProperty windowName,iconName;

  /* These calls store window_name and icon_name into
   * XTextProperty structures and set their other fields
   * properly */
  if(XStringListToTextProperty(&window_name, 1, &windowName) == 0)
  {
      (void)fprintf(stderr , "%s: structure allocation for \
             windowName failed.\n",progname);
      exit(-1);
  }
  
  if(XStringListToTextProperty(&icon_name, 1 ,&iconName) == 0)
  {
      (void)fprintf(stderr , "%s: structure allocation for \
             iconName failed.\n",progname);
      exit(-1);
  }

  /* Whether application should be normal or iconified
   * when first mapped */
  wm_hints.initial_state = NormalState;

  /* Does application need keyboard input? */
  wm_hints.input = True;
  wm_hints.icon_pixmap = icon_pixmap;
  wm_hints.flags = StateHint | IconPixmapHint | InputHint;

  /* These are used by the window manager to get information
   * about this application from the resourece database */
  class_hints.res_name = progname;
  class_hints.res_class = "Basicwin";
  
  XSetWMProperties(display, win , &windowName, &iconName,
	   argv, argc, &size_hints, & wm_hints,
	   &class_hints);
  }

  /* Select event types wanted */
  XSelectInput(display, win, ExposureMask | KeyPressMask |
           ButtonPressMask | StructureNotifyMask);
  
  load_font(&font_info);

  /* Create GC for text and drawing */
  getGC(win, &gc, font_info);

  /*Display window */
  XMapWindow(display,win);

  /* Get events, use first to display text and graphics */
  while(1)
  {
    XNextEvent(display,&report);
    switch(report.type){
    case Expose:
      /* Unless this is the last contiguous expose,
       * don't draw the window */
      if(report.xexpose.count != 0)
	break;
      
      /* if window too mall to use */
      if(window_size == TOO_SMALL)
	TooSmall(win, gc, font_info);
      else {
	/* Place text in window */
	place_text(win, gc, font_info, width, height);

	/* Place graphics in window */
	place_graphics(win, gc, width, height);
      }
      break;
    case ConfigureNotify:
      /* Window has been resized; change width
       * and height to send to place_text and
       * place_graphics in next Expose */
      width = report.xconfigure.width;
      height = report.xconfigure.height;
      if((width < size_hints.min_width) ||
	      (height < size_hints.min_height))
	window_size = TOO_SMALL;
      else
	window_size = BIG_ENOUGH;
      break;
    case ButtonPress:
      /* Trickle down into KeyPress (no break) */
    case KeyPress:
      XUnloadFont(display, font_info->fid);
      XFreeGC(display,gc);
      XCloseDisplay(display);
      exit(1);
    default:
      /* All events selected by StructureNotifyMask
       * except ConfigureNotify are thrown away here,
       * since noting is done with them */
      break;
    }
  }
}

void getGC(Window win, GC* gc, XFontStruct *font_info)
{
  unsigned long valuemask = 0; /* Ignore XGCvalues and
				* use defaults */
  XGCValues values;

  unsigned int line_width = 6;
  int line_style = LineOnOffDash;
  int cap_style = CapRound;
  int join_style = JoinRound;
  int dash_offset = 0;
  static char dash_list[] = {12,14};
  int list_length = 2;

  /* Create default Grahpics Context */
  *gc = XCreateGC(display, win, valuemask, &values);

  /* Specify font */
  XSetFont(display, *gc, font_info->fid);

  /* Specify black foreground since default window background
   * is white and default foreground is undefined */
  XSetForeground(display, *gc, BlackPixel(display,screen_num));

  /* Set line attributes */
  XSetLineAttributes(display, *gc, line_width, line_style,
	  cap_style, join_style);

  /* Set dashes */
  XSetDashes(display, *gc, dash_offset, dash_list, list_length);
}

void load_font(XFontStruct **font_info)
{
  char *fontname = "9x15";

  /* Load font and get font information structure */
  if((*font_info = XLoadQueryFont(display,fontname)) == NULL)
  {
      (void)fprintf(stderr , "%s: Cannot open 9x15 font \n",
	       progname);
      exit(-1);
  }
}

void place_text(Window win,GC gc,XFontStruct *font_info,unsigned int win_width,unsigned int win_height)
{
  char *string1 = "Hi! I'm window, who are you";
  char *string2 = "To terminamte program; Press and key";
  char *string3 = "or button while in this window.";
  char *string4 = "Screen Dimensions:";
  int len1,len2,len3,len4;
  int width1,width2,width3;
  char cd_height[50],cd_width[50],cd_depth[50];
  int font_height;
  int initial_y_offset,x_offset;

  /* Need length for both XTextWidth and XDrawString */
  len1 = strlen(string1);
  len2 = strlen(string2);
  len3 = strlen(string3);

  /* Get string widths for centering */
  width1 = XTextWidth(font_info, string1, len1);
  width2 = XTextWidth(font_info, string2, len2);
  width3 = XTextWidth(font_info, string3, len3);

  font_height = font_info->ascent + font_info->descent;

  /* Output text, centered on each line */
  XDrawString(display, win, gc, (win_width - width1)/2,
	      font_height,
	      string1,len1);
  XDrawString(display, win, gc, (win_width - width2)/2,
	      (int)(win_height - (2 * font_height)),
	       string2,len2);
  XDrawString(display, win, gc, (win_width - width3)/2,
	      (int)(win_height - font_height),
	      string3,len3);

  /* Copy numbers into string variables */
  (void)sprintf(cd_height, " Height - %d pixels",
		DisplayHeight(display,screen_num));
  (void)sprintf(cd_width, " Width - %d pixels",
		DisplayWidth(display,screen_num));
  (void)sprintf(cd_depth, " Depth - %d plane(s)",
		DefaultDepth(display,screen_num));

  /* Reuse these for same purpose */
  len4 = strlen(string4);
  len1 = strlen(cd_height);
  len2 = strlen(cd_width);
  len3 = strlen(cd_depth);

  /* To center strings vertically, we place the first string
   * so that the top of it is two font_heights above the center
   * of the window; since the baseline of the string is what
   * we need to locate for XDrawString and the baseline is
   * on font_info -> ascent below the top of the character,
   * the final offset of the origin up from the center of
   * the window is one font_height + one descent */

  initial_y_offset = win_height/2 - font_height -
            font_info->descent;
  x_offset = (int) win_width/4;
  XDrawString(display, win, gc, x_offset, (int)initial_y_offset,
	      string4,len4);

  XDrawString(display, win, gc, x_offset, (int)initial_y_offset + 
	      font_height,cd_height,len1);
  XDrawString(display, win, gc, x_offset, (int)initial_y_offset +
	      2 * font_height, cd_width,len2);
  XDrawString(display, win, gc, x_offset, (int)initial_y_offset +
	      3 * font_height, cd_depth,len3);
}

void place_graphics(Window win, GC gc, unsigned int window_width, unsigned int window_height)
{
  int x,y;
  int width,height;

  height = window_height/2;
  width = 3 * window_width /4;
  x = window_width/2 - width /2; /* Center */
  y = window_height/2 - height/2;
  XDrawRectangle(display,win,gc,x,y,width,height);
}

void TooSmall(Window win, GC gc, XFontStruct font_info)
{
  char  *string1 = "Too Small";
  int y_offset,x_offset;

  y_offset = font_info.ascent + 2;
  x_offset = 2;

  /* Output text, centered on each line */
  XDrawString(display, win, gc, x_offset, y_offset, string1,
	      strlen(string1));
}






