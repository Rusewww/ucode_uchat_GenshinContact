#include "client.h"

static void req_addroom(GtkButton *btn, t_chat *chat) {
    gchar *room_name = mx_get_buffer_text("buffer_roomname", chat->builder);
    t_dtp *dtp = mx_new_room_request(room_name, "", DB_GLOBAL_CHAT);
    mx_send(chat->out, dtp);
    mx_free_request(&dtp);
    mx_reset_addroom(NULL, chat->builder);
    (void)btn;
}

void mx_connect_addroom(t_chat *chat) {
    GtkButton *btn = GTK_BUTTON(gtk_builder_get_object(chat->builder, "btn_addroom_apply"));
    g_signal_connect(btn, "clicked", G_CALLBACK(req_addroom), chat);
}

static gboolean is_valid_auth_data(gchar *login, gchar *pass, GtkLabel *label) {
    if (*login == '\0' || *pass == '\0') {
        gtk_label_set_text(label, MX_ERRMSG_NODATA);
        return FALSE;
    } else if (!mx_match_search(login, MX_LOGIN_REGEX)) {
        gtk_label_set_text(label, MX_ERRMSG_INVALID_LOGIN);
        return FALSE;
    }
    return TRUE;
}

static void choose_auth(GtkButton *btn, t_chat *chat) {
    GtkNotebook *note = GTK_NOTEBOOK(gtk_builder_get_object(chat->builder, "notebook_auth"));
    guint page_num = gtk_notebook_get_current_page(note);

    if (page_num == 0) {
        GObject *label_error = gtk_builder_get_object(chat->builder, "label_autherror_login");
        gchar *login = mx_get_buffer_text("buffer_login", chat->builder);
        gchar *password = mx_get_buffer_text("buffer_password", chat->builder);
        if (is_valid_auth_data(login, password, GTK_LABEL(label_error))) {
            mx_send_auth_request(login, password, chat, RQ_LOG_IN);
        }
    } else {
        GObject *label_error = gtk_builder_get_object(chat->builder, "label_autherror_signup");
        gchar *login = mx_get_buffer_text("buffer_login", chat->builder);
        gchar *password = mx_get_buffer_text("buffer_password", chat->builder);
        gchar *confirm = mx_get_buffer_text("buffer_password_confirm", chat->builder);
        if (is_valid_auth_data(login, password, GTK_LABEL(label_error))) {
            if (!strcmp(password, confirm)) {
                mx_send_auth_request(login, password, chat, RQ_SIGN_UP);
            } else {
                gtk_label_set_text(GTK_LABEL(label_error), MX_ERRMSG_DIFPASS);
            }
        }
    }
    (void) btn;
}

void mx_connect_authorization(t_chat *chat) {
    GtkButton *btn_confirm = GTK_BUTTON(gtk_builder_get_object(chat->builder, "btn_auth_confirm"));
    g_signal_connect(btn_confirm, "clicked", G_CALLBACK(choose_auth), chat);
}

static void ban(GObject *btn, t_chat *chat) {
    t_groom *groom = mx_get_selected_groom(chat->builder, MX_LOCAL_ROOMS);
    t_dtp *dtp = NULL;
    dtp = mx_ban_member_request(groom->id, (guint64)g_object_get_data(btn, "member_id"));
    mx_send(chat->out, dtp);
    mx_free_request(&dtp);
}

void mx_connect_ban_member(t_chat *chat) {
    GObject *btn = gtk_builder_get_object(chat->builder, "btn_ban");
    g_signal_connect(btn, "clicked", G_CALLBACK(ban), chat);
}

static void open_f(GtkButton *btn, t_chat *chat) {
    t_groom *groom = mx_get_selected_groom(chat->builder, MX_LOCAL_ROOMS);
    GObject *popup = gtk_builder_get_object(chat->builder, "filechooser");
    gchar *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(popup));
    mx_reset_messege_room(NULL, chat->builder);
    mx_upload_file(filename, groom->id, chat);
    g_free(filename);
    (void)btn;
}

void mx_connect_filechooser(t_chat *chat) {
    GObject *btn = gtk_builder_get_object(chat->builder,"btn_filechooser_open");
    g_signal_connect(btn, "clicked", G_CALLBACK(open_f), chat);
}

