// Try XDG Shell and EGL together 
// POSIX
#define _POSIX_C_SOURCE 200112L
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

// Linux specific
#include <linux/input.h>

#include <EGL/egl.h>
#include <GL/gl.h>

// wayland
#include <wayland-client.h>
#include <wayland-egl.h>
#include <wayland-cursor.h>

#include "xdg-shell-client-protocol.h"

#ifndef True
#define True 1
#endif

#ifndef False
#define False 0
#endif

#define WIDTH 256
#define HEIGHT 256


static _Bool running = True;



#if 0
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

#endif

/* Wayland code */
struct client_state {
    /* Globals */
    struct wl_display *wl_display;
    struct wl_registry *wl_registry;
    struct wl_shm *wl_shm;
    struct wl_compositor *wl_compositor;
    struct xdg_wm_base *xdg_wm_base;
    struct wl_cursor_theme *cursor_theme;
    struct wl_subcompositor *wl_subcompositor;
    /* Global GL/EGL */
    EGLDisplay egl_display;

    /* Objects */
    struct wl_surface *wl_surface;
    struct xdg_surface *xdg_surface;
    struct xdg_toplevel *xdg_toplevel;
    struct wl_egl_window *wl_egl_window;
    /* Objects GL/EGL**/
    EGLContext egl_context;
    EGLSurface egl_surface;

    struct wl_keyboard *wl_keyboard;
    struct wl_pointer *wl_pointer;
    
    int width;
    int height;
    unsigned int border_size;
    unsigned int title_size;

    _Bool maximized;
#if 0
    struct csd_decoration decorations[9]; // corresponding to resize_cursor
    struct csd_button buttons[3]; // number of button_type
#endif
    _Bool button_pressed;
    struct wl_surface * wl_current_surface;    // last entered surface

    _Bool inhibit_motion;

};

#if 0
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
#endif

static void delete_window( struct client_state* state )
{
    eglDestroySurface( state->egl_display, state->egl_surface );
    wl_egl_window_destroy( state->wl_egl_window );
#if 0
    wl_shell_surface_destroy( state->shell_surface );
#endif
     if(state->xdg_wm_base) {
        xdg_toplevel_destroy(state->xdg_toplevel);
        xdg_surface_destroy(state->xdg_surface);
    }
    wl_surface_destroy( state->wl_surface );
    eglDestroyContext( state->egl_display, state->egl_context );
}

