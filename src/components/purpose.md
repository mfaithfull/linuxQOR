This folder is the top level for components.

Components are QOR modules.
Components are optional for QOR projects, the main QOR classes don't depend on them. You can use them if you need them or ignore them if you don't.
Many components will be specializations of QOR classes such as Workflows or Pipelines
Components can be dependent on any main QOR modules and on other components
Components folder should be arranged such that modules are under a hierarchy similar to where they would be if they were main QOR modules

Components are outside the main QOR and as such things a little more wild west here.
Put experimental stuff into 
qor::exp::
By the time it reaches v 1.0.0 it should migrate or more likely but copied to 
qor:components
qor::io::components
qor::io::network::components
qor::pipeline::components
qor::data::components
etc.
or into the main qor if it's a fundamental module

/app - Put Roles here along with any derivatives of App, OptionGetter, or the basic Workflow
/data - Resource and Data Pipeline components. Currently holds experimental TTF reader for example
/io - IO components. Write me a qor_pipeconnector that does Windows Named Pipes and Unix Sockets. Put it here. Thanks!
/protocols - Echo, HTTP, SMTP, SNMP, ObEx for bluetooth... Feel free to add these and others.
/qor - Components extending primary QOR functionality. The Log Aggregator lives here ATM.
/ui - UI Components - As of July 2026 this is a dumping ground for experimental bits.
