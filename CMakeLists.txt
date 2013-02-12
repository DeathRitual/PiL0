project(PiL0)

add_executable(PiL0 header/libraries/lexer.c pl.c header/lexer.h header/language.h)

install(TARGETS PiL0 RUNTIME DESTINATION bin)
add_subdirectory(header)