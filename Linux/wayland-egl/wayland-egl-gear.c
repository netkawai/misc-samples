// gcc -o wayland-egl wayland-egl.c -lwayland-client -lwayland-egl -lEGL -lGL
// client-side decoration came from https://github.com/christianrauch/wayland_window_decoration_example for client-side decoration.
/*
 * Copyright (C) 1999-2001  Brian Paul   All Rights Reserved.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * BRIAN PAUL BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
 * AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/*
 * Ported to WAYLAND/EGL/GL with CSD
 * Kawai 2023-02-06
 */

// POSIX
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

// Linux specific
#include <linux/input.h>

// GL/EGL
#include <EGL/egl.h>
#include <GL/gl.h>

// wayland
#include <wayland-client.h>
#include <wayland-egl.h>
#include <wayland-cursor.h>

#include "xdg-shell-client-protocol.h"

#define WIDTH 256
#define HEIGHT 256

static struct wl_display* display;
static struct wl_compositor* compositor = NULL;
static struct wl_seat *seat = NULL;
static struct wl_shm *shm = NULL;
static struct wl_cursor_theme *cursor_theme = NULL;
static struct wl_surface *cursor_surface = NULL;

static struct wl_subcompositor *subcompositor = NULL;

static struct xdg_wm_base *xdg_wm_base = NULL;
static struct xwl_shell* shell = NULL;

static _Bool running = True;


struct csd_button {
    struct wl_surface *surface;
    struct wl_subsurface *subsurface;
    struct wl_egl_window *egl_window;
    EGLSurface egl_surface;
    EGLContext egl_context;
    EGLDisplay egl_display;
    double r, g, b, a;

    enum button_type {
        CSD_CLOSE, 
        CSD_MAXIMIZE, 
        CSD_MINIMIZE
    } function;
};

static void button_init(struct csd_button * target, // out
        struct wl_compositor* compositor, 
        struct wl_subcompositor* subcompositor, 
        struct wl_surface* source,
        EGLConfig config, 
        int32_t x, 
        int32_t y, 
        enum button_type fnct, 
        double _r, 
        double _g, 
        double _b, 
        double _a)
{
        target->function = fnct;
        target->r=_r; target->g=_g; target->b=_b; target->a=_a;
        target->surface = wl_compositor_create_surface(compositor);
        target->subsurface = wl_subcompositor_get_subsurface(subcompositor, target->surface, source);
        wl_subsurface_set_desync(target->subsurface);
        target->egl_display = eglGetDisplay(display);
        target->egl_context = eglCreateContext (target->egl_display, config, EGL_NO_CONTEXT, NULL);
        eglInitialize(target->egl_display, NULL, NULL);
        wl_subsurface_set_position(target->subsurface, x, y);
        target->egl_window = wl_egl_window_create(target->surface, 10, 8);
        target->egl_surface = eglCreateWindowSurface(target->egl_display, config, target->egl_window, NULL);
}

static void button_draw(struct csd_button * target)
{
        eglMakeCurrent(target->egl_display, target->egl_surface, target->egl_surface, target->egl_context);
        glClearColor(target->r, target->g, target->b, target->a);
        glClear(GL_COLOR_BUFFER_BIT);
        eglSwapBuffers(target->egl_display, target->egl_surface);
}


struct xdg_toplevel_edge_cursor
{
    enum xdg_toplevel_resize_edge edge;
    char* cursor_name; 
    int x;
    int y;
    int w;
    int h;
};

static const struct xdg_toplevel_edge_cursor resize_cursor[] = {
    {XDG_TOPLEVEL_RESIZE_EDGE_NONE, "grabbing" ,1,0,0,1},
    {XDG_TOPLEVEL_RESIZE_EDGE_TOP, "top_side",1,1,0,1},
    {XDG_TOPLEVEL_RESIZE_EDGE_BOTTOM, "bottom_side",1,1,0,1},
    {XDG_TOPLEVEL_RESIZE_EDGE_LEFT, "left_side",1,1,0,1},
    {XDG_TOPLEVEL_RESIZE_EDGE_TOP_LEFT, "top_left_corner",0,0,1,1},
    {XDG_TOPLEVEL_RESIZE_EDGE_BOTTOM_LEFT, "bottom_left_corner" ,0,0,1,1},
    {XDG_TOPLEVEL_RESIZE_EDGE_RIGHT, "right_side" ,1,1,0,1},
    {XDG_TOPLEVEL_RESIZE_EDGE_TOP_RIGHT, "top_right_corner" ,0,0,1,1},
    {XDG_TOPLEVEL_RESIZE_EDGE_BOTTOM_RIGHT, "bottom_right_corner" ,0,0,1,1}
};

 
 

