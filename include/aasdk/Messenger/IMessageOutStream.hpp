/*
*  This file is part of aasdk library project.
*  Copyright (C) 2018 f1x.studio (Michal Szwaj)
*
*  aasdk is free software: you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation; either version 3 of the License, or
*  (at your option) any later version.

*  aasdk is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with aasdk. If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <aasdk/Messenger/Message.hpp>
#include <aasdk/Messenger/MessageId.hpp>
#include <aasdk/Messenger/Promise.hpp>


namespace aasdk::messenger {

class IMessageOutStream {
 public:
  using Pointer = std::shared_ptr<IMessageOutStream>;

  IMessageOutStream() = default;
  virtual ~IMessageOutStream() = default;

    virtual void stream(Message::Pointer message, SendPromise::Pointer promise) = 0;
};

}
