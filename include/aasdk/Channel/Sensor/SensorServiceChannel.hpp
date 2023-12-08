#pragma once

#include <aasdk/Channel/ServiceChannel.hpp>
#include <aasdk/Channel/Sensor/ISensorServiceChannel.hpp>


namespace aasdk::channel::sensor {

class SensorServiceChannel
    : public ISensorServiceChannel, public ServiceChannel, public std::enable_shared_from_this<SensorServiceChannel> {
 public:
  SensorServiceChannel(asio::io_service::strand &strand, messenger::IMessenger::Pointer messenger);

  void receive(ISensorServiceChannelEventHandler::Pointer eventHandler) override;
  messenger::ChannelId getId() const override;
  void sendChannelOpenResponse(const proto::messages::ChannelOpenResponse &response,
                               SendPromise::Pointer promise) override;
    void sendSensorEventIndication(const proto::messages::SensorEventIndication& indication, SendPromise::Pointer promise) override;
    void sendSensorStartResponse(const proto::messages::SensorStartResponseMessage& response, SendPromise::Pointer promise) override;

private:
    using std::enable_shared_from_this<SensorServiceChannel>::shared_from_this;
    void messageHandler(const messenger::Message::Pointer& message, ISensorServiceChannelEventHandler::Pointer eventHandler);
    void handleSensorStartRequest(const common::DataConstBuffer& payload, const ISensorServiceChannelEventHandler::Pointer& eventHandler);
    void handleChannelOpenRequest(const common::DataConstBuffer& payload, const ISensorServiceChannelEventHandler::Pointer& eventHandler);
};

}
