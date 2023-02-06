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

#include <stdio.h>

#include <wayland-client.h>
#include <wayland-egl.h>

#include <wayland-cursor.h>

#include <string.h>
#include <sys/time.h>
#include <unistd.h>

// Linux specific
#include <linux/input.h>

#include <EGL/egl.h>
#include <GL/gl.h>
#include <string.h>

#define WIDTH 256
#define HEIGHT 256

static struct wl_display* display;
static struct wl_compositor* compositor = NULL;
static struct xdg_wm_base *xdg_wm_base = NULL;
static struct wl_seat *seat = NULL;
static struct wl_shm *shm = NULL;
static struct wl_cursor_theme *cursor_theme = NULL;
static struct wl_surface *cursor_surface = NULL;

static struct wl_subcompositor *subcompositor = NULL;

static EGLDisplay egl_display;
static bool running = false;

struct cs_decoration;

struct cs_button;

struct window {
    EGLContext egl_context;
    struct wl_surface *surface;
    struct xdg_surface *xdg_surface;
    struct xdg_toplevel *xdg_toplevel = NULL;
    struct wl_egl_window *egl_window;
    EGLSurface egl_surface;

    struct wl_keyboard *wl_keyboard;
    struct wl_pointer *wl_pointer;
    
    int width;
    int height;
    uint border_size;
    uint title_size;

    bool maximised = false;
#define DECORATION_ARRAY_LENGTH 9
    cs_decoration decorations[DECORATION_ARRAY_LENGTH]; // 
#define TOTAL_CSD_BUTTONS 
    cs_button buttons[TOTAL_CSD_BUTTONS];

    bool button_pressed = false;
    wl_surface * current_surface = NULL;    // last entered surface

    bool inhibit_motion = false;
};

struct button {
    struct wl_surface *surface;
    struct wl_subsurface *subsurface;
    struct wl_egl_window *egl_window;
    EGLSurface egl_surface;
    EGLContext egl_context;
    EGLDisplay egl_display;
    double r, g, b, a;
    bool initialized = false;

    enum button_type {
        CLOSE, 
        MAXIMISE, 
        MINIMISE
    } function;
};

static void button_init(struct button * target, // out
        wl_compositor* compositor, 
        wl_subcompositor* subcompositor, 
        wl_surface* source,
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
        target->subsurface = wl_subcompositor_get_subsurface(subcompositor, surface, source);
        target->wl_subsurface_set_desync(subsurface);
        target->egl_display = eglGetDisplay(display);
        target->egl_context = eglCreateContext (egl_display, config, EGL_NO_CONTEXT, NULL);
        eglInitialize(egl_display, NULL, NULL);
        wl_subsurface_set_position(subsurface, x, y);
        target->egl_window = wl_egl_window_create(surface, 10, 8);
        target->egl_surface = eglCreateWindowSurface(egl_display, config, egl_window, NULL);
}

static void button_draw(struct button * target)
{
        eglMakeCurrent(egl_display, egl_surface, egl_surface, egl_context);
        glClearColor(r, g, b, a);
        glClear(GL_COLOR_BUFFER_BIT);
        eglSwapBuffers(egl_display, egl_surface);
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



    void calc_dim(const int main_w, const int main_h, int &x, int &y, int &w, int &h) {
        // get position and dimension from type and main surface
        switch (function) {
        case XDG_TOPLEVEL_RESIZE_EDGE_NONE:
            x=0; y=-title_bar_size;
            w=main_w; h=title_bar_size;
            break;
        case XDG_TOPLEVEL_RESIZE_EDGE_TOP:
            x=0; y=-title_bar_size-border_size;
            w=main_w; h=border_size;
            break;
        case XDG_TOPLEVEL_RESIZE_EDGE_BOTTOM:
            x=0; y=main_h;
            w=main_w; h=border_size;
            break;
        case XDG_TOPLEVEL_RESIZE_EDGE_LEFT:
            x=-border_size; y=-title_bar_size;
            w=border_size; h=main_h+title_bar_size;
            break;
        case XDG_TOPLEVEL_RESIZE_EDGE_TOP_LEFT:
            x=-border_size; y=-border_size-title_bar_size;
            w=border_size; h=border_size;
            break;
        case XDG_TOPLEVEL_RESIZE_EDGE_BOTTOM_LEFT:
            x=-border_size; y=main_h;
            w=border_size; h=border_size;
            break;
        case XDG_TOPLEVEL_RESIZE_EDGE_RIGHT:
            x=main_w; y=-title_bar_size;
            w=border_size; h=main_h+title_bar_size;
            break;
        case XDG_TOPLEVEL_RESIZE_EDGE_TOP_RIGHT:
            x=main_w; y=-border_size-title_bar_size;
            w=border_size; h=border_size;
            break;
        case XDG_TOPLEVEL_RESIZE_EDGE_BOTTOM_RIGHT:
            x=main_w; y=main_h;
            w=border_size; h=border_size;
            break;
        }
    }

    void resize(const int main_w, const int main_h) {
        int x,y,w,h;
        calc_dim(main_w, main_h, x, y, w, h);
        wl_subsurface_set_position(subsurface, x, y);
//        egl_window = wl_egl_window_create(surface, w, h);
        wl_egl_window_resize(egl_window, w, h, 0, 0);

    }

    void draw() const {
        eglMakeCurrent(egl_display, egl_surface, egl_surface, egl_context);
        //glClearColor(1.0, 1.0, 0.0, 1.0);
        glClearColor(r, g, b, a);
        glClear(GL_COLOR_BUFFER_BIT);
        eglSwapBuffers(egl_display, egl_surface);
    }
};


