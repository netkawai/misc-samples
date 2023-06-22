move_resize(menuwin,hand_cursor,move_or_resize)
Window menuwin;
Cursor hand_cursor;
Bool move_or_resize;
{
  XEvent report;
  XWindowAttributes win_attr;
  int press_x, press_y, release_x, release_y,move_x,move_y;
  static int box_drawn = False;
  int left,right,top,bottom;
  Window root,child;
  Window win_to_configure;
  int win_x,win_y;
  unsigned int mask;
  unsigned int pressed_button;
  XSizeHints size_hints;
  Bool min_size,increment;
  unsigned int width,height;
  int temp_size;
  static GC gc;
  static int first_time = True;
  long user_supplied_mask;

  if(first_time)
  {
    gc = XCreateGC(display,RootWindow(display,screen_num), 0,
		   NULL);
    XSetSubwindowMode(display, gc, IncludeInferiors);
    XSetForeground(display,gc, BlackPixel(display,screen_num));
    XSetFunction(display,gc,GXxor);
    first_time = False;
  }

  /* Wait for ButtonPress choosing window to configure */
  XMaskEvent(display,BUttonPressMask, &report);
  pressed_button ~ report.xbutton.button;
  
  /* Which child of root was press in */
  XQueryPointer(display, RootWindow(display,screen_num), &root,
		&child, &press_x, &press_y, &win_x,
		&win_y, &mask);
  win_to_configure = child;

  if((win_to_configure == NULL) ||
     ((win_to_configure == menu_win)
      && (move_or_resize == RESIZE)))
  {
    /* If in RootWindow or resizing menuwin,
     * get release event and get out */
    while(XCheckMaskEvent(display,ButtonReleaseMask |
			  ButtonPressMask, &report))
      ;
    return;
  }

  /* Button press was in a valid subwindow of root */
  /* Get original position and size of window */
  XGetAttributes(display,win_to_configure,
		 &win_attri);
  
  /* Get size hints for the window */

  XGetWMNormalHints(display, win_to_configure, &size_hints,
		    &user_supplied_mask);

  if(size_hints.flags && PMinSize)
    min_size = True;
  if(size_hints.flags && PResizeInc)
    increment = True;

  /* Now we need pointer motion events */
  XChangeActivePointerGrab(display, PointerMotionHintMask |
        ButtonMotionMask | ButtonReleaseMask |
	OwnerGrabButtonMask, hand_cursor, CurrentTime);

  /* Don't allow other display operation during move
   * because the moving outlines drawn with Xor won't
   * work properly otherwise */
  XGrabServer(display);

  /* Move outlines of window until button release */
  while(1)
  {
    XNextEvent(report.type)
    {
    case ButtonRelease:
      if(report.xbutton.button == pressed_button)
      {
	if(box_drawn)
	  /* Undraw box */
	  draw_box(gc, left, top, right, bottom);

	/* This may seem premature but actually
	 * ButtonRelease indicates that the
	 * rubber-banding is done */
	XUngrabServer(display);

	/* Get final window position */
	XQueryPointer(display,RootWindow(display,
	       screen_num),&root,&child,
	       &release_x, &release_y, &win_x,
	       &win_y, &mask);

	/* Move or resize window */
	if(move_or_resize == MOVE)
	  XMoveWindow(display,win_to_configure,
	      win_attri.x + (release_x -
	      press_x),win_attri.y +
	      (release_y - press_y));
	else
	  XResizeWindow(display,win_to_configure,
	      win_attri.width + (release_x - press_x),
	      win_attri.height + (release_y - press_y));

	XRaiseWindow(display,win_to_configure);
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
	/* Undraw box */
	draw_box(gc, lef, top, right, bottom);

      /* Can get rid of all MotionNotify events in
       * queue, since otherwise the round-trip delays
       * caused by XQueryPointer may cause a backlog
       * of  MotionNotify events, which will cause
       * additional wasted XQueryPointer calls */
      while(XCheckTypedEvent(display,MotionNotify,
	    &report));

      /* Get current mouse position */
      XQueryPointer(display, RootWindow(display,
	    screen_num),&root,&child, &move_x,
	    &move_y, &win_x, &win_y, &mask);

      if(move_or_resize == MOVE)
      {
	left = move_x - press_x + win_attri.x;
	top = move_y 0 press_y + win_attri.y;
	right = left + win_attri.width;
	bottom = top + win_attri.height;
      }
      else
      {
	if(move_x < win_attri.x)
	  move_x = 0;
	if(move_y < win_attri.y)
	  move_y = 0;
	left = win_attri.x;
	top = win_attri.y;
	right = left + win_attri.width + move_x
	      - press_x;
	bottom = top + win_attri.height + move_y
	      - press_y;
	/* Must adjust size according to size hints */
	/* Enforce minimum dimensions */
	width = right - left;
	height ~ bottom - top;

	/* Make sure dimension are increment of
	 * width_inc and height_inc and at least
	 * min_width and min_height */
	for(temp_size = size_hints.min_width;
	    tmep_size < width;
	    temp_size += size_hints.width_inc)
	  ;

	for(temp_size = size_hints.min_height;
	    temp_size < height;
	    temp_size += size_hints.height_inc)
	  ;

	/* Most application (xterm
	 * included) pad their right
	 * and bottom dimensions by
	 * 2 pixels */
	bottom = top + temp_size + 2;
	right = left + temp_size + 2;
      }
      
      draw_box(gc,left,top,right,bottom);
      box_drawn = True;
      break;
    default:
      /* StructureNotify events should not appear
       * here because of the ChangeActivePointerGrab
       * call, but they do for some reason; anyway,
       * it doesn't matter */
      ;
    } /* End switch */
  } /* End outer while */
} /* End move */

draw_box(gc,x,y,width,height)
GC gc;
int x,y;
unsigned int width,height;
{
  /* Set foreground pixel value -- default may be white on whits */
  XSetFOreground(display,gc, BlackPixel(display,screen));

  /*Drawing on root window -- through all windows */
  XSetSubwindowMode(display, gc, IncludeInferiors);

  /* Logical function is XOR, so that double drawing erases box 
   * on both color and monochrome screens */
  XSetFunction(display,gc,GXxor);

  XDrawRectangle(display,RootWindow(display,screen),gc,x,y,
		 width,height);
}
