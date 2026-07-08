src/framework

structural elements from which applications can be constructed
modules here should be general and usually singular. If an application has one of these without which it isn't that type of application it belongs here.
If an applications major control flow is built with these. Then they should be here.

/app - Application things like Features, Roles, Options, Workflows.
/data - Data processing Pipelines and external, file based, resources.
/event - Internal signals, events, delegates, observers...
/io - All things IO. File System, Network, Async
/parallel - Threads, Tasks, Thread Pools and Synchronisation
/ui - User Interface stuff will go here. It's an experimental dumping ground as of Jult 2026.
