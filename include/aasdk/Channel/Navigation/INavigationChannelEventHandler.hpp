#pragma once

#include <aasdk/Error/Error.hpp>
#include <aasdk/Common/Data.hpp>
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
#include <aasdk_proto/ChannelOpenRequestMessage.pb.h>

namespace aasdk::channel::navigation {

class INavigationChannelEventHandler {
 public:
  using Pointer = std::shared_ptr<INavigationChannelEventHandler>;

  INavigationChannelEventHandler() = default;
  virtual ~INavigationChannelEventHandler() = default;

  virtual void onNavigationStatus(const proto::messages::NavigationRequestMessage& request) = 0;
  virtual void onNavigationTurn(const proto::messages::NavigationTurnMessage& request) = 0;
  virtual void onNavigationDistance(const proto::messages::NavigationDistanceMessage& request) = 0;
  virtual void onChannelOpenRequest(const proto::messages::ChannelOpenRequest& request) = 0;
  virtual void onChannelError(const error::Error& e) = 0;
};

}
