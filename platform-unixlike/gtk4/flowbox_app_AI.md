To achieve this using GPU rendering, we will use GtkFlowBox (the responsive grid engine) and standard GtkButton widgets.
In GTK 4, "changing the background color" is best handled via CSS. This is the most efficient way to communicate with the GPU (GSK), as the renderer translates CSS properties into optimized GPU render nodes.
Complete C Source Code
```
#include <gtk/gtk.h>
// Helper to apply CSS to a widget (Window)static void apply_bg_color(GtkWidget *widget, const char *color_name) {
    GtkStyleContext *context = gtk_widget_get_style_context(widget);
    GtkCssProvider *provider = gtk_css_provider_new();
    
    // Construct CSS to change the window background
    char *css = g_strdup_printf("window { background-color: %s; }", color_name);
    
    gtk_css_provider_load_from_data(provider, css, -1);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), 
                                   GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    
    g_free(css);
    g_object_unref(provider);
}
// Callback functions for each buttonstatic void on_red_clicked(GtkButton *btn, gpointer user_data) {
    apply_bg_color(GTK_WIDGET(user_data), "red");
}
static void on_yellow_clicked(GtkButton *btn, gpointer user_data) {
    apply_bg_color(GTK_WIDGET(user_data), "yellow");
}
static void on_green_clicked(GtkButton *btn, gpointer user_data) {
    apply_bg_color(GTK_WIDGET(user_data), "green");
}
static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Responsive GPU FlowBox");
    gtk_window_set_default_size(GTK_WINDOW(window), 650, 400);

    // 1. Create the FlowBox (The responsive layout engine)
    GtkWidget *flowbox = gtk_flow_box_new();
    gtk_flow_box_set_valign(GTK_FLOW_BOX(flowbox), GTK_ALIGN_START);
    gtk_flow_box_set_max_children_per_line(GTK_FLOW_BOX(flowbox), 10);
    gtk_flow_box_set_selection_mode(GTK_FLOW_BOX(flowbox), GTK_SELECTION_NONE);
    
    // Spacing between buttons
    gtk_flow_box_set_column_spacing(GTK_FLOW_BOX(flowbox), 10);
    gtk_flow_box_set_row_spacing(GTK_FLOW_BOX(flowbox), 10);

    // 2. Create 3 Buttons with specific sizes
    const char *labels[] = {"Red", "Yellow", "Green"};
    void (*callbacks[])(GtkButton*, gpointer) = {on_red_clicked, on_yellow_clicked, on_green_clicked};

    for (int i = 0; i < 3; i++) {
        GtkWidget *button = gtk_button_new_with_label(labels[i]);
        
        // Request 200x100 size
        gtk_widget_set_size_request(button, 200, 100);
        
        g_signal_connect(button, "clicked", G_CALLBACK(callbacks[i]), window);
        
        // Add to FlowBox
        gtk_flow_box_insert(GTK_FLOW_BOX(flowbox), button, -1);
    }

    gtk_window_set_child(GTK_WINDOW(window), flowbox);
    gtk_window_present(GTK_WINDOW(window));
}
int main(int argc, char **argv) {
    GtkApplication *app = gtk_application_new("org.example.flowbox", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}
```
Why this is Responsive and GPU-Friendly:

* GtkFlowBox: If you shrink the window width to less than 600px (3 * 200px), the buttons will automatically "flow" to the next line. This is the Fluid Grid behavior in GTK.
* CSS Rendering: When apply_bg_color is called, GTK updates the GSK Render Tree. The GPU (via OpenGL/Vulkan/Metal) simply clears the background with the new color during the next frame. No pixels are "painted" on the CPU.
* Size Requests: gtk_widget_set_size_request ensures the buttons stay at 200x100 regardless of the text length.

------------------------------
To make this even more "production-ready," would you like to:

* Add animations so the background color fades smoothly (using CSS transitions)?
* Use a GtkScrolledWindow so the FlowBox can scroll if there are many rows of buttons?
* See how to use GtkCenterBox if you want the buttons centered instead of top-aligned?