static const int num_of_decorations = sizeof(resize_cursor)/sizeof(struct xdg_toplevel_edge_cursor);

static char* get_cursor_name_from_edge(enum xdg_toplevel_resize_edge edge)
{
    for(int i = 0 ; i < num_of_decorations ; i++)
    {
        if(resize_cursor[i].edge == edge)
            return resize_cursor[i].cursor_name;
    }
    return "";
}

struct csd_decoration {
    struct wl_surface *surface;
    struct wl_subsurface *subsurface;
    struct wl_egl_window *egl_window;
    EGLSurface egl_surface;
    EGLContext egl_context;
    EGLDisplay egl_display;
    double r, g, b, a;
    uint border_size;
    uint title_bar_size;

    enum xdg_toplevel_resize_edge edge_function;

};


static void decoration_init(struct csd_decoration *target, // out
    struct wl_compositor* compositor, struct wl_subcompositor* subcompositor, struct wl_surface* source,
               const uint _border_size, const uint _title_bar_size,
               EGLConfig config, enum xdg_toplevel_resize_edge type,
               double _r, double _g, double _b, double _a)
{
    target->edge_function = type;
    target->r=_r; target->g=_g; target->b=_b; target->a=_a;
    target->border_size = _border_size;
    target->title_bar_size = _title_bar_size;

    target->surface = wl_compositor_create_surface(compositor);
    target->subsurface = wl_subcompositor_get_subsurface(subcompositor, target->surface, source);
    wl_subsurface_set_desync(target->subsurface);
    target->egl_display = eglGetDisplay(display);
    target->egl_context = eglCreateContext (target->egl_display, config, EGL_NO_CONTEXT, NULL);
    eglInitialize(target->egl_display, NULL, NULL);
    wl_subsurface_set_position(target->subsurface, 0, 0);
    target->egl_window = wl_egl_window_create(target->surface, 1, 1);
    target->egl_surface = eglCreateWindowSurface(target->egl_display, config, target->egl_window, NULL);
}

static void decoration_calc_dim(struct csd_decoration *target, int main_w,  int main_h, int x, int y, int w, int h) {
    // get position and dimension from type and main surface
    switch (target->edge_function) {
    case XDG_TOPLEVEL_RESIZE_EDGE_NONE:
        x = 0;      y = -target->title_bar_size;
        w = main_w; h = target->title_bar_size;
        break;
    case XDG_TOPLEVEL_RESIZE_EDGE_TOP:
        x = 0;      y = -target->title_bar_size-target->border_size;
        w = main_w; h = target->border_size;
        break;
    case XDG_TOPLEVEL_RESIZE_EDGE_BOTTOM:
        x = 0;      y = main_h;
        w = main_w; h = target->border_size;
        break;
    case XDG_TOPLEVEL_RESIZE_EDGE_LEFT:
        x = -target->border_size; y= -target->title_bar_size;
        w = target->border_size;  h= main_h + target->title_bar_size;
        break;
    case XDG_TOPLEVEL_RESIZE_EDGE_TOP_LEFT:
        x=-target->border_size; y=-target->border_size-target->title_bar_size;
        w=target->border_size; h=target->border_size;
        break;
    case XDG_TOPLEVEL_RESIZE_EDGE_BOTTOM_LEFT:
        x=-target->border_size; y=main_h;
        w=target->border_size; h=target->border_size;
        break;
    case XDG_TOPLEVEL_RESIZE_EDGE_RIGHT:
        x=main_w; y=-target->title_bar_size;
        w=target->border_size; h=main_h+target->title_bar_size;
        break;
    case XDG_TOPLEVEL_RESIZE_EDGE_TOP_RIGHT:
        x=main_w; y=-target->border_size-target->title_bar_size;
        w=target->border_size; h=target->border_size;
        break;
    case XDG_TOPLEVEL_RESIZE_EDGE_BOTTOM_RIGHT:
        x=main_w; y=main_h;
        w=target->border_size; h=target->border_size;
        break;
    }
}

