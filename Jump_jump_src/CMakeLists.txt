cmake_minimum_required(VERSION 3.26)
project(JUMP_JUMP C)

set(CMAKE_C_STANDARD 11)
include_directories("D:/mingw(CLion+VSC)/mingw(CLion+VSC)/SDL2-2.26.0-allinone/x86_64-w64-mingw32/include")
link_directories("D:/mingw(CLion+VSC)/mingw(CLion+VSC)/SDL2-2.26.0-allinone/x86_64-w64-mingw32/lib")
add_executable(JUMP_JUMP main.c
        main.h
)

target_link_libraries(JUMP_JUMP PRIVATE
        mingw32
        SDL2main
        SDL2
        SDL2_image
        SDL2_mixer
        SDL2_ttf

)