 Constrained RAndom Verification Enviroment - CRAVE
====================================================

For feedback and bug reports: crave@systemc-verification.org

 Preparations
--------------

When installing directly from the git repository some additional steps are
required. These are not necessary when using the packaged source code. Both
require some packages to be installed on the system.

To get CRAVE from the git repository execute the following commands:

    git clone --recursive [crave-git-URL]

This will download crave and its direct dependencies. External dependencies
will be downloaded by the bootstrap script when executing make.

Please install the following packages to ensure the external packages will
build correctly:

* zlib and bzip2 development libraries (e.g. zlib1g-dev, libbz2-dev)
* CMake (at least v2.8.9)


 SystemC Support
-----------------

You need to set the environment variable SYSTEMC_HOME to your SystemC installation 
directory (which also must contains a static library). Otherwise, CRAVE will automatically 
download and build systemc-2.3.1 from GitHub.


 Other external dependencies
-----------------------

CRAVE uses the following external dependencies:

* [Boost] - you need at least version 1.50.0, set the environment variable BOOST_ROOT to your 
boost installation directory (e.g. /usr) or CRAVE will automatically download and build boost-1.55.0. 
For faster build, it is recommended to choose the first option.
* [Glog] - similarly, set GLOG_ROOT or CRAVE will automatically download and build glog. GLOG_ROOT can be set to an invalid path (e.g. 'export GLOG_ROOT=do/not/use') to disable Glog. In this case a very simple logger will be used instead.
* [Boolector] - Boolector can be disabled by via the environment variable CRAVE_SOLVERS (see below).
* [CUDD] - CUDD can be disabled by by via the environment variable CRAVE_SOLVERS (see below), but this is not recommended.
* [SWORD] - SWORD can be enabled by via the environment variable CRAVE_SOLVERS (see below). It is only available as pre-compiled binary for Linux x86 and x86_64 and might be incompatible with some platforms such as RHEL 5.
* [CVC4] - CVC4 can be enabled by by via the environment variable CRAVE_SOLVERS (see below). Building CVC4 takes considerable time so it is disabled by default.
* [Z3] - Z3 can be enabled by via the environment variable CRAVE_SOLVERS (see below). Building Z3 takes considerable time so it is disabled by default.
* [STP] - STP can be enabled by via the environment variable CRAVE_SOLVERS (see below). Building STP takes considerable time so it is disabled by default.
* [Yices2] - Yices2 can be enabled by via the environment variable CRAVE_SOLVERS (see below). Building Yices2 takes considerable time so it is disabled by default.

If the environment variable CRAVE_SOLVERS is not set, Boolector and CUDD will be used.
For example, to build CRAVE with CUDD, STP and Z3, call 'export CRAVE_SOLVERS='cudd stp z3'' before 'make'.
Please make sure that at least one SMT backend is enabled.


 Build and install CRAVE
-------------------------

1. Call 'make' to build CRAVE, the dependencies should
   automatically download and build.
2. Call 'make test', all tests should pass (with very few execeptions).
3. Call 'make install'.

To clean CRAVE you can delete the build directory (e.g. 'rm -Rf build') and call 'make' again.

 Examples
----------

You will find several examples in the examples/ directory. These will
demonstrate the core features of CRAVE using CRAVE-2.0 API (subdirectory crave2_api/)
as well as the new experimental API (subdirectory experimental_api/, requires compiler
support for C++11). The basic examples are available in both APIs as listed below:

  * ex1_seed_dist              : global seed and simple distribution.
  * ex2_constr_ref             : constraints and references.
  * ex3_inh_soft_next          : constraint inheritance, soft constraints and custom next()/randomize().
  * ex4_constr_mng             : enable/disable constraints.
  * ex5_vec_constr             : vector constraints.
  * ex6_inline_gen             : inline generator.
  * ex7_rand_enum              : constraints on enumeration types.
  * ex8_make_expression        : expression layer usage.

to execute these examples look into the build/examples directory.
See also the other examples and the test cases in crave/tests, which cover all implemented features.

 Documentation
-------------------------

The doxygen documentation can be generated in doc/crave-doxygen by calling 'make doxygen'.

 Configuration file
-----------------------

CRAVE will try to find and if not found, create a configuration file crave.cfg in 
the executing directory. The default configuration is shown in the following:

    <crave>
    	<backend>auto</backend>
    	<seed>0</seed>
    </crave>
    <logger>
    	<filename>crave</filename>
    	<directory>./logs</directory>
    	<level>2</level>
    	<filesize>100</filesize>
    </logger>

The value 'auto' means that CRAVE will automatically select a backend among 
the available SMT solvers. Other possible values are for example 'boolector', 'sword', 'z3', etc.
Change the seed to a positive integer, if you want to use this integer as the fixed seed.
For further information, especially on the logger, please refer to the doxygen documentation.

[Boost]: http://www.boost.org
[CUDD]:  http://vlsi.colorado.edu/~fabio/CUDD/
[SWORD]: http://www.informatik.uni-bremen.de/agra/eng/sword.php
[Z3]: https://github.com/Z3Prover/z3
[Boolector]: http://fmv.jku.at/boolector/
[CVC4]: http://cvc4.cs.nyu.edu
[Yices2]: http://yices.csl.sri.com/
[STP]: https://github.com/stp/stp
[Glog]: https://code.google.com/p/google-glog/