static void decoration_resize(struct csd_decoration *target, const int main_w, const int main_h) {
    int x,y,w,h;
    decoration_calc_dim(target,main_w, main_h, x, y, w, h);
    wl_subsurface_set_position(target->subsurface, x, y);
//        egl_window = wl_egl_window_create(surface, w, h);
    wl_egl_window_resize(target->egl_window, w, h, 0, 0);

}

static void decoration_draw(struct csd_decoration *target) {
    eglMakeCurrent(target->egl_display, target->egl_surface, target->egl_surface, target->egl_context);
    //glClearColor(1.0, 1.0, 0.0, 1.0);
    glClearColor(target->r, target->g, target->b, target->a);
    glClear(GL_COLOR_BUFFER_BIT);
    eglSwapBuffers(target->egl_display, target->egl_surface);
}



struct app_window {
    EGLDisplay egl_display;
    EGLContext egl_context;
    struct wl_surface *surface;
    struct xdg_surface *xdg_surface;
    struct xdg_toplevel *xdg_toplevel;
    struct wl_egl_window *egl_window;
    EGLSurface egl_surface;

    struct wl_keyboard *wl_keyboard;
    struct wl_pointer *wl_pointer;
    
    int width;
    int height;
    uint border_size;
    uint title_size;

    _Bool maximized;
    struct csd_decoration decorations[9]; // corresponding to resize_cursor
    struct csd_button buttons[3]; // number of button_type

    _Bool button_pressed;
    struct wl_surface * current_surface;    // last entered surface

    _Bool inhibit_motion;
};


// listeners for seat (input)
static void pointer_enter (void *data, struct wl_pointer *pointer, uint32_t serial, struct wl_surface *surface, wl_fixed_t surface_x, wl_fixed_t surface_y) {
    struct app_window *w = (struct app_window*)(data);
    w->current_surface = surface;

    char* cursor_str = "left_ptr";

    for(int i = 0 ; i < num_of_decorations; i++)
    {
        if(w->decorations[i].surface==surface) {
            char* ret_cursor = get_cursor_name_from_edge(w->decorations[i].edge_function);
            if(strlen(ret_cursor) > 0) {
                cursor_str = ret_cursor;
            }
        }
    }

    struct wl_cursor *cursor = wl_cursor_theme_get_cursor(cursor_theme, cursor_str);
    struct wl_cursor_image *cursor_image = cursor->images[0];
    wl_pointer_set_cursor(pointer, serial, cursor_surface, cursor_image->hotspot_x, cursor_image->hotspot_y);
    wl_surface_commit(cursor_surface);
    wl_surface_attach(cursor_surface, wl_cursor_image_get_buffer(cursor_image), 0, 0);
    wl_surface_damage(cursor_surface, 0, 0, cursor_image->width, cursor_image->height);
}

static void pointer_leave (void *data, struct wl_pointer *pointer, uint32_t serial, struct wl_surface *surface) {
    struct app_window *w = (struct app_window*)(data);
    w->button_pressed = False;
}

static void pointer_motion (void *data, struct wl_pointer *pointer, uint32_t time, wl_fixed_t x, wl_fixed_t y) {
//    std::cout << "pointer motion " << wl_fixed_to_double(x) << " " << wl_fixed_to_double(y) << std::endl;
}

