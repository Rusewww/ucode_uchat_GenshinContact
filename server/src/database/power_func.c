#include "server.h"


void mx_db_update_room_power(sqlite3 *db, int power, int room_id) {
    int new_power = power + mx_get_power_of_room(db, room_id);
    sqlite3_str *sqlite_str = sqlite3_str_new(db);
    char *request = NULL;
    int rv = SQLITE_OK;

    sqlite3_str_appendf(sqlite_str, "update rooms set power = %llu"
                                    " where id = %llu", new_power, room_id);
    request = sqlite3_str_finish(sqlite_str);
    rv = sqlite3_exec(db, request, 0, 0, 0);
    mx_error_sqlite(rv, "db upadte room power");
    sqlite3_free(request);
}

int mx_get_power_of_room(sqlite3 *db, int room_id) {
    int rv = SQLITE_OK;
    sqlite3_stmt *stmt;
    int power_of_room = 0;

    sqlite3_prepare_v2(db, "select power from rooms where id = ?1", -1, &stmt, NULL);
    mx_error_sqlite(rv, "get power of room");
    sqlite3_bind_int64(stmt, 1, room_id);
    mx_error_sqlite(sqlite3_step(stmt), "get power of room ");
    if (sqlite3_column_int64(stmt, 0))
        power_of_room = sqlite3_column_int64(stmt, 0);
    sqlite3_finalize(stmt);
    return power_of_room;
}

int mx_get_power_of_message(sqlite3 *db, int message_id) {
    int rv = SQLITE_OK;
    sqlite3_stmt *stmt;
    int power_of_message = 0;

    sqlite3_prepare_v2(db, "select power from messages where message_id = ?1", -1, &stmt, NULL);
    mx_error_sqlite(rv, "get power of message");
    sqlite3_bind_int64(stmt, 1, message_id);
    mx_error_sqlite(sqlite3_step(stmt), "get power of message");
    if (sqlite3_column_int64(stmt, 0))
        power_of_message = sqlite3_column_int64(stmt, 0);
    sqlite3_finalize(stmt);
    return power_of_message;
}


