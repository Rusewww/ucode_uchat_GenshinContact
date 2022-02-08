#pragma once

#include <gio/gio.h>
#include "utils.h"
#include "protocol.h"
#include "sqlite3.h"
#include "database.h"

#define MX_DELAY (1000000 / 20)

typedef struct s_client t_client;
typedef struct s_info t_info;
typedef struct s_send_helper t_send_helper;
typedef struct s_file_helper t_file_helper;

struct s_file_helper {
    t_client *client;
    int size;
    int room_id;
    char *name;
};

struct s_info {
    GHashTable *users;
    sqlite3* database;
    gboolean (*request_handler[RQ_COUNT_REQUEST])(t_dtp *dtp, t_client *chat);
};

struct s_client {
    GSocketConnection *conn;
    GDataOutputStream *out;
    GDataInputStream *in;
    GInputStream *in_s;
    char *msg;
    t_db_user *user;
    t_info *info;
    gboolean upload_file;
    gboolean is_file;
};

struct s_send_helper {
    GHashTable *table;
    t_dtp *data;
};

gssize mx_send(GDataOutputStream *out, t_dtp *dtp);
t_info *mx_init_info(void);
void mx_deinit_info(t_info **info);
void mx_deinit_client(t_client **client);
void mx_init_receiver(t_info *chat);

void mx_get_client_info(t_client *client);
void mx_connect_client(t_client *client);
void mx_disconnect_client(t_client *client);
void mx_delete_client(void **client);
void *mx_receiver(void *arg);
void mx_send_to_all(t_dtp *data, t_client *client, guint64 room_id);
void mx_daemon(void);
//Authorization
gboolean mx_valid_authorization_data(t_dtp *data, char **login, char **pass, t_client *client);
void mx_correct_data(t_client *client);
gdouble mx_get_used_power(guint64 chars);
