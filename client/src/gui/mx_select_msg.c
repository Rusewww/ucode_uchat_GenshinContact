#include "../../inc/client.h"

void mx_reset_select_count(t_groom *groom) {
    groom->select_own = 0;
    groom->select_another = 0;
    groom->select_notedit = 0;
    groom->select_all = 0;
}

void mx_select_msg(gpointer *eventbox, gpointer *event, t_signal_data *data) {
    t_groom *groom = mx_get_selected_groom(data->chat->builder, MX_LOCAL_ROOMS);
    t_gmsg *gmsg = (t_gmsg *) g_object_get_data(G_OBJECT(data->row_msg), "gmsg");
    gboolean is_own = !g_strcmp0(data->chat->login, gmsg->login);
    gboolean is_customer = !g_strcmp0(data->chat->login, groom->customer);

    if (!mx_widget_is_visible("box_editing_msg", data->chat->builder)) {
        if (gtk_list_box_row_is_selected(GTK_LIST_BOX_ROW(data->row_msg))) {
            gtk_list_box_unselect_row(GTK_LIST_BOX(groom->box_messages), GTK_LIST_BOX_ROW(data->row_msg));
            if (gmsg->type == DB_STICKER || gmsg->type == DB_FILE_MSG)
                groom->select_notedit--;
            if (is_own)
                groom->select_own--;
            else
                groom->select_another--;
            (void) groom;
        } else {
            gtk_list_box_select_row(GTK_LIST_BOX(groom->box_messages), GTK_LIST_BOX_ROW(data->row_msg));

            if (gmsg->type == DB_STICKER || gmsg->type == DB_FILE_MSG)
                groom->select_notedit++;
            if (is_own)
                groom->select_own++;
            else
                groom->select_another++;
            (void) groom;
        }
        groom->select_all = groom->select_own + groom->select_another;
        if (groom->select_all > 0) {
            mx_switch_room_header(data->chat->builder, MX_MSG_CTRL);
            if (groom->select_all == 1 && groom->select_notedit == 0
                && groom->select_another == 0) {
                mx_widget_set_visibility_by_name(data->chat->builder, "btn_edit_msg", TRUE);
            } else
                mx_widget_set_visibility_by_name(data->chat->builder, "btn_edit_msg", FALSE);
            if (groom->select_all > 0
                && (groom->select_another == 0 || is_customer)) {
                mx_widget_set_visibility_by_name(data->chat->builder, "btn_delete_msg", TRUE);
            } else
                mx_widget_set_visibility_by_name(data->chat->builder, "btn_delete_msg", FALSE);
        } else
            mx_switch_room_header(data->chat->builder, MX_ROOM_CTRL);
    }
    mx_label_set_num("label_msg_count",
                     data->chat->builder, groom->select_all);
    (void) eventbox;
    (void) event;
}
