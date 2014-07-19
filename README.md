Jail++ (Rail compiler)
==================================================
The *Jail Constructions ltd.* proudly presents you our newest product:
**Jail++**, a Rail to Java ByteCode compiler in C++.


### Hey! What's Rail?
[Esolang-Wiki](http://esolangs.org/wiki/Rail): Esoteric 2D computer language


### Who are we
The *Jail Constructions ltd.* are a group of 15 computer science students from the
[Freie Universität Berlin](http://www.inf.fu-berlin.de) in Germany. During the
summer term 2014, we developed a compiler that translates Rail code into JVM Bytecode.
In addition, we programmed an **Integrated Development Environment (IDE)** for Rail
coders. It simplifies the process of writing rails and offers features like undo/redo,
rail rotation and mirroring, individually configurable syntax highlighting, compiler
integration, I/O text areas and many more.


### Project Goals
- implementation of all available Rail commands
  - data types
  - functions
  - lists
  - recursions
  - junctions
  - stack operations
- pipeline interfaces
- [AST](http://en.wikipedia.org/wiki/Abstract_syntax_tree) Serialisation
- [IDE](http://en.wikipedia.org/wiki/Integrated_development_environment) for Rail with syntax highlighting


Contents
--------------------------------------
1. Development
2. Building the compiler
3. Compiler usage
4. Building the IDE
5. Contribution Guide




1. Development
--------------------------------------
### Guidelines
This project was developed for Linux (Ubuntu 14.04) and has not been tested on any other
operating systems.

- **no Libs** STL only
- **std=C++11** TODO


2. Building the compiler
--------------------------------------
We recommend you to use Eclipse to build the compiler. In *tutorials/eclipseCdt_Cpp11.txt* 
you will find a step-by-step guide to configure the project correctly.


3. Compiler usage
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



4. Building the IDE
--------------------------------------
You will need **Qt** to build the Rail IDE. Check *tutorials/Qt5Tutorial.txt* to install Qt5
from bottom up.

Additionally, you should install the *Qt Creator* to run the IDE (see *tutorials/EditorTutorial.txt* for a
step-by-step guide).

The usage of the IDE should be very intuitive, so just check out the available menu options.



5. Contribution Guide
--------------------------------------
Contribution in any form is (currently) limited to students that are involved.







