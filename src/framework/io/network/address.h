// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PLATFORM_NETWORK_ADDRESS
#define QOR_PP_H_PLATFORM_NETWORK_ADDRESS

#include <string>

namespace qor{ namespace io { namespace network{

#pragma pack(push,1)

    class qor_pp_module_interface(QOR_SOCKETS) Address
    {
    public:

        Address();
        Address(const Address& src);
        Address& operator = (const Address& src);
        Address(unsigned short AddressFamily);
        ~Address();

        void SetIPV4Address(unsigned char a, unsigned char b, unsigned char c, unsigned char d);
        std::string GetIPV4Address() const;
        unsigned short GetPort() const;
        void SetPort(unsigned short port);

        unsigned short sa_family;

        union sa
        {
            char sa_data[30];

            struct BluetoothAddress
            {
                uint64_t		        btAddr;         // Bluetooth device address
                qor::GUID				serviceClassId; // [OPTIONAL] system will query SDP for port
                unsigned long			port;           // RFCOMM channel or L2CAP PSM
            }BluetoothAddress;

            struct IrDaAddress
            {
                unsigned char  irdaDeviceID[4];
                char	irdaServiceName[25];
            }IrDaAddress;

            struct IPAddress
            {
                unsigned short sin_port;
                struct sin_addr
                {
                    union S_un
                    {
                        struct S_un_b
                        {
                            unsigned char s_b1, s_b2, s_b3, s_b4;
                        }b;
                        struct S_un_w
                        {
                            unsigned short s_w1, s_w2;
                        }w;
                        unsigned long S_addr;
                    }S_un;
                }sin_addr;
                char sin_zero[8];
            }IPAddress;
        }sa;
    };

#pragma pack( pop )

}}}//qor::io::network

#endif//QOR_PP_H_PLATFORM_NETWORK_ADDRESS