Add log
To see how GTK 4 handles the "reflow" of the fluid grid in real-time, you can connect to the size-allocate signal on the GtkFlowBox. This signal fires whenever the layout engine recalculates the positions of its children due to a window resize.
Updated Code with Layout Logging
Add this callback and signal connection to your existing sample:
```
#include <gtk/gtk.h>
// Callback to log the "Reflow" eventsstatic void on_layout_changed(GtkWidget *widget, int width, int height, int baseline, gpointer user_data) {
    // GtkFlowBox doesn't have a direct "get_columns" method, 
    // but we can see the allocated width change in real-time.
    static int last_width = 0;
    
    if (width != last_width) {
        g_print("[Layout Log] FlowBox Resized: %dpx wide. Buttons are reflowing...\n", width);
        
        // Logic check: based on 200px buttons + 10px spacing
        int max_cols = width / 210; 
        if (max_cols < 1) max_cols = 1;
        if (max_cols > 3) max_cols = 3;
        
        g_print("             Estimated Layout: %d Columns\n", max_cols);
        last_width = width;
    }
}
// ... (Rest of your previous activate function) ...
static void activate(GtkApplication *app, gpointer user_data) {
    // ... (Window and FlowBox creation code) ...
    
    GtkWidget *flowbox = gtk_flow_box_new();
    
    // Connect the resize/layout logger
    g_signal_connect(flowbox, "size-allocate", G_CALLBACK(on_layout_changed), NULL);

    // ... (Button creation and window setup) ...
}
```
Why this is helpful for your goal

* Automatic Handling: You will notice that on_layout_changed is called by GTK automatically. You don't have to manually catch SDL_WINDOWEVENT_RESIZED and loop through buttons.
* GPU Synchronization: Even as these logs fire, GSK is updating the render tree. On Wayland or macOS, the window manager coordinates with GTK to ensure the background color and button positions update in a single atomic frame, preventing "tearing."
* Resolution Independence: On a Retina Mac, the width reported in the log will be in logical points, not physical pixels. GTK handles the math so a 200-point button looks the same size on every screen.

Pro-Tip: GTK Inspector
Since you are using jhbuild, you can run your app like this to see the GPU nodes live:

GTK_DEBUG=interactive ./gsk_rect_app

Then, click on the "Visual" tab. You can see exactly which GSK Render Nodes (Color, Texture, Transform) the GPU is using to draw your buttons and background.
How would you like to proceed?

* Would you like to see how to make the buttons animate to their new positions during a resize?
* Should we add a GtkScrolledWindow so the buttons don't get cut off if the window height is too small?
* Are you interested in how to use CSS to make the buttons change shape when the window is narrow?

The error happens because GtkFlowBox is a widget, and alignment functions (like valign) belong to the base GtkWidget class. The warning is because GTK 4 is moving away from GtkStyleContext toward specialized per-widget CSS providers.
1. Fix the Error (The valign function)
In GTK 4, you use the generic gtk_widget_set_valign for all widgets. There is no gtk_flow_box_set_valign.
Change this:
```
gtk_flow_box_set_valign(GTK_FLOW_BOX(flowbox), GTK_ALIGN_START);
```
To this:
```
gtk_widget_set_valign(flowbox, GTK_ALIGN_START);
```
2. Fix the Warning (CSS Deprecation)
While gtk_style_context_add_provider still works, the "modern" GTK 4 way is to add the provider to the Display (globally) or use a specific widget's style context more carefully. However, for a simple background change, the most stable non-deprecated way is to use gtk_style_context_add_provider_for_display.
Change your apply_bg_color function to this:
```
static void apply_bg_color(GtkWidget *widget, const char *color_name) {
    GtkCssProvider *provider = gtk_css_provider_new();
    char *css = g_strdup_printf("window { background-color: %s; }", color_name);
    
    gtk_css_provider_load_from_data(provider, css, -1);
    
    // Add to the display so it affects the window
    gtk_style_context_add_provider_for_display(gdk_display_get_default(),
                                               GTK_STYLE_PROVIDER(provider),
                                               GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    
    g_free(css);
    g_object_unref(provider);
}
```
Why this happened

* C Strictness: Modern C compilers (like the one in your jhbuild environment) do not allow calling functions they haven't seen a "header declaration" for. Since gtk_flow_box_set_valign doesn't exist in gtkflowbox.h, the compiler assumed it was a mistake.
* GTK 4.10+ Changes: GTK is actively deprecating GtkStyleContext in favor of simpler CSS handling. Using add_provider_for_display is the safest way to avoid these warnings in newer versions.

------------------------------
After applying these fixes:

* Does the FlowBox now compile and "reflow" the buttons as expected?
* Would you like to see how to use GtkGrid if you want a more rigid (non-flowing) responsive layout?
* Should we add GDK_DEBUG=portals to your environment to check if macOS specific features are being triggered?


* GtkWidget Alignment Documentation
* GTK 4.10 Deprecations List


