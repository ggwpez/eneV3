TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    token.cpp \
    main.cpp \
    lexer.cpp \
    parser.cpp \
    scope/scope.cpp \
    scope/itype.cpp \
    scope/array_t.cpp \
    scope/struct_t.cpp \
    scope/value_t.cpp \
    scope/ptr_t.cpp \
    scope/void_t.cpp \
    errors.cpp \
    scope/scoper.cpp \
    uast.cpp \
    tast.cpp \
    utast.cpp \
    compiler.cpp \
    io.cpp \
    warnings.cpp \
    target.cpp \
    il_gas.cpp \
    il_nasm.cpp \
    il.cpp

HEADERS += \
    token.hpp \
    lexer.hpp \
    parser.h \
    errors.hpp \
    scope/scope.hpp \
    scope/itype.h \
    scope/array_t.h \
    scope/struct_t.h \
    scope/value_t.h \
    scope/ptr_t.h \
    string_def.h \
    scope/void_t.h \
    il.h \
    scope/scoper.h \
    tast.h \
    uast.h \
    utast.h \
    ast.h \
    target.h \
    compiler.h \
    io.h \
    warnings.h \
    il_gas.h \
    il_nasm.h \
    il.h

QMAKE_CXXFLAGS += -fpermissive -std=c++11 -w -O1

OTHER_FILES += \
    template.nasm \
    template_16.nasm \
    template_32.nasm \
    template_64.nasm
