TEMPLATE = app
TARGET = ROBD
QT += core \
    sql \
    network
HEADERS += Worker_Thread.hpp \
    STDIO_Filter.hpp \
    IO_Thread.hpp
SOURCES += STDIO_Filter.cpp \
    IO_Thread.cpp \
    main.cpp
FORMS += 
RESOURCES += 
