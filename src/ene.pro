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
    parsing/parser.h \
    scope/scope.hpp \
    scope/scoper.h \
    io.h \
    string_def.h \
    target.h \
    praep.h \
    name_mng.h \
    parsing/fmod.h \
    gen/gen.h \
    gen/opcode.h \
    compiler/cmp_args.h \
    compiler/compiler.h \
    compiler/cmp_alloc.h \
    ast/ast.h \
    ast/tast.h \
    ast/uast.h \
    ast/utast.h \
    scope/types/array_t.h \
    scope/types/fptr_t.h \
    scope/types/itype.h \
    scope/types/ptr_t.h \
    scope/types/struct_t.h \
    scope/types/value_t.h \
    scope/types/void_t.h

SOURCES += \
    errors/errors.cpp \
    errors/warnings.cpp \
    il/il.cpp \
    il/il_gas.cpp \
    il/il_nasm.cpp \
    lexing/lexer.cpp \
    lexing/token.cpp \
    parsing/parser.cpp \
    scope/scope.cpp \
    scope/scoper.cpp \
    io.cpp \
    main.cpp \
    target.cpp \
    praep.cpp \
    name_mng.cpp \
    parsing/fmod.cpp \
    gen/gen.cpp \
    gen/opcode.cpp \
    compiler/cmp_args.cpp \
    compiler/compiler.cpp \
    compiler/cmp_alloc.cpp \
    ast/tast.cpp \
    ast/uast.cpp \
    ast/utast.cpp \
    scope/types/array_t.cpp \
    scope/types/fptr_t.cpp \
    scope/types/itype.cpp \
    scope/types/ptr_t.cpp \
    scope/types/struct_t.cpp \
    scope/types/value_t.cpp \
    scope/types/void_t.cpp
