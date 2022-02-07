#include "../../inc/client.h"

static void help_add(t_gmsg *msg, t_chat *chat, gint pos) {
    t_groom *groom = mx_get_groom_by_id(msg->room_id, chat->builder);
    GtkWidget *row = gtk_list_box_row_new();
    GtkWidget *event = mx_create_message_row(chat, msg);
    GtkListBox *box = NULL;
    t_signal_data *data = NULL;
    if (!groom) {
        return;
    }
    box = groom->box_messages;
    gtk_widget_set_can_focus(row, FALSE);
    data = mx_create_sig_data(chat, NULL, GTK_LIST_BOX_ROW(row));
    g_signal_connect(event, "button_release_event", G_CALLBACK(mx_select_msg), data);
    g_object_set_data_full(G_OBJECT(row), "sigdata", data, (GDestroyNotify) mx_free_sigdata);
    msg->row_msg = GTK_LIST_BOX_ROW(row);
    g_object_ref(row);
    gtk_container_add(GTK_CONTAINER(row), event);
    groom->is_updated = TRUE;
    gtk_list_box_insert(box, row, pos);
    gtk_list_box_row_changed(groom->row_room);
    groom->is_updated = FALSE;
    groom->is_watched = FALSE;
    gtk_widget_show_all(GTK_WIDGET(box));
    g_object_set_data_full(G_OBJECT(row), "gmsg", msg, (GDestroyNotify) mx_delete_gmsg);
}

void mx_add_message_to_room_new(t_gmsg *msg, t_chat *chat) {
    help_add(msg, chat, -1);
}

void mx_add_message_to_room_old(t_gmsg *msg, t_chat *chat) {
    help_add(msg, chat, 0);
}
