#include "server.h"

char *mx_get_text_message_by_id(sqlite3 *db, int message_id) {
    sqlite3_stmt *stmt;
    char *result = NULL;
    int rv = SQLITE_OK;

    rv = sqlite3_prepare_v2(db, "select message from messages where message_"
                                "id = ?1", -1, &stmt, 0);
    mx_error_sqlite(rv, "get test message by id");
    sqlite3_bind_int64(stmt, 1, message_id);
    mx_error_sqlite(sqlite3_step(stmt), "get test message by id");
    if (sqlite3_column_text(stmt, 0) != NULL)
        result = strdup((const char*)sqlite3_column_text(stmt, 0));
    return result;
}

gboolean mx_is_owner_msg(sqlite3 *db, int user_id, int msg_id) {
    sqlite3_stmt *stmt;
    int rv = 0;
    gboolean is_owner = FALSE;

    rv = sqlite3_prepare_v2(db, "select * from messages where message_id = ?1 "
                                "and user_id = ?2 ", -1, &stmt, NULL);
    mx_error_sqlite(rv, "is owner message");
    sqlite3_bind_int64(stmt, 1, msg_id);
    sqlite3_bind_int64(stmt, 2, user_id);
    if ((rv = sqlite3_step(stmt)) == SQLITE_ROW)
        is_owner = TRUE;
    mx_error_sqlite(rv, "is owner message");
    sqlite3_finalize(stmt);
    return is_owner;
}

void mx_delete_all_messages(sqlite3 *db, int room_id) {
    sqlite3_str *sqlite_str = sqlite3_str_new(db);
    char *request = NULL;

    sqlite3_str_appendf(sqlite_str, "delete from messages where "
                                    "room_id = %llu", room_id);
    request = sqlite3_str_finish(sqlite_str);
    mx_error_sqlite(sqlite3_exec(db, request, 0, 0, 0), "delete all message");
    sqlite3_free(request);
}

