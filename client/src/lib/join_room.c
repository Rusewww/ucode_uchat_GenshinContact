#include "client.h"

t_dtp *mx_join_room_request(guint64 room_id) {
    cJSON *json_result = cJSON_CreateObject();

    if (!cJSON_AddNumberToObject(json_result, "type", RQ_JOIN_ROOM))
        return NULL;
    if (!cJSON_AddNumberToObject(json_result, "room_id", room_id))
        return NULL;
    return mx_get_transport_data(json_result);
}

gboolean mx_join_room_handler(t_dtp *data, t_chat *chat) {
    t_dtp *get_msgs = NULL;
    cJSON *room_id = cJSON_GetObjectItemCaseSensitive(data->json, "id");

    if (!mx_new_room_handler(data, chat))
        return FALSE;
    get_msgs = mx_get_new_msgs_request(0, room_id->valuedouble);
    mx_send(chat->out, get_msgs);
    mx_free_request(&get_msgs);
    return TRUE;
}
