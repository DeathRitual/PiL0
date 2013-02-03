project(PiL0)

add_executable(PiL0 tokdef.c adt_main.c)

install(TARGETS PiL0 RUNTIME DESTINATION bin)