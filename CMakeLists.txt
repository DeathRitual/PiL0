project(PiL0)

add_executable(PiL0 header/libraries/parser.c header/libraries/err_handling.c header/libraries/lexer.c pl.c header/err_handling.h header/lexer.h header/language.h header/parser.h)

install(TARGETS PiL0 RUNTIME DESTINATION bin)
add_subdirectory(header)