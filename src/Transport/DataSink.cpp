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

#include <aasdk/Transport/DataSink.hpp>
#include <aasdk/Error/Error.hpp>
#include <iostream>

namespace aasdk::transport {

DataSink::DataSink()
    : buf_(std::unique_ptr<common::Data::value_type[]>(new common::Data::value_type[common::cStaticDataSize])),
      max_size_(common::cStaticDataSize) {
}

common::DataBuffer DataSink::fill() {
  std::lock_guard<std::mutex> lock(mutex_);
  if (head_ + cChunkSize > max_size_) {
    std::cout << "linearizing \n";
    auto *newbuf = new common::Data::value_type[common::cStaticDataSize];
    std::copy(&buf_[tail_], &buf_[tail_ + size()], &newbuf[0]);
    buf_.reset(newbuf);
    head_ = size();
    tail_ = 0;
  }
  return {&buf_[head_], cChunkSize};
}

void DataSink::commit(common::Data::size_type writeSize) {
  std::lock_guard<std::mutex> lock(mutex_);
  if (writeSize > cChunkSize) {
    throw error::Error(error::ErrorCode::DATA_SINK_COMMIT_OVERFLOW);
  }
  if (full()) {
    tail_ = (tail_ + writeSize) % max_size_;
  }

  head_ = (head_ + writeSize) % max_size_;
}

common::Data::size_type DataSink::getAvailableSize() {
  return size();
}

common::Data DataSink::consume(common::Data::size_type readSize) {
  std::lock_guard<std::mutex> lock(mutex_);
  if (empty() || size() < readSize) {
    throw error::Error(error::ErrorCode::DATA_SINK_CONSUME_UNDERFLOW);
  }

  common::Data data(readSize, 0);
  std::copy(&buf_[tail_], &buf_[tail_ + readSize], data.begin());
  tail_ = (tail_ + readSize) % max_size_;

  return data;
}

bool DataSink::empty() const {
  //if head and tail are equal, we are empty
  return (!full() && (head_ == tail_));
}

bool DataSink::full() const {
  //If tail is ahead the head by 1, we are full
  return ((head_ + 1) % max_size_) == tail_;
}

size_t DataSink::capacity() const {
  return max_size_;
}

size_t DataSink::size() const {
  size_t size = max_size_;

  if (!full()) {
    if (head_ >= tail_) {
      size = head_ - tail_;
    } else {
      size = max_size_ + head_ - tail_;
    }
  }

  return size;
}

}
