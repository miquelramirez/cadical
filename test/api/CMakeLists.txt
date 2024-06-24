include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# compilation flags
add_definitions(
        -DQUIET
        -DNBUILD
)

add_compile_options(
        -Wall -Wpedantic -fPIC
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
add_dependencies(api_example cadical_static)
target_link_libraries(api_example cadical_static)

add_executable(example_constraint example_constraint.cpp)
add_dependencies(example_constraint cadical_static)
target_link_libraries(example_constraint cadical_static)

add_executable(learn learn.cpp)
add_dependencies(learn cadical_static)
target_link_libraries(learn cadical_static)

add_executable(morenmore morenmore.cpp)
add_dependencies(morenmore cadical_static)
target_link_libraries(morenmore cadical_static)

add_executable(newdelete newdelete.cpp)
add_dependencies(newdelete cadical_static)
target_link_libraries(newdelete cadical_static)

add_executable(terminate terminate.cpp)
add_dependencies(terminate cadical_static)
target_link_libraries(terminate cadical_static)

add_executable(traverse traverse.cpp)
add_dependencies(traverse cadical_static)
target_link_libraries(traverse cadical_static)

add_executable(unit unit.cpp)
add_dependencies(unit cadical_static)
target_link_libraries(unit cadical_static)

add_executable(lazy_int_vars lazy_int_vars.cpp)
add_dependencies(lazy_int_vars cadical_static)
target_link_libraries(lazy_int_vars cadical_static)

add_executable(eager_int_vars eager_int_vars.cpp)
add_dependencies(eager_int_vars cadical_static)
target_link_libraries(eager_int_vars cadical_static)

