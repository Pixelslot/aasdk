#include <aasdk_proto/ControlMessageIdsEnum.pb.h>
#include <aasdk_proto/SensorChannelMessageIdsEnum.pb.h>
#include <aasdk/Channel/Sensor/ISensorServiceChannelEventHandler.hpp>
#include <aasdk/Channel/Sensor/SensorServiceChannel.hpp>
#include <aasdk/Common/Log.hpp>


namespace aasdk::channel::sensor
{

SensorServiceChannel::SensorServiceChannel(asio::io_service::strand& strand,  messenger::IMessenger::Pointer messenger)
    : ServiceChannel(strand, std::move(messenger), messenger::ChannelId::SENSOR)
{

}

void SensorServiceChannel::receive(ISensorServiceChannelEventHandler::Pointer eventHandler)
{
    auto receivePromise = messenger::ReceivePromise::defer(strand_);
    receivePromise->then([this, self = this->shared_from_this(), eventHandler](const messenger::Message::Pointer& message) {
                           this->messageHandler(message, eventHandler);},
                         [&](const error::Error &e) { AASDK_LOG(ERROR) << "channel error: " << e.what()
                                                                       << ", channel: " << static_cast<int>(this->getId()) << "\n"; });

    messenger_->enqueueReceive(channelId_, std::move(receivePromise));
}

messenger::ChannelId SensorServiceChannel::getId() const
{
    return channelId_;
}

void SensorServiceChannel::sendChannelOpenResponse(const proto::messages::ChannelOpenResponse& response, SendPromise::Pointer promise)
{
    auto message(std::make_shared<messenger::Message>(channelId_, messenger::EncryptionType::ENCRYPTED, messenger::MessageType::CONTROL));
    message->insertPayload(messenger::MessageId(proto::ids::ControlMessage::CHANNEL_OPEN_RESPONSE).getData());
    message->insertPayload(response);

    this->send(std::move(message), std::move(promise));
}

void SensorServiceChannel::messageHandler(const messenger::Message::Pointer &message, ISensorServiceChannelEventHandler::Pointer eventHandler)
{
    messenger::MessageId messageId(message->getPayload());
    common::DataConstBuffer payload(message->getPayload(), messageId.getSizeOf());

    switch(messageId.getId())
    {
    case proto::ids::SensorChannelMessage::SENSOR_START_REQUEST:
        this->handleSensorStartRequest(payload, eventHandler);
        break;
    case proto::ids::ControlMessage::CHANNEL_OPEN_REQUEST:
        this->handleChannelOpenRequest(payload, eventHandler);
        break;
    default:
        AASDK_LOG(error) << "[SensorServiceChannel] message not handled: " << messageId.getId();
        this->receive(std::move(eventHandler));
        break;
    }
}

void SensorServiceChannel::sendSensorEventIndication(const proto::messages::SensorEventIndication& indication, SendPromise::Pointer promise)
{
    auto message(std::make_shared<messenger::Message>(channelId_, messenger::EncryptionType::ENCRYPTED, messenger::MessageType::SPECIFIC));
    message->insertPayload(messenger::MessageId(proto::ids::SensorChannelMessage::SENSOR_EVENT_INDICATION).getData());
    message->insertPayload(indication);

    this->send(std::move(message), std::move(promise));
}

void SensorServiceChannel::sendSensorStartResponse(const proto::messages::SensorStartResponseMessage& response, SendPromise::Pointer promise)
{
    auto message(std::make_shared<messenger::Message>(channelId_, messenger::EncryptionType::ENCRYPTED, messenger::MessageType::SPECIFIC));
    message->insertPayload(messenger::MessageId(proto::ids::SensorChannelMessage::SENSOR_START_RESPONSE).getData());
    message->insertPayload(response);

    this->send(std::move(message), std::move(promise));
}

void SensorServiceChannel::handleSensorStartRequest(const common::DataConstBuffer& payload, const ISensorServiceChannelEventHandler::Pointer &eventHandler)
{
    proto::messages::SensorStartRequestMessage request;
    if(request.ParseFromArray(payload.cdata, payload.size))
    {
        eventHandler->onSensorStartRequest(request);
    }
    else
    {
        eventHandler->onChannelError(error::Error(error::ErrorCode::PARSE_PAYLOAD));
    }
}

void SensorServiceChannel::handleChannelOpenRequest(const common::DataConstBuffer& payload, const ISensorServiceChannelEventHandler::Pointer &eventHandler)
{
    proto::messages::ChannelOpenRequest request;
    if(request.ParseFromArray(payload.cdata, payload.size))
    {
        eventHandler->onChannelOpenRequest(request);
    }
    else
    {
        eventHandler->onChannelError(error::Error(error::ErrorCode::PARSE_PAYLOAD));
    }
}

}
