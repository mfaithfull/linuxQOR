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
#include "src/framework/data/dio/structureadapter.h"
#include "src/framework/data/dio/ishelpers.h"
#include "src/framework/data/dio/port.h"
#include "src/framework/data/dio/adapter.h"
#include "src/framework/data/dio/functionadapter.h"
#include "src/framework/data/dio/interface.h"

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

using PaymentStructure = dio::StructureAdapter<
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
struct PaymentAdapter : public dio::Adapter<PaymentStructure, storage, paymentfields>
{
    PaymentAdapter(storage& src) : dio::Adapter<PaymentStructure, storage, paymentfields>(src,
        storage::accno, 
        storage::ammount, 
        storage::log)
    { }
};

template<class storage>
struct PaymentPort : public dio::Port<PaymentStructure, storage, paymentfields>
{
    PaymentPort(storage& src) : dio::Port<PaymentStructure, storage, paymentfields>(src,
        src.accno, 
        src.ammount, 
        src.log)
    { }
};

void OnPaymentAmmountChange(float ammount)
{
    std::cout << "ammount changed " << ammount << std::endl;
}

struct CallTarget
{
    size_t size{0};

    void Increase(){ ++size; }
    size_t Set(size_t newSize){ size_t oldSize = size; size = newSize; return oldSize; }
};

enum class tf
{
    Set,
    L
};

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

    CallTarget target;
    
    dio::MemberFunctionAdapter<CallTarget, void, void> fad(target, qor_pp_make_function(&CallTarget::Increase));
    fad();

    dio::MemberFunctionAdapter<CallTarget, size_t, size_t> setter(target, qor_pp_make_function(&CallTarget::Set));

    size_t result = setter(57000);

    auto lFunc = [&target](size_t newSize) -> size_t
    {
        return target.Set(newSize);
    };

    using SetterInterface = dio::Interface< CallTarget, dio::MemberFunctionAdapter<CallTarget, size_t, size_t>, decltype(lFunc)>;

    SetterInterface iSetter(target, setter, lFunc);

    std::get<static_cast<int>(tf::Set)>(iSetter)(271);
    std::get<static_cast<int>(tf::L)>(iSetter)(869);
    
}
