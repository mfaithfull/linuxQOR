// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include <string>
#include <tuple>
#include <stdexcept>
#include <iostream>
#include <type_traits>

#include "src/qor/tdd/tdd.h"
#include "src/framework/data/dio/collectionadapter.h"
#include "src/framework/data/dio/interfaceadapter.h"
#include "src/framework/data/dio/ishelpers.h"
#include "src/framework/data/dio/port.h"
#include "src/framework/data/dio/adapter.h"
#include "src/qor/function/make_function.h"

namespace domain{

    struct datatype
    {        
        unsigned int accno;
        float ammount;
        std::string name;
        std::vector<std::string> log;
    };

}//domain

using namespace qor;

using AccNoAdapter = dio::adapter_apply< dio::ReadOnlyValueAdapter, unsigned int, 
    dio::StreamOut, dio::ToString, dio::Name >::type;

using AmmountAdapter = dio::adapter_apply< dio::ValueAdapter, float, 
    dio::Read, dio::EnhancedNumericWrite, dio::StreamIO, dio::Name >::type;

using LogAdapter = dio::adapter_apply< dio::CollectionAdapter, std::vector<std::string>, 
    dio::ReadCollection, dio::Append, dio::StreamOutCollection >::type;

using PaymentInterface = dio::InterfaceAdapter<
        AccNoAdapter,
        AmmountAdapter,
        LogAdapter >;

enum class paymentfields
{
    account,
    ammount,
    log
};

template<class storage>
struct PaymentAdapter : public dio::Adapter<PaymentInterface, storage, paymentfields>
{
    PaymentAdapter(storage& src) : dio::Adapter<PaymentInterface, storage, paymentfields>(src,
        storage::accno, 
        storage::ammount, 
        storage::log)
    { }
};

template<class storage>
struct PaymentPort : public dio::Port<PaymentInterface, storage, paymentfields>
{
    PaymentPort(storage& src) : dio::Port<PaymentInterface, storage, paymentfields>(src,
        src.accno, 
        src.ammount, 
        src.log)
    { }
};

void OnPaymentAmmountChange(float ammount)
{
    std::cout << "ammount changed " << ammount << std::endl;
}

qor_pp_test_case(dio_basics)
{
    //std::cout << compiler::demangle<LogAdapter>() << std::endl;    

    domain::datatype data{ 20015678, 56.8f, "name" };           //domain object
    using adapter_t = PaymentAdapter<domain::datatype>;         //type alias for PaymentAdapter
    adapter_t payment(data);                                    //create a payment
    payment.at<paymentfields::account>().SetName("Account");
    auto a = payment.at<paymentfields::account>().Get();        //Read<T> fields are readable with Get()
    payment.at<paymentfields::ammount>().SetName("Payment Ammount");
    payment.at<paymentfields::ammount>().SetMinimum(0.01f);
    payment.at<paymentfields::ammount>().SetMaximum(10000.0f);
    payment.at<paymentfields::ammount>().SetOnChange(qor_pp_make_function(&OnPaymentAmmountChange));
    payment.at<paymentfields::ammount>().Set(103.8f);           //Write<T> fields are writeable with Set()    

    payment.for_each_field([](size_t& index, const auto& x)     //Apply a lamda template to each field in turn
    {                        
        if constexpr(dio::is_streamoutable<decltype(x)>())
        {
            if (index++ > 0) std::cout << ", ";
            if constexpr(dio::is_named<decltype(x)>())
            {
                std::cout << x.GetName() << ": ";
            }
            x.operator << (std::cout);                          //StreamOut fields are serializable with << barring some type deduction issues
        }
    });

    std::cout << std::endl;

    if constexpr(dio::is_stringable< adapter_t::type_of_field<paymentfields::account>::type >())
    {
        std::cout << payment.at<paymentfields::account>().toString();//ToString fields are convertible with toString()
    }

    using port_t = PaymentPort<domain::datatype>;
    port_t pay(data);
    pay.at<paymentfields::ammount>().Set(34.67f);
    pay.at<paymentfields::log>().push_back("Set ammount to 34.67");
}
