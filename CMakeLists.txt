project(PiL0)

add_executable(PiL0 lexer.c pl.c lexer.h language.h)

install(TARGETS PiL0 RUNTIME DESTINATION bin)