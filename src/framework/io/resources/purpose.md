An asynchronous general resource manager.
Applications require all kinds of external file based resources. They cannot gaurentee that these will be available and they are often external so that they can be user or environment modified. e.g configuration files.
Writing custom per resource code is tiresome and error prone as cases such as missing/inaccessible files, corrupted/unparsable files or modified/per-platform directory structures are often missed or covered only by duplicating a lot a code.
We centralise the aqusition of external resources and shunt it to a background task which works through the process of resource discovery,
unique identification, classification and parsing to validated in memory objects where possible.
This is equally applicable for game assets, fonts for a desktop application, images for a GUI, or json configuration files for a service.

Use by adding the ResourceHub Feature to your Application's Role
Then add Paths or specific FileIndexes to the ResourceHub or use a Claimer<> to synchronously acquire a resource based object.

See the resourced intermediate example for details.