static void pointer_button (void *data, struct wl_pointer *pointer, uint32_t serial, uint32_t time, uint32_t button, uint32_t state) {
//    std::cout << "pointer button " << button << ", state " << state << std::endl;

    struct app_window *w = (struct app_window*)(data);
    w->button_pressed = (button==BTN_LEFT) && (state==WL_POINTER_BUTTON_STATE_PRESSED);

    if(w->button_pressed) {
        for(int i = 0; i < num_of_decorations ; i++) {
            if(w->decorations[i].surface==w->current_surface) {
                switch(w->decorations[i].edge_function) {
                case XDG_TOPLEVEL_RESIZE_EDGE_NONE:
                    if(w->xdg_toplevel) {
                        xdg_toplevel_move(w->xdg_toplevel, seat, serial);
                    }
                    break;
                default:
                    if(w->xdg_toplevel) {
                        xdg_toplevel_resize(w->xdg_toplevel, seat, serial, w->decorations[i].edge_function);
                    }
                    break;
                }
            }
        }

        for(int i = 0 ; i < 3 ; i ++) {
            if(w->buttons[i].surface==w->current_surface) {
                switch (w->buttons[i].function) {
                case CSD_CLOSE:
                    running = False;
                    break;
                case CSD_MAXIMIZE:
                    if(w->maximized) {
                        if(w->xdg_toplevel) {
                            xdg_toplevel_unset_maximized(w->xdg_toplevel);
                        }
                    }
                    else {
                        // store original window size
//                        wl_egl_window_get_attached_size(w->egl_window, &w->width, &w->height);
                        if(w->xdg_toplevel) {
                            xdg_toplevel_set_maximized(w->xdg_toplevel);
                        }
                    }
                    w->maximized = !w->maximized;
                    break;
                case CSD_MINIMIZE:
                    if(w->xdg_toplevel) {
                        xdg_toplevel_set_minimized(w->xdg_toplevel);
                    }
                    break;
                }
            }
        }
    }
}

static void pointer_axis (void *data, struct wl_pointer *pointer, uint32_t time, uint32_t axis, wl_fixed_t value) {
}

static struct wl_pointer_listener pointer_listener = {&pointer_enter, &pointer_leave, &pointer_motion, &pointer_button, &pointer_axis};

static void
wl_seat_capabilities(void *data, struct wl_seat *wl_seat, uint32_t capabilities)
{
    struct app_window *window = (struct app_window*)(data);
    if(window == NULL)
    {
        fprintf(stderr, "no window pointer\n");
        return;
    }


    if (capabilities & WL_SEAT_CAPABILITY_POINTER) {
        window->wl_pointer = wl_seat_get_pointer (seat);
        wl_pointer_add_listener (window->wl_pointer, &pointer_listener, data);
        cursor_surface = wl_compositor_create_surface(compositor);
    }
   if (capabilities & WL_SEAT_CAPABILITY_KEYBOARD) {
        window->wl_keyboard = wl_seat_get_keyboard (seat);
//        wl_keyboard_add_listener (keyboard, &keyboard_listener, NULL);
   }    
}

static void
wl_seat_name(void *data, struct wl_seat *wl_seat, const char *name)
{
       fprintf(stderr, "seat name: %s\n", name);
}
// declaration
static const struct wl_seat_listener seat_listener = {
       .capabilities = wl_seat_capabilities,
       .name = wl_seat_name,
};
//


static void delete_window( struct app_window* window )
{
    eglDestroySurface( window->egl_display, window->egl_surface );
    wl_egl_window_destroy( window->egl_window );
    //wl_shell_surface_destroy( window->shell_surface );
     if(xdg_wm_base) {
        xdg_toplevel_destroy(window->xdg_toplevel);
        xdg_surface_destroy(window->xdg_surface);
    }
    wl_surface_destroy( window->surface );
    eglDestroyContext( window->egl_display, window->egl_context );
}

static void window_resize(struct app_window *window, const int width, const int height, _Bool full) {
//    std::cout << "config " << width << " " << height << std::endl;
    // main surface with from full surface
    int main_w, main_h;
    if(full) {
        main_w = width-2*window->border_size;
        main_h = height-2*window->border_size-window->title_size;
//        std::cout << "new size " << main_w << " " << main_h << std::endl;
    }
    else {
        main_w = width;
        main_h = height;
    }

#define C_MAX(a,b) ((a) > (b) ? (a) : (b))
    // set minimum size (50,50)
    main_w = C_MAX(main_w, 50);
    main_h = C_MAX(main_h, 50);

    // resize main surface
    wl_egl_window_resize(window->egl_window, main_w, main_h, 0, 0);

    // resize all decoration elements
    for(int i = 0 ; i < num_of_decorations ; i++) { 
        decoration_resize(&window->decorations[i],main_w,main_h); 
    }
}

