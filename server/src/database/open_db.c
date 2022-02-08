#include "server.h"

sqlite3 *mx_open_db(char *name_db) {
    sqlite3 *db;

    if (sqlite3_open(name_db, &db) != SQLITE_OK)
        mx_logger(MX_LOG_FILE, G_LOG_LEVEL_WARNING, "error open database");
    mx_create_table(db);
    return db;
}

