include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# compilation flags
add_definitions(
        -DQUIET
        -DCGAL_HEADER_ONLY
        -DNBUILD
)

add_compile_options(
        -Wall -pedantic -fPIC
)

# build profile dependant flags

if (CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-DDEBUG)
    add_compile_options(-pg)
    SET(LINK_LIBS
            cadical-debug)
elseif(CMAKE_BUILD_TYPE MATCHES Release)
    add_definitions(-DNDEBUG)
    add_compile_options(-O3)
    SET(LINK_LIBS
            cadical)
endif()

SET(SOURCES example.cpp)

add_executable(api_example ${SOURCES})

SET(LINK_FLAGS
        "-Wl,--no-as-needed,--no-allow-shlib-undefined,--no-undefined -L${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")

set_target_properties(api_example PROPERTIES
        LINK_LIBRARIES cadical)