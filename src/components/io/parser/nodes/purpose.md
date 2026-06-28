Parser node classes form the Abstract Syntax Tree. The initial output of a parse.
A node is emitted when the production for that node type matches 
e.g. HEXDIGIT = DIGIT | "A" | "B" | "C" | "D" | "E" | "F"
If we just got a lexical token of 'F' then we can Emit a HEXDIGIT Node

When a node is emitted it may consume existing nodes from the nodes stack to build a model

For example: If this is Node stack when we Emit a Type B -

Node stack:

node type A
node type A
node type B
node type X
...

Emitting a B attempts to build a B-Model

pop an A from the stack and model it.
B-Model, with 1 A-Model
pop an A from the stack and model it.
B-Model, with 2 A-Models

pop a B from the stack. 
Matches what we're emitting so we know we're done

Attach the model to the B
B Node with a B-Model containing 2 A-Models

push the B Node back onto the stack

node type B with B-Model attached
node type X
...

This process will repeat for X and other Nodes down the stack until we have just 1 node with a Model attached.
If the parse succeeded that Node will represent the terminal token we're parsing for e.g. A Document or top 
level Object. The attached Model will be the pure object model, DOM or equivalent. The parsed thing without
any parser specific data attached. We can take it and use it in the calling code.

You might ask how does the B Type Node get onto the Node stack in the first place?
That's done by the Prepare function of the associated Parser State
State::Prepare speculatively pushes a B Node onto the node stack if the Production 
being matched can produce a B.
If Emit is never called because the Production didn't match then State::Fail is
called which just pops the B Node off the stack and throws it away.
Many productions may be tried before a matching one is found.
