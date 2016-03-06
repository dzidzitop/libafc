libafc
======

utils to facilitate C++ development

Build instruction (Unix-like systems)
-------------------------------------

Here, `${basedir}` denotes the root directory of the libafc codebase.

1. install the build tool [`ninja`](https://github.com/martine/ninja)
2. install GCC g++ 4.7+
3. install the libraries (including development versions; use your package manager for this):
    * `cppunit` (if tests are going to be built)
4. execute `ninja sharedLib` in `${basedir}`. The shared library `libafc.so` will be created in `${basedir}/build`
5. execute `ninja staticLib` in `${basedir}`. The static library `libafc.a` will be created in `${basedir}/build`
6. execute `ninja testBinary` in `${basedir}`. The executable `libafc_test` will be created in `${basedir}/build`. It contains unit tests created for libafc

System requirements
-------------------

* GCC g++ 4.7+
* ninja 1.3.3+
* cppunit 1.13.2+
