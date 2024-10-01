if(NOT CC_LIBS_ONLY_BUILD)
    add_subdirectory (ch)
endif()

if (NOT STATIC_LIBRARY)
    add_subdirectory (ChakraCore)
endif()