static int had_displayed = 0;

static void draw_window( struct app_window* window )
{
    eglMakeCurrent(window->egl_display, window->egl_surface, window->egl_surface, window->egl_context);

   if(had_displayed == 0)
   {
      const GLubyte* vendor = glGetString(GL_VENDOR);
      const GLubyte* renderer = glGetString(GL_RENDERER);
      const GLubyte* glversion = glGetString(GL_VERSION);
      printf("Vendor: %s\n",vendor);
      printf("Renderer: %s\n",renderer);
      printf("Version: %s\n",glversion);
      had_displayed =  1;
   }
   else
   {
        if(had_displayed < 10)
        {
            printf("call loop"); 
        }
        had_displayed++;
   }

    /* Here event loop I believe */
#if 0
    draw_gear_loop();
#endif

    glClearColor( 0.0, 1.0, 0.0, 1.0 );
    glClear( GL_COLOR_BUFFER_BIT );
    eglSwapBuffers( window->egl_display, window->egl_surface );
#if 0
     // draw all decoration elements
    for(int i = 0 ; i < num_of_decorations ; i++) { 
        decoration_draw(&(window->decorations[i])); 
    }

    for(int i = 0 ; i < 3 ; i++ ) { 
        button_draw(&(window->buttons[i])); 
    }
#endif
}

#if 0 // Shell
static void shell_surface_ping( void* data, struct wl_shell_surface* shell_surface, uint32_t serial )
{
    wl_shell_surface_pong( shell_surface, serial );
}

static void shell_surface_configure
    (
    void* data,
    struct wl_shell_surface* shell_surface,
    uint32_t edges,
    int32_t width,
    int32_t height
    )
{
    struct window* window = data;
    wl_egl_window_resize( window->egl_window, width, height, 0, 0 );
}

static void shell_surface_popup_done( void* data, struct wl_shell_surface* shell_surface )
{
}

static struct wl_shell_surface_listener shell_surface_listener = 
{
    &shell_surface_ping,
    &shell_surface_configure,
    &shell_surface_popup_done
};
#endif

// XDG top level
static void xdg_surface_handle_configure(void *data,
        struct xdg_surface *xdg_surface, uint32_t serial) {

    fprintf(stderr,"xdg_surface_configure");
    xdg_surface_ack_configure(xdg_surface, serial);
}

static struct xdg_surface_listener xdg_surface_listener = {
    .configure = xdg_surface_handle_configure,
};

void xdg_toplevel_handle_configure(void *data, struct xdg_toplevel *xdg_toplevel, int32_t width, int32_t height, struct wl_array *states) {
    if (width==0 || height==0)
        return;
    struct app_window *window = (struct app_window*)(data);
    window_resize(window, width, height, True);
}

static void xdg_toplevel_handle_close(void *data, struct xdg_toplevel *xdg_toplevel) {
    running = False;
}

static struct xdg_toplevel_listener xdg_toplevel_listener = {
    .configure = xdg_toplevel_handle_configure,
    .close = xdg_toplevel_handle_close,
};

void xdg_wm_base_ping(void *data, struct xdg_wm_base *xdg_wm_base, uint32_t serial) {
    xdg_wm_base_pong(xdg_wm_base, serial);
}

static const struct xdg_wm_base_listener xdg_wm_base_listener = {
    .ping = xdg_wm_base_ping
};

