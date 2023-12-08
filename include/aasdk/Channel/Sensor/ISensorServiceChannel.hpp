#pragma once

#include <memory>
#include <aasdk/Messenger/ChannelId.hpp>
#include <aasdk/Channel/Promise.hpp>
#include <aasdk_proto/SensorEventIndicationMessage.pb.h>
#include <aasdk_proto/ChannelOpenResponseMessage.pb.h>
#include <aasdk_proto/SensorStartResponseMessage.pb.h>
#include <aasdk/Channel/Sensor/ISensorServiceChannelEventHandler.hpp>


namespace aasdk::channel::sensor {

class ISensorServiceChannel {
 public:
  using Pointer = std::shared_ptr<ISensorServiceChannel>;

  ISensorServiceChannel() = default;
  virtual ~ISensorServiceChannel() = default;

    virtual void receive(ISensorServiceChannelEventHandler::Pointer eventHandler) = 0;
    virtual messenger::ChannelId getId() const = 0;
    virtual void sendChannelOpenResponse(const proto::messages::ChannelOpenResponse& response, SendPromise::Pointer promise) = 0;
    virtual void sendSensorEventIndication(const proto::messages::SensorEventIndication& indication, SendPromise::Pointer promise) = 0;
    virtual void sendSensorStartResponse(const proto::messages::SensorStartResponseMessage& response, SendPromise::Pointer promise) = 0;
};

}
