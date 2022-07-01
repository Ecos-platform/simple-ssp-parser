# simple-ssp-parser

A simple C++ library handling XML parsing of systems following 
the [System Structure and Parameterization](https://ssp-standard.org/) standard.

### Usage
```cpp
// Zipped .ssp archive
const std::string sspArchive = "../folder/mySystem.ssp";
SystemStructureDescription desc = parse_ssp(sspArchive);

// Non-zipped folder
const std::string sspArchive = "../someFolder";
SystemStructureDescription desc = parse_ssp(sspArchive);
```
