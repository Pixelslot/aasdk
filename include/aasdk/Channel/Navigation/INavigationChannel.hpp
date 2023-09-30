#pragma once

#include <aasdk_proto/NavigationChannelData.pb.h>
#include <aasdk_proto/NavigationChannelMessageIdsEnum.pb.h>
#include <aasdk_proto/NavigationDistanceMessage.pb.h>
#include <aasdk_proto/NavigationDistanceUnitEnum.pb.h>
#include <aasdk_proto/NavigationImageOptionsData.pb.h>
#include <aasdk_proto/NavigationRequestMessage.pb.h>
#include <aasdk_proto/NavigationRequestStateEnum.pb.h>
#include <aasdk_proto/NavigationTurnEventEnum.pb.h>
#include <aasdk_proto/NavigationLaneEventEnum.pb.h>
#include <aasdk_proto/NavigationTurnMessage.pb.h>
#include <aasdk_proto/NavigationTurnSideEnum.pb.h>
#include <aasdk/Common/Data.hpp>
#include <aasdk/Channel/Promise.hpp>
#include <aasdk/Channel/Navigation/INavigationChannelEventHandler.hpp>
#include <aasdk_proto/ChannelOpenResponseMessage.pb.h>



namespace aasdk::channel::navigation {

class INavigationChannel {
 public:
  using Pointer = std::shared_ptr<INavigationChannel>;

  INavigationChannel() = default;
  virtual ~INavigationChannel() = default;

  virtual void receive(INavigationChannelEventHandler::Pointer eventHandler) = 0;
  virtual messenger::ChannelId getId() const = 0;
  virtual void sendChannelOpenResponse(const proto::messages::ChannelOpenResponse& response, SendPromise::Pointer promise) = 0;

};

}
