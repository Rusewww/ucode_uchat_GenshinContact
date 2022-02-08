#include  "server.h"

void mx_insert_contact(sqlite3 *db, int user_id, int contact_id, int type) {
    sqlite3_str *sqlite_str = sqlite3_str_new(db);
    char *request = NULL;

    mx_delete_contact(db, user_id, contact_id);
    sqlite3_str_appendf(sqlite_str, "insert into contacts(user_id, contact_id,"
                                    " type)values(%llu, %llu, %d)", user_id,  contact_id, type);
    request = sqlite3_str_finish(sqlite_str);
    mx_error_sqlite(sqlite3_exec(db, request, 0, 0, 0), "insert_contact");
    sqlite3_free(request);
}

void mx_delete_contact(sqlite3 *db, int user_id, int contact_id) {
    sqlite3_str *sqlite_str = sqlite3_str_new(db);
    char *request = NULL;

    sqlite3_str_appendf(sqlite_str, "delete from contacts where user_id = %llu"
                                    " and contact_id = %llu", user_id, contact_id);
    request = sqlite3_str_finish(sqlite_str);
    mx_error_sqlite(sqlite3_exec(db, request, 0, 0, 0), "delete contact");
    sqlite3_free(request);
}

static cJSON *create_object_contact(sqlite3_stmt *stmt) {
    cJSON *contact = cJSON_CreateObject();

    cJSON_AddNumberToObject(contact, "user_id", sqlite3_column_int64(stmt, 0));
    cJSON_AddNumberToObject(contact, "contact_id", sqlite3_column_int64(stmt, 1));
    cJSON_AddNumberToObject(contact, "type", sqlite3_column_int(stmt, 2));
    return contact;
}

cJSON *mx_get_contacts(sqlite3 *db, int user_id, int type) {
    sqlite3_stmt *stmt;
    int rv = SQLITE_OK;
    cJSON *contacts = cJSON_CreateArray();


    rv = sqlite3_prepare_v2(db, "select * from contacts where user_id = ?1 " "and type = ?2", -1, &stmt, 0);
    mx_error_sqlite(rv, "get_contacts");
    sqlite3_bind_int64(stmt, 1, user_id);
    sqlite3_bind_int(stmt, 2, type);
    while ((rv = sqlite3_step(stmt) == SQLITE_ROW)) {
        cJSON_AddItemToArray(contacts, create_object_contact(stmt));
    }
    mx_error_sqlite(rv, "get_contacts");
    return contacts;
}


