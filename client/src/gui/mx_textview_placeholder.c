#include "../../inc/client.h"

static gboolean set(GtkWidget *text_view, GdkEvent *event, t_chat *chat) {
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
    const gchar *placeholder = "Enter your message";
    GtkTextIter start;
    GtkTextIter end;
    gtk_text_buffer_get_start_iter(buffer, &start);
    gtk_text_buffer_get_end_iter(buffer, &end);
    if (strlen(gtk_text_buffer_get_text(buffer, &start, &end, FALSE)) == 0) {
        gtk_text_buffer_set_text(buffer, placeholder, -1);
        chat->msg_placeholder = TRUE;
    }
    (void)event;
    return FALSE;
}

void mx_connect_set_placeholder(t_chat *chat) {
    GObject *text_view = gtk_builder_get_object(chat->builder, "msg_entry");
    g_signal_connect(GTK_TEXT_VIEW(text_view), "focus-out-event", G_CALLBACK(set), chat);
}

static gboolean unset(GtkWidget *text_view, GdkEvent  *event, t_chat *chat) {
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
    const gchar *placeholder = "";
    if (chat->msg_placeholder) {
        gtk_text_buffer_set_text(buffer, placeholder, -1);
        chat->msg_placeholder = FALSE;
    }
    (void)event;
    return FALSE;
}

void mx_connect_unset_placeholder(t_chat *chat) {
    GObject *text_view = gtk_builder_get_object(chat->builder, "msg_entry");
    g_signal_connect(GTK_TEXT_VIEW(text_view), "focus-in-event", G_CALLBACK(unset), chat);
}
