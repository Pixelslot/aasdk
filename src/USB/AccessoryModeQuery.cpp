#include <aasdk/USB/AccessoryModeQuery.hpp>
#include <aasdk/USB/USBEndpoint.hpp>


namespace aasdk::usb
{

AccessoryModeQuery::AccessoryModeQuery(asio::io_service& ioService, IUSBEndpoint::Pointer usbEndpoint)
    : strand_(ioService)
    , usbEndpoint_(std::move(usbEndpoint))
{

}

void AccessoryModeQuery::cancel()
{
    usbEndpoint_->cancelTransfers();
}

}
