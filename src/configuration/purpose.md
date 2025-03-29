The src/configuration folder contains the primary qor header from which we'll generate a PCH and which must be the first include in all translation units.

#include "src/configuration/configuration.h"

This folder will also contain any alternative/optional configuration headers that might be required. For example we might have a max_performance header that turns off options for all debugging info, compile time reporting, even exceptions. Another max_debugging header which turns everything on including excessive logging and compile time reporting.

