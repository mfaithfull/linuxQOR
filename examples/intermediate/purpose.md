A folder of intermediate QOR examples.

server & client :-
    server runs the NetworkServer background service on port 12345 serving the Echo Protocol.    
    client does a single connect and nominal send and recieve flow using the NetworkClient.
    The Echo Protocol is simple, it just bounces what you send it but is implemented as a full model protocol
    in src/components/protocols/echo. Both requests and responses are 'parsed'.

    The NetworkServer is intended to be a production ready component. It leverages the Thread Pool, Async IO and Workflows to handle concurrent client connections and separate per connection processing. It's fully protocol agnostic and connection logging is provided.

    These examples establish a minimal interprocess socket connection and drive the Client and Server workflows through their paces.
    Now we're ready to write real client server applications.


