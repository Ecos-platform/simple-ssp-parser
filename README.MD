# simple-ssp-parser

A simple C++ library handling XML parsing of systems following 
the [System Structure and Parameterization](https://ssp-standard.org/) standard.

### Building

#### vcpkg (using manifest mode)
Call CMake with:

```
-DCMAKE_TOOLCHAIN_FILE=[path to vcpkg]/scripts/buildsystems/vcpkg.cmake
```

Then call CMake as usual.


### Usage
```cpp
#include <ssp/ssp.hpp>

// Zipped .ssp archive
const std::string sspArchive = "../folder/mySystem.ssp";
SystemStructureDescription desc(sspArchive);

// Non-zipped folder
const std::string sspFolder = "../someFolder";
SystemStructureDescription desc(sspFolder);
```
