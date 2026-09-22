// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_PROTOCOLS_URI
#define QOR_PP_H_COMPONENTS_PROTOCOLS_URI

#include <string>

namespace qor{ namespace data{

    /*
    SchemeDelimiter read only string
    UriSchemeFile
    UriSchemeFtp
    UriSchemeFtps
    UriSchemeGopher
    UriSchemeHttp
    UriSchemeHttps
    UriSchemeMailto
    UriSchemeNetPipe
    UriSchemeNetTcp
    UriSchemeNews
    UriSchemeNntp
    UriSchemeSftp
    UriSchemeSsh
    UriSchemeTelnet
    UriSchemeWs
    UriSchemeWss     //WebSocket Secure
    */

    /*
    AbsolutePath
    AbsoluteUri
    Authority
    DnsSafeHost
    Fragment
    Host
    HostNameType
    IdnHost
    isAbsoluteUri
    IsDefaultPort
    IsFile
    isLoopback
    isUNC
    LocalPath
    OriginalString
    PaathAndQuery
    Port
    Query
    Scheme
    Segments
    UserEscaped
    UserInfo
    */

    /*
    Canonicalize
    URIHostNameType CheckHostName()
    CheckSchemeName
    CheckSecurity
    Compare
    Equals
    Escape
    EscapeDataString
    EscapeString
    EscapeUriString
    FromHex
    GetComponents
    GetHashCode    
    GetLeftPart
    GetObjectData
    HexEscape
    HexUnescape
    IsBadFileSystemCharacter
    IsBaseOf
    IsExcludedCharacter
    IsHexDigit
    IsHexEncoding
    IsReservedCharacter
    IsWellFormedOriginalString
    IsWellFormaedUriString
    MakeRelative
    MakeRelativeUri
    Parse
    TryCreate
    TryUnescapeDataString
    Unescape
    UnescapeDataString
    ToString
    ==
    !=

    */

    //URIBuilder
    
    enum class URIFormat
    {
        UriEscaped	 = 1,   //Escaping is performed according to the rules in RFC 2396.
        Unescaped	 = 2,   //No escaping is performed.
        SafeUnescaped= 3,   //Characters that have a reserved meaning in the requested URI components remain escaped. All others are not escaped.
    };

    //URIFormatException
    //URIKind
    enum class URIKind
    {
        Indeterminate = 0,
        Absolute = 1,
        Relative = 2
    };
    //URIParser
    //URIPartial
    //URITypeConverter
    enum class URIHostNameType
    {
        Unknown = 0,
        Basic,
        Dns,
        IPV4,
        IPv6,
    };

    struct URICreationOptions
    {
        bool DangerousDisablePathAndQueryCanonicalization;
    };

    enum class URIComponents : int64_t
    {
        SerializationInfoString	= -2147483648,      //The complete Uri context that is needed for Uri Serializers. The context includes the IPv6 scope.
        Scheme	                = 1,                //The Scheme data.
        UserInfo	            = 2,                //The UserInfo data.
        Host	                = 4,                //The Host data.
        Port	                = 8,                //The Port data.
        SchemeAndServer	        = 13,               //The Scheme, Host, and Port data.
        Path	                = 16,               //The LocalPath data.
        Query	                = 32,               //The Query data.
        PathAndQuery	        = 48,               //The LocalPath and Query data. Also see PathAndQuery.
        HttpRequestUrl	        = 61,               //The Scheme, Host, Port, LocalPath, and Query data.
        Fragment	            = 64,               //The Fragment data.
        AbsoluteUri	            = 127,              //The Scheme, UserInfo, Host, Port, LocalPath, Query, and Fragment data.
        StrongPort	            = 128,              //The Port data. If no port data is in the Uri and a default port has been assigned to the Scheme, the default port is returned. If there is no default port, -1 is returned.
        HostAndPort	            = 132,              //The Host and Port data. If no port data is in the Uri and a default port has been assigned to the Scheme, the default port is returned. If there is no default port, -1 is returned.
        StrongAuthority	        = 134,              //The UserInfo, Host, and Port data. If no port data is in the Uri and a default port has been assigned to the Scheme, the default port is returned. If there is no default port, -1 is returned.
        NormalizedHost	        = 256,              //The normalized form of the Host.
        KeepDelimiter	        = 1073741824,       //Specifies that the delimiter should be included.
    };

    class qor_pp_module_interface(QOR_URI) URI
    {
    public:
        URI(std::string_view uri);
        //URI(StreamingContext); //i.e. from a DataSource
        URI(std::string_view, URICreationOptions );
        URI(std::string_view, URIKind);
        URI(URI&, std::string_view);
        URI(URI&, URI&);
    };

}}//qor::data
#endif//QOR_PP_H_COMPONENTS_PROTOCOLS_URI
