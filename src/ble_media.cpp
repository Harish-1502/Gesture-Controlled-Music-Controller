#include "ble_media.h"

#include <BleKeyboard.h>

static BleKeyboard bleKeyboard("ESP32 Keyboard");
static bool bleStarted = false;

void ble_media_init() {
    if (bleStarted) {
        return;
    }

    bleKeyboard.begin();
    bleStarted = true;
}

ErrorCode ble_media_send_checked(GestureEvent event) {
    if (event == GestureEvent::None) {
        return ErrorCode::BleCommandFailed;
    }

    if (!ble_media_is_connected()) {
        return ErrorCode::BleDisconnected;
    }

    ble_media_send(event);
    return ErrorCode::None;
}

bool ble_media_is_connected() {
    return bleKeyboard.isConnected();
}

void ble_media_send(GestureEvent event) {
    switch (event) {
        case GestureEvent::PlayPause:
            bleKeyboard.write(KEY_MEDIA_PLAY_PAUSE);
            break;

        case GestureEvent::VolumeUp:
            bleKeyboard.write(KEY_MEDIA_VOLUME_UP);
            break;

        case GestureEvent::VolumeDown:
            bleKeyboard.write(KEY_MEDIA_VOLUME_DOWN);
            break;

        case GestureEvent::NextTrack:
            bleKeyboard.write(KEY_MEDIA_NEXT_TRACK);
            break;

        case GestureEvent::PreviousTrack:
            bleKeyboard.write(KEY_MEDIA_PREVIOUS_TRACK);
            break;

        case GestureEvent::None:
        default:
            break;
    }
}
