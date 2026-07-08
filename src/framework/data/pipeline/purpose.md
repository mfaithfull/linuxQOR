A data pipeline framework for io and data processing.
Uses Sink and Source concepts
A pipeline is a chain of elements from source to sink.


client <-- sink <-- buffer <-- source <-- connection to device/file/socket/etc
pipeline    ^--------------------^

Data always flows from source to sink

In Pull mode calls flow from sink to source, i.e. the pipeline pumps by calling the sink which call the source to read data as required
In Push mode calls flow from source to sink, i.e. the pipeline pumps by calling the source which calls the source to write data as it's available

specialised IO Sinks and Source are available along with filters which act as both sink and source and inline filters that acts as buffers


sink <-- (inline filter with 2 buffers and a processing function ) <-- source

both that sink and the source see the inline filter as a simple buffer 


Buffers work in units (fixed number of bytes) and support a reader and a writer.
Reads and Writes have to be requested to reserve space and acknowledged or rejected to update the buffer read and write heads.
Buffers will only provide available (already read completed) space for writes and only provide available (already wite completed) data for reads
Buffers a semi circular, i.e. they behave as circular but you can't write across the end as clients do the actual writing so only continguously available chunks wil be provided for reading/writing.

[--------------4K buffer-----------------]

1K write request, updates the write point

[-------W------4K buffer-----------------]

Client recieves this buffer
[-------]

Client writes data

[ddddddd]

Client acknowledges write. Write point will move back if client didn't use all the requested space. Will move back all the way on write rejection

[dddddddW------4K buffer-----------------]

Client requests read

[RddddddW------4K buffer-----------------]

Client recieves this buffer

[ddddddd]

Client acknowledges read

[-------R|W------4K buffer---------------]

Buffer is now nominally empty as all data has been read. 
Available Write space is 3K
Available Read space is 0 bytes

This scheme enables multiple writes before a valid read so for example the buffer can be sized for the largest valid token if being used for parsing. It's also safe to have separate reading and writing threads as they won't tread on one another as long as there's one of each.
If we need multiple reader threads we'll have to create a multiplexing filter with subscribed sink buffers.