Q.	What does QOR stand for?
A.	Querysoft Open Runtime.

Q.	What is the QOR?
A.	A free, open source, C++, cross platform, advanced software framework.

Q.	What? I thought you already have a cross platfrom QOR repo. Why linuxQOR ?
A.	The old QOR repo is 8 years out of date and although I haven't exactly stayed focussed on it or checked anything in. That doesn't mean nothing was done.
	However the 'current' state of play in early 2025 is so far from what's in that old repo that there is no fixing it and no benefit to doing so. 
	Every file would be rewritten twice or deleted to bring git up to date. 
	The old repo will eventually die but it contains things that have been temporarily descoped from the project to make it more tractable so it stays for now.

	The QOR was always intended to be portable across platforms and architectures but in the past I always developed on Windows.
	This repo is intended to be a ground up Linux implementation of the QOR concept which will learn from the older code until that can be fully retired.
	Cross platform and architecture portability is still the goal but in order to do that you have to actually develop hour by hour on multiple platforms.
	There are however only so many hours so this development will be intentionally Linux first as the previous attempt was intentionally Windows first.

	As for Mac. I'll go there if and when I get one. 
	There are often very old Mac Books going for almost reasonable prices in the local CEX but so far I've not been inspired to buy one.
	The QOR long ago became a large complex project with long compile times and to develop at pace some pretty strong hardware is needed.

Q.	Is it done yet?
A.	No. See STATUS.md for the current status.

Q.	Why would I use the QOR?
A.	Because you want to write your application once and run it everywhere, forever, at full speed.
	You want to be able to read, understand and change if necessary all the source in your product.
	When a completely new hardware platform, Operating system or toolchain comes along you want to be there first with your existing code.

Q.	How do I use the QOR?
A.	1.	Read a little about the organization of the QOR and decide how you want to use.
	2.	Configure it.
	3.	Build it with your chosen Compiler for your chosen OS and hardware.
	4.	Write your application against the QOR headers e.g. QOR/include/
	5.	Link your application against the QOR runtime you built in step 3.

Q.	What makes the QOR special?
A.	The modular architecture and Aspect Oriented approach of the QOR mean it's easier to adapt to new platforms than other frameworks.
	The Aspect separation of the QOR means even the simplest applications can take advantage of advanced interception techniques and the integrated unit test framework with minimal effort.
	The QOR uses advanced C++ techniques so that you don't have to. It allows you to do as much or as little memory and object lifetime management as you want to.
	The QOR is designed to be extended. It is modular even at the sub-Object level so you can replace just the parts you want without re-writing or duplicating thousands of lines of code.
