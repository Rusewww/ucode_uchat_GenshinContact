#include "server.h"

t_info *mx_init_info(void) {
    t_info *info = (t_info *) mx_malloc(sizeof(t_info));
    info->users = g_hash_table_new(g_str_hash, g_str_equal);
    info->database = mx_open_db(MX_DB);
    mx_init_receiver(info);
    return info;
}

void mx_de_init_client(t_client **client) {
    if (client && *client) {
        if ((*client)->info->users && (*client)->out && (*client)->user && !(*client)->is_file) {
            g_hash_table_remove((*client)->info->users, (*client)->user->login);
        }
        if ((*client)->msg) {
            g_free((*client)->msg);
        }
        if ((*client)->conn) {
            g_io_stream_close(G_IO_STREAM((*client)->conn), NULL, NULL);
        }
        (*client)->msg = NULL;
        g_free(*client);
        *client = NULL;
    }
}