void mx_go_down(GtkButton *btn, GtkBuilder *builder) {
    t_groom *groom = mx_get_selected_groom(builder, MX_LOCAL_ROOMS);
    GtkAdjustment *adj = gtk_scrolled_window_get_vadjustment(groom->page);
    gtk_adjustment_set_value(adj, gtk_adjustment_get_upper(adj));
    (void) btn;
}

void mx_req_send_message(GtkButton *btn, t_chat *chat) {
    gchar *message_text = mx_get_buffer_text("buffer_message", chat->builder);
    t_groom *room = mx_get_selected_groom(chat->builder, MX_LOCAL_ROOMS);
    t_dtp *dtp = NULL;
    g_strstrip(message_text);
    if (room && !chat->msg_placeholder && strlen(message_text) > 0) {
        mx_trim_message(&message_text);
        dtp = mx_msg_request(message_text, room->id);
        mx_send(chat->out, dtp);
        mx_free_request(&dtp);
        mx_clear_buffer_text("buffer_message", chat->builder);
    }
    g_free(message_text);
    mx_reset_messege_room(NULL, chat->builder);
    (void) btn;
}

void mx_req_edit_message(GtkButton *btn, t_chat *chat) {
    t_gmsg *gmsg = mx_get_selected_gmsg(chat->builder);
    gchar *new_text = mx_get_buffer_text("buffer_message", chat->builder);
    t_dtp *dtp = NULL;
    g_strstrip(new_text);
    if (strcmp(gmsg->msg, new_text) && strlen(new_text) > 0) {
        mx_trim_message(&new_text);
        dtp = mx_edit_msg_request(new_text, gmsg->room_id, gmsg->message_id);
        mx_send(chat->out, dtp);
        mx_free_request(&dtp);
    } else {
        mx_switch_room_header(chat->builder, MX_MSG_CTRL);
    }
    mx_clear_buffer_text("buffer_message", chat->builder);
    mx_hide_msg_editing(NULL, chat->builder);
    g_free(new_text);
    (void) btn;
}

void mx_connect_send_message(t_chat *chat) {
    GObject *btn_send = gtk_builder_get_object(chat->builder, "btn_send_msg");
    GObject *btn_edit = gtk_builder_get_object(chat->builder, "btn_edit_msg_apply");
    GObject *textview = gtk_builder_get_object(chat->builder, "msg_entry");
    g_signal_connect(textview, "key-press-event", G_CALLBACK(mx_send_message_handle_enter), chat);
    g_signal_connect(textview, "key-release-event", G_CALLBACK(mx_send_message_handle_shift), chat);
    g_signal_connect(btn_send, "clicked", G_CALLBACK(mx_req_send_message), chat);
    g_signal_connect(btn_edit, "clicked", G_CALLBACK(mx_req_edit_message), chat);
}

void mx_connect_message_ctrl(t_chat *chat) {
    GObject *btn_delete = gtk_builder_get_object(chat->builder, "btn_delete_msg");
    GObject *btn_edit = gtk_builder_get_object(chat->builder, "btn_edit_msg");
    GObject *btn_unselect = gtk_builder_get_object(chat->builder, "btn_unselect_msg");
    g_signal_connect(btn_delete, "clicked", G_CALLBACK(mx_delete_selected_msgs), chat);
    g_signal_connect(btn_edit, "clicked", G_CALLBACK(mx_show_edit_msg), chat);
    g_signal_connect(btn_unselect, "clicked", G_CALLBACK(mx_unselect_msg), chat->builder);
}

void mx_connect_profile_settings(t_chat *chat) {
    GtkButton *btn = GTK_BUTTON(gtk_builder_get_object(chat->builder,"btn_logout"));
    GtkButton *edit = GTK_BUTTON(gtk_builder_get_object(chat->builder,"btn_edit_profile"));
    GObject *popup = gtk_builder_get_object(chat->builder, "popup_user_info");
    GObject *sett = gtk_builder_get_object(chat->builder, "btn_profile_sett");
    g_signal_connect(btn, "clicked", G_CALLBACK(mx_req_logout), chat);
    g_signal_connect(edit, "clicked", G_CALLBACK(mx_req_edit_desc), chat);
    g_signal_connect(popup, "show", G_CALLBACK(mx_req_get_member_info), chat);
    g_signal_connect(sett, "clicked", G_CALLBACK(mx_set_profile_info), chat);
}

