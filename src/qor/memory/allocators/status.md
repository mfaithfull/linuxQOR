Status: Stable
Ready for overall 0.5 interim release

The DebugAllocator Line and File meta data are currently useless
They need to be derived from the current FunctionContext if it's 
available. However we can't call new_ref< ICurrentThread >()
to get it or we risk an infinite loop back to the Allocator.
Care will be need and we might require the FunctionContext to
cache the information with qor_reporting from where it can
be retrieved. Any solution will be slow but tracking memory
allocation always is.
