qor_reflection intends to provide a refelction mechanism sufficient to iterate the members of an aggregate in a type safe manner providing their names, types and sizes.
Built in or at least more standard reflection is expected in C++26 and in compilers within the next couple of years. It's likely we'll switch to that once it becomes generally supported.
Meanwhile Antony Polukhin's excellent PFR library will be a good fit and should give us everything we need without being overcomplicated to use.
