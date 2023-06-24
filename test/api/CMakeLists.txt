include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# compilation flags
add_definitions(
        -DQUIET
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
add_dependencies(api_example cadical)
target_link_libraries(api_example cadical)

add_executable(example_constraint example_constraint.cpp)
add_dependencies(example_constraint cadical)
target_link_libraries(example_constraint cadical)

add_executable(learn learn.cpp)
add_dependencies(learn cadical)
target_link_libraries(learn cadical)

add_executable(morenmore morenmore.cpp)
add_dependencies(morenmore cadical)
target_link_libraries(morenmore cadical)

add_executable(newdelete newdelete.cpp)
add_dependencies(newdelete cadical)
target_link_libraries(newdelete cadical)

add_executable(terminate terminate.cpp)
add_dependencies(terminate cadical)
target_link_libraries(terminate cadical)

add_executable(traverse traverse.cpp)
add_dependencies(traverse cadical)
target_link_libraries(traverse cadical)

add_executable(unit unit.cpp)
add_dependencies(unit cadical)
target_link_libraries(unit cadical)


