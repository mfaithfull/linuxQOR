# JSON Generator

The writing half of the JSON format, mirroring `parser/`. Generator states
(`qor::data::generator::json`) are `qor::components::generator::GeneratorState`
(`qor::workflow::State`) descendants that consume populated `model::json::*`
data and emit octets into the generator `Context` (`PutOctet()`) through a
`qor::components::generator::Generator` (`qor::workflow::Workflow`).

Composite states push `ASCIIChar` sub-states onto the workflow stack to emit
the characters of a JSON production; the output is later pumped through a
pipeline `GeneratorSource` to a sink.

Grammar per RFC 8259:

    value = string / number / true / false / null / object / array
