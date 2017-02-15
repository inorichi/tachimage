cmake_minimum_required(VERSION 3.4.1)

set(tachimage_SRCS
            borders.cpp
            crop.cpp
            tachimage.cpp)

add_library(tachimage SHARED ${tachimage_SRCS})

target_link_libraries(tachimage log jnigraphics)
