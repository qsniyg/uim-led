add_executable(uim-led main.c uim.c led.c)
target_link_libraries(uim-led uim uim-scm X11)
