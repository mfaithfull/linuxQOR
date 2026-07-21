The QOR uses the CMake build system.
I'd recommend installing the latest CMake version for your platform, especially if you're considering porting to WASM. (Please do by the way)
You'll need at least 3.10.0 to build the QOR. 4+ is recommended.

I develop in vscode on Windows and Ubuntu so those environments are known to work. I'd start there first.
Visual Studio 2022 also works and your IDE of choice will too but may need a set of project files creating to support it.

For vscode you'll need the C/C++, C/C++ DevTools, C/C++ Extension Pack, C/C++ Themes, CMake and maybe the PowerShell extensions. All from Microsoft

Currently supported compilers are The Visual Studio 2022 CL compiler (You'll need Visual Studio 2022 Community installed)
and GCC 13.1.0, 13.3.0 (Default on Ubuntu 24.04 I believe)

On Windows open a Windows Terminal and from there open a VS 2022 Developer Command Prompt in Administrator Mode
cd to where you put the top level QOR files (like this one):
code .

On Ubuntu just cd to where you put the top level QOR files:
code .&
(If your Ubunutu is WSL you don't need to install VSCode, it will use the one from the Windows host OS)

If your Compiler isn't exactly one of those supported you'll need to add support for it under 
src/platform/compiler/
It's pretty easy to do for another version of GCC or MSVC/CL as they're going to be almost the same as an existing version.

On Windows you'll need to get hold of EGL (OpenGLES) libraries - e.g. The PowerVR SDK
On Ubuntu you'll need to install liburing and get dev libraries for X & Wayland if you want to build everything.

These are the setup steps from the GithHub workflow which builds the QOR on a 'clean' Ubunutu which clearly has the build-essentials et al as it's intended as a build machine.

    mkdir liburing
    pushd liburing
    git clone https://github.com/axboe/liburing.git
    pushd liburing
    ./configure
    make
    sudo make install
    popd
    popd
    sudo apt-get update
    sudo apt-get install -y libegl1-mesa-dev libgles2-mesa-dev libx11-dev libxext-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev libxkbcommon-dev libwayland-dev libwayland-egl-backend-dev wayland-protocols

Once you have your Compiler/Toolchain/IDE/CMake in place try building the 'basic' executable
If that's fully or partially successful then to get an idea of how much is working build 'QORTest'
If that builds and runs you're good to go (There may be one or two test failures but if there are more feel free to track down the causes)

Build times should be < 30 mins for everything on a reasonable PC (Github builds ~26mins at time of writing). 
A from scratch of the HTTPServer (incomplete) example takes ~38s on my unreasonable PC.