static void create_window( struct app_window* window, int32_t width, int32_t height )
{
    const uint border_size = 5;
    const uint title_size = 15;

    eglBindAPI( EGL_OPENGL_API );
    EGLint attributes[] = {EGL_RED_SIZE, 8, EGL_GREEN_SIZE, 8, EGL_BLUE_SIZE, 8, EGL_ALPHA_SIZE, 8, EGL_NONE};
    EGLConfig config;
    EGLint num_config;
    eglChooseConfig( window->egl_display, attributes, &config, 1, &num_config );
    window->egl_context = eglCreateContext( window->egl_display, config, EGL_NO_CONTEXT, NULL );

    window->width = width;
    window->height = height;
    window->border_size = border_size;
    window->title_size = title_size;

    window->surface = wl_compositor_create_surface( compositor );

    if(True) {
        window->xdg_surface = xdg_wm_base_get_xdg_surface(xdg_wm_base, window->surface);
        xdg_surface_add_listener(window->xdg_surface, &xdg_surface_listener, window);

        wl_display_roundtrip( display );

        window->xdg_toplevel = xdg_surface_get_toplevel(window->xdg_surface);
        xdg_toplevel_add_listener(window->xdg_toplevel, &xdg_toplevel_listener, window);

        xdg_toplevel_set_title(window->xdg_toplevel, "wayland-egl-gear");
        xdg_toplevel_set_app_id(window->xdg_toplevel, "wayland-egl-gear");


    }
    else {
        fprintf(stderr, "no xdg_wm_base\n");
#if 0
    window->shell_surface = wl_shell_get_shell_surface( shell, window->surface );
    wl_shell_surface_add_listener( window->shell_surface, &shell_surface_listener, window );
    wl_shell_surface_set_toplevel( window->shell_surface );
#endif

    }


    window->egl_window = wl_egl_window_create( window->surface, width, height );
    window->egl_surface = eglCreateWindowSurface( window->egl_display, config, window->egl_window, NULL );
#if 0
    for(int i = 0 ; i < num_of_decorations ; i ++)
    {
        decoration_init(&window->decorations[i],
            compositor, subcompositor, window->surface, border_size, title_size, config, 
            resize_cursor[i].edge, 
            resize_cursor[i].x,resize_cursor[i].y,resize_cursor[i].w,resize_cursor[i].h);
    }

    button_init(&window->buttons[0], compositor, subcompositor, window->decorations[0].surface, config, 5, 4, CSD_CLOSE, 0,0,0,1);
    button_init(&window->buttons[0], compositor, subcompositor, window->decorations[0].surface, config, 20, 4, CSD_MAXIMIZE,  0.5,0.5,0.5,1);
    button_init(&window->buttons[0], compositor, subcompositor, window->decorations[0].surface, config, 35, 4, CSD_MINIMIZE, 1,1,1,1);
#endif
}

// listeners for global registry
static void registry_add_object
    (
    void* data,
    struct wl_registry* registry,
    uint32_t name,
    const char* interface,
    uint32_t version
    )
{
    if( !strcmp( interface, "wl_compositor" ) )
    {
        compositor = wl_registry_bind( registry, name, &wl_compositor_interface, 1 );
    }
    else if (!strcmp(interface, "wl_subcompositor")) {
        subcompositor = wl_registry_bind(registry, name, &wl_subcompositor_interface, 1);
    }
#if 0
    else if (!strcmp(interface,"wl_seat")) 
    {
        seat = wl_registry_bind (registry, name, &wl_seat_interface, 1);
        wl_seat_add_listener (seat, &seat_listener, data);
    }
#endif
#if 0
    else if (!strcmp(interface, "wl_shm")) {
        shm = wl_registry_bind(registry, name, &wl_shm_interface, version);
        cursor_theme = wl_cursor_theme_load(NULL, 32, shm);
    }
#endif
    else if (!strcmp(interface, xdg_wm_base_interface.name)) {
        xdg_wm_base = wl_registry_bind(registry, name, &xdg_wm_base_interface, version);
        xdg_wm_base_add_listener(xdg_wm_base, &xdg_wm_base_listener, data);
    }    
    else if( !strcmp( interface, "wl_shell" ) )
    {
        shell = wl_registry_bind( registry, name, &wl_shell_interface, 1 );
    }

}

static void registry_remove_object( void* data, struct wl_registry* registry, uint32_t name )
{
    // do nothing
}

// declaration
static struct wl_registry_listener registry_listener = 
{
    &registry_add_object,
    &registry_remove_object
};
//

int main()
{
    display = wl_display_connect( NULL );

    if(!display) {
        fprintf(stderr, "cannot connect to Wayland server\n");
        return -1;
    }

    struct app_window window;
    memset(&window,0,sizeof(struct app_window));

    struct wl_registry* registry = wl_display_get_registry( display );
    wl_registry_add_listener( registry, &registry_listener, &window );

    // This call the attached listener global_registry_handler
    wl_display_roundtrip( display );

    window.egl_display = eglGetDisplay( display );
    eglInitialize( window.egl_display, NULL, NULL );

    create_window( &window, WIDTH, HEIGHT );

    while( running )
    {
        wl_display_dispatch_pending( display );
        draw_window( &window );
    }

    delete_window( &window );
    eglTerminate( window.egl_display );
    wl_display_disconnect( display );
    return 0;
}



