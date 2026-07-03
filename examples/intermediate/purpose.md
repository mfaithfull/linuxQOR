A folder of intermediate QOR examples.

jsonread :-
    Takes a JSON file and pumps it through the QOR JSON Parser to produce an object model.
    Then writes it out again to std::cout

    The parser and pipepline infrastructure is plugable so the same approach can be used to read any well known format.
    
    The JSON parser uses the QOR Workflow infrastructure and encodes the ABNF for JSON as directly as possible.
    As such it's not optimised either for speed or space but in practice it's pretty fast and allows trading speed for
    memory usage depending on the size of buffer chunks you read at once. The larger the chunk the more stack space the
    parser will use. This can get very large, e.g. 28MB with 8K buffers which requires, OS specific, stack size adjustment.
    I'd recommend keeping the chunks to 2KB unless you have a need for more speed.

server & client :-
    server runs the NetworkServer background service on port 12345 serving the Echo Protocol.    
    client does a single connect and nominal send and recieve flow using the NetworkClient.
    The Echo Protocol is simple, it just bounces what you send it but is implemented as full model protocol
    in src/components/protocols/echo. Both requests and responses are 'parsed'.

    The NetworkServer is intended to be a production ready component. It leverages the Thread Pool, Async IO and Workflows to handle concurrent client connections and separate per connection processing. It's fully protocol agnostic and connection logging is provided.

    These examples establish a minimal interprocess socket connection and drive the Client and Server workflows through their paces.
    Now we're ready to write real client server applications.


