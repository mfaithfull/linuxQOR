 - Q.	What does QOR stand for?
 - A.	Querysoft Open Runtime.
---
 - Q.	What is the QOR?
 - A.	A free, open source, C++, cross platform, advanced software framework.
---
 - Q.	Is it done yet?
 - A.	No. See [STATUS.md](https://github.com/mfaithfull/linuxQOR/blob/main/STATUS.md) for the current status.
---
 - Q.	Why would I use the QOR?
 - A.	Because you want to write your application once and run it everywhere, forever, at full speed. You want to be able to read, understand and change if necessary all the source in your product. When a completely new hardware platform, Operating system or toolchain comes along you want to be there first with your existing code.
---
 - Q.	How do I use the QOR?
 - A.
 	1.	Read a little about the organization of the QOR and decide how you want to use.
	2.	Configure it.
	3.	Build it with your chosen Compiler for your chosen OS and hardware.
	4.	Write your application against the QOR SDK headers e.g. sdk/app.h
	5.	Link your application against the QOR runtime you built in step 3.
---
 - Q.	What makes the QOR special?
 - A.	The modular architecture and Aspect Oriented approach of the QOR mean it's easier to adapt to new platforms than other frameworks. The Aspect separation of the QOR means even the simplest applications can take advantage of advanced interception techniques and the integrated unit test framework with minimal effort. The QOR uses advanced C++ techniques so that you don't have to. It allows you to do as much or as little memory and object lifetime management as you want to. The QOR is designed to be extended. It is modular so you can replace just the parts you want without re-writing or duplicating thousands of lines of code.
---
 - Q.	Do I have to use your application framework to take advantage of the other libraries?
 - A.	No. It's perfectly possible to use most of the QOR without using a qor::app::Application or Roles or Features. 
	You will need to link 	qor_module into your executable and provide a ThisModule implementation so that everything you do use get's initialised but apart from that you only need the actual dependencies of the module or modules you want to use. You can use the QOR for IO, Parallelism, State Machines or just for the smart pointers and memory management if that's what you need.
---

Get your question added here (Yes you Captain Obvious as well!). Ask anything. There are no bad questions only inadequate answers.

