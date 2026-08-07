The src/configuration folder contains the primary qor header from which we'll generate a PCH and which must be the first include in all translation units.

#include "src/configuration/configuration.h"

This folder will also contain any alternative/optional configuration headers that might be required. 
