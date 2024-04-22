# Following: https://vicrucann.github.io/tutorials/quick-cmake-doxygen/

option(BUILD_DOCS "Build Documentation" OFF)

if (BUILD_DOCS)
    message("Doxygen Enabled")
    find_package(Doxygen)
    if (DOXYGEN_FOUND)
        message("Doxygen build started")

        configure_file(${CMAKE_SOURCE_DIR}/Doxyfile ${CMAKE_BINARY_DIR}/Doxyfile @ONLY)

        set(DOXYGEN_OUT ${CMAKE_BINARY_DIR}/Doxyfile)
        # Custom target doc_doxygen
        add_custom_target(doc_doxygen
                COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
                WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                # Use of VERBATIM is recommended as it enables correct behavior. When VERBATIM is not given the behavior
                # is platform specific because there is no protection of tool-specific special characters.
                VERBATIM
        )
    else (DOXYGEN_FOUND)
        message(WARNING "Doxygen needs to be installed to generate the doxygen documentation")
    endif (DOXYGEN_FOUND)
else (BUILD_DOCS)
    message(STATUS "Doxygen disabled")
endif ()


