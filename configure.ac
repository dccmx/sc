AC_PREREQ([2.68])
AC_INIT([sc], [0.1], [dccmx@dccmx.com])
AM_INIT_AUTOMAKE([foreign])
AC_LANG([C++])
AM_MAINTAINER_MODE
AC_CONFIG_SRCDIR([src/main.cc])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

# Checks for libraries.
AC_CHECK_HEADER(google/protobuf/stubs/common.h,,
        [AC_MSG_ERROR([
ERROR: protobuf headers are required.

You must either install protobuf from google,
or if you have it installed in a custom location
you must add '-Iincludedir' to CXXFLAGS
and '-Llibdir' to LDFLAGS.

If you did not specify a prefix when installing
protobuf, try 
'./configure CXXFLAGS=-I/usr/local/include LDFLAGS=-L/usr/local/lib'
In some 64-bit environments, try LDFLAGS=-L/usr/local/lib64.
])])

pbc_savelibs="$LIBS"
LIBS="$LIBS -lprotoc -lprotobuf -lpthread"

AC_TRY_LINK([#include <google/protobuf/compiler/command_line_interface.h>],
        [google::protobuf::compiler::CommandLineInterface cli;],
        [],
        [AC_MSG_ERROR([
ERROR:
protobuf test program failed to link:
perhaps you need to add -Llibdir to your LDFLAGS.])])

LIBS="$pbc_savelibs"

AC_CHECK_LIB([protobuf], [main])
AC_CHECK_LIB([protoc], [main])
AC_CHECK_LIB([pthread], [main])

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL

# Checks for library functions.

AC_CONFIG_FILES([Makefile
                 src/Makefile])
AC_OUTPUT

AC_MSG_NOTICE([Configuration Summary:
    //   ) )  //   ) ) 
   ((        //        
     \\\\     //         
       ) ) //          
((___ / / ((____/ /    

Type 'make' to build.
send complains and bugs to dccmx@dccmx.com
])
