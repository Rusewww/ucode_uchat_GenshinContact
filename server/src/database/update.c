#include "server.h"

void mx_edit_permission_of_user(sqlite3 *db, int user_id, int room_id, int new) {
    sqlite3_str *sqlite_str = sqlite3_str_new(db);
    char *request = NULL;

    sqlite3_str_appendf(sqlite_str, "update members set permission = %d where"
                                    " id = %llu and where room_id = %llu",
                        new, user_id, room_id);
    request = sqlite3_str_finish(sqlite_str);
    mx_error_sqlite(sqlite3_exec(db, request, 0, 0, 0),
                                 "mx_edit_permission_of_user");
}

void mx_edit_token(sqlite3 *db, int user_id, char *new) {
    sqlite3_stmt *stmt;
    int rv = SQLITE_OK;

    rv = sqlite3_prepare_v2(db, "update users set token = ?1 where "
                                "id = ?2", -1, &stmt, NULL);
    mx_error_sqlite(rv, "edit token");
    sqlite3_bind_text(stmt, 1, new, -1, SQLITE_STATIC);
    sqlite3_bind_int64(stmt, 2, user_id);
    mx_error_sqlite(sqlite3_step(stmt), "edit token");
    sqlite3_finalize(stmt);
}

void mx_edit_desc_room_by_id(sqlite3 *db, int room_id, char *new) {
    sqlite3_stmt *stmt;
    int rv = SQLITE_OK;

    rv = sqlite3_prepare_v2(db, "update rooms set desc = ?1 "
                                "where id = ?2", -1, &stmt, NULL);
    mx_error_sqlite(rv, "mx_edit_desc_room_by_id");
    sqlite3_bind_text(stmt, 1, new, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, room_id);
    mx_error_sqlite(sqlite3_step(stmt), "mx_edit_desc_room_by_id");
    sqlite3_finalize(stmt);
}

void mx_edit_desc_user(sqlite3 *db, int user_id, char *new) {
    sqlite3_stmt *stmt;
    int rv = SQLITE_OK;
    
    rv = sqlite3_prepare_v2(db, "update users set desc = ?1" 
                                " where id = ?2", -1, &stmt, NULL);
    mx_error_sqlite(rv, "mx_edit_desc_user");
    sqlite3_bind_text(stmt, 1, new, -1, SQLITE_STATIC);
    sqlite3_bind_int64(stmt, 2, user_id);
    mx_error_sqlite(sqlite3_step(stmt), "mx_edit_desc_user");
    sqlite3_finalize(stmt);
}

