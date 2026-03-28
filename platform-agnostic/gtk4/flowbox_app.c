#include <gtk/gtk.h>

// Helper to apply CSS to a widget (Window)
static void apply_bg_color(GtkWidget *widget, const char *color_name) {
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


#include <gtk/gtk.h>

// Callback to log the "Reflow" events
static void on_layout_changed(GtkWidget *widget, int width, int height, int baseline, gpointer user_data) {
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

static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Responsive GPU FlowBox");
    gtk_window_set_default_size(GTK_WINDOW(window), 650, 400);

    // 1. Create the FlowBox (The responsive layout engine)
    GtkWidget *flowbox = gtk_flow_box_new();

        // Connect the resize/layout logger
    g_signal_connect(flowbox, "size-allocate", G_CALLBACK(on_layout_changed), NULL);

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
