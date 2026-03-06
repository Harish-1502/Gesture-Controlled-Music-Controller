#pragma once

#include "gestures.h"

void ble_media_init();
bool ble_media_is_connected();
void ble_media_send(GestureEvent event);