#pragma once

#include "utils.h"
#include "sqlite3.h"
#include "protocol.h"

#define MX_DB "database.db"

typedef struct s_member t_member;
typedef struct s_db_user t_db_user;
typedef struct s_db_message t_db_message;
typedef struct s_db_room t_db_room;

typedef enum e_type_db {
    DB_NEW_MESSAGE = 0,
    DB_OLD_MESSAGE,
    DB_SECOND,
    DB_MILISECOND,
    DB_MICROSECOND
}            t_type_db;

struct s_member{
    int user_id;
    int room_id;
    int date;
    int prm;
};

struct s_db_user {
    int user_id;
    int date;
    char *pass;
    char *token;
    char *name;
    char *login;
    char *desc;
};

struct s_db_message {
    int user_id;
    int room_id;
    int message_id;
    int date;
    int status;
    int type;
    int file_size;
    int power;
    char *message;
    char *file_name;
};

struct s_db_room {
    int date;
    int room_id;
    int customer_id;
    int type;
    int power;
    char *desc;
    char *room_name;
};

cJSON *mx_json_is_valid(cJSON *data, char *str, cJSON_bool (*check_valid)(const cJSON *const));
t_db_user *mx_parse_json_user(cJSON *user_j);
t_db_room *mx_parse_json_room(cJSON *room_j);
t_db_message *mx_parse_message(cJSON *message_j);

//create table
void mx_create_table(sqlite3 *db);

// function
guint64 mx_get_time(gint8 type);
sqlite3 *mx_open_db(char *name_db);
void mx_close_db(sqlite3 *db);
char *mx_create_request_message_by_id(sqlite3 *db, int room_id, int type);

//count
int mx_get_count_users(sqlite3 *db);
int mx_get_count_rooms(sqlite3 *db);
int mx_get_count_messages(sqlite3 *db, int id);

// error
gboolean mx_error_sqlite(int rv, char *error);

// queue
void mx_db_push_queue_by_id(sqlite3 *db, int user_id, char *request);
void mx_db_pop_queue_by_id(sqlite3 *db, int user_id);
char *mx_get_queue(sqlite3 *db, int user_id);
void mx_clean_queue(sqlite3 *db, int user_id);

// get
cJSON *mx_get_object_room(sqlite3_stmt *stmt);
cJSON *mx_get_object_user(sqlite3_stmt *stmt);
t_db_user *mx_get_user_by_login(sqlite3 *db, char *login);
t_db_user *mx_get_user_by_token(sqlite3 *db, char *token);
t_db_user *mx_get_user_by_id(sqlite3 *db, int user_id);
t_db_room *mx_get_room_by_id(sqlite3 *db, int id);
cJSON *mx_get_users(sqlite3 *db, int date);
cJSON *mx_get_rooms(sqlite3 * db, int date, int user_id);
cJSON *mx_get_new_messages_by_id(sqlite3 *db, int room_id, int date, int count);
cJSON *mx_get_old_messages_by_id(sqlite3 *db, int room_id, int date, int count);

// edit
void mx_edit_user_name_by_id(sqlite3 *db, int id, char *new_name);
void mx_edit_room_name_by_id(sqlite3 *db, int id, char *new_name);
void mx_edit_message_by_id(sqlite3 *db, int id, char *new);
void mx_edit_token(sqlite3 *db, int user_id, char *new);
void mx_edit_desc_user(sqlite3 *db, int user_id, char *new);
void mx_edit_desc_room_by_id(sqlite3 *db, int room_id, char *new);
void mx_edit_permission_of_user(sqlite3 *db, int user_id, int room_id, int new);

// delete
void mx_delete_message_by_id(sqlite3 *db, int id);
void mx_delete_user_by_id(sqlite3 *db, int id);
void mx_delete_room_by_id(sqlite3 *db, int room_id);
void mx_delete_all_messages(sqlite3 *db, int room_id);

// insert yes
void mx_insert_room_into_db(sqlite3 *db, t_db_room *room);
void mx_insert_user_into_db(sqlite3 *db, t_db_user *user);
void mx_insert_message(sqlite3 *db, t_db_message *message);

// free struct
void mx_free_room(t_db_room **room);
void mx_free_message(t_db_message **message);
void mx_free_user(t_db_user **user);


//contact
void mx_insert_contact(sqlite3 *db, int user_id, int contact_id, int type);
void mx_delete_contact(sqlite3 *db, int user_id, int contact_id);
cJSON *mx_get_contacts(sqlite3 *db, int user_id, int type);

// user
gboolean mx_is_member(sqlite3 *db, int user_id, int room_id);

// search
gboolean mx_check_user_by_login(sqlite3 *db, char *login);
cJSON *mx_search_room(sqlite3 *db, char *str_search, int user_id);
cJSON *mx_search_user(sqlite3 *db, char *str_search);
cJSON *mx_search_message(sqlite3 *db, char *str_search, int room_id);

//members
cJSON *mx_get_json_members(sqlite3 *db, int room_id);
GList *mx_get_login_members(sqlite3 *db, int room_id);
void mx_insert_member_into_db(sqlite3 *db, int room_id, int user_id, int permission);
void mx_edit_type_member(sqlite3 *db, int room_id, int user_id, int new_type);
int mx_get_type_member(sqlite3 *db, int user_id, int room_id);

// message
gboolean mx_is_owner_msg(sqlite3 *db, int user_id, int msg_id);
cJSON *mx_get_object_message(sqlite3_stmt *stmt);
char *mx_get_text_message_by_id(sqlite3 *db, int message_id);

// power
int mx_get_power_of_message(sqlite3 *db, int message_id);
int mx_get_power_of_room(sqlite3 *db, int room_id);
void mx_db_update_room_power(sqlite3 *db, int power, int room_id);
