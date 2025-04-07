qor_objectcontext intends to provide a generic object wrapper which can be used to refer to any object
The concept and pattern of implementation is similar to Boost:Any or Henney's Any as it is known.
The TypedAny class however is not intended to be used as an Any in client code, it is purely to facilitate the AOP feature of the QOR
If you need a modern Any, Boost.Any 
