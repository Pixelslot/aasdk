#pragma once

#include <asio.hpp>
#include <aasdk/Messenger/IMessenger.hpp>
#include <aasdk/Channel/ServiceChannel.hpp>
#include <aasdk/Channel/Navigation/INavigationChannel.hpp>

namespace aasdk::channel::navigation {

class NavigationChannel
    : public INavigationChannel, public ServiceChannel, public std::enable_shared_from_this<NavigationChannel> {
 public:
  NavigationChannel(asio::io_service::strand &strand, messenger::IMessenger::Pointer messenger);

  void receive(INavigationChannelEventHandler::Pointer eventHandler) override;
  messenger::ChannelId getId() const override;
  void sendChannelOpenResponse(const proto::messages::ChannelOpenResponse &response,
                               SendPromise::Pointer promise) override;

 private:
  using std::enable_shared_from_this<NavigationChannel>::shared_from_this;
  void messageHandler(const messenger::Message::Pointer& message, INavigationChannelEventHandler::Pointer eventHandler);
  void handleChannelOpenRequest(const common::DataConstBuffer& payload, const INavigationChannelEventHandler::Pointer& eventHandler);

  void handleNavigationStatus(const common::DataConstBuffer &payload,
                              const INavigationChannelEventHandler::Pointer& eventHandler);
  void handleNavigationTurn(const common::DataConstBuffer &payload,
                            const INavigationChannelEventHandler::Pointer& eventHandler);
  void handleNavigationDistance(const common::DataConstBuffer &payload,
                                const INavigationChannelEventHandler::Pointer& eventHandler);
};

}
