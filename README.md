# IP labs setup for Win with MSYS2 + Clang

## PREFACE
This is a starter project for the [Image Processing](https://cv.utcluj.ro/index.php/teaching.html) laboratories at [UTCN](https://www.utcluj.ro/en/).  
This document will present a sample idea in order to get set up on [VS Code](https://code.visualstudio.com/) for C++ development with the [OpenCV](https://opencv.org/) framework using [MSYS2](https://www.msys2.org/) + [UCRT](https://github.com/MicrosoftDocs/cpp-docs/blob/main/docs/windows/universal-crt-deployment.md) with the LLVM [Clang](https://clang.llvm.org/) compiler / [LLDB](https://lldb.llvm.org/) debugger, [Ninja](https://ninja-build.org/) build system and [CMake](https://cmake.org/)

## INSTALLING MSYS2 AND REQUIRED PACKAGES
Download and install MSYS2 (https://www.msys2.org/#installation) which will give you the capability to install [ArchLinux](https://archlinux.org/) based packages under Windows.   

Once it has been installed, start a **UCRT64** based shell

![1_msys2_ucrt64.png](assets%2FREADME%2F1_msys2_ucrt64.png)

Inside the shell, install the necessary packages for development.  
These can be found at https://packages.msys2.org/package/

Required MSYS2 Packages
```shell
 pacman -S base-devel mingw-w64-ucrt-x86_64-clang mingw-w64-ucrt-x86_64-lldb mingw-w64-ucrt-x86_64-ninja mingw-w64-ucrt-x86_64-cmake mingw-w64-ucrt-x86_64-opencv mingw-w64-ucrt-x86_64-qt6-5compat
```
![2_pacman.png](assets%2FREADME%2F2_pacman.png)

Once the packages have been successfully installed, then close the shell.

## Setting up Windows environment variables

Next, you will need to add `bin` and the `lib` directories MSYS2's UCRT location where the packages were installed to the `Path` environment variable  

First open _Edit environment variables for your account_ from the Start Menu

![3_start_menu.png](assets%2FREADME%2F3_start_menu.png)

then search for the `Path` variable and click on _Edit..._

![4_edit_env.png](assets%2FREADME%2F4_edit_env.png)

Now you will need to add two new entries, one for the `bin`, and one for the `lib` directory  
Make sure to use the path where you installed MSYS2  
So, if it was installed under the default `C:\msys2` directory, then you would add the following

![5_add_to_path.png](assets%2FREADME%2F5_add_to_path.png)

Once completed, click OK ... OK ... OK to close the dialogs and the settings pane

## Install VS Code extensions

Next you will need to install an extension that will help you with some of underlying functionalities and setup  
The [C/C++ Extension Pack](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools-extension-pack) is highly recommended for simplicity  

![6_code_cpp_extension_pack.png](assets%2FREADME%2F6_code_cpp_extension_pack.png)

