C++ Rail Compiler
==================================================

### Project Goals
1. TODO
2. TODO
3. TODO


### Hey! What's Rail?
[Esolang-Wiki](http://esolangs.org/wiki/Rail): Esoteric 2d computer language


### Who are we
TODO



Contents
--------------------------------------
TODO: src

TODO: Documentation

TODO: Tests

TODO: Continuous Integration




Development
--------------------------------------
### Guidelines
TODO
- **no Libs** STL only
- **std=C++11** TODO
	


Building the Compiler
--------------------------------------
Have a look @ tutorials/*


Compiler usage
--------------------------------------

```bash
cppRail (-i|-d) <srcFile> [-s <dstFile>] [-g <dstFile>] [-h]
```

### Parameters
- **-i <srcFile>** compiles <srcFile> to .class
- **-o <dstFile>** compiles source to <dstFile> 
- **-s <dstFile>** serializes the ASG to csv
- **-d <srcFile>** deserializes <srcFile> and compiles to .class
- **-g <dstFile>** generates a graphviz .dot file as ASG-Visualisation
- **-q** quiet mode (Warnings, Errors, Exceptions only)
- **-h** Commandline Help

You can either user -i or -d, not both



Contribution Guide
--------------------------------------
Contribution in any form is (currently) limited to students thar are involved.







