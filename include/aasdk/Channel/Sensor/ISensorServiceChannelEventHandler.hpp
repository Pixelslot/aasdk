#pragma once

#include <aasdk_proto/SensorStartRequestMessage.pb.h>
#include <aasdk_proto/ChannelOpenRequestMessage.pb.h>
#include <aasdk/Error/Error.hpp>


namespace aasdk::channel::sensor {

class ISensorServiceChannelEventHandler {
 public:
  using Pointer = std::shared_ptr<ISensorServiceChannelEventHandler>;

  ISensorServiceChannelEventHandler() = default;
  virtual ~ISensorServiceChannelEventHandler() = default;

    virtual void onChannelOpenRequest(const proto::messages::ChannelOpenRequest& request) = 0;
    virtual void onSensorStartRequest(const proto::messages::SensorStartRequestMessage& request) = 0;
    virtual void onChannelError(const error::Error& e) = 0;
};

}
