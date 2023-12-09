#include <aasdk_proto/ControlMessageIdsEnum.pb.h>
#include <aasdk_proto/PhoneStatusChannelMessageIdsEnum.pb.h>
#include <aasdk_proto/PhoneStatusIndicationMessage.pb.h>
#include <aasdk/Channel/PhoneStatus/IPhoneStatusServiceChannelEventHandler.hpp>
#include <aasdk/Channel/PhoneStatus/PhoneStatusServiceChannel.hpp>
#include <aasdk/Common/Log.hpp>


namespace aasdk::channel::phonestatus
        {

            PhoneStatusServiceChannel::PhoneStatusServiceChannel(asio::io_service::strand& strand,  messenger::IMessenger::Pointer messenger)
                    : ServiceChannel(strand, std::move(messenger), messenger::ChannelId::PHONE_STATUS)
            {

            }

            void PhoneStatusServiceChannel::receive(IPhoneStatusServiceChannelEventHandler::Pointer eventHandler)
            {
                auto receivePromise = messenger::ReceivePromise::defer(strand_);
                receivePromise->then([this, self = this->shared_from_this(), eventHandler](const messenger::Message::Pointer& message) {
                                       this->messageHandler(message, eventHandler);},
                                     [&](const error::Error &e) { AASDK_LOG(ERROR) << "channel error: " << e.what()
                                                                                   << ", channel: " << static_cast<int>(this->getId()) << "\n"; });

                messenger_->enqueueReceive(channelId_, std::move(receivePromise));
            }

            messenger::ChannelId PhoneStatusServiceChannel::getId() const
            {
                return channelId_;
            }

            void PhoneStatusServiceChannel::sendChannelOpenResponse(const proto::messages::ChannelOpenResponse& response, SendPromise::Pointer promise)
            {
                auto message(std::make_shared<messenger::Message>(channelId_, messenger::EncryptionType::ENCRYPTED, messenger::MessageType::CONTROL));
                message->insertPayload(messenger::MessageId(proto::ids::ControlMessage::CHANNEL_OPEN_RESPONSE).getData());
                message->insertPayload(response);

                this->send(std::move(message), std::move(promise));
            }

            void PhoneStatusServiceChannel::messageHandler(const messenger::Message::Pointer& message, IPhoneStatusServiceChannelEventHandler::Pointer eventHandler)
            {
                messenger::MessageId messageId(message->getPayload());
                common::DataConstBuffer payload(message->getPayload(), messageId.getSizeOf());

                switch(messageId.getId())
                {
                    case proto::ids::PhoneStatusChannelMessage::PHONE_STATUS:
                        this->handlePhoneStatusMessage(payload, eventHandler);
                        break;
                    case proto::ids::PhoneStatusChannelMessage::PHONE_STATUS_INPUT:
                        AASDK_LOG(info) << "[PhoneStatusServiceChannel] PHONE_STATUS_INPUT";
                        break;
                    case proto::ids::ControlMessage::CHANNEL_OPEN_REQUEST:
                        this->handleChannelOpenRequest(payload, eventHandler);
                        break;
                    default:
                        AASDK_LOG(error) << "[PhoneStatusServiceChannel] message not handled: " << messageId.getId();
                        this->receive(std::move(eventHandler));
                        break;
                }
            }

            void PhoneStatusServiceChannel::handleChannelOpenRequest(const common::DataConstBuffer& payload, const IPhoneStatusServiceChannelEventHandler::Pointer& eventHandler)
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

            void PhoneStatusServiceChannel::handlePhoneStatusMessage(const common::DataConstBuffer& payload, const IPhoneStatusServiceChannelEventHandler::Pointer& eventHandler){
                proto::messages::PhoneStatus request;
                if(request.ParseFromArray(payload.cdata, payload.size))
                {
                    eventHandler->onPhoneStatusMessage(request);
                }
                else
                {
                    eventHandler->onChannelError(error::Error(error::ErrorCode::PARSE_PAYLOAD));
                }
            }

        }
