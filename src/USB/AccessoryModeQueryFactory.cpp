#include <aasdk/USB/AccessoryModeQueryFactory.hpp>
#include <aasdk/USB/AccessoryModeSendStringQuery.hpp>
#include <aasdk/USB/AccessoryModeStartQuery.hpp>
#include <aasdk/USB/AccessoryModeProtocolVersionQuery.hpp>
#include <aasdk/USB/AccessoryModeSendStringType.hpp>



namespace aasdk::usb
{

AccessoryModeQueryFactory::AccessoryModeQueryFactory(usb::IUSBWrapper& usbWrapper, asio::io_service& ioService)
    : usbWrapper_(usbWrapper)
    , ioService_(ioService)
{

}

IAccessoryModeQuery::Pointer AccessoryModeQueryFactory::createQuery(AccessoryModeQueryType queryType, IUSBEndpoint::Pointer usbEndpoint)
{
    switch(queryType)
    {
    case AccessoryModeQueryType::PROTOCOL_VERSION:
        return std::make_shared<AccessoryModeProtocolVersionQuery>(ioService_, usbWrapper_, std::move(usbEndpoint));

    case AccessoryModeQueryType::SEND_DESCRIPTION:
        return std::make_shared<AccessoryModeSendStringQuery>(ioService_, usbWrapper_, std::move(usbEndpoint),
                                                              AccessoryModeSendStringType::DESCRIPTION, "Android Auto");

    case AccessoryModeQueryType::SEND_MANUFACTURER:
        return std::make_shared<AccessoryModeSendStringQuery>(ioService_, usbWrapper_, std::move(usbEndpoint),
                                                              AccessoryModeSendStringType::MANUFACTURER, "Mazda");

    case AccessoryModeQueryType::SEND_MODEL:
        return std::make_shared<AccessoryModeSendStringQuery>(ioService_, usbWrapper_, std::move(usbEndpoint),
                                                              AccessoryModeSendStringType::MODEL, "Android Auto");

    case AccessoryModeQueryType::SEND_SERIAL:
        return std::make_shared<AccessoryModeSendStringQuery>(ioService_, usbWrapper_, std::move(usbEndpoint),
                                                              AccessoryModeSendStringType::SERIAL, "HU-AAAAAA001");

    case AccessoryModeQueryType::SEND_URI:
        return std::make_shared<AccessoryModeSendStringQuery>(ioService_, usbWrapper_, std::move(usbEndpoint),
                                                              AccessoryModeSendStringType::URI, "https://www.google.com/maps");

    case AccessoryModeQueryType::SEND_VERSION:
        return std::make_shared<AccessoryModeSendStringQuery>(ioService_, usbWrapper_, std::move(usbEndpoint),
                                                              AccessoryModeSendStringType::VERSION, "2.0.1");

    case AccessoryModeQueryType::START:
        return std::make_shared<AccessoryModeStartQuery>(ioService_, usbWrapper_, std::move(usbEndpoint));

    default:
        return nullptr;
    }
}

}
