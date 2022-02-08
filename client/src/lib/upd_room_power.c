#include "client.h"
gboolean mx_upd_handler(t_dtp *data, t_chat *chat) {
    cJSON *room_id = cJSON_GetObjectItemCaseSensitive(data->json, "room_id");
    cJSON *power = cJSON_GetObjectItemCaseSensitive(data->json, "power");
    gdouble power_d = 0;
    if (chat){
    }
    if (!cJSON_IsNumber(room_id) || !cJSON_IsNumber(power))
        return FALSE;
    power_d = mx_get_used_power(power->valuedouble);
    return TRUE;
}