static void decoration_init(struct csd_decoration *target, // out
    wl_compositor* compositor, wl_subcompositor* subcompositor, wl_surface* source,
               const uint _border_size, const uint _title_bar_size,
               EGLConfig config, enum xdg_toplevel_resize_edge type,
               double _r, double _g, double _b, double _a)
{
    function = type;
    r=_r; g=_g; b=_b; a=_a;
    border_size = _border_size;
    title_bar_size = _title_bar_size;

    surface = wl_compositor_create_surface(compositor);
    subsurface = wl_subcompositor_get_subsurface(subcompositor, surface, source);
    wl_subsurface_set_desync(subsurface);
    egl_display = eglGetDisplay(display);
    egl_context = eglCreateContext (egl_display, config, EGL_NO_CONTEXT, NULL);
    eglInitialize(egl_display, NULL, NULL);
    wl_subsurface_set_position(subsurface, 0, 0);
    egl_window = wl_egl_window_create(surface, 1, 1);
    egl_surface = eglCreateWindowSurface(egl_display, config, egl_window, NULL);

}

// listeners for seat (input)
static void
wl_seat_capabilities(void *data, struct wl_seat *wl_seat, uint32_t capabilities)
{
       struct client_state *state = data;
       /* TODO */
}

static void
wl_seat_name(void *data, struct wl_seat *wl_seat, const char *name)
{
       fprintf(stderr, "seat name: %s\n", name);
}
// declaration
static const struct wl_seat_listener wl_seat_listener = {
       .capabilities = wl_seat_capabilities,
       .name = wl_seat_name,
};
//

// listeners for registry
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
    else if (!strcmp(interface,"wl_seat")) 
    {
        seat = static_cast<wl_seat*>(wl_registry_bind (registry, name, &wl_seat_interface, 1));
        wl_seat_add_listener (seat, &seat_listener, data);
    }
}

static void registry_remove_object( void* data, struct wl_registry* registry, uint32_t name )
{
}
// declaration
static struct wl_registry_listener registry_listener = 
{
    &registry_add_object,
    &registry_remove_object
};
//

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

static void create_window( struct window* window, int32_t width, int32_t height )
{
    eglBindAPI( EGL_OPENGL_API );
    EGLint attributes[] = {EGL_RED_SIZE, 8, EGL_GREEN_SIZE, 8, EGL_BLUE_SIZE, 8, EGL_NONE};
    EGLConfig config;
    EGLint num_config;
    eglChooseConfig( egl_display, attributes, &config, 1, &num_config );
    window->egl_context = eglCreateContext( egl_display, config, EGL_NO_CONTEXT, NULL );

    window->surface = wl_compositor_create_surface( compositor );
    window->shell_surface = wl_shell_get_shell_surface( shell, window->surface );
    wl_shell_surface_add_listener( window->shell_surface, &shell_surface_listener, window );
    wl_shell_surface_set_toplevel( window->shell_surface );
    window->egl_window = wl_egl_window_create( window->surface, width, height );
    window->egl_surface = eglCreateWindowSurface( egl_display, config, window->egl_window, NULL );
    eglMakeCurrent( egl_display, window->egl_surface, window->egl_surface, window->egl_context );
}

static void delete_window( struct window* window )
{
    eglDestroySurface( egl_display, window->egl_surface );
    wl_egl_window_destroy( window->egl_window );
    wl_shell_surface_destroy( window->shell_surface );
    wl_surface_destroy( window->surface );
    eglDestroyContext( egl_display, window->egl_context );
}

static int had_displayed = 0;

static void draw_window( struct window* window )
{
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

    /* Here event loop I believe */
    draw_gear_loop();
#if 0
    glClearColor( 0.0, 1.0, 0.0, 1.0 );
    glClear( GL_COLOR_BUFFER_BIT );
    eglSwapBuffers( egl_display, window->egl_surface );
#endif

}

int main()
{
    display = wl_display_connect( NULL );
    struct wl_registry* registry = wl_display_get_registry( display );
    wl_registry_add_listener( registry, &registry_listener, NULL );
    wl_display_roundtrip( display );

    egl_display = eglGetDisplay( display );
    eglInitialize( egl_display, NULL, NULL );

    struct window window;
    create_window( &window, WIDTH, HEIGHT );

    while( running )
    {
        wl_display_dispatch_pending( display );
        draw_window( &window );
    }

    delete_window( &window );
    eglTerminate( egl_display );
    wl_display_disconnect( display );
    return 0;
}



/* Gears */

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
