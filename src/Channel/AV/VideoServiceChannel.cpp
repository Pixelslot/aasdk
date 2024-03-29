#include <aasdk_proto/ControlMessageIdsEnum.pb.h>
#include <aasdk_proto/AVChannelMessageIdsEnum.pb.h>
#include <aasdk/Messenger/Timestamp.hpp>
#include <aasdk/Channel/AV/IVideoServiceChannelEventHandler.hpp>
#include <aasdk/Channel/AV/VideoServiceChannel.hpp>
#include <aasdk/Common/Log.hpp>


namespace aasdk::channel::av
{

VideoServiceChannel::VideoServiceChannel(asio::io_service::strand& strand, messenger::IMessenger::Pointer messenger)
    : ServiceChannel(strand, std::move(messenger), messenger::ChannelId::VIDEO)
{

}

void VideoServiceChannel::receive(IVideoServiceChannelEventHandler::Pointer eventHandler)
{
    auto receivePromise = messenger::ReceivePromise::defer(strand_);
    receivePromise->then([this, self = this->shared_from_this(), eventHandler](const messenger::Message::Pointer& message) {
                           this->messageHandler(message, eventHandler);},
                         [&](const error::Error &e) {  AASDK_LOG(ERROR) << "channel error: " << e.what()
                                                                   << ", channel: " << static_cast<int>(this->getId()) << "\n"; });

    messenger_->enqueueReceive(channelId_, std::move(receivePromise));
}

messenger::ChannelId VideoServiceChannel::getId() const
{
    return channelId_;
}

void VideoServiceChannel::sendChannelOpenResponse(const proto::messages::ChannelOpenResponse& response, SendPromise::Pointer promise)
{
    auto message(std::make_shared<messenger::Message>(channelId_, messenger::EncryptionType::ENCRYPTED, messenger::MessageType::CONTROL));
    message->insertPayload(messenger::MessageId(proto::ids::ControlMessage::CHANNEL_OPEN_RESPONSE).getData());
    message->insertPayload(response);

    this->send(std::move(message), std::move(promise));
}

void VideoServiceChannel::sendAVChannelSetupResponse(const proto::messages::AVChannelSetupResponse& response, SendPromise::Pointer promise)
{
    auto message(std::make_shared<messenger::Message>(channelId_, messenger::EncryptionType::ENCRYPTED, messenger::MessageType::SPECIFIC));
    message->insertPayload(messenger::MessageId(proto::ids::AVChannelMessage::SETUP_RESPONSE).getData());
    message->insertPayload(response);

    this->send(std::move(message), std::move(promise));
}

void VideoServiceChannel::sendVideoFocusIndication(const proto::messages::VideoFocusIndication& indication, SendPromise::Pointer promise)
{
    auto message(std::make_shared<messenger::Message>(channelId_, messenger::EncryptionType::ENCRYPTED, messenger::MessageType::SPECIFIC));
    message->insertPayload(messenger::MessageId(proto::ids::AVChannelMessage::VIDEO_FOCUS_INDICATION).getData());
    message->insertPayload(indication);

    this->send(std::move(message), std::move(promise));
}

void VideoServiceChannel::sendAVMediaAckIndication(const proto::messages::AVMediaAckIndication& indication, SendPromise::Pointer promise)
{
    auto message(std::make_shared<messenger::Message>(channelId_, messenger::EncryptionType::ENCRYPTED, messenger::MessageType::SPECIFIC));
    message->insertPayload(messenger::MessageId(proto::ids::AVChannelMessage::AV_MEDIA_ACK_INDICATION).getData());
    message->insertPayload(indication);

    this->send(std::move(message), std::move(promise));
}

void VideoServiceChannel::messageHandler(const messenger::Message::Pointer& message, IVideoServiceChannelEventHandler::Pointer eventHandler)
{
    messenger::MessageId messageId(message->getPayload());
    common::DataConstBuffer payload(message->getPayload(), messageId.getSizeOf());

    switch(messageId.getId())
    {
    case proto::ids::AVChannelMessage::SETUP_REQUEST:
        this->handleAVChannelSetupRequest(payload, eventHandler);
        break;
    case proto::ids::AVChannelMessage::START_INDICATION:
        this->handleStartIndication(payload, eventHandler);
        break;
    case proto::ids::AVChannelMessage::STOP_INDICATION:
        this->handleStopIndication(payload, eventHandler);
        break;
    case proto::ids::AVChannelMessage::AV_MEDIA_WITH_TIMESTAMP_INDICATION:
        this->handleAVMediaWithTimestampIndication(payload, eventHandler);
        break;
    case proto::ids::AVChannelMessage::AV_MEDIA_INDICATION:
        eventHandler->onAVMediaIndication(payload);
        break;
    case proto::ids::ControlMessage::CHANNEL_OPEN_REQUEST:
        this->handleChannelOpenRequest(payload, eventHandler);
        break;
    case proto::ids::AVChannelMessage::VIDEO_FOCUS_REQUEST:
        this->handleVideoFocusRequest(payload, eventHandler);
        break;
    default:
        AASDK_LOG(error) << "[VideoServiceChannel] message not handled: " << messageId.getId();
        this->receive(std::move(eventHandler));
        break;
    }
}

void VideoServiceChannel::handleAVChannelSetupRequest(const common::DataConstBuffer& payload, const IVideoServiceChannelEventHandler::Pointer& eventHandler)
{
    proto::messages::AVChannelSetupRequest request;
    if(request.ParseFromArray(payload.cdata, payload.size))
    {
        eventHandler->onAVChannelSetupRequest(request);
    }
    else
    {
        eventHandler->onChannelError(error::Error(error::ErrorCode::PARSE_PAYLOAD));
    }
}

void VideoServiceChannel::handleStartIndication(const common::DataConstBuffer& payload, const IVideoServiceChannelEventHandler::Pointer& eventHandler)
{
    proto::messages::AVChannelStartIndication indication;
    if(indication.ParseFromArray(payload.cdata, payload.size))
    {
        eventHandler->onAVChannelStartIndication(indication);
    }
    else
    {
        eventHandler->onChannelError(error::Error(error::ErrorCode::PARSE_PAYLOAD));
    }
}

void VideoServiceChannel::handleStopIndication(const common::DataConstBuffer& payload, const IVideoServiceChannelEventHandler::Pointer& eventHandler)
{
    proto::messages::AVChannelStopIndication indication;
    if(indication.ParseFromArray(payload.cdata, payload.size))
    {
        eventHandler->onAVChannelStopIndication(indication);
    }
    else
    {
        eventHandler->onChannelError(error::Error(error::ErrorCode::PARSE_PAYLOAD));
    }
}

void VideoServiceChannel::handleChannelOpenRequest(const common::DataConstBuffer& payload, const IVideoServiceChannelEventHandler::Pointer& eventHandler)
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

void VideoServiceChannel::handleVideoFocusRequest(const common::DataConstBuffer& payload, const IVideoServiceChannelEventHandler::Pointer& eventHandler)
{
    proto::messages::VideoFocusRequest request;
    if(request.ParseFromArray(payload.cdata, payload.size))
    {
        eventHandler->onVideoFocusRequest(request);
    }
    else
    {
        eventHandler->onChannelError(error::Error(error::ErrorCode::PARSE_PAYLOAD));
    }
}

void VideoServiceChannel::handleAVMediaWithTimestampIndication(const common::DataConstBuffer& payload, const IVideoServiceChannelEventHandler::Pointer& eventHandler)
{
    if(payload.size >= sizeof(messenger::Timestamp::ValueType))
    {
        messenger::Timestamp timestamp(payload);
        eventHandler->onAVMediaWithTimestampIndication(timestamp.getValue(), common::DataConstBuffer(payload.cdata, payload.size, sizeof(messenger::Timestamp::ValueType)));
    }
    else
    {
        eventHandler->onChannelError(error::Error(error::ErrorCode::PARSE_PAYLOAD));
    }
}

}
