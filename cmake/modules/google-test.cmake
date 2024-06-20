#Reference: http://google.github.io/googletest/quickstart-cmake.html

message(STATUS "Enabled GoogleTest")

#Fix for a stupid message I was getting about CMP0135
if (CMAKE_MINOR_VERSION GREATER_EQUAL 25)
    # Set download timestamp policy
    cmake_policy(SET CMP0135 NEW)
endif ()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

#Copied GLOB_RECURSE from https://stackoverflow.com/questions/27992346/how-to-use-cmake-glob-recurse-for-only-some-subdirectories
file(GLOB_RECURSE TESTS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.h"
)

#Remove MolSim.cpp from the tests because Main shouldn't be unit tested, it should only ever be integration tested
#and it gives an error about multiple definitions of main
list(FILTER TESTS EXCLUDE REGEX "MolSim.cpp")

add_executable(MolSimTests ${TESTS})

target_include_directories(MolSimTests
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/tests
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/libxsd
)

target_link_libraries(
        MolSimTests
        PUBLIC
        GTest::gtest_main
        #For Mocking, similar to Mockito in Java
        GTest::gmock_main
        spdlog::spdlog
        XercesC::XercesC
)
enable_testing()

include(GoogleTest)

gtest_discover_tests(MolSimTests)





