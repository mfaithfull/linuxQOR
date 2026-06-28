Provides a general parser state machine.
The Parser class is derived from qor::Workflow but doesn't use the standard Run function. 
It adds Parse and FinalParse instead.
A basic set of parser tokens states and productions are provided matching RFC5234 
https://datatracker.ietf.org/doc/html/rfc5234
These can be used to build a lot of simple parsers but don't have to be used at all.

The Parser is designed to be attached to a qor::Pipeline via the Context class and to be used with a
ParserSink to Sink data from any pipeline into a Parser. The Context class is general enough however that 
you can attached it to any kind of data you like to provide data to the parser.

What can you parse with derivatives of this parser class?
In principle anything because the matching logic and in particular the per Node-type Emit function can
be as complex as you like. However I'm not claiming this will handle C++ most vexing parse for example
or other complex or ambiguous cases. Even if it did it would likely be catastrophically inefficient.
So what is this useful for?
There are a lot of relatively simple languages that this design will handle well if not
optimally. JSON, BSON, HTTP, SQL, HTML (might be a bit strict), XML, SMTP, many Kafka message formats and more.

The main advantage is that every grammar production gets it's own class, closely matching the ABNF and hopefully
comprehensible in it's intention.

Here is the Production/State delcaration for a top level  JSON Object

    //object = begin-object [ member *( value-separator member ) ] end-object

    class qor_pp_module_interface(QOR_JSON) object : public Sequence
    {
    public: 
        
        object(Parser* parser) :
            Sequence(parser,
                new_ref< begin_object >(parser),
                new_ref< Sequence >(parser,
                    new_ref< Optional >(parser,
                        new_ref< Sequence >(parser,
                            new_ref< member >(parser),
                            new_ref< ZeroOrMore >(parser,
                                new_ref< Sequence >(parser,
                                    new_ref< value_separator >(parser),
                                    new_ref< member >(parser)
                                )
                            )
                        )
                    ),
                    new_ref< end_object >(parser)
                ),
            static_cast< uint64_t >(jsonToken::object)){ }

        virtual ~object() = default;

        virtual void Prepare();
        virtual void Emit();
        virtual void Fail();
    };


Here's how to get from the ABNF to the declaration 

object = _"begin-object [ member *( value-separator member ) ] end-object"_

//Always break things into Sequences when possible

object = Sequence< begin-object, _"[ member *( value-separator member ) ] end-object"_ >

//[] from ABNF becomes Optional

object = Sequence< begin-object, Sequence< Optional< _"member *( value-separator member )"_ >, end-object > >

//*() from ABNF becomes ZeroOrMore

object = Sequence< begin-object, Sequence< Optional< Sequence< _"member"_, ZeroOrMore< _"value-separator member"_ > > >, end-object > >
...

The lower level productions like begin-object and member will of course needs their own states to be broken down to the level of
specific byte sequences.

For example:

    //begin-object    = ws %x7B ws  ; { left curly bracket

    class qor_pp_module_interface(QOR_JSON) begin_object : public Sequence
    {
        public: begin_object(Parser* parser) :
            Sequence(parser,
                new_ref<ws>(parser),
                new_ref<Sequence>(parser,
                    new_ref<Specific>(parser, (byte)0x7B),
                    new_ref<ws>(parser)
                ),
            static_cast<uint64_t>(jsonToken::begin_object)){ }
        virtual ~begin_object() = default;
        virtual void Emit(){ };//Emit nothing
    };

At the heart of this production, which allows leading and trailing white space (ws), we have a Specific
parameterised with (byte)0x7B.

This will match with only and exactly that byte, the { left curly bracket symbol. Removing it from the 
stream and producing a begin-object. Only Specific or Range matches with exact values like this move the
parser forward through the input bytes (lexing). Everything else combines symbols already read into higher level 
constructs (parsing).

Writing parsers is hard. However it's a lot easier with a framework that lets you turn your grammar productions
directly into a few lines of code. If and when you need optimal performance you can hand code something faster.

For reference JSON has about 30 productions, YAML 1.2.2 has 211, Powershell > 300, C++ maybe thousands...
