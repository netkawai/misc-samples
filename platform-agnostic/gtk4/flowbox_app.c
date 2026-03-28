#include <gtk/gtk.h>

static void apply_bg_color(GtkWidget *widget, const char *color_name) {
    GtkCssProvider *provider = gtk_css_provider_new();
    char *css = g_strdup_printf("window { background-color: %s; }", color_name);
    
    // Modern GTK 4.12+ way: load directly from the string
    gtk_css_provider_load_from_string(provider, css);
    
    // Apply globally to the current display
    gtk_style_context_add_provider_for_display(gdk_display_get_default(),
                                               GTK_STYLE_PROVIDER(provider),
                                               GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    
    g_free(css);
    g_object_unref(provider);
}

// Callback functions for each button
static void on_red_clicked(GtkButton *btn, gpointer user_data) {
    apply_bg_color(GTK_WIDGET(user_data), "red");
}

static void on_yellow_clicked(GtkButton *btn, gpointer user_data) {
    apply_bg_color(GTK_WIDGET(user_data), "yellow");
}

static void on_green_clicked(GtkButton *btn, gpointer user_data) {
    apply_bg_color(GTK_WIDGET(user_data), "green");
}

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

static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Responsive GPU FlowBox");
    gtk_window_set_default_size(GTK_WINDOW(window), 650, 400);

    // Instead of connecting to flowbox "size-allocate"
    // Connect to the window's width notification
    g_signal_connect(window, "notify::default-width", G_CALLBACK(on_window_resize), NULL);
 
    // 1. Create the FlowBox (The responsive layout engine)
    GtkWidget *flowbox = gtk_flow_box_new();

    gtk_widget_set_valign(flowbox, GTK_ALIGN_START);
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
