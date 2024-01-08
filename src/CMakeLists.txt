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
    SET(LIBNAME cadical-debug)
elseif(CMAKE_BUILD_TYPE MATCHES Release)
    add_definitions(-DNDEBUG)
    add_compile_options(-O3)
    SET(LIBNAME cadical)
endif()

# libraries
set(SOURCES
        # A
        analyze.cpp arena.cpp assume.cpp averages.cpp
        # B
        backtrack.cpp backward.cpp bins.cpp block.cpp
        # C
        cadical.cpp ccadical.cpp checker.cpp clause.cpp collect.cpp compact.cpp
        condition.cpp contract.cpp config.cpp cover.cpp constrain.cpp
        # D
        decide.cpp decompose.cpp deduplicate.cpp drattracer.cpp
        # E
        elim.cpp ema.cpp extend.cpp external.cpp external_propagate.cpp
        # F
        file.cpp flip.cpp flags.cpp format.cpp frattracer.cpp
        # G
        gates.cpp
        # I
        idruptracer.cpp instantiate.cpp internal.cpp ipasir.cpp
        # L
        limit.cpp logging.cpp lookahead.cpp lratbuilder.cpp lratchecker.cpp lrattracer.cpp lucky.cpp
        # M
        message.cpp minimize.cpp
        # O
        occs.cpp options.cpp
        # P
        parse.cpp phases.cpp probe.cpp probe.cpp proof.cpp propagate.cpp profile.cpp
        # Q
        queue.cpp
        # R
        random.cpp reap.cpp restart.cpp reduce.cpp rephase.cpp report.cpp resources.cpp restore.cpp
        # S
        score.cpp signal.cpp solution.cpp solver.cpp shrink.cpp stats.cpp subsume.cpp
        # T
        terminal.cpp ternary.cpp transred.cpp
        # U
        util.cpp
        # V
        var.cpp veripbtracer.cpp version.cpp vivify.cpp
        # W
        walk.cpp watch.cpp
        )

set(HEADERS
        cadical.hpp ccadical.h)

install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/" # source directory
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/cadical # target directory
        COMPONENT devel
        FILES_MATCHING # install only matched files
        PATTERN "*.hpp" # select header files
        )

add_library(cadical SHARED ${HEADERS} ${SOURCES})

add_executable(mobical mobical.cpp)
add_dependencies(mobical cadical)

target_link_libraries(mobical cadical)

set_target_properties (cadical PROPERTIES
        OUTPUT_NAME ${LIBNAME}
        SOVERSION ${LIBRARY_SOVERSION}
        FRAMEWORK ON)

install(TARGETS cadical LIBRARY DESTINATION lib
        NAMELINK_SKIP
        COMPONENT runtime/Con
        # For Mac
        FRAMEWORK DESTINATION "/Library/Frameworks")

install(TARGETS cadical LIBRARY DESTINATION lib
        NAMELINK_ONLY
        COMPONENT devel
        # For Mac
        FRAMEWORK DESTINATION "/Library/Frameworks")