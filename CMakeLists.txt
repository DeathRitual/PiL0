project(PiL0)

add_executable(PiL0 tokdef.c lexer.c tokdef.h)

install(TARGETS PiL0 RUNTIME DESTINATION bin)