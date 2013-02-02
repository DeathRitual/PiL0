project(PiL0)

add_executable(PiL0 adt_main.c adt.c)

install(TARGETS PiL0 RUNTIME DESTINATION bin)