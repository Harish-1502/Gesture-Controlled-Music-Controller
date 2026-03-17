#pragma once
#include "gestures.h"

bool ble_media_is_connected();
void ble_media_send(GestureEvent event);

// test helper controls
void ble_media_set_connected(bool connected);
GestureEvent ble_media_get_last_event();
void ble_media_reset();