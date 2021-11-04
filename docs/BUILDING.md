# Building Instructions

If you get errors while building, it's either because:
 
 1) You forgot to do the `iowin32.h` thing

 2) The repo is literally not in a buildable state lol

## For fresh dummies

 * Install [Git](https://git-scm.com/downloads), [Visual Studio](https://visualstudio.microsoft.com/downloads/) (just the Build Tools are enough) and [VS Code](https://code.visualstudio.com/).

 * Clone the repository by navigating to a directory using the command line and using `git clone https://github.com/HJfod/BetterGD --recursive`

    * If the submodules weren't cloned properly, do `git submodule update --init --recursive`

 * Open the downloaded repo with VS Code (right-click -> Open With Code)

 * Install the [CMake Tools](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools) extension

 * Configure project with CMake Tools (a popup should appear for it, or press F1 and type `>CMake: Configure`)

    * Make sure to select Visual Studio/MSVC, Release/RelWithDebInfo and x86/32-bit

 * Open `submodules/Zipper/minizip/iowin32.h` and add `#include <ioapi.h>` on Line 24 (make sure to save the file)

 * Click `Build` on the bottom status bar

 * If the build was succesful, you should see `Build finished with exit code 0`

## For experienced elites

 * `git pull`

 * `git submodule update --init --recursive`

 * `mkdir build`

 * `cd build`

 * `cmake .. -A Win32 -Thost=x86 --build`

## For the chaddest of chads

 * In `submodules/Zipper/minizip/iowin32.h` add `#include <ioapi.h>`

 * CMake + MSVC
