#pragma once

#include <asio.hpp>
#include <list>
#include <aasdk/USB/IUSBHub.hpp>
#include <aasdk/USB/IAccessoryModeQueryChainFactory.hpp>


namespace aasdk
{
namespace usb
{

class IUSBWrapper;

class USBHub: public IUSBHub, public std::enable_shared_from_this<USBHub>
{
public:
    USBHub(IUSBWrapper& usbWrapper, asio::io_service& ioService, IAccessoryModeQueryChainFactory& queryChainFactory);

    void start(Promise::Pointer promise) override;
    void cancel() override;
    
private:
    typedef std::list<IAccessoryModeQueryChain::Pointer> QueryChainQueue;
    using std::enable_shared_from_this<USBHub>::shared_from_this;
    void handleDevice(libusb_device* device);
    bool isAOAPDevice(const libusb_device_descriptor& deviceDescriptor) const;
    static int hotplugEventsHandler(libusb_context* usbContext, libusb_device* device, libusb_hotplug_event event, void* uerData);

    IUSBWrapper& usbWrapper_;
    asio::io_service::strand strand_;
    IAccessoryModeQueryChainFactory& queryChainFactory_;
    Promise::Pointer hotplugPromise_;
    Pointer self_;
    HotplugCallbackHandle hotplugHandle_;
    QueryChainQueue queryChainQueue_;

    static constexpr uint16_t cGoogleVendorId = 0x18D1;
    static constexpr uint16_t cAOAPId = 0x2D00;
    static constexpr uint16_t cAOAPWithAdbId = 0x2D01;

    USBHub(const USBHub&) = delete;
};

}
}
