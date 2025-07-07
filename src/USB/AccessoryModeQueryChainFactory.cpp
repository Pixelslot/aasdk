#include <aasdk/USB/AccessoryModeQueryChainFactory.hpp>
#include <aasdk/USB/AccessoryModeQueryChain.hpp>


namespace aasdk::usb
{

AccessoryModeQueryChainFactory::AccessoryModeQueryChainFactory(IUSBWrapper& usbWrapper,
                                                               asio::io_service& ioService,
                                                               IAccessoryModeQueryFactory& queryFactory)
    : usbWrapper_(usbWrapper)
    , ioService_(ioService)
    , queryFactory_(queryFactory)
{

}

IAccessoryModeQueryChain::Pointer AccessoryModeQueryChainFactory::create()
{
    return std::make_shared<AccessoryModeQueryChain>(usbWrapper_, ioService_, queryFactory_);
}

}
