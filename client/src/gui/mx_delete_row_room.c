#include "../../inc/client.h"

void mx_unselect_room(t_groom *groom, GtkBuilder *build) {
    mx_set_default_room_sett(build);
    if (groom->page) {
        gtk_widget_hide(GTK_WIDGET(groom->page));
    }
    if (groom->row_room) {
        gtk_widget_hide(GTK_WIDGET(groom->row_room));
    }
    mx_set_room_widgets_visibility(build, FALSE);
}

void mx_delete_row_room(GtkListBoxRow *row, GtkBuilder *build) {
    t_groom *groom = g_object_get_data(G_OBJECT(row), "groom");
    if (gtk_list_box_row_is_selected(row)) {
        mx_unselect_room(groom, build);
    }
    if (groom->page) {
        gtk_widget_destroy(GTK_WIDGET(groom->page));
    }
    gtk_widget_destroy(GTK_WIDGET(row));
}
