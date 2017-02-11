cmake_minimum_required(VERSION 3.4.1)

set(my_SRCS borders.cpp
            crop.cpp
            tachimage.cpp)

add_library(tachimage SHARED ${my_SRCS})

target_link_libraries(tachimage log jnigraphics)
