#include "../../inc/client.h"

static gboolean filter_help(GtkListBoxRow *row, t_filter_data *data) {
    t_groom *groom = (t_groom *) g_object_get_data(G_OBJECT(row), "groom");
    char *room_name = (char *) gtk_label_get_text(groom->label_name);
    if (!strncasecmp(room_name, data->search_name, strlen(data->search_name))) {
        data->is_found_rooms = TRUE;
        return TRUE;
    } else {
        data->is_found_rooms = (data->is_found_rooms || FALSE);
        return FALSE;
    }
}

void mx_search_delim_set_visibility(GtkBuilder *builder, gboolean is_visible) {
    mx_widget_set_visibility_by_name(builder, "label_search_delim_global", is_visible);
    mx_widget_set_visibility_by_name(builder, "label_search_delim_local", is_visible);
}

void mx_search_local_rooms(GtkBuilder *builder, t_filter_data *data) {
    GObject *listbox = gtk_builder_get_object(builder, "listbox_rooms");
    gtk_list_box_set_filter_func(GTK_LIST_BOX(listbox), (GtkListBoxFilterFunc) filter_help, data, NULL);
    if (!data->is_found_rooms) {
        mx_widget_set_visibility_by_name(builder, "label_search_nothing_local", TRUE);
    } else {
        mx_widget_set_visibility_by_name(builder, "label_search_nothing_local", FALSE);
    }
}

void mx_clear_global_search(GtkBuilder *builder) {
    GObject *box = (gtk_builder_get_object(builder, "listbox_global_rooms"));
    GtkListBoxRow *row = gtk_list_box_get_row_at_index(GTK_LIST_BOX(box), 0);
    mx_widget_set_visibility_by_name(builder, "label_search_nothing_global", FALSE);
    while (row) {
        mx_delete_row_room(row, builder);
        row = gtk_list_box_get_row_at_index(GTK_LIST_BOX(box), 0);
    }
}

gboolean mx_stop_search_room(gpointer *entry, gpointer *data, GtkBuilder *builder) {
    GObject *listbox = gtk_builder_get_object(builder, "listbox_rooms");
    GObject *sentry = gtk_builder_get_object(builder, "sentry_rooms");

    if (!mx_get_selected_groom(builder, MX_GLOBAL_ROOMS)) {
        mx_search_delim_set_visibility(builder, FALSE);
        gtk_entry_set_text(GTK_ENTRY(sentry), "");
        gtk_list_box_set_filter_func(GTK_LIST_BOX(listbox), NULL, NULL, NULL);
        gtk_list_box_invalidate_filter(GTK_LIST_BOX(listbox));
        mx_widget_set_visibility_by_name(builder, "label_search_nothing_local", FALSE);
        mx_clear_global_search(builder);
    }

    (void) entry;
    (void) data;
    return FALSE;
}

void mx_add_message_to_found(t_gmsg *gmsg, t_chat *chat) {
    GtkWidget *row = gtk_list_box_row_new();
    GtkWidget *event = mx_create_message_row(chat, gmsg);
    GObject *box = gtk_builder_get_object(chat->builder,
                                          "listbox_found_msgs");

    gtk_widget_set_can_focus(row, FALSE);
    gmsg->row_msg = GTK_LIST_BOX_ROW(row);
    g_object_ref(row);
    gtk_container_add(GTK_CONTAINER(row), event);
    gtk_list_box_insert(GTK_LIST_BOX(box), row, -1);
    gtk_widget_show_all(GTK_WIDGET(box));
}

void mx_clear_found_msgs(GtkBuilder *builder) {
    GObject *box = (gtk_builder_get_object(builder, "listbox_found_msgs"));
    GtkListBoxRow *row = gtk_list_box_get_row_at_index(GTK_LIST_BOX(box), 0);

    mx_widget_set_visibility_by_name(builder,
                                     "label_search_nothing_msgs", FALSE);
    while (row) {
        gtk_widget_destroy(GTK_WIDGET(row));
        row = gtk_list_box_get_row_at_index(GTK_LIST_BOX(box), 0);
    }
    mx_widget_set_visibility_by_name(builder, "stack_messege_rooms", TRUE);
    mx_widget_set_visibility_by_name(builder, "scrlldwnd_found_msgs", FALSE);
    mx_widget_set_visibility_by_name(builder, "label_found_messages", FALSE);
}

gboolean mx_stop_search_message(gpointer *entry,
                                gpointer *data, GtkBuilder *builder) {
    GObject *sentry = gtk_builder_get_object(builder, "sentry_messages");

    gtk_entry_set_text(GTK_ENTRY(sentry), "");
    mx_clear_found_msgs(builder);
    (void)entry;
    (void)data;
    return FALSE;
}

static gboolean filter_member_login(GtkListBoxRow *row, gchar *search_login) {
    GList *gl_row = gtk_container_get_children(GTK_CONTAINER(row));
    GList *gl_box = gtk_container_get_children(GTK_CONTAINER(gl_row->data));
    GtkLabel *label = GTK_LABEL(gl_box->data);
    gchar *login = (gchar *) gtk_label_get_text(label);
    if (!g_ascii_strncasecmp(login, search_login, strlen(search_login))) {
        return TRUE;
    } else {
        return FALSE;
    }
}

void mx_search_members(GtkBuilder *builder, gchar *search_login) {
    GObject *listbox = gtk_builder_get_object(builder, "listbox_info_members");
    gtk_list_box_set_filter_func(GTK_LIST_BOX(listbox), (GtkListBoxFilterFunc) filter_member_login, search_login, NULL);
}

gboolean mx_stop_search_members(gpointer *entry, gpointer *data, GtkBuilder *builder) {
    GObject *listbox = gtk_builder_get_object(builder, "listbox_info_members");
    GObject *sentry = gtk_builder_get_object(builder, "sentry_messages");
    gtk_entry_set_text(GTK_ENTRY(sentry), "");
    gtk_list_box_set_filter_func(GTK_LIST_BOX(listbox), NULL, NULL, NULL);
    gtk_list_box_invalidate_filter(GTK_LIST_BOX(listbox));
    (void) data;
    (void) entry;
    return FALSE;
}