void mx_connect_room_settings(t_chat *chat) {
    GObject *sett = gtk_builder_get_object(chat->builder,"btn_show_room_sett");
    GObject *apply = gtk_builder_get_object(chat->builder,"btn_roomsett_apply");
    GObject *delete = gtk_builder_get_object(chat->builder,"btn_roomsett_delete");
    GObject *clear = gtk_builder_get_object(chat->builder,"btn_clear_history");

    g_signal_connect(sett, "clicked", G_CALLBACK(mx_set_room_sett), chat);
    g_signal_connect(apply, "clicked", G_CALLBACK(mx_req_room_sett), chat);
    g_signal_connect(clear, "clicked", G_CALLBACK(mx_req_room_clear), chat);
    g_signal_connect(delete, "clicked", G_CALLBACK(mx_req_room_del), chat);
}

void mx_connect_search(t_chat *chat) {
    GObject *sentry_rooms = gtk_builder_get_object(chat->builder,"sentry_rooms");
    GObject *sentry_messages = gtk_builder_get_object(chat->builder,"sentry_messages");
    GObject *sentry_members = gtk_builder_get_object(chat->builder,"sentry_members");

    g_signal_connect(GTK_SEARCH_ENTRY(sentry_rooms), "search-changed",G_CALLBACK(mx_start_search_room), chat);
    g_signal_connect(GTK_SEARCH_ENTRY(sentry_messages), "search-changed",G_CALLBACK(mx_start_search_msgs), chat);
    g_signal_connect(GTK_SEARCH_ENTRY(sentry_members), "search-changed",G_CALLBACK(mx_start_search_members), chat);
}

#include "client.h"

static void send_sticker(GtkButton *btn, t_chat *chat) {
    t_groom *groom = mx_get_selected_groom(chat->builder, MX_LOCAL_ROOMS);
    t_dtp *dtp = NULL;
    GtkImage *image = GTK_IMAGE(gtk_button_get_image(btn));
    gchar *sticker_name;

    gtk_image_get_icon_name(image, (const gchar **) &sticker_name, NULL);
    dtp = mx_sticker_request(sticker_name, groom->id);
    mx_send(chat->out, dtp);
    mx_free_request(&dtp);
    mx_reset_messege_room(NULL, chat->builder);
}

