#include "server.h"

void mx_edit_room_name_by_id(sqlite3 *db, int id, char *new_name) {
    sqlite3_stmt *stmt;
    int rv = SQLITE_OK;

    rv = sqlite3_prepare_v2(db, "update rooms set name = ?1 "
                                "where id = ?2", -1,
                            &stmt, NULL);
    mx_error_sqlite(rv, "edit room name by id");
    sqlite3_bind_text(stmt, 1, new_name, -1, SQLITE_STATIC);
    sqlite3_bind_int64(stmt, 2, id);
    mx_error_sqlite(sqlite3_step(stmt), "edit room name by id");
    sqlite3_finalize(stmt);
}

void mx_edit_user_name_by_id(sqlite3 *db, int id, char *new_name) {
    sqlite3_stmt *stmt;
    int rv = SQLITE_OK;

    rv = sqlite3_prepare_v2(db, "update users set login = ?1 "
                                "where id = ?2", -1,
                            &stmt, NULL);
    mx_error_sqlite(rv, "edit_user_name_by_id");
    sqlite3_bind_text(stmt, 1, new_name, -1, SQLITE_STATIC);
    sqlite3_bind_int64(stmt, 2, id);
    mx_error_sqlite(sqlite3_step(stmt), "edit_user_name_by_id");
    sqlite3_finalize(stmt);
}

void mx_edit_message_by_id(sqlite3 *db, int id, char *new) {
    sqlite3_str *str = sqlite3_str_new(db);
    sqlite3_stmt *stmt;
    char *request = NULL;

    sqlite3_str_appendf(str, "update messages set message = ?1 where "
                             "message_id = %lu", id);
    request = sqlite3_str_finish(str);
    mx_error_sqlite(sqlite3_prepare(db, request, -1, &stmt, NULL), 
                    "edit message by id");
    sqlite3_bind_text(stmt, 1, new, -1, SQLITE_STATIC);
    mx_error_sqlite(sqlite3_step(stmt), "edit message by id");
    sqlite3_free(request);
    sqlite3_finalize(stmt);

    int power = strlen(new);
    int room_id = 0;

    sqlite3_prepare_v2(db, "select room_id, power from messages "
                                "where message_id = ?1", -1, &stmt, NULL);
    sqlite3_bind_int64(stmt, 1, id);
    mx_error_sqlite(sqlite3_step(stmt), "update power message");
    if (sqlite3_column_int64(stmt, 0))
        room_id = sqlite3_column_int64(stmt, 0);
    if (sqlite3_column_int64(stmt, 1))
        power += sqlite3_column_int64(stmt, 1);
    sqlite3_finalize(stmt);
    sqlite3_prepare_v2(db, "update messages set power = ?1 where "
                                "message_id = ?2", -1, &stmt, NULL);
    sqlite3_bind_int64(stmt, 1, power);
    sqlite3_bind_int64(stmt, 2, id);
    mx_error_sqlite(sqlite3_step(stmt), "update power message");
    sqlite3_finalize(stmt); 
    mx_db_update_room_power(db, power, room_id);
}

void mx_edit_type_member(sqlite3 *db, int room_id, int user_id, int new_type) {
    sqlite3_str *sqlite_str = sqlite3_str_new(db);
    char *request = NULL;
    int rv = SQLITE_OK;

    sqlite3_str_appendf(sqlite_str, "update members set permission = %d where "
                                    "room_id = %llu and user_id = %llu",
                        new_type, room_id, user_id);
    request = sqlite3_str_finish(sqlite_str);
    rv = sqlite3_exec(db, request, 0, 0, 0);
    mx_error_sqlite(rv, "edit type member");
    sqlite3_free(request);
}
