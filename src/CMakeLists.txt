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
    SET(LIBNAME cadical-debug)
elseif(CMAKE_BUILD_TYPE MATCHES Release)
    add_definitions(-DNDEBUG)
    add_compile_options(-O3)
    SET(LIBNAME cadical)
endif()

# libraries
set(SOURCES analyze.cpp arena.cpp assume.cpp
        averages.cpp backtrack.cpp backward.cpp bins.cpp
        block.cpp ccadical.cpp checker.cpp clause.cpp
        collect.cpp compact.cpp condition.cpp config.cpp cover.cpp
        decide.cpp decompose.cpp deduplicate.cpp
        elim.cpp ema.cpp extend.cpp external.cpp
        file.cpp flags.cpp format.cpp gates.cpp
        instantiate.cpp internal.cpp ipasir.cpp
        limit.cpp logging.cpp lucky.cpp
        message.cpp minimize.cpp observer.hpp
        occs.cpp options.cpp parse.cpp phases.cpp
        probe.cpp probe.cpp proof.cpp propagate.cpp
        queue.cpp random.cpp restart.cpp reduce.cpp rephase.cpp
        report.cpp resources.cpp resources.cpp restore.cpp
        score.cpp signal.cpp solution.cpp solver.cpp
        stats.cpp subsume.cpp terminal.cpp ternary.cpp
        tracer.cpp transred.cpp util.cpp var.cpp version.cpp
        vivify.cpp walk.cpp walk.cpp watch.cpp
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