void mx_connect_stickers(t_chat *chat) {
    GObject *sticker_1 = gtk_builder_get_object(chat->builder, "sticker_1");
    GObject *sticker_2 = gtk_builder_get_object(chat->builder, "sticker_2");
    GObject *sticker_3 = gtk_builder_get_object(chat->builder, "sticker_3");
    GObject *sticker_4 = gtk_builder_get_object(chat->builder, "sticker_4");
    GObject *sticker_5 = gtk_builder_get_object(chat->builder, "sticker_5");
    GObject *sticker_6 = gtk_builder_get_object(chat->builder, "sticker_6");
    GObject *sticker_7 = gtk_builder_get_object(chat->builder, "sticker_7");
    GObject *sticker_8 = gtk_builder_get_object(chat->builder, "sticker_8");
    GObject *sticker_9 = gtk_builder_get_object(chat->builder, "sticker_9");
    GObject *sticker_10 = gtk_builder_get_object(chat->builder, "sticker_10");
    GObject *sticker_11 = gtk_builder_get_object(chat->builder, "sticker_11");
    GObject *sticker_12 = gtk_builder_get_object(chat->builder, "sticker_12");
    GObject *sticker_13 = gtk_builder_get_object(chat->builder, "sticker_13");
    GObject *sticker_14 = gtk_builder_get_object(chat->builder, "sticker_14");
    GObject *sticker_15 = gtk_builder_get_object(chat->builder, "sticker_15");
    GObject *sticker_16 = gtk_builder_get_object(chat->builder, "sticker_16");
    GObject *sticker_17 = gtk_builder_get_object(chat->builder, "sticker_17");
    GObject *sticker_18 = gtk_builder_get_object(chat->builder, "sticker_18");
    GObject *sticker_19 = gtk_builder_get_object(chat->builder, "sticker_19");
    GObject *sticker_20 = gtk_builder_get_object(chat->builder, "sticker_20");
    GObject *sticker_21 = gtk_builder_get_object(chat->builder, "sticker_21");
    GObject *sticker_22 = gtk_builder_get_object(chat->builder, "sticker_22");
    GObject *sticker_23 = gtk_builder_get_object(chat->builder, "sticker_23");
    GObject *sticker_24 = gtk_builder_get_object(chat->builder, "sticker_24");
    GObject *sticker_25 = gtk_builder_get_object(chat->builder, "sticker_25");
    GObject *sticker_26 = gtk_builder_get_object(chat->builder, "sticker_26");
    GObject *sticker_27 = gtk_builder_get_object(chat->builder, "sticker_27");
    GObject *sticker_28 = gtk_builder_get_object(chat->builder, "sticker_28");
    GObject *sticker_29 = gtk_builder_get_object(chat->builder, "sticker_29");
    GObject *sticker_30 = gtk_builder_get_object(chat->builder, "sticker_30");

    g_signal_connect(sticker_1, "clicked", G_CALLBACK(send_sticker), chat);
    g_signal_connect(sticker_2, "clicked", G_CALLBACK(send_sticker), chat);
    g_signal_connect(sticker_3, "clicked", G_CALLBACK(send_sticker), chat);
    g_signal_connect(sticker_4, "clicked", G_CALLBACK(send_sticker), chat);
    g_signal_connect(sticker_5, "clicked", G_CALLBACK(send_sticker), chat);
    g_signal_connect(sticker_6, "clicked", G_CALLBACK(send_sticker), chat);
    g_signal_connect(sticker_7, "clicked", G_CALLBACK(send_sticker), chat);
    g_signal_connect(sticker_8, "clicked", G_CALLBACK(send_sticker), chat);
    g_signal_connect(sticker_9, "clicked", G_CALLBACK(send_sticker), chat);
    g_signal_connect(sticker_10, "clicked", G_CALLBACK(send_sticker), chat);
    g_signal_connect(sticker_11, "clicked", G_CALLBACK(send_sticker), chat);
    g_signal_connect(sticker_12, "clicked", G_CALLBACK(send_sticker), chat);
    g_signal_connect(sticker_13, "clicked", G_CALLBACK(send_sticker), chat);
    g_signal_connect(sticker_14, "clicked", G_CALLBACK(send_sticker), chat);
    g_signal_connect(sticker_15, "clicked", G_CALLBACK(send_sticker), chat);
    g_signal_connect(sticker_16, "clicked", G_CALLBACK(send_sticker), chat);
    g_signal_connect(sticker_17, "clicked", G_CALLBACK(send_sticker), chat);
    g_signal_connect(sticker_18, "clicked", G_CALLBACK(send_sticker), chat);
    g_signal_connect(sticker_19, "clicked", G_CALLBACK(send_sticker), chat);
    g_signal_connect(sticker_20, "clicked", G_CALLBACK(send_sticker), chat);
    g_signal_connect(sticker_21, "clicked", G_CALLBACK(send_sticker), chat);
    g_signal_connect(sticker_22, "clicked", G_CALLBACK(send_sticker), chat);
    g_signal_connect(sticker_23, "clicked", G_CALLBACK(send_sticker), chat);
    g_signal_connect(sticker_24, "clicked", G_CALLBACK(send_sticker), chat);
    g_signal_connect(sticker_25, "clicked", G_CALLBACK(send_sticker), chat);
    g_signal_connect(sticker_26, "clicked", G_CALLBACK(send_sticker), chat);
    g_signal_connect(sticker_27, "clicked", G_CALLBACK(send_sticker), chat);
    g_signal_connect(sticker_28, "clicked", G_CALLBACK(send_sticker), chat);
    g_signal_connect(sticker_29, "clicked", G_CALLBACK(send_sticker), chat);
    g_signal_connect(sticker_30, "clicked", G_CALLBACK(send_sticker), chat);

}

static void req_join_to_room(GtkButton *btn, t_chat *chat) {
    t_groom *groom = mx_get_selected_groom(chat->builder, MX_GLOBAL_ROOMS);
    t_dtp *dtp = mx_join_room_request(groom->id);
    gtk_list_box_unselect_all(groom->box_rooms);
    mx_stop_search_room(NULL, NULL, chat->builder);
    mx_send(chat->out, dtp);
    mx_free_request(&dtp);
    (void) btn;
}

void mx_connect_join_to_room(t_chat *chat) {
    GObject *btn = gtk_builder_get_object(chat->builder, "btn_join");

    g_signal_connect(GTK_BUTTON(btn), "clicked", G_CALLBACK(req_join_to_room), chat);
}
