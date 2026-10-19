#include "src/configuration/configuration.h"

namespace qor { namespace protocols {

    // On MSVC a shared QOR module must export at least one symbol
    // (qor_pp_export == __declspec(dllexport)) so the compiler emits the
    // <target>.lib import library that a test executable can link against.
    // This placeholder is the future YAML module public API entry point.
    qor_pp_export void init_yaml(void)
    {
        // placeholder for future YAML protocol initialisation
    }

}}//qor::protocols