In GTK 4, the size-allocate signal was removed from the public API. It is now a virtual method intended only for internal widget implementations. Attempting to connect to it with g_signal_connect results in the "invalid signal" error you received. [1, 2, 3, 4] 
To achieve the same logging behavior in a modern way, you should use property notification on the window or widget's dimensions.
Fix: Replace the signal with property notification
Instead of connecting to size-allocate, connect to the notify::width property of the window. This fires whenever the window's width changes.
1. Update the Logger Callback
```
static void on_window_resize(GObject *object, GParamSpec *pspec, gpointer user_data) {
    GtkWindow *window = GTK_WINDOW(object);
    int width = gtk_window_get_default_width(window);
    
    // Logic check: based on 200px buttons + 10px spacing
    int max_cols = width / 210; 
    if (max_cols < 1) max_cols = 1;
    if (max_cols > 3) max_cols = 3;
    
    g_print("[Layout Log] Window Width: %dpx | Estimated Columns: %d\n", width, max_cols);
}
```
2. Update the Signal Connection
In your activate function, connect to the window's property notification:

// Instead of connecting to flowbox "size-allocate"// Connect to the window's width notification
g_signal_connect(window, "notify::default-width", G_CALLBACK(on_window_resize), NULL);

Why this is the correct fix:

* GTK 4 Architecture: GTK 4 encourages monitoring state changes (properties) rather than low-level layout events.
* Property Notify: The notify:: signal is the standard way in GObject to watch any property (like default-width or default-height).
* No Runtime Error: Because notify is a built-in signal for all GObjects, this will not crash or throw "invalid signal" errors. [5, 6] 

------------------------------
Now that the logger is fixed:

* Do the logs appear in your terminal when you drag the window edge?
* Would you like to see how to add a GtkScrolledWindow so your 3 buttons don't disappear if the window becomes too small?
* Should we investigate how to make the custom rounded rectangle widget a child of the GtkFlowBox?


[1] [https://discourse.gnome.org](https://discourse.gnome.org/t/gtk4-any-way-to-connect-to-a-window-resize-signal/14869)
[2] [https://discourse.gnome.org](https://discourse.gnome.org/t/gtk4-any-way-to-connect-to-a-window-resize-signal/14869)
[3] [https://stackoverflow.com](https://stackoverflow.com/questions/70488187/get-calculated-size-of-widget-in-gtk-4-0)
[4] [https://stackoverflow.com](https://stackoverflow.com/questions/70488187/get-calculated-size-of-widget-in-gtk-4-0)
[5] [https://stackoverflow.com](https://stackoverflow.com/questions/70265598/how-to-handle-window-resizes-in-gtk4)
[6] [https://docs.gtk.org](https://docs.gtk.org/gtk4/migrating-3to4.html)


The error occurs because gtk_window_get_default_width is not a valid function in GTK 4. In GTK 4, windows (and all widgets) have their current size stored in their layout.
To get the actual, live width of a window or widget during a resize, you should use gtk_widget_get_width.
Fixed Logger Callback
Replace your on_window_resize function with this version:
```
static void on_window_resize(GObject *object, GParamSpec *pspec, gpointer user_data) {
    // We can cast the object to a GtkWidget since GtkWindow is a widget
    GtkWidget *window_widget = GTK_WIDGET(object);
    
    // Get the current width as it appears on screen
    int width = gtk_widget_get_width(window_widget);
    
    // Prevent logging 0 during initial setup
    if (width <= 0) return;

    // Logic check: based on 200px buttons + 10px spacing
    int max_cols = width / 210; 
    if (max_cols < 1) max_cols = 1;
    if (max_cols > 3) max_cols = 3;
    
    g_print("[Layout Log] Window Width: %dpx | Estimated Columns: %d\n", width, max_cols);
}
```
Why this works:

* Widget API: In GTK 4, the functions gtk_widget_get_width() and gtk_widget_get_height() are the standard way to query the allocated size of any UI element.
* Property Consistency: By connecting to notify::default-width, the callback is triggered whenever the window's width property changes. However, inside the callback, gtk_widget_get_width gives you the actual rendered dimension.
* Implicit Declarations: Your compiler (Clang/GCC) is strict. It only allows functions explicitly defined in the headers (like gtk/gtk.h). Since gtk_window_get_default_width doesn't exist, it failed.

------------------------------
Now that the compiler error is fixed:

* Do you see the logs in your terminal when you resize the window?
* Would you like to see how to make the buttons themselves responsive (e.g., changing their labels or colors based on the window width)?
* Should we investigate how to center the buttons within the GtkFlowBox so they don't stay stuck to the left?


* GtkWidget get_width Documentation



