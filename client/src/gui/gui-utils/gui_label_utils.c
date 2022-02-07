#include "client.h"

void mx_clear_label_by_name(gchar *label_name, GtkBuilder *builder) {
    GtkLabel *label = GTK_LABEL(gtk_builder_get_object(builder, label_name));
    gtk_label_set_text(label, "\0");
}

void mx_label_set_num(gchar *widget_name, GtkBuilder *builder, gint number) {
    GObject *widget = gtk_builder_get_object(builder, widget_name);
    gchar *number_str = g_strdup_printf("%d", number);
    gtk_label_set_text(GTK_LABEL(widget), number_str);
    g_free(number_str);
}
