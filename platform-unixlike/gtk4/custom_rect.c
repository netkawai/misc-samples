#include <gtk/gtk.h>

/* Define a custom widget for the rounded rectangle */
#define MY_TYPE_RECT (my_rect_get_type ())
G_DECLARE_FINAL_TYPE (MyRect, my_rect, MY, RECT, GtkWidget)

struct _MyRect { GtkWidget parent_instance; };
G_DEFINE_TYPE (MyRect, my_rect, GTK_TYPE_WIDGET)

/* 1. Rendering: Use GtkSnapshot to create a GSK Rounded Color Node */
static void my_rect_snapshot (GtkWidget *widget, GtkSnapshot *snapshot) {
    int w = gtk_widget_get_width (widget);
    int h = gtk_widget_get_height (widget);
    
    float rect_w = 200.0f;
    float rect_h = 100.0f;
    float radius = 20.0f; // Increased radius for visibility
    
    graphene_rect_t bounds = GRAPHENE_RECT_INIT((w - rect_w) / 2.0f, (h - rect_h) / 2.0f, rect_w, rect_h);

    /* --- INVESTIGATION LOGS --- */
    g_print ("[GSK Log] Widget Size: %dx%d\n", w, h);
    g_print ("[GSK Log] Rect Bounds: x=%.1f, y=%.1f, w=%.1f, h=%.1f\n", 
             bounds.origin.x, bounds.origin.y, bounds.size.width, bounds.size.height);
    g_print ("[GSK Log] Applying Radius: %.1f\n", radius);

    /* 1. Initialize the Rounded Rect structure */
    GskRoundedRect rounded_rect;
    gsk_rounded_rect_init_from_rect (&rounded_rect, &bounds, radius);

    /* 2. PUSH CLIP: This tells the GPU to mask everything inside the rounded area */
    gtk_snapshot_push_rounded_clip (snapshot, &rounded_rect);

    /* 3. Draw the color (now it will be clipped by the rounded mask) */
    gtk_snapshot_append_color (snapshot, &(GdkRGBA){0.2, 0.4, 0.8, 1.0}, &bounds);

    /* 4. POP CLIP: Stop clipping so text/other elements aren't cut off */
    gtk_snapshot_pop (snapshot);

    /* 5. Draw Text (Outside the clip so it doesn't get cut at corners) */
    PangoLayout *layout = gtk_widget_create_pango_layout (widget, "Click");
    PangoFontDescription *font = pango_font_description_from_string ("Sans Bold 14");
    pango_layout_set_font_description (layout, font);

    int text_w, text_h;
    pango_layout_get_pixel_size (layout, &text_w, &text_h);
    
    gtk_snapshot_save (snapshot);
    gtk_snapshot_translate (snapshot, &GRAPHENE_POINT_INIT(
        bounds.origin.x + (bounds.size.width - text_w) / 2.0f,
        bounds.origin.y + (bounds.size.height - text_h) / 2.0f
    ));
    gtk_snapshot_append_layout (snapshot, layout, &(GdkRGBA){1, 1, 1, 1});
    gtk_snapshot_restore (snapshot);

    g_object_unref (layout);
    pango_font_description_free (font);
}

/* 2. Hit Testing: Detect clicks inside the rectangle area */
static void on_click (GtkGestureClick *gesture, int n_press, double x, double y, gpointer user_data) {
    GtkWidget *widget = GTK_WIDGET (user_data);
    int w = gtk_widget_get_width (widget);
    int h = gtk_widget_get_height (widget);
    
    /* Define the same hit-box area */
    float rect_x = (w - 200) / 2.0f;
    float rect_y = (h - 100) / 2.0f;

    if (x >= rect_x && x <= rect_x + 200 && y >= rect_y && y <= rect_y + 100) {
        g_print ("Rectangle clicked! Exiting...\n");
        exit (0);
    }
}

static void my_rect_init (MyRect *self) {
    GtkGesture *gesture = gtk_gesture_click_new ();
    g_signal_connect (gesture, "pressed", G_CALLBACK (on_click), self);
    gtk_widget_add_controller (GTK_WIDGET (self), GTK_EVENT_CONTROLLER (gesture));
}

static void my_rect_class_init (MyRectClass *klass) {
    GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);
    widget_class->snapshot = my_rect_snapshot;
}

/* Main Application Entry */
static void activate (GtkApplication *app, gpointer user_data) {
    GtkWidget *window = gtk_application_window_new (app);
    gtk_window_set_title (GTK_WINDOW (window), "GSK Rounded Rect");
    gtk_window_set_default_size (GTK_WINDOW (window), 400, 300);

    GtkWidget *my_rect_widget = g_object_new (MY_TYPE_RECT, NULL);
    gtk_window_set_child (GTK_WINDOW (window), my_rect_widget);
    gtk_window_present (GTK_WINDOW (window));
}

int main (int argc, char **argv) {
    GtkApplication *app = gtk_application_new ("org.example.rect", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
    int status = g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref (app);
    return status;
}
