Yes please!

The QOR project is open for contributions. This is way more than one crazy person can handle.

How to get started?
    Checkout the code
    Build some of the example projects (See BUILDING.md)
    Read the code (At least until it starts to make sense)
    Ask questions (Especially if the code isn't making sense)
    Decide what you'd like to contribute.
    Pitch in with suggestions, reviews, extensions, fixes...


What can you I work on?

A simple stsrt that literally anyone can do might be to add a new CodePage to src/qor/essentials/text/codepage/codepages
There are 250 or more code page definitions available online - https://www.iana.org/assignments/character-sets/character-sets.xhtml
It would be nice to support more of them.
The existing CodePage classes provide the template for how to add another one.
You'll want to add a new String type specific to your CodePage and tests at least for converting to and from that type to UTF-8
The conversion itself is made available by implementing the CodePage and the declaring the derived String type, you don't have to write it separately.

Adding tests, unit or operational for anything is always welcome. Doesn't matter how simple they are. 
If they test something that matters then they matter.

If you know anything about Unicode we could do with a tool that turns the Unicode Database into a usable set of datastructures.
Code for normalisation would be nice. Fancy implementing the Bidi algorithm?

If all that's too easy you could take on Text Shaping. I've started pulling apart the prickly hedge that is HarfBuzz to extract the OpenType Shaper. That's a real challenge.

More interested in performance? How about creating a harness to drag race the FastHeap custom memory allocator against standard new and delete in a variety of scenarios.

Something bigger? Could you take on adding Mac OS support? Start with clang-llvm Compiler support and work up from there. I'd do the FileSystem next as it's going to be very similar to Linux. It would probably take a week though.

All that backend stuff not for you? How about designing a Widget set for rendering in OpenGL or Vulkan/MoltenVK. The sky is the limit. You can have anything you can imagine as long as you can describe it completely enough. 

A YAML Parser? Yes we definitely need one of those. Preferably based on the same base as the existing JSON parser. Can you upgrade that to handle UTF-8 first though please. YAML has 211 productions, It's going to take a month or more.

A Database engine? Absolutely, the more the better.

Pipeline components for Kafka? 100%

Something completely different I haven't thought of? Probably yes. Let me know what you're planning.