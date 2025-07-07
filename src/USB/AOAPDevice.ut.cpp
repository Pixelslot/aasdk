#include <boost/test/unit_test.hpp>
#include <USB/UT/USBWrapper.mock.hpp>
#include <aasdk/USB/AOAPDevice.hpp>


namespace aasdk
{
namespace usb
{
namespace ut
{

BOOST_AUTO_TEST_CASE(AOAPDevice_OutEndpointFirst)
{
    USBWrapperMock usbWrapperMock;
    asio::io_service ioService;
    USBWrapperMock::DummyDeviceHandle dummyDeviceHandle;
    DeviceHandle deviceHandle(reinterpret_cast<libusb_device_handle*>(&dummyDeviceHandle), [](auto*) {});

    libusb_endpoint_descriptor endpointDescriptor[2];
    endpointDescriptor[0].bEndpointAddress = LIBUSB_ENDPOINT_OUT + 4;
    endpointDescriptor[1].bEndpointAddress = LIBUSB_ENDPOINT_IN + 6;

    libusb_interface_descriptor interfaceDescriptor;
    interfaceDescriptor.bInterfaceNumber = 55;
    interfaceDescriptor.bNumEndpoints = 2;
    interfaceDescriptor.endpoint = endpointDescriptor;

    EXPECT_CALL(usbWrapperMock, releaseInterface(deviceHandle, interfaceDescriptor.bInterfaceNumber));
    AOAPDevice aoapDevice(usbWrapperMock, ioService, deviceHandle, &interfaceDescriptor);
    BOOST_TEST(endpointDescriptor[0].bEndpointAddress == aoapDevice.getOutEndpoint().getAddress());
    BOOST_TEST(endpointDescriptor[1].bEndpointAddress == aoapDevice.getInEndpoint().getAddress());
}

BOOST_AUTO_TEST_CASE(AOAPDevice_InEndpointFirst)
{
    USBWrapperMock usbWrapperMock;
    asio::io_service ioService;
    USBWrapperMock::DummyDeviceHandle dummyDeviceHandle;
    DeviceHandle deviceHandle(reinterpret_cast<libusb_device_handle*>(&dummyDeviceHandle), [](auto*) {});

    libusb_endpoint_descriptor endpointDescriptor[2];
    endpointDescriptor[0].bEndpointAddress = LIBUSB_ENDPOINT_IN + 4;
    endpointDescriptor[1].bEndpointAddress = LIBUSB_ENDPOINT_OUT + 6;

    libusb_interface_descriptor interfaceDescriptor;
    interfaceDescriptor.bInterfaceNumber = 55;
    interfaceDescriptor.bNumEndpoints = 2;
    interfaceDescriptor.endpoint = endpointDescriptor;

    EXPECT_CALL(usbWrapperMock, releaseInterface(deviceHandle, interfaceDescriptor.bInterfaceNumber));
    AOAPDevice aoapDevice(usbWrapperMock, ioService, deviceHandle, &interfaceDescriptor);
    BOOST_TEST(endpointDescriptor[0].bEndpointAddress == aoapDevice.getInEndpoint().getAddress());
    BOOST_TEST(endpointDescriptor[1].bEndpointAddress == aoapDevice.getOutEndpoint().getAddress());
}

}
}
}
