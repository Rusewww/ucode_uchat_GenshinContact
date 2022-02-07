#include "client.h"

void mx_err_auth_data_handler(GtkBuilder *builder) {
    GObject *label = gtk_builder_get_object(builder, "label_auth_error_login");
    gtk_label_set_text(GTK_LABEL(label), "The login or password inccorect");
}

void mx_err_user_exist_handler(GtkBuilder *builder) {
    GObject *label = gtk_builder_get_object(builder, "label_auth_error_signup");
    gtk_label_set_text(GTK_LABEL(label), "User already exist");
}

void mx_err_cli_exist_handler(GtkBuilder *builder) {
    GObject *label = gtk_builder_get_object(builder, "label_auth_error_login");
    gtk_label_set_text(GTK_LABEL(label), "User already authorized");
}
