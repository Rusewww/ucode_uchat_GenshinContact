#include "client.h"

void mx_start_search_room(GtkSearchEntry *sentry, t_chat *chat) {
    gchar *search = (gchar *) gtk_entry_get_text(GTK_ENTRY(sentry));
    t_filter_data *filter_data = mx_create_filter_data(search);
    mx_clear_global_search(chat->builder);
    mx_search_delim_set_visibility(chat->builder, TRUE);
    if (!strlen(search)) {
        mx_stop_search_room(NULL, NULL, chat->builder);
    } else {
        mx_search_local_rooms(chat->builder, filter_data);
        t_dtp *dtp = mx_search_rooms_request(search);
        mx_send(chat->out, dtp);
        mx_free_request(&dtp);
    }
    mx_free_filter_data(filter_data);
}

void mx_start_search_msgs(GtkSearchEntry *sentry, t_chat *chat) {
    gchar *search = (gchar *) gtk_entry_get_text(GTK_ENTRY(sentry));
    mx_reset_room_for_search(chat->builder);
    mx_clear_found_msgs(chat->builder);
    if (!strlen(search)) {
        mx_stop_search_message(NULL, NULL, chat->builder);
    } else {
        t_groom *groom = mx_get_selected_groom(chat->builder, MX_LOCAL_ROOMS);
        t_dtp *dtp = mx_search_msgs_request(search, groom->id);
        mx_send(chat->out, dtp);
        mx_free_request(&dtp);
        mx_widget_set_visibility_by_name(chat->builder, "stack_messege_rooms", FALSE);
        mx_widget_set_visibility_by_name(chat->builder, "scrlldwnd_found_msgs", TRUE);
        mx_widget_set_visibility_by_name(chat->builder, "label_found_messages", TRUE);
    }
}

void mx_start_search_members(GtkSearchEntry *sentry, t_chat *chat) {
    gchar *search = (gchar *) gtk_entry_get_text(GTK_ENTRY(sentry));
    if (!strlen(search)) {
        mx_stop_search_members(NULL, NULL, chat->builder);
    } else {
        mx_search_members(chat->builder, search);
    }
}
