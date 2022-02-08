#include "server.h"

gboolean mx_error_sqlite(int rv, char *error) {
    if (rv != SQLITE_OK && rv != SQLITE_DONE && rv != SQLITE_ROW) {
        mx_logger(MX_LOG_FILE, G_LOG_LEVEL_WARNING, error);
        return TRUE;
    }
    return FALSE;
}
