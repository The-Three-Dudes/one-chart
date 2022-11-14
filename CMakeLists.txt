cmake_minimum_required(VERSION 3.23)
project(one-chart
        VERSION 0.0.3
        DESCRIPTION ""
        HOMEPAGE_URL "https://github.com/The-Three-Dudes/one-chart"
        LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 11)

include(FetchContent)

FetchContent_Declare(
        doctest
        GIT_REPOSITORY https://github.com/doctest/doctest.git
        GIT_TAG b7c21ec5ceeadb4951b00396fc1e4642dd347e5f
        #https://github.com/doctest/doctest/releases/tag/v2.4.9
)

set(GLFW_BUILD_DOCS FALSE)
set(GLFW_BUILD_EXAMPLES FALSE)
set(GLFW_BUILD_TESTS FALSE)
FetchContent_Declare(
	glfw
	GIT_REPOSITORY https://github.com/glfw/glfw.git
	GIT_TAG 7482de6071d21db77a7236155da44c172a7f6c9e
	#https://github.com/glfw/glfw/releases/tag/3.3.8
)
FetchContent_MakeAvailable(doctest glfw)

add_executable(one-chart main.cpp)

add_dependencies(${PROJECT_NAME} glfw)

target_link_libraries(${PROJECT_NAME} PRIVATE doctest glfw)

set(ENABLE_TESTING ON CACHE ON "Enable testing" FORCE)

set(DOCTEST_CONFIG_DISABLE ON)
if(ENABLE_TESTING)
    include(CTest)
    set(DOCTEST_CONFIG_DISABLE OFF)
    include(${doctest_SOURCE_DIR}/scripts/cmake/doctest.cmake)
    doctest_discover_tests(one-chart)
endif()
configure_file("${CMAKE_SOURCE_DIR}/include/doctest_proxy.h.in" "${CMAKE_SOURCE_DIR}/include/doctest_proxy.h")

# create macros for development introspection
# print all variables
macro(print_all_variables)
    message(STATUS "print_all_variables------------------------------------------{")
    get_cmake_property(_variableNames VARIABLES)
    foreach (_variableName ${_variableNames})
        message(STATUS "${_variableName}=${${_variableName}}")
    endforeach()
    message(STATUS "print_all_variables------------------------------------------}")
endmacro()
# print target libraries
macro(print_target_libraries)
    message(STATUS "print_target_libraries---------------------------------------{")
    get_target_property(OUT ${PROJECT_NAME} LINK_LIBRARIES)
    message(STATUS ${OUT})
    message(STATUS "print_target_libraries---------------------------------------}")
endmacro()

#print_all_variables()
