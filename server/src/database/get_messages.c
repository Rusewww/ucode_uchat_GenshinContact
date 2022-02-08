#include "server.h"

cJSON *mx_get_object_message(sqlite3_stmt *stmt) {
    cJSON *o_m = cJSON_CreateObject();

    cJSON_AddNumberToObject(o_m, "user_id", sqlite3_column_int64(stmt, 0));
    cJSON_AddNumberToObject(o_m, "room_id", sqlite3_column_int64(stmt, 1));
    cJSON_AddNumberToObject(o_m, "message_id", sqlite3_column_int64(stmt, 2));
    cJSON_AddNumberToObject(o_m, "date", sqlite3_column_int64(stmt, 3));
    cJSON_AddStringToObject(o_m, "message",
                            MX_J_STR((char*)sqlite3_column_text(stmt, 4)));
    cJSON_AddNumberToObject(o_m, "msg_type", sqlite3_column_int(stmt, 5));
    cJSON_AddNumberToObject(o_m, "file_size", sqlite3_column_int(stmt, 6));
    cJSON_AddStringToObject(o_m, "file_name",
                            MX_J_STR((char*)sqlite3_column_text(stmt, 7)));
    cJSON_AddNumberToObject(o_m, "status", sqlite3_column_int(stmt, 8));
    cJSON_AddNumberToObject(o_m, "power", sqlite3_column_int64(stmt, 9));
    return o_m;
}

static cJSON *get_messages_by_id(sqlite3_stmt *stmt, int room_id,  int count, int date) {
    cJSON *room = cJSON_CreateObject();
    cJSON *message = cJSON_CreateArray();

    sqlite3_bind_int64(stmt, 1, date);
    for (gint64 i = 0; i < count 
                       && sqlite3_step(stmt) == SQLITE_ROW; i++) {
        cJSON_AddItemToArray(message, mx_get_object_message(stmt));
    }
    cJSON_AddItemToObject(room, "room_id", cJSON_CreateNumber(room_id));
    cJSON_AddItemToObject(room, "messages", message);
    sqlite3_finalize(stmt);
    return room;
}

cJSON *mx_get_new_messages_by_id(sqlite3 *db, int room_id, int date, int count) {
    sqlite3_stmt *stmt;
    int rv = SQLITE_OK;
    char *request = mx_create_request_message_by_id(db, room_id, DB_NEW_MESSAGE);

    rv = sqlite3_prepare_v2(db, request, -1, &stmt, NULL);
    mx_error_sqlite(rv, "get_new_message by id");
    sqlite3_free(request);
    return get_messages_by_id(stmt, room_id, count, date);
}

cJSON *mx_get_old_messages_by_id(sqlite3 *db, int room_id, int date, int count) {
    sqlite3_stmt *stmt;
    char *request = mx_create_request_message_by_id(db, room_id,  DB_OLD_MESSAGE);

    mx_error_sqlite(sqlite3_prepare_v2(db, request, -1, &stmt, NULL), "get old message by id");
    sqlite3_free(request);
    return get_messages_by_id(stmt, room_id, count, date);
}

