#include "client.h"

t_dtp *mx_upload_file_request(const char *name, goffset size,
                              char *token, guint64 room_id) {
    cJSON *json_result = cJSON_CreateObject();

    if (!cJSON_AddNumberToObject(json_result, "type", RQ_UPLOAD_FILE))
        return NULL;
    if (!cJSON_AddStringToObject(json_result, "name", MX_J_STR(name)))
        return NULL;
    if (!cJSON_AddNumberToObject(json_result, "size", size))
        return NULL;
    if (!cJSON_AddNumberToObject(json_result, "room_id", room_id))
        return NULL;
    if (!cJSON_AddStringToObject(json_result, "token", MX_J_STR(token)))
        return NULL;
    return mx_get_transport_data(json_result);
}
