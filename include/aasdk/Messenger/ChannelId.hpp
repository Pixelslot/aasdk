#pragma once

#include <string>

namespace aasdk::messenger {

enum class ChannelId {
  CONTROL,
  INPUT,
  SENSOR,
  VIDEO,
  MEDIA_AUDIO,
  SPEECH_AUDIO,
  SYSTEM_AUDIO,
  AV_INPUT,
  BLUETOOTH,
  PHONE_STATUS,
  NOTIFICATION,
  NAVIGATION,
  NONE = 255
};

std::string channelIdToString(ChannelId channelId);

}
