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
