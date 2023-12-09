#include <aasdk/Channel/AV/SpeechAudioServiceChannel.hpp>


namespace aasdk::channel::av
{

SpeechAudioServiceChannel::SpeechAudioServiceChannel(asio::io_service::strand& strand, messenger::IMessenger::Pointer messenger)
    : AudioServiceChannel(strand, std::move(messenger), messenger::ChannelId::SPEECH_AUDIO)
{

}

}
