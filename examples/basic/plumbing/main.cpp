// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

//An application with a Pipeline using the QOR SDK

//For a simpler example of the flow of a QOR application see the roller example

#include "sdk/using_framework.h"
#include "sdk/using_platform.h"
#include "sdk/components/framework.h"

constexpr const char* appName = "Plumbing";
qor_pp_implement_module(appName)

qor_pp_module_requires(ICurrentThread);
qor_pp_module_requires(IFileSystem);

void TraditionalFileRead(FileSystem::ref filesystem, filesystem::Index& index);
void PipelineFileProcessor(FileSystem::ref filesystem, filesystem::Index& index);

int main()
{
    /*To use facilities from the operating system, first we add them
    to the Platform object. This works just like adding features
    to the Application Role*/
    ThePlatform(qor_shared)->AddSubsystem<FileSystem>();

    return AppBuilder().Build(appName)->SetRole<Role>().Run(
        []()->int
        {
            /*Retrieve the file system singleton from the platform.
            The platform is a shared object so synchronisation is required*/
            auto fileSystem = ThePlatform()(qor_shared).GetSubsystem<FileSystem>();
            
            /*All the functions on the fileSystem object are const so 
            once we have it, it's safe to use without synchronisation*/

            /*A filesystem::Index is how we refer to a specific file without opening it.
            Internally it's a directory entry, which is just the index of a file*/
            io::filesystem::Index index(fileSystem->CurrentPath(), "alice.txt");

            if(!index.Exists())
            {
                std::cout << "Please place a file called alice.txt in the folder " << index.GetPath().ToString() << " to make this example work. The file contents will be encoded into output.txt" << std::endl;
                return EXIT_FAILURE;
            }

            /*If we want to do simple step-by-step processing
            we can use the traditional way of dealing with files.*/
            TraditionalFileRead(fileSystem, index);

            /*However what if we want to transcode the whole file
            to base64 and write it out to another file in one step?*/
            PipelineFileProcessor(fileSystem, index);
        
            return EXIT_SUCCESS;
        });
}

void TraditionalFileRead(FileSystem::ref fileSystem, filesystem::Index& index)
{
    auto file = fileSystem->Open(index, OpenFor::ReadOnly);         //Obtain a file object from the file system    
    FileReader reader(file);                                        //Initialise a reader for the file    
    std::string line = reader.ReadLine();                           //Read a line from the file    
    std::cout << line << std::endl;                                 //process the data

    //RAII ensures the file is closed here as the file object is destroyed

    //This could of course be compressed to the single statement:
    //std::cout << (FileReader(index, OpenFor::ReadOnly).ReadLine()) << std::endl;
}

void PipelineFileProcessor(FileSystem::ref fileSystem, io::filesystem::Index& input)
{    
    io::filesystem::Index output(fileSystem->CurrentPath(), "output.txt");      //An output file to go with the input file

    if(output.Exists())                                             //Clean up any old output
    {
        fileSystem->Delete(output);
    }

    Base64EncodeFilter encode(4096);                                //Set up a base 64 encoder with 4K of buffer space

    /*We use File connectors to connect to the input and output files. These encapsulate everything file related for 
    the pipeline. If we used Socket connectors or Pipe connectors or DBus connectors, the rest of the pipeline would
     be identical and oblivious*/
    FileConnector inputConnector(input, WithFlags::None, ShareMode::Owner_Read, OpenFor::ReadOnly);
    FileConnector outputConnector(output, WithFlags::CreateNew, ShareMode::Owner_Read, OpenFor::ReadWrite);

    //Create a pipeline with the connectors at each end
    Pipeline fileProcessor(inputConnector, outputConnector, Element::Push);
    fileProcessor.InsertInlineFilter(encode);   //Insert the base 64 encoder as a filter
    fileProcessor.Connect();    //Connect to the sink and the source. This opens the files at both ends
    fileProcessor.PumpAll();    //Pump all the available data. This will read, encode and write all the data in the inputfile

    //Everything we've created here is on the stack so it's all cleaned up and the files closed automatically.

    /*This could also be done as a single statement
    Pipeline(
        FileConnector(input,WithFlags::None,ShareMode::Owner_Read,OpenFor::ReadOnly),
        FileConnector(output,WithFlags::CreateNew,ShareMode::Owner_Write,OpenFor::ReadWrite),
        Element::Push).
        InsertInlineFilter(Base64EncodeFilter(8192)).
            Connect().
            PumpAll();
    */
}
