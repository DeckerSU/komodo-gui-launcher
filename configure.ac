AC_INIT([komodo-gui-launcher], [1.0])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_CONFIG_SRCDIR([main.cpp])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_INSTALL

# Set the CXXFLAGS to include -fPIC
CXXFLAGS="$CXXFLAGS -fPIC"

# Check for Qt5
PKG_CHECK_MODULES([QT5], [Qt5Widgets Qt5Network])

# Set C++ standard
AC_LANG([C++])
AC_PROG_CXXCPP

# Generate output files
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
