#pragma once

#include <aasdk/Channel/ServiceChannel.hpp>
#include <aasdk/Channel/PhoneStatus/IPhoneStatusServiceChannel.hpp>


namespace aasdk::channel::phonestatus
        {

            class PhoneStatusServiceChannel: public IPhoneStatusServiceChannel, public ServiceChannel, public std::enable_shared_from_this<PhoneStatusServiceChannel>
            {
            public:
                PhoneStatusServiceChannel(asio::io_service::strand& strand, messenger::IMessenger::Pointer messenger);

                void receive(IPhoneStatusServiceChannelEventHandler::Pointer eventHandler) override;
                messenger::ChannelId getId() const override;
                void sendChannelOpenResponse(const proto::messages::ChannelOpenResponse& response, SendPromise::Pointer promise) override;

            private:
                using std::enable_shared_from_this<PhoneStatusServiceChannel>::shared_from_this;
                void messageHandler(const messenger::Message::Pointer& message, IPhoneStatusServiceChannelEventHandler::Pointer eventHandler);
                void handleChannelOpenRequest(const common::DataConstBuffer& payload, const IPhoneStatusServiceChannelEventHandler::Pointer& eventHandler);
                void handlePhoneStatusMessage(const common::DataConstBuffer& payload, const IPhoneStatusServiceChannelEventHandler::Pointer& eventHandler);
            };

        }
