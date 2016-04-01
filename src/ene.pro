TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -fpermissive -std=c++11 -w -O0 -fno-exceptions

OTHER_FILES += \
    template.nasm \
    template_16.nasm \
    template_32.nasm \
    template_64.nasm \
    templates/template_16.nasm \
    templates/template_16.s \
    templates/template_32.nasm \
    templates/template_32.s \
    templates/template_64.nasm \
    templates/template_64.s

HEADERS += \
    errors/errors.hpp \
    errors/warnings.h \
    il/il.h \
    il/il_gas.h \
    il/il_nasm.h \
    lexing/lexer.hpp \
    lexing/token.hpp \
    parsing/ast/ast.h \
    parsing/ast/tast.h \
    parsing/ast/uast.h \
    parsing/ast/utast.h \
    parsing/parser.h \
    scope/array_t.h \
    scope/itype.h \
    scope/ptr_t.h \
    scope/scope.hpp \
    scope/scoper.h \
    scope/struct_t.h \
    scope/value_t.h \
    scope/void_t.h \
    compiler.h \
    io.h \
    string_def.h \
    target.h \
    scope/fptr_t.h \
    praep.h \
    name_mng.h \
    parsing/fmod.h

SOURCES += \
    errors/errors.cpp \
    errors/warnings.cpp \
    il/il.cpp \
    il/il_gas.cpp \
    il/il_nasm.cpp \
    lexing/lexer.cpp \
    lexing/token.cpp \
    parsing/ast/tast.cpp \
    parsing/ast/uast.cpp \
    parsing/ast/utast.cpp \
    parsing/parser.cpp \
    scope/array_t.cpp \
    scope/itype.cpp \
    scope/ptr_t.cpp \
    scope/scope.cpp \
    scope/scoper.cpp \
    scope/struct_t.cpp \
    scope/value_t.cpp \
    scope/void_t.cpp \
    compiler.cpp \
    io.cpp \
    main.cpp \
    target.cpp \
    scope/fptr_t.cpp \
    praep.cpp \
    name_mng.cpp \
    parsing/fmod.cpp
