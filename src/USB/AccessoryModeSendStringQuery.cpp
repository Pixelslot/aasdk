#include <iomanip>
#include <aasdk/USB/AccessoryModeSendStringQuery.hpp>
#include <aasdk/USB/USBEndpoint.hpp>


namespace aasdk::usb
{

AccessoryModeSendStringQuery::AccessoryModeSendStringQuery(asio::io_service& ioService, IUSBWrapper& usbWrapper, IUSBEndpoint::Pointer usbEndpoint,
                                                           AccessoryModeSendStringType sendStringType, const std::string& queryValue)
    : AccessoryModeQuery(ioService, std::move(usbEndpoint)), sendStringType_(sendStringType)
{
    data_.resize(8);
    data_.insert(data_.end(), queryValue.begin(), queryValue.end());
    data_.push_back('\0');

    usbWrapper.fillControlSetup((data_).data(), LIBUSB_ENDPOINT_OUT | USB_TYPE_VENDOR, ACC_REQ_SEND_STRING, 0, static_cast<uint16_t>(sendStringType_), data_.size() - 8);
}

void AccessoryModeSendStringQuery::start(Promise::Pointer promise)
{
    strand_.dispatch([this, self = this->shared_from_this(), promise = std::move(promise)]() mutable {
        if(promise_ != nullptr)
        {
            promise->reject(error::Error(error::ErrorCode::OPERATION_IN_PROGRESS));
        }
        else
        {
            promise_ = std::move(promise);

            auto usbEndpointPromise = IUSBEndpoint::Promise::defer(strand_);
            usbEndpointPromise->then([this, self = this->shared_from_this()](size_t bytesTransferred) mutable {
                    promise_->resolve(usbEndpoint_);
                    promise_.reset();
                },
                [this, self = this->shared_from_this()](const error::Error& e) mutable {
                    promise_->reject(e);
                    promise_.reset();
                });

            usbEndpoint_->controlTransfer(common::DataBuffer(data_), cTransferTimeoutMs, std::move(usbEndpointPromise));
        }
    });
}

}
