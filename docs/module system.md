As part of self aware software each executable needs to know what libraries are loaded into it and each loadable library or executable needs to know what static libraries linked into it.
To achieve this each QOR static library must expose a singleton Library object which registered itself with the containing module on construction
Each Module must expose a singleton Module object which registers libraries and can iterate over them and in turn registers itself with the executable module on construction
Each executable module must expose a singleton Host object which registers loaded Modules and can iterate over them
This top level Host object will also act as a type registry for dependency injection down the road
