#include "../../inc/client.h"

void mx_switch_theme(GtkWidget *widget, t_chat *chat) {
    gchar *current_theme = NULL;

    if (g_file_get_contents(MX_THEME_FILE, &current_theme, NULL, NULL)) {
        if (!g_strcmp0("electro", current_theme)) {
            g_file_set_contents(MX_THEME_FILE, "anemo", -1, NULL);
            mx_css_connect(MX_ANEMO_THEME, chat);
            change_theme_icon(chat, "electro-theme");
        } else {
            g_file_set_contents(MX_THEME_FILE, "electro", -1, NULL);
            mx_css_connect(MX_ELECTRO_THEME, chat);
            change_theme_icon(chat, "anemo-theme");
        }
    } else {
        g_file_set_contents(MX_THEME_FILE, "anemo", -1, NULL);
        mx_css_connect(MX_ANEMO_THEME, chat);
        change_theme_icon(chat, "electro-theme");
    }
    if (current_theme)
        g_free(current_theme);
    (void) widget;
}

void mx_connect_theme_switcher(t_chat *chat) {
    GObject *btn = gtk_builder_get_object(chat->builder, "btn_change_theme");
    g_signal_connect(btn, "clicked", G_CALLBACK(mx_switch_theme), chat);
}
