July 2026. Portable Modules are now versioned, identifying those that are ready for 0.5
HTTP is the next major element to be cleaned up and completed. Needs a couple of systematic refactors and generator
infrastructure.

========================================================================================================================

July 2026. A version 0.5 interim release plan has been put together along with a long list of things that need doing before and after.
Scrubbing the module export interfaces is under way. It's a pretty major piece of work and won't be complete soon. However the base libraries
are now clean and some clean examples can be built. Anything using pipeline components won't and that's the next target.
r/cpp says we need a GUI builder with a better than JSON human readable serialisation format to compete with Qt. They may be right.
However they also say a lot of other things that we've already done.

========================================================================================================================

June 2026. A lot of quality work on the fundamentals is completed. Builds with all the pedantry turned on are now possible. Many /qor libraries are now clean even with everything cranked up. Some like qor_mock probably never can be given what they do. Pipelines still need a lot of work. The basic RFC5234 parser framework is just about done. Edge cases around tiny buffer sizes and parse completion need bottoming out. Character encoding support is needed and a corresponding generator framework for writing text based files. Along with one for writing portable binary formats.
Basic UTF-8, UTF-16 and UCS4 String classes are done. Integration with qor_error is worked out although it requires double linking under GCC which is unfortunate and maybe resolvable. Otherwise we'll have to drop the error raising interface into another module.
With a lot of the /qor and framework libraries stabilized and in routine use and much of the work going on in components we may be in ver 0.5 territory.

========================================================================================================================

End of May 2026. A lot of Win32, especially GUI primitives, has been added. Some project level restructuring is under way to acommodate cross platform GUI. A lot of experimental code and tech examples have been done. The next stage will likely be consolidation and a push for even more quality of the fundamentals to support a larger project.
Multiple test executables will be needed as the test themselves will otherwise become to bulky and take too long.
We'll need to separate operational tests like the Thread Pool performance stuff from simple, loop unrolled, unit tests that can be rapidly iterated. Some CMake work required for multiple executables in a build.

========================================================================================================================

1st March 2026. Serialization over pipelines now occuring although not heavily supported. We have working if incomplete examples of serialization and deserialization with parsing and generating of the file content. Lots of tidying up and consolidation to be done. Font loading has been started and the techniques are now being applied to HTTP Requests and Responses.
========================================================================================================================

End of JAN 2026. Reached another inflection point. The QOR now probably qualifies as a large project given that I've lost count of the number of modules. The oprganiztional structure is holding however and neither dependencies or sizes have got out of control. This kind of proves it works at least at this scale. Nothing scales forever but the QOR structure is probably good for anything I can linearly write myself in a year or two.
The next logical step is a to get a real grip on serialization over pipelines. This opens up loading fonts and images and
a return the the web server to write out responses.

66 extant cross platform libraries according to cmake.
========================================================================================================================

The style of QOR code should now be clear, given the size of the codebase. The ubiquitous classes and 'utilities' of the QOR are all at least sketched out now.
If there's anything that fits the mould of code that exists to make other code easier to write then suggestions of extensions, specializations and improvements to the qor modules are welcome.

The direction of travel from here will be structural and control flow modules for the framework and some initial components for customizing memory management



========================================================================================================================
Just getting setup with the new linuxQOR repo and setting out the project.


Not really open to contributions at this stage because and only because there's no way for anyone else to comprehend the intent or what has/hasn't already been done.
However the QOR project is way too big for one person. As soon as the intent, style and overall direction is comprehensible to you, you're welcome to pitch in.

See my separate rant about why so much fundamental 'Open Source' isn't actually open, on request.
