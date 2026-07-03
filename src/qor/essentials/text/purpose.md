Provides String classes representing UTF-8, UTF-16, UCS4/UTF-32 and Code Page specifc encodings
The qor::String class is the UTF8String class because UTF-8 is the closest thing to a universal standard
Move UTF-8 strings around internally in std::string. It's find for holding the bytes and 
universally compatible. If you need to do Character by Character operations then convert to
a qor::String (UTF8String) to get string operations that respect multibyte UTF-8 
For display we'll turn the UTF-8 into UCS4/UTF-32 to get Code Points for looking up glyphs
All String operations are implemented once, on AbstractString, in terms of iterators.
This is not perfromance optimal but it is easier to extend.
