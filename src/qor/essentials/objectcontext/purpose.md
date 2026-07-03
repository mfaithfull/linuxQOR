qor_objectcontext is a header only library intended to provide generic object wrappers which can be used to refer to any object
The concept and pattern of implementation is similar to Boost:Any or Henney's Any.
The TypedAny class however is not intended to be used as an Any in client code, it is purely to facilitate the AOP features of the QOR
If you need a modern Any use Boost.Any. These classes are moderately unsafe. i.e. Safe as long as used as intended.