/* Gears */
#if 0
#ifndef M_PI
#define M_PI 3.14159265
#endif


static void
draw_gear_loop()
{
    static int frames = 0;
    static double tRot0 = -1.0, tRate0 = -1.0;
    double dt, t = current_time();
    if (tRot0 < 0.0)
    tRot0 = t;
    dt = t - tRot0;
    tRot0 = t;

    /* advance rotation for next frame */
    angle += 70.0 * dt;  /* 70 degrees per second */
    if (angle > 3600.0)
        angle -= 3600.0;

    switch (surface_type) 
    {
        case GEARS_WINDOW:
            draw();
            eglSwapBuffers(eman->dpy, eman->win);
        break;

	    case GEARS_PBUFFER:
	        draw();
	        if (!eglCopyBuffers(eman->dpy, eman->pbuf, eman->xpix))
	            break;
	        copy_gears(eman, w, h, window_w, window_h);
	    break;

    	case GEARS_PBUFFER_TEXTURE:
            eglMakeCurrent(eman->dpy, eman->pbuf, eman->pbuf, eman->ctx);
	        draw();
	        texture_gears(eman, surface_type);
	    break;

	    case GEARS_PIXMAP:
	        draw();
	        copy_gears(eman, w, h, window_w, window_h);
	    break;

	    case GEARS_PIXMAP_TEXTURE:
            eglMakeCurrent(eman->dpy, eman->pix, eman->pix, eman->ctx);
	        draw();
	        texture_gears(eman, surface_type);
	    break;

	    case GEARS_RENDERBUFFER:
	        glBindFramebuffer(GL_FRAMEBUFFER_EXT, eman->fbo);
	        draw();
	        glBindFramebuffer(GL_FRAMEBUFFER_EXT, 0);
	        texture_gears(eman, surface_type);
	    break;
    }

    frames++;

    if (tRate0 < 0.0)
        tRate0 = t;
    
    if (t - tRate0 >= 5.0) 
    {
        GLfloat seconds = t - tRate0;
        GLfloat fps = frames / seconds;
        printf("%d frames in %3.1f seconds = %6.3f FPS\n", frames, seconds,
                fps);
        tRate0 = t;
        frames = 0;
    }
}


static GLfloat view_rotx = 20.0, view_roty = 30.0, view_rotz = 0.0;
static GLint gear1, gear2, gear3;
static GLfloat angle = 0.0;

/*
 *
 *  Draw a gear wheel.  You'll probably want to call this function when
 *  building a display list since we do a lot of trig here.
 * 
 *  Input:  inner_radius - radius of hole at center
 *          outer_radius - radius at center of teeth
 *          width - width of gear
 *          teeth - number of teeth
 *          tooth_depth - depth of tooth
 */
