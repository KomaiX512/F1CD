# F1CarDesigner

This project is a Formula One Car Designer application built in C++ using GLFW, OpenGL, and ImGui. It provides a graphical user interface to design, compare, and save car configurations.

## Overview

This guide provides detailed instructions to set up the development environment and build the project on a Windows machine. The instructions are designed for users with minimal computer science background.

---

## Prerequisites

1. **Visual Studio 2019 or later** (Community Edition is fine)
   - During installation, select the **Desktop development with C++** workload.
   - This provides the MSVC compiler and Windows SDK.

2. **CMake 3.10 or later**
   - Download and install from [https://cmake.org/download/](https://cmake.org/download/)
   - Make sure to add CMake to your system PATH during installation.

3. **Git**
   - Download and install from [https://git-scm.com/download/win](https://git-scm.com/download/win)
   - Used to clone the repository.

4. **vcpkg** (C++ package manager)
   - Used to install GLFW3 dependency easily.
   - The setup script will automate this, or you can install manually (see below).

---

## Step 1: Clone the Repository

Open **PowerShell** or **Command Prompt** and run:

```powershell
git clone https://github.com/KomaiX512/F1CarDesigner.git
cd F1CarDesigner
```

---

## Step 2: Install Dependencies Using vcpkg

You can either run the provided setup script `setup.ps1` (recommended) or follow these manual steps:

### Manual vcpkg installation

```powershell
# Clone vcpkg
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg

# Bootstrap vcpkg
.\bootstrap-vcpkg.bat

# Install GLFW3
.\vcpkg.exe install glfw3:x64-windows

# Integrate vcpkg with Visual Studio
.\vcpkg.exe integrate install
```

---

## Step 3: Build the Project

Back in the `F1CarDesigner` directory, create a build directory and run CMake:

```powershell
mkdir build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=../vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build . --config Release
```

This will build the `F1CarDesigner.exe` executable.

---

## Step 4: Run the Application

From the `build` directory, run:

```powershell
.\Release\F1CarDesigner.exe
```

---

## Troubleshooting

- Ensure Visual Studio C++ tools are installed.
- Make sure CMake is in your PATH.
- If GLFW3 is not found, verify vcpkg installation and integration.
- The `fonts` directory must be present alongside the executable for proper font rendering.
- If you encounter permission issues, try running PowerShell as Administrator.

---

## Optional: Automated Setup Script

You can run the provided `setup.ps1` PowerShell script to automate dependency installation and build steps.

---

## Contact

For further assistance, please contact the project maintainer.
