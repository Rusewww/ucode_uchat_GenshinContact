#include "client.h"

void mx_css_connect(char *theme, t_chat *chat) {
    GError *err = NULL;
    if (chat->css_prov == NULL) {
        chat->css_prov = gtk_css_provider_new();
    } else {
        gtk_style_context_remove_provider_for_screen(gdk_screen_get_default(), GTK_STYLE_PROVIDER(chat->css_prov));
        g_clear_object(&chat->css_prov);
        chat->css_prov = gtk_css_provider_new();
    }
    gtk_css_provider_load_from_path(chat->css_prov, theme, &err);
    if (err) {
        mx_logger(MX_LOG_FILE, G_LOG_LEVEL_ERROR, "No CSS themes");
    }
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(), GTK_STYLE_PROVIDER(chat->css_prov),
                                              GTK_STYLE_PROVIDER_PRIORITY_USER);
}

void change_theme_icon(t_chat *chat, gchar *icon_name) {
    GObject *img = gtk_builder_get_object(chat->builder, "image19");
    gtk_image_set_from_icon_name(GTK_IMAGE(img), icon_name, GTK_ICON_SIZE_DND);
    gtk_image_set_pixel_size(GTK_IMAGE(img), 15);
}

void mx_css_connect_from_file(t_chat *chat) {
    gchar *current_theme = NULL;
    if (g_file_get_contents(MX_THEME_FILE, &current_theme, NULL, NULL)
        && strcmp("anemo", current_theme) == 0) {
        g_file_set_contents(MX_THEME_FILE, "anemo", -1, NULL);
        mx_css_connect(MX_ANEMO_THEME, chat);
        change_theme_icon(chat, "electro-theme");
    } else {
        g_file_set_contents(MX_THEME_FILE, "dark", -1, NULL);
        mx_css_connect(MX_ELECTRO_THEME, chat);
        change_theme_icon(chat, "anemo-theme");
    }
    if (current_theme) {
        g_free(current_theme);
    }
}