static void
gear(GLfloat inner_radius, GLfloat outer_radius, GLfloat width,
     GLint teeth, GLfloat tooth_depth)
{
   GLint i;
   GLfloat r0, r1, r2;
   GLfloat angle, da;
   GLfloat u, v, len;

   r0 = inner_radius;
   r1 = outer_radius - tooth_depth / 2.0;
   r2 = outer_radius + tooth_depth / 2.0;

   da = 2.0 * M_PI / teeth / 4.0;

   glShadeModel(GL_FLAT);

   glNormal3f(0.0, 0.0, 1.0);

   /* draw front face */
   glBegin(GL_QUAD_STRIP);
   for (i = 0; i <= teeth; i++) {
      angle = i * 2.0 * M_PI / teeth;
      glVertex3f(r0 * cos(angle), r0 * sin(angle), width * 0.5);
      glVertex3f(r1 * cos(angle), r1 * sin(angle), width * 0.5);
      if (i < teeth) {
	 glVertex3f(r0 * cos(angle), r0 * sin(angle), width * 0.5);
	 glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da),
		    width * 0.5);
      }
   }
   glEnd();

   /* draw front sides of teeth */
   glBegin(GL_QUADS);
   da = 2.0 * M_PI / teeth / 4.0;
   for (i = 0; i < teeth; i++) {
      angle = i * 2.0 * M_PI / teeth;

      glVertex3f(r1 * cos(angle), r1 * sin(angle), width * 0.5);
      glVertex3f(r2 * cos(angle + da), r2 * sin(angle + da), width * 0.5);
      glVertex3f(r2 * cos(angle + 2 * da), r2 * sin(angle + 2 * da),
		 width * 0.5);
      glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da),
		 width * 0.5);
   }
   glEnd();

   glNormal3f(0.0, 0.0, -1.0);

   /* draw back face */
   glBegin(GL_QUAD_STRIP);
   for (i = 0; i <= teeth; i++) {
      angle = i * 2.0 * M_PI / teeth;
      glVertex3f(r1 * cos(angle), r1 * sin(angle), -width * 0.5);
      glVertex3f(r0 * cos(angle), r0 * sin(angle), -width * 0.5);
      if (i < teeth) {
	 glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da),
		    -width * 0.5);
	 glVertex3f(r0 * cos(angle), r0 * sin(angle), -width * 0.5);
      }
   }
   glEnd();

   /* draw back sides of teeth */
   glBegin(GL_QUADS);
   da = 2.0 * M_PI / teeth / 4.0;
   for (i = 0; i < teeth; i++) {
      angle = i * 2.0 * M_PI / teeth;

      glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da),
		 -width * 0.5);
      glVertex3f(r2 * cos(angle + 2 * da), r2 * sin(angle + 2 * da),
		 -width * 0.5);
      glVertex3f(r2 * cos(angle + da), r2 * sin(angle + da), -width * 0.5);
      glVertex3f(r1 * cos(angle), r1 * sin(angle), -width * 0.5);
   }
   glEnd();

   /* draw outward faces of teeth */
   glBegin(GL_QUAD_STRIP);
   for (i = 0; i < teeth; i++) {
      angle = i * 2.0 * M_PI / teeth;

      glVertex3f(r1 * cos(angle), r1 * sin(angle), width * 0.5);
      glVertex3f(r1 * cos(angle), r1 * sin(angle), -width * 0.5);
      u = r2 * cos(angle + da) - r1 * cos(angle);
      v = r2 * sin(angle + da) - r1 * sin(angle);
      len = sqrt(u * u + v * v);
      u /= len;
      v /= len;
      glNormal3f(v, -u, 0.0);
      glVertex3f(r2 * cos(angle + da), r2 * sin(angle + da), width * 0.5);
      glVertex3f(r2 * cos(angle + da), r2 * sin(angle + da), -width * 0.5);
      glNormal3f(cos(angle), sin(angle), 0.0);
      glVertex3f(r2 * cos(angle + 2 * da), r2 * sin(angle + 2 * da),
		 width * 0.5);
      glVertex3f(r2 * cos(angle + 2 * da), r2 * sin(angle + 2 * da),
		 -width * 0.5);
      u = r1 * cos(angle + 3 * da) - r2 * cos(angle + 2 * da);
      v = r1 * sin(angle + 3 * da) - r2 * sin(angle + 2 * da);
      glNormal3f(v, -u, 0.0);
      glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da),
		 width * 0.5);
      glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da),
		 -width * 0.5);
      glNormal3f(cos(angle), sin(angle), 0.0);
   }

   glVertex3f(r1 * cos(0), r1 * sin(0), width * 0.5);
   glVertex3f(r1 * cos(0), r1 * sin(0), -width * 0.5);

   glEnd();

   glShadeModel(GL_SMOOTH);

   /* draw inside radius cylinder */
   glBegin(GL_QUAD_STRIP);
   for (i = 0; i <= teeth; i++) {
      angle = i * 2.0 * M_PI / teeth;
      glNormal3f(-cos(angle), -sin(angle), 0.0);
      glVertex3f(r0 * cos(angle), r0 * sin(angle), -width * 0.5);
      glVertex3f(r0 * cos(angle), r0 * sin(angle), width * 0.5);
   }
   glEnd();
}
#endif