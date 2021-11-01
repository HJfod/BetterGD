# Building

Building may require the following propietary modifications:

 * In `submodules/Zipper/minizip/iowin32.h` add `#include <ioapi.h>`

After that, it should be just building the CMake project and then building the solution.
