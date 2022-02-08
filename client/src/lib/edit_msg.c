#include "client.h"

t_dtp *mx_edit_msg_request(char *msg, guint64 room_id, guint64 msg_id) {
    cJSON *json_result = cJSON_CreateObject();

    if (!cJSON_AddNumberToObject(json_result, "type", RQ_EDIT_MSG))
        return NULL;
    if (!cJSON_AddNumberToObject(json_result, "room_id", room_id))
        return NULL;
    if (!cJSON_AddNumberToObject(json_result, "msg_id", msg_id))
        return NULL;
    if (!cJSON_AddStringToObject(json_result, "msg", MX_J_STR(msg)))
        return NULL;
    return mx_get_transport_data(json_result);
}

gboolean mx_edit_msg_handler(t_dtp *data, t_chat *chat) {
    cJSON *msg = cJSON_GetObjectItemCaseSensitive(data->json, "msg");
    cJSON *room_id = cJSON_GetObjectItemCaseSensitive(data->json, "room_id");
    cJSON *msg_id = cJSON_GetObjectItemCaseSensitive(data->json, "msg_id");
    cJSON *power = cJSON_GetObjectItemCaseSensitive(data->json, "power");
    gdouble power_d = 0;
    if(power_d){
    }
    if (!cJSON_IsString(msg) || !cJSON_IsNumber(room_id) || !cJSON_IsNumber(msg_id) || !cJSON_IsNumber(power)) {
        return FALSE;
    }
    power_d = mx_get_u(power->valuedouble);
    mx_gupd_msg_text(msg_id->valuedouble, room_id->valuedouble, msg->valuestring, chat->builder);
    return TRUE;
}
