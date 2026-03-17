#include "ble_media_mock.h"

static bool g_connected = false;
static GestureEvent g_lastEvent = GestureEvent::None;

bool ble_media_is_connected() {
    return g_connected;
}

void ble_media_send(GestureEvent event) {
    g_lastEvent = event;
}

void ble_media_set_connected(bool connected) {
    g_connected = connected;
}

GestureEvent ble_media_get_last_event() {
    return g_lastEvent;
}

void ble_media_reset() {
    g_connected = false;
    g_lastEvent = GestureEvent::None;
}