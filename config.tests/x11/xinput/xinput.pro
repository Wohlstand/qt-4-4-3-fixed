SOURCES = xinput.cpp
CONFIG += x11
CONFIG -= qt
LIBS += -lXi
irix-*:DEFINES+=Q_OS_IRIX
