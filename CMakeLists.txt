cmake_minimum_required(VERSION 3.10)

# define project name, version
project(PSEMolDyn_GroupA VERSION 0.0.4)

# let ccmake and cmake-gui offer the default build type options
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug;Release;RelWithDebInfo;MinSizeRel")

# set Release as the default build type if it is not yet set.
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING
            "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif (NOT CMAKE_BUILD_TYPE)

#TODO: Add to Report
# set Optimisation flags for Release
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_C_FLAGS_RELEASE "-O3")

#TODO: Add to Report
if (OPTIMISATION)
    message(STATUS "Further Optimisation flags enabled")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -Ofast -funroll-loops -finline-functions -fprefetch-loop-arrays -flto=auto")
    set(CMAKE_C_FLAGS_RELEASE "-O3 -Ofast -funroll-loops -finline-functions -fprefetch-loop-arrays -flto=auto")
endif ()

# collect all cpp files
file(GLOB_RECURSE MY_SRC
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
        # header don't need to be included but this might be necessary for some IDEs
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/modules)

# create make target
add_executable(MolSim ${MY_SRC})

# set cxx standard. You may raise this if you want.
target_compile_features(MolSim
        PRIVATE
        cxx_std_17
)

target_include_directories(MolSim
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/libxsd
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# activate all compiler warnings. Clean up your code :P
# depending on the compiler different flags are used
target_compile_options(MolSim
        PRIVATE
        $<$<CXX_COMPILER_ID:GNU>:-Wall>
        # disable some spam warnings for icpc...
        $<$<CXX_COMPILER_ID:Intel>:-w3 -wd383,981,1418,1572,2259>
)

find_package(XercesC REQUIRED)
find_package(Boost COMPONENTS program_options REQUIRED)

target_link_libraries(MolSim
        # stuff that is used in headers and source files
        PUBLIC
        Boost::program_options
        XercesC::XercesC
        spdlog::spdlog
        gtest_main
        gmock_main
)

#TODO: ADD TO REPORT
if (PROFILING)
    message(STATUS "Profiling enabled")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")
    SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -pg")
endif ()


include(doxygen)

include(spdlog)

include(google-test)

include(CTest)