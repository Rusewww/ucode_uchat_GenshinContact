#include "../../inc/client.h"

GtkWidget *mx_msgcreate_eventbox() {
    GtkWidget *new_box = gtk_event_box_new();
    gtk_widget_set_can_focus(new_box, FALSE);
    return new_box;
}

GtkWidget *mx_msgcreate_box_main(GtkWidget *eventbox, gboolean is_own) {
    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    if (is_own) {
        mx_widget_set_class(main_box, "main_own_msg_box");
    } else {
        mx_widget_set_class(main_box, "main_msg_box");
    }
    gtk_container_add(GTK_CONTAINER(eventbox), GTK_WIDGET(main_box));
    return main_box;
}

void mx_msgcreate_box_info(GtkWidget *main_box, t_gmsg *gmsg, gboolean is_own, t_chat *chat) {
    GtkWidget *box_info = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    if (is_own) {
        mx_msg_create_own_content(box_info, gmsg, chat);
    } else {
        mx_msg_create_content(main_box, box_info, gmsg, chat);
    }
    gtk_box_pack_end(GTK_BOX(main_box), box_info, FALSE, TRUE, 0);
    GtkWidget *box_status = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    if (is_own) {
        gtk_box_pack_end(GTK_BOX(box_info), box_status, FALSE, TRUE, 0);
    } else {
        gtk_box_pack_start(GTK_BOX(box_info), box_status, FALSE, TRUE, 0);
    }
    gtk_widget_set_valign(box_status, GTK_ALIGN_END);
    mx_msg_create_l_time(box_status, gmsg, is_own);
}
