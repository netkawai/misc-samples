// gcc -o x11-egl x11-egl.c -lX11 -lEGL -lGL

/* Standard C include file */
#include <stdio.h>
#include <string.h>

/* EGL and GL */
#include <EGL/egl.h>
#include <GL/gl.h>

/* Xlib include files */
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>

#define WIDTH 256
#define HEIGHT 256

static Display* x11_display;
static EGLDisplay egl_display;
static char running = 1;

struct window
{
    EGLContext egl_context;
    Window x11_window;
    EGLSurface egl_surface;
};
#if 0
// listeners
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
    else if( !strcmp( interface, "wl_shell" ) )
    {
        shell = wl_registry_bind( registry, name, &wl_shell_interface, 1 );
    }
}

static void registry_remove_object( void* data, struct wl_registry* registry, uint32_t name )
{
}

static struct wl_registry_listener registry_listener = 
{
    &registry_add_object,
    &registry_remove_object
};

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

static void create_window( struct window* window, int32_t width, int32_t height )
{
    eglBindAPI( EGL_OPENGL_API );
    EGLint attributes[] = {EGL_RED_SIZE, 8, EGL_GREEN_SIZE, 8, EGL_BLUE_SIZE, 8, EGL_NONE};
    EGLConfig config;
    EGLint num_config;
    eglChooseConfig( egl_display, attributes, &config, 1, &num_config );
    window->egl_context = eglCreateContext( egl_display, config, EGL_NO_CONTEXT, NULL );

    Window root = DefaultRootWindow(x11_display);
    window->x11_window = XCreateSimpleWindow(x11_display, root, 0,0, width, height,
                                       0,0,0xffffffff); 

    // window->egl_window = wl_egl_window_create( window->surface, width, height );
    window->egl_surface = eglCreateWindowSurface( egl_display, config, window->x11_window, NULL );
    eglMakeCurrent( egl_display, window->egl_surface, window->egl_surface, window->egl_context );
}

static void delete_window( struct window* window )
{
    eglDestroySurface( egl_display, window->egl_surface );
    // wl_egl_window_destroy( window->egl_window );
    // wl_shell_surface_destroy( window->shell_surface );
    // wl_surface_destroy( window->surface );
    eglDestroyContext( egl_display, window->egl_context );
    //XDestroyWindow(window->x11_window);
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

    glClearColor( 0.0, 1.0, 0.0, 1.0 );
    glClear( GL_COLOR_BUFFER_BIT );
    eglSwapBuffers( egl_display, window->egl_surface );
}

int main()
{
    x11_display = XOpenDisplay( NULL );

    egl_display = eglGetDisplay( x11_display );
    eglInitialize( egl_display, NULL, NULL );

    struct window window;
    create_window( &window, WIDTH, HEIGHT );

    /*Display window */
    XMapWindow(x11_display, window.x11_window);

    /* Get events, use first to display text and graphics */
    while(running)
    {
        while ( XPending ( x11_display ) ) {
            XEvent report;
            XNextEvent(x11_display,&report);
            switch(report.type){
                case ButtonPress:
                /* Trickle down into KeyPress (no break) */
                case KeyPress:
                running = 0;
                break;
                default:
                /* All events selected by StructureNotifyMask
                 * except ConfigureNotify are thrown away here,
                 * since noting is done with them */
                break;
            } 
        }
        draw_window( &window );
    }

    delete_window( &window );
    eglTerminate( egl_display );
    // wl_display_disconnect( x11_display );
    XDestroyWindow(x11_display,window.x11_window);
    XCloseDisplay(x11_display);
    return 0;
}
