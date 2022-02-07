#include "../../inc/client.h"

void mx_msg_create_own_content(GtkWidget *info_box, t_gmsg *gmsg, t_chat *chat) {
    if (gmsg->type == DB_TEXT_MSG) {
        mx_widget_set_class(info_box, "box_own_msg_info");
        mx_msg_create_l_text(info_box, gmsg, TRUE);
    } else if (gmsg->type == DB_STICKER) {
        mx_widget_set_class(info_box, "box_own_sticker_info");
        mx_msg_create_img_sticker(info_box, gmsg, TRUE);
    } else {
        mx_widget_set_class(info_box, "box_own_file_info");
        mx_msg_create_file(info_box, gmsg, TRUE, chat);
    }
}

void mx_msg_create_content(GtkWidget *main_box, GtkWidget *box_info, t_gmsg *gmsg, t_chat *chat) {
    if (gmsg->type == DB_TEXT_MSG) {
        mx_widget_set_class(box_info, "box_msg_info");
        mx_msg_create_l_text(box_info, gmsg, FALSE);
    } else if (gmsg->type == DB_STICKER) {
        mx_widget_set_class(box_info, "box_sticker_info");
        mx_msg_create_img_sticker(box_info, gmsg, FALSE);
    } else {
        mx_widget_set_class(box_info, "box_file_info");
        mx_msg_create_file(box_info, gmsg, FALSE, chat);
    }
    mx_msg_create_l_login(main_box, gmsg);
}
