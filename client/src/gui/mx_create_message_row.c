#include "../../inc/client.h"

GtkWidget *mx_create_message_row(t_chat *chat, t_gmsg *gmsg) {
    gboolean is_own = !g_strcmp0(chat->login, gmsg->login);
    GtkWidget *event_box = NULL;
    event_box = mx_create_reg_message_row(gmsg, is_own, chat);
    return event_box;
}
