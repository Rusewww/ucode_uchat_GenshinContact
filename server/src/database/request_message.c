#include "server.h"

static void message_old(sqlite3_str **str) {
    sqlite3_str_appendall(*str, " and date < ?1");
}

static void message_new(sqlite3_str **str) {
    sqlite3_str_appendall(*str, " and date > ?1");
}

char *mx_create_request_message_by_id(sqlite3 *db, int room_id,
                                      int type) {
    sqlite3_str *str = sqlite3_str_new(db);
    char *request = NULL;
    void (*func[])(sqlite3_str **) = {message_new, message_old};

    sqlite3_str_appendf(str, "select * from messages where room_id = %llu",
                        room_id);
    func[type](&str);
    sqlite3_str_appendall(str, " order by date desc");
    request = sqlite3_str_finish(str);
    return request;
}
