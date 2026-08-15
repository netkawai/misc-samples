#include <cairo.h>
#include <stdio.h>

int main() {
    // 1. Create a surface to draw on (Width, Height)
    cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 640, 480);
    cairo_t *cr = cairo_create(surface);

    // 2. Draw a Background (White)
    cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
    cairo_paint(cr);

    // 3. Draw a Blue Rectangle
    cairo_set_source_rgb(cr, 0.1, 0.4, 0.7);
    cairo_rectangle(cr, 150, 150, 340, 180);
    cairo_fill(cr);

    // 4. Draw Text
    cairo_set_source_rgb(cr, 1.0, 1.0, 1.0); // White text
    cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, 40.0);
    
    cairo_move_to(cr, 200, 250);
    cairo_show_text(cr, "Hello Cairo!");

    // 5. Export to PNG
    cairo_surface_write_to_png(surface, "output.png");

    // 6. Cleanup
    cairo_destroy(cr);
    cairo_surface_destroy(surface);

    printf("Image generated: output.png\n");
    return 0;
}