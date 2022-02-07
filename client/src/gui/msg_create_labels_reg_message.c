#include "../../inc/client.h"

void mx_msg_create_l_login(GtkWidget *main_box, t_gmsg *gmsg) {
    GtkWidget *login = gtk_label_new(gmsg->login);
    mx_widget_set_class(login, "sender_login");
    gtk_box_pack_start(GTK_BOX(main_box), login, FALSE, FALSE, 0);
    gtk_widget_set_halign(login, GTK_ALIGN_START);
}

void mx_msg_create_img_sticker(GtkWidget *info_box, t_gmsg *gmsg, gboolean is_own) {
    GtkWidget *sticker = gtk_image_new_from_icon_name(gmsg->msg, GTK_ICON_SIZE_DIALOG);
    gtk_image_set_pixel_size(GTK_IMAGE(sticker), 128);
    if (is_own) {
        gtk_box_pack_end(GTK_BOX(info_box), sticker, FALSE, FALSE, 0);
    } else {
        gtk_box_pack_start(GTK_BOX(info_box), sticker, FALSE, FALSE, 0);
    }
}

void mx_msg_create_l_text(GtkWidget *box_info, t_gmsg *gmsg, gboolean is_own) {
    GtkWidget *text = gtk_label_new(NULL);
    if (is_own) {
        gtk_box_pack_end(GTK_BOX(box_info), text, FALSE, FALSE, 0);
    } else {
        gtk_box_pack_start(GTK_BOX(box_info), text, FALSE, FALSE, 0);
    }
    gtk_label_set_xalign(GTK_LABEL(text), 0.00);
    gtk_widget_set_halign(text, GTK_ALIGN_START);
    gtk_label_set_line_wrap(GTK_LABEL(text), TRUE);
    gtk_label_set_line_wrap_mode(GTK_LABEL(text), PANGO_WRAP_WORD_CHAR);
    gtk_label_set_markup(GTK_LABEL(text), mx_format_text(gmsg->msg));
    gmsg->label_text = GTK_LABEL(text);
    g_object_ref(text);
}

void mx_msg_create_l_time(GtkWidget *info, t_gmsg *gmsg, gboolean is_own) {
    GtkWidget *time = gtk_label_new(NULL);
    char *small_time = mx_get_string_time(gmsg->date, 0);
    char *big_time = mx_get_string_time(gmsg->date, 1);
    if (is_own) {
        gtk_widget_set_halign(time, GTK_ALIGN_END);
    } else {
        gtk_widget_set_halign(time, GTK_ALIGN_START);
    }
    gtk_widget_set_valign(time, GTK_ALIGN_END);
    gtk_box_pack_start(GTK_BOX(info), time, FALSE, TRUE, 0);
    gtk_label_set_text(GTK_LABEL(time), small_time);
    gtk_widget_set_tooltip_text(time, big_time);
    g_free(small_time);
    g_free(big_time);
}
