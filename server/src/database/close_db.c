#include "server.h"

void mx_close_db(sqlite3 *db) {
    int rv = sqlite3_close(db);

    if (rv != SQLITE_OK) {
        mx_logger(MX_LOG_FILE, G_LOG_LEVEL_WARNING, "close database");
    }
}
