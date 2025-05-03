# PowerShell setup script for Windows environment setup and build

# Check if vcpkg directory exists
if (-Not (Test-Path -Path ".\vcpkg")) {
    Write-Host "Cloning vcpkg..."
    git clone https://github.com/microsoft/vcpkg.git
    if ($LASTEXITCODE -ne 0) {
        Write-Error "Failed to clone vcpkg. Please check your internet connection and git installation."
        exit 1
    }
}

# Bootstrap vcpkg
Write-Host "Bootstrapping vcpkg..."
cd vcpkg
.\bootstrap-vcpkg.bat
if ($LASTEXITCODE -ne 0) {
    Write-Error "Failed to bootstrap vcpkg."
    exit 1
}

# Install GLFW3
Write-Host "Installing GLFW3 via vcpkg..."
.\vcpkg.exe install glfw3:x64-windows
if ($LASTEXITCODE -ne 0) {
    Write-Error "Failed to install GLFW3."
    exit 1
}

# Integrate vcpkg with Visual Studio
Write-Host "Integrating vcpkg with Visual Studio..."
.\vcpkg.exe integrate install
if ($LASTEXITCODE -ne 0) {
    Write-Error "Failed to integrate vcpkg."
    exit 1
}

cd ..

# Create build directory if it doesn't exist
if (-Not (Test-Path -Path ".\build")) {
    New-Item -ItemType Directory -Path ".\build"
}

cd build

# Run CMake configuration
Write-Host "Configuring project with CMake..."
cmake .. -DCMAKE_TOOLCHAIN_FILE=../vcpkg/scripts/buildsystems/vcpkg.cmake
if ($LASTEXITCODE -ne 0) {
    Write-Error "CMake configuration failed."
    exit 1
}

# Build the project
Write-Host "Building the project..."
cmake --build . --config Release
if ($LASTEXITCODE -ne 0) {
    Write-Error "Build failed."
    exit 1
}

Write-Host "Build completed successfully."
Write-Host "To run the application, execute:"
Write-Host "  .\Release\F1CarDesigner.exe"
