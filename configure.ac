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
# PKG_CHECK_MODULES([QT5], [Qt5Widgets Qt5Network])
PKG_CONFIG_PATH="$PKG_CONFIG_PATH" PKG_CHECK_MODULES([QT5], [Qt5Widgets Qt5Network])

# Find Qt5 qmake
AC_PATH_PROG([QT5_QMAKE], [qmake], [no])

if test "$QT5_QMAKE" != "no"; then
    # Use qmake to find the paths to moc, uic, and rcc
    QT5_MOC=`$QT5_QMAKE -query QT_INSTALL_BINS`/moc
    QT5_UIC=`$QT5_QMAKE -query QT_INSTALL_BINS`/uic
    QT5_RCC=`$QT5_QMAKE -query QT_INSTALL_BINS`/rcc

    # Check if the tools exist and are executable
    if test -x "$QT5_MOC" -a -x "$QT5_UIC" -a -x "$QT5_RCC"; then
        AC_SUBST([QT5_MOC])
        AC_SUBST([QT5_UIC])
        AC_SUBST([QT5_RCC])
        AC_MSG_NOTICE([checking if Qt5 development tools is detectable])
    else
        AC_MSG_ERROR([Could not find Qt5 tools using qmake. Please ensure Qt5 is properly installed.])
    fi
else
    AC_MSG_ERROR([Could not find qmake. Please install Qt5 development tools.])
fi

# Set C++ standard
AC_LANG([C++])
AC_PROG_CXXCPP

# Generate output files
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
