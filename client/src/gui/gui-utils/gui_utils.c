#include "client.h"

gchar *mx_get_buffer_text(gchar *buff_name, GtkBuilder *builder) {
    GObject *buf = gtk_builder_get_object(builder, buff_name);
    GtkTextIter start;
    GtkTextIter end;
    if (GTK_IS_TEXT_BUFFER(buf)) {
        gtk_text_buffer_get_start_iter(GTK_TEXT_BUFFER(buf), &start);
        gtk_text_buffer_get_end_iter(GTK_TEXT_BUFFER(buf), &end);
        return gtk_text_buffer_get_text(GTK_TEXT_BUFFER(buf), &start, &end, FALSE);
    } else {
        return (gchar *) gtk_entry_buffer_get_text(GTK_ENTRY_BUFFER(buf));
    }
}

void mx_clear_buffer_text(gchar *buff_name, GtkBuilder *builder) {
    GObject *buf = gtk_builder_get_object(builder, buff_name);
    GtkTextIter start;
    GtkTextIter end;
    if (GTK_IS_TEXT_BUFFER(buf)) {
        gtk_text_buffer_get_start_iter(GTK_TEXT_BUFFER(buf), &start);
        gtk_text_buffer_get_end_iter(GTK_TEXT_BUFFER(buf), &end);
        gtk_text_buffer_delete(GTK_TEXT_BUFFER(buf), &start, &end);
    } else {
        gtk_entry_buffer_delete_text(GTK_ENTRY_BUFFER(buf), 0, -1);
    }
}

void mx_focus_out(GtkWidget *widget, GdkEvent *event, gpointer *user_data) {
    gtk_widget_hide(widget);
    (void) user_data;
    (void) event;
}

gchar *mx_entry_get_text(gchar *entry_name, GtkBuilder *builder) {
    GtkEntry *entry = GTK_ENTRY(gtk_builder_get_object(builder, entry_name));
    return (gchar *) gtk_entry_get_text(entry);
}
