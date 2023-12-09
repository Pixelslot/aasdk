#include <aasdk/Channel/AV/SystemAudioServiceChannel.hpp>


namespace aasdk::channel::av
{

SystemAudioServiceChannel::SystemAudioServiceChannel(asio::io_service::strand& strand, messenger::IMessenger::Pointer messenger)
    : AudioServiceChannel(strand, std::move(messenger), messenger::ChannelId::SYSTEM_AUDIO)
{

}

}
