#pragma once

#include <asio.hpp>
#include <aasdk/Messenger/IMessenger.hpp>
#include <aasdk/Channel/Promise.hpp>

namespace aasdk::channel
{

class ServiceChannel
{
protected:
    ServiceChannel(asio::io_service::strand& strand,
                   messenger::IMessenger::Pointer messenger,
                   messenger::ChannelId channelId);

    virtual ~ServiceChannel() = default;
    void send(messenger::Message::Pointer message, SendPromise::Pointer promise);

    asio::io_service::strand& strand_;
    messenger::IMessenger::Pointer messenger_;
    messenger::ChannelId channelId_;
};

}