static void window_resize( struct client_state* state, const int width, const int height, _Bool full) {
//    std::cout << "config " << width << " " << height << std::endl;
    // main surface with from full surface
    int main_w, main_h;
    if(full) {
        main_w = width-2*state->border_size;
        main_h = height-2*state->border_size-state->title_size;
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
    // >>
    //wl_egl_window_resize(window->egl_window, main_w, main_h, 0, 0);

    // resize all decoration elements
#if 0
    for(int i = 0 ; i < num_of_decorations ; i++) { 
        decoration_resize(&window->decorations[i],main_w,main_h); 
    }
#endif
}

static int had_displayed = 0;

static void draw_window( struct client_state* state )
{
    eglMakeCurrent(state->egl_display, state->egl_surface, state->egl_surface, state->egl_context);

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

    glClearColor( 0.0, 1.0, 0.0, 1.0 );
    glClear( GL_COLOR_BUFFER_BIT );
    eglSwapBuffers( state->egl_display, state->egl_surface );
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
static _Bool xdg_surface_initialized = False;
static void xdg_surface_handle_configure(void *data,
        struct xdg_surface *xdg_surface, uint32_t serial) {

    printf("xdg_surface_configure is called");
    xdg_surface_ack_configure(xdg_surface, serial);
}

static struct xdg_surface_listener xdg_surface_listener = {
    .configure = xdg_surface_handle_configure,
};

void xdg_toplevel_handle_configure(void *data, struct xdg_toplevel *xdg_toplevel, int32_t width, int32_t height, struct wl_array *states) {
    if (width==0 || height==0)
        return;
    struct client_state *state = (struct client_state*)(data);
    window_resize(state, width, height, True);
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

static void create_window( struct client_state* state, int32_t width, int32_t height )
{
    const unsigned int border_size = 5;
    const unsigned int title_size = 15;

    eglBindAPI( EGL_OPENGL_API );
    EGLint attributes[] = {EGL_RED_SIZE, 8, EGL_GREEN_SIZE, 8, EGL_BLUE_SIZE, 8, EGL_ALPHA_SIZE, 8, EGL_NONE};
    EGLConfig config;
    EGLint num_config;
    eglChooseConfig( state->egl_display, attributes, &config, 1, &num_config );
    state->egl_context = eglCreateContext( state->egl_display, config, EGL_NO_CONTEXT, NULL );

    state->width = width;
    state->height = height;
    state->border_size = border_size;
    state->title_size = title_size;

    state->wl_surface = wl_compositor_create_surface( state->wl_compositor );

    if(state->xdg_wm_base) {
        state->xdg_surface = xdg_wm_base_get_xdg_surface(state->xdg_wm_base, state->wl_surface);
        xdg_surface_add_listener(state->xdg_surface, &xdg_surface_listener, state);

        state->xdg_toplevel = xdg_surface_get_toplevel(state->xdg_surface);
        //xdg_toplevel_add_listener(state->xdg_toplevel, &xdg_toplevel_listener, state);

        xdg_toplevel_set_title(state->xdg_toplevel, "wayland-egl-xdg-shell");
        //xdg_toplevel_set_app_id(state->xdg_toplevel, "wayland-egl-gear");
    }
    else {
        fprintf(stderr, "no xdg_wm_base\n");
#if 0
        window->shell_surface = wl_shell_get_shell_surface( shell, window->surface );
        wl_shell_surface_add_listener( window->shell_surface, &shell_surface_listener, window );
        wl_shell_surface_set_toplevel( window->shell_surface );
#endif
    }

    if(state->wl_shm)
    {
        state->cursor_theme = wl_cursor_theme_load(NULL, 32, state->wl_shm);
    }


    state->wl_egl_window = wl_egl_window_create( state->wl_surface, width, height );
    state->egl_surface = eglCreateWindowSurface( state->egl_display, config, state->wl_egl_window, NULL );
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
    struct client_state *state = data;
    if( !strcmp( interface, "wl_compositor" ) )
    {
        state->wl_compositor = wl_registry_bind( registry, name, &wl_compositor_interface, 4 );
    }
    else if (!strcmp(interface, "wl_subcompositor")) {
        state->wl_subcompositor = wl_registry_bind(registry, name, &wl_subcompositor_interface, version);
    }
#if 0
    else if (!strcmp(interface,"wl_seat")) 
    {
        seat = wl_registry_bind (registry, name, &wl_seat_interface, 1);
        wl_seat_add_listener (seat, &seat_listener, data);
    }
#endif
    else if (!strcmp(interface, "wl_shm")) {
        state->wl_shm = wl_registry_bind(registry, name, &wl_shm_interface, version);
    }
    else if (!strcmp(interface, xdg_wm_base_interface.name)) {
	if(version != 1)
	{
		printf("xdg_wm_base latest version:%d\n",version);		
		exit(-1);
	}
        state->xdg_wm_base = wl_registry_bind(registry, name, &xdg_wm_base_interface, 1);
        xdg_wm_base_add_listener(state->xdg_wm_base, &xdg_wm_base_listener, data);
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

    struct client_state state = { 0 };
     state.wl_display = wl_display_connect( NULL );

    if(!state.wl_display) {
        fprintf(stderr, "cannot connect to Wayland server\n");
        return -1;
    }


    state.wl_registry = wl_display_get_registry( state.wl_display );
    wl_registry_add_listener( state.wl_registry, &registry_listener, &state );

    wl_display_roundtrip( state.wl_display );

    state.egl_display = eglGetDisplay( state.wl_display );
    eglInitialize( state.egl_display, NULL, NULL );

   create_window( &state, WIDTH, HEIGHT );

    wl_surface_commit(state.wl_surface);

    // This call the attached listener global_registry_handler
    wl_display_roundtrip( state.wl_display );
    
   

    while( running )
    {
        wl_display_dispatch_pending( state.wl_display );
        draw_window( &state );
    }

    delete_window( &state );
    eglTerminate( state.egl_display );
    wl_display_disconnect( state.wl_display );
    return 0;
}



