
cmake_minimum_required(VERSION 3.4.1)

include_directories(
                common
                source )

add_library(
             native-lib

             SHARED

             native-lib.cpp
             source/JavaListener.cpp
             source/Worker.cpp
             source/time-util.cpp
             source/YUVQueue.cpp
             source/Consumer.cpp
             )



find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )


target_link_libraries(
                       native-lib


                       ${log-lib} )
