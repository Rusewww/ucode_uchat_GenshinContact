#include "server.h"

cJSON *mx_get_object_user(sqlite3_stmt *stmt) {
    cJSON *user = cJSON_CreateObject();

    cJSON_AddNumberToObject(user, "id", sqlite3_column_int64(stmt, 0));
    cJSON_AddStringToObject(user, "name",  (char*)sqlite3_column_text(stmt, 1));
    cJSON_AddStringToObject(user, "login",  (char*)sqlite3_column_text(stmt, 2));
    cJSON_AddStringToObject(user, "pass",  (char*)sqlite3_column_text(stmt, 3));
    cJSON_AddStringToObject(user, "token",  (char*)sqlite3_column_text(stmt, 4));
    cJSON_AddNumberToObject(user, "date", sqlite3_column_int64(stmt, 5));
    cJSON_AddStringToObject(user, "desc", MX_J_STR((char*)sqlite3_column_text(stmt, 6)));
    return user;
}

cJSON *mx_get_users(sqlite3 *db, int date) {
    cJSON *users = cJSON_CreateArray();
    sqlite3_stmt *stmt;
    int rv = 0;

    rv = sqlite3_prepare_v2(db, "select * from users where date > ?1 order by " "date desc", -1, &stmt, NULL);
    mx_error_sqlite(rv, "get users");
    sqlite3_bind_int(stmt, 1, date);
    while ((rv = sqlite3_step(stmt)) == SQLITE_ROW)
        cJSON_AddItemToArray(users, mx_get_object_user(stmt));
    mx_error_sqlite(rv, "get users");
    sqlite3_finalize(stmt);
    return users;
}

