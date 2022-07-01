# simple-ssp-parser

A simple C++ library handling XML parsing of systems following 
the [System Structure and Parameterization](https://ssp-standard.org/) standard.

### Building

simple-ssp-parser uses [conan](https://conan.io/) under-the-hood for managing dependencies:

```
pip install conan
```

Then call CMake as usual.

### Installing

`simple-ssp-parser` is available through the conan remote ais:

`conan remote add ais https://ais.jfrog.io/artifactory/api/conan/ais-conan-local`

Then add a dependency to: </br>
`simple-ssp-parser/<version>@ais/stable` (stable channel -> releases) </br>
`simple-ssp-parser/<version>@ais/testing` (development builds -> master) </br>
`simple-ssp-parser/<version>@ais/testing-<branch>` (development builds -> branches)



### Usage
```cpp
#include <ssp/ssp_parser.hpp>

// Zipped .ssp archive
const std::string sspArchive = "../folder/mySystem.ssp";
SystemStructureDescription desc = parse_ssp(sspArchive);

// Non-zipped folder
const std::string sspFolder = "../someFolder";
SystemStructureDescription desc = parse_ssp(sspFolder);
```
