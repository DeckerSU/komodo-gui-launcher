AUTOMAKE_OPTIONS = foreign

# Ensure the moc command is available
MOC = $(QT5_MOC)

# Define the name of the binary program
bin_PROGRAMS = komodo-gui-launcher

# Specify the sources for the program
komodo_gui_launcher_SOURCES = main.cpp MainWindow.cpp \
                              MainWindow.h \
                              moc_MainWindow.cpp

# Rule to automatically generate moc_MainWindow.cpp from MainWindow.h
moc_MainWindow.cpp: MainWindow.h
	$(MOC) $(srcdir)/MainWindow.h -o moc_MainWindow.cpp

# Include Qt5 module flags for compiling and linking
AM_CPPFLAGS = $(QT5_CFLAGS)
komodo_gui_launcher_LDADD = $(QT5_LIBS)

