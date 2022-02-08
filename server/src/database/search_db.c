#include "server.h"

static char *parse_str_search(char *str) {
    char *tmp = NULL;
    int size = (int)strlen(str);
    int tmp_point = 0;

    for (int i = 0; i < (int)strlen(str); i++) {
        if (str[i] == '%' || str[i] == '_' || str[i] == '\\')
            size++;
    }
    tmp = malloc(size + 1);
    tmp[size] = 0;
    for (int i = 0; i < (int)strlen(str); i++, tmp_point++) {
        if (str[i] == '%' || str[i] == '_' || str[i] == '\\') {
            tmp[tmp_point++] = '\\';
            tmp[tmp_point] = str[i];
        }
        else
            tmp[tmp_point] = str[i];
    }
    return tmp;
}

cJSON *mx_search_user(sqlite3 *db, char *str_search) {
    int rv = SQLITE_OK;
    sqlite3_stmt *stmt;
    char *str_search_join = NULL;
    cJSON *users = cJSON_CreateArray();
    str_search = parse_str_search(str_search);

    str_search_join = g_strjoin("", "%%", str_search, "%%", NULL);
    sqlite3_prepare_v2(db, "select * from users where name like ?1 "
                           "escape '\\'",
                       -1, &stmt, 0),
    sqlite3_bind_text(stmt, 1, str_search_join, -1, SQLITE_STATIC);
    while ((rv = sqlite3_step(stmt)) == SQLITE_ROW)
        cJSON_AddItemToArray(users, mx_get_object_user(stmt));
    mx_error_sqlite(rv, "db search room");
    g_free(str_search_join);
    mx_free((void **)&str_search);
    sqlite3_finalize(stmt);
    return users;
}

cJSON *mx_search_room(sqlite3 *db, char *str_search, int user_id) {
    int rv = SQLITE_OK;
    sqlite3_stmt *stmt;
    char *str_search_join = NULL;
    cJSON *rooms = cJSON_CreateArray();

    str_search = parse_str_search(str_search);
    str_search_join = g_strjoin("", str_search, "%%", NULL);
    rv = sqlite3_prepare_v2(db, "select * from rooms where name like ?1 escape"
                                " '\\' and id not in(select room_id from membe"
                                "rs where user_id = ?2)", -1, &stmt, NULL);
    sqlite3_bind_text(stmt, 1, str_search_join, -1, SQLITE_STATIC);
    sqlite3_bind_int64(stmt, 2, user_id);
    while ((rv = sqlite3_step(stmt)) == SQLITE_ROW)
        cJSON_AddItemToArray(rooms, mx_get_object_room(stmt));
    mx_error_sqlite(rv, "db search room");
    g_free(str_search_join);
    mx_free((void **)&str_search);
    sqlite3_finalize(stmt);
    return rooms;
}

gboolean mx_check_user_by_login(sqlite3 *db, char *login) {
    sqlite3_stmt *stmt;
    int rv = SQLITE_OK;

    rv = sqlite3_prepare_v2(db, "select * from users where login = ?1",
                       -1, &stmt, 0);
    mx_error_sqlite(rv, "mx_check_user_by_login");
    sqlite3_bind_text(stmt, 1, login, -1, SQLITE_STATIC);
    if ((rv = sqlite3_step(stmt)) == SQLITE_ROW) {
        sqlite3_finalize(stmt);
        return TRUE;
    }
    mx_error_sqlite(rv, "mx_check_user_by_login");
    sqlite3_finalize(stmt);
    return FALSE;
}

cJSON *mx_search_message(sqlite3 *db, char *str_search, int room_id) {
    int rv = SQLITE_OK;
    char *str_search_join = NULL;
    sqlite3_stmt *stmt;
    cJSON *messages = cJSON_CreateArray();

    str_search = parse_str_search(str_search);
    str_search_join = g_strjoin("", "%%", str_search, "%%", NULL);
    sqlite3_prepare_v2(db, "select * from messages where room_id = ?2"
                           " and message like ?1 escape '\\' and type = ?3",
                       -1, &stmt, NULL),
    sqlite3_bind_text(stmt, 1, str_search_join, -1, SQLITE_STATIC);
    sqlite3_bind_int64(stmt, 2, room_id);
    sqlite3_bind_int(stmt, 3, DB_TEXT_MSG);
    while ((rv = sqlite3_step(stmt)) == SQLITE_ROW)
        cJSON_AddItemToArray(messages, mx_get_object_message(stmt));
    mx_error_sqlite(rv, "db search message");
    sqlite3_free(str_search_join);
    mx_free((void**)&str_search);
    sqlite3_finalize(stmt);
    return messages;
}
