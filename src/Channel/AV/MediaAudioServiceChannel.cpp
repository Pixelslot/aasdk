#include <aasdk/Channel/AV/MediaAudioServiceChannel.hpp>


namespace aasdk::channel::av
{

MediaAudioServiceChannel::MediaAudioServiceChannel(asio::io_service::strand& strand, messenger::IMessenger::Pointer messenger)
    : AudioServiceChannel(strand, std::move(messenger), messenger::ChannelId::MEDIA_AUDIO)
{

}

}
