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

#include <limits>
#include <aasdk/Common/Data.hpp>
#include <mutex>
#include <memory>

namespace aasdk::transport {

class DataSink {
 public:
  DataSink();

  common::DataBuffer fill();
  void commit(common::Data::size_type size);

  common::Data::size_type getAvailableSize();
  common::Data consume(common::Data::size_type size);

 private:
  static constexpr common::Data::size_type cChunkSize = 16384;
  std::mutex mutex_;
  std::unique_ptr<common::Data::value_type[]> buf_;
  size_t head_ = 0;
  size_t tail_ = 0;
  const size_t max_size_;

  [[nodiscard]] bool empty() const;
  [[nodiscard]] bool full() const;
  [[nodiscard]] size_t capacity() const;
  [[nodiscard]] size_t size() const;
};

}
