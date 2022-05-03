#pragma once

#include <asio.hpp>
#include <list>
#include <aasdk/Messenger/IMessenger.hpp>
#include <aasdk/Messenger/IMessageInStream.hpp>
#include <aasdk/Messenger/IMessageOutStream.hpp>
#include <aasdk/Messenger/ChannelReceiveMessageQueue.hpp>
#include <aasdk/Messenger/ChannelReceivePromiseQueue.hpp>


namespace aasdk
{
namespace messenger
{

class Messenger: public IMessenger, public std::enable_shared_from_this<Messenger>
{
public:
    Messenger(asio::io_service& ioService, IMessageInStream::Pointer messageInStream, IMessageOutStream::Pointer messageOutStream);
    void enqueueReceive(ChannelId channelId, ReceivePromise::Pointer promise) override;
    void enqueueSend(Message::Pointer message, SendPromise::Pointer promise) override;
    void stop() override;

private:
    using std::enable_shared_from_this<Messenger>::shared_from_this;
    typedef std::list<std::pair<Message::Pointer, SendPromise::Pointer>> ChannelSendQueue;
    void doSend();
    void inStreamMessageHandler(Message::Pointer message);
    void outStreamMessageHandler(ChannelSendQueue::iterator queueElement);
    void rejectReceivePromiseQueue(const error::Error& e);
    void rejectSendPromiseQueue(const error::Error& e);
    void parseMessage(Message::Pointer message, ReceivePromise::Pointer promise);

    asio::io_service::strand receiveStrand_;
    asio::io_service::strand sendStrand_;
    IMessageInStream::Pointer messageInStream_;
    IMessageOutStream::Pointer messageOutStream_;

    ChannelReceivePromiseQueue channelReceivePromiseQueue_;
    ChannelReceiveMessageQueue channelReceiveMessageQueue_;
    ChannelSendQueue channelSendPromiseQueue_;

    Messenger(const Messenger&) = delete;
};

}
}
