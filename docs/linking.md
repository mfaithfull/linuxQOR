There are a lot of QOR libraries. Which ones do I need to link to?

To build a minimum executable that uses a qor::Application you must link to:

qor_module(static)      Your executable must qualify as a QOR module in order to use the QOR
qor_host                Your executable needs this to host all the other QOR modules
qor_compiler(static)    Required to adapt to the differences between what compilers build from your code. e.g. binary vtable layout for mocking
qor_platform            Required to adapt to the target operating system
qor_application         Required for the qor::Application and qor::AppBuilder clases

See the outline example examples/basic/outline for a minimal QOR executable project

Linking to these libraries will also require these other modules from the QOR to be available at runtime

qor_objectcontext
qor_injection
qor_datastructures
qor_thread
qor_issue
qor_error
qor_sync
qor_role

Beyond this the answer is you have to link to what you use. 
When you use something declared in a QOR header you need to link to the QOR library that implements it.
If this is too difficult to keep track of and you're using almost everything anyway then linking to all
the QOR libraries, the QOR Framework libraries, and the QOR Platform libraries for your platform is safe,
although not super efficient.
If you use optional components from the qor::components namespace you must
link these individually as there are less gaurentees here. It may not be possible, and is not advisable to try,
to link all the QOR commponents as you won't need them all and they may clash due to providing alternative
implementations of the same declarations.

