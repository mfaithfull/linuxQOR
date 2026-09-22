
#include "src/configuration/configuration.h"
#include "uri.h"

namespace qor{ namespace data{

    URI::URI(std::string_view uri)
    {

    }

    //URI::URI(StreamingContext); //i.e. from a DataSource
    URI::URI(std::string_view, URICreationOptions )
    {

    }
        
    URI::URI(std::string_view, URIKind)
    {

    }

    URI::URI(URI&, std::string_view)
    {

    }

    URI::URI(URI&, URI&)
    {
        
    }

}}//qor::data