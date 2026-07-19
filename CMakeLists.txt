cmake_minimum_required(VERSION 3.16)

project(webpdf
    VERSION 1.9.0
    DESCRIPTION "HTML/CSS/SVG to PDF renderer library"
    HOMEPAGE_URL "https://github.com/hggq/webpdf"
    LANGUAGES CXX)

# ---------------------------------------------------------------------------
# Detect whether webpdf is the top-level project (affects default options).
# ---------------------------------------------------------------------------
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    set(WEBPDF_IS_TOP_LEVEL ON)
else()
    set(WEBPDF_IS_TOP_LEVEL OFF)
endif()

# ---------------------------------------------------------------------------
# Options
# ---------------------------------------------------------------------------
option(WEBPDF_BUILD_EXAMPLES "Build the example programs"            ${WEBPDF_IS_TOP_LEVEL})
option(WEBPDF_INSTALL        "Generate install rules for webpdf"     ${WEBPDF_IS_TOP_LEVEL})
option(WEBPDF_INSTALL_FONTS  "Install the bundled fonts as data"     ${WEBPDF_IS_TOP_LEVEL})
option(WEBPDF_FETCH_ZLIB     "Download & build zlib if not found"    OFF)
option(ENABLE_ASAN           "Build with AddressSanitizer"           OFF)

# ---------------------------------------------------------------------------
# Global settings
# ---------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(WEBPDF_IS_TOP_LEVEL AND NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
        "Debug" "Release" "RelWithDebInfo" "MinSizeRel")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# ---------------------------------------------------------------------------
# Dependency: zlib (with optional FetchContent fallback for Windows)
# ---------------------------------------------------------------------------
set(_webpdf_zlib_bundled OFF)
find_package(ZLIB QUIET)

if(NOT ZLIB_FOUND AND (WEBPDF_FETCH_ZLIB OR WIN32))
    message(STATUS "webpdf: system zlib not found -> fetching zlib via FetchContent")
    include(FetchContent)
    set(SKIP_INSTALL_ALL ON)                       # suppress zlib's own install rules
    set(ZLIB_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    FetchContent_Declare(zlib
        URL      https://github.com/madler/zlib/releases/download/v1.3.1/zlib-1.3.1.tar.gz
        URL_HASH SHA256=9a93b2b7dfdac77ceba5a558a580e74667dd6fede4585b91eefb60f03b72df23)
    FetchContent_MakeAvailable(zlib)
    set(_webpdf_zlib_bundled ON)
elseif(NOT ZLIB_FOUND)
    # Produce a clear, actionable error (install zlib or pass -DWEBPDF_FETCH_ZLIB=ON).
    find_package(ZLIB REQUIRED)
endif()

# ---------------------------------------------------------------------------
# Sources / headers
# ---------------------------------------------------------------------------
set(WEBPDF_SOURCES
    src/webpdf.cpp
    src/request.cpp
    src/gzip.cpp
    src/ttfparser.cpp
    src/parsecss.cpp
    src/parsehtml.cpp
    src/parsesvg.cpp
)

set(WEBPDF_PUBLIC_HEADERS
    include/webpdf/webpdf.h
    include/webpdf/request.h
    include/webpdf/gzip.h
    include/webpdf/ttfparser.h
    include/webpdf/parsecss.h
    include/webpdf/parsehtml.h
    include/webpdf/parsesvg.h
)

# ---------------------------------------------------------------------------
# Library target
# ---------------------------------------------------------------------------
add_library(webpdf ${WEBPDF_SOURCES})
add_library(webpdf::webpdf ALIAS webpdf)

set_target_properties(webpdf PROPERTIES
    VERSION   ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    POSITION_INDEPENDENT_CODE ON
    OUTPUT_NAME "webpdf"
)

target_compile_features(webpdf PUBLIC cxx_std_20)

# Headers ship flat under <prefix>/include/webpdf and are included as
# #include "webpdf.h"; that directory is what consumers add to their path.
target_include_directories(webpdf
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/webpdf>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/webpdf>
)

# zlib linkage.
if(_webpdf_zlib_bundled)
    set(_webpdf_zlib_link zlibstatic)
    target_include_directories(webpdf PRIVATE "${zlib_SOURCE_DIR}" "${zlib_BINARY_DIR}")
    # Bundled zlib is a build-time detail; keep it out of the exported usage
    # requirements so install(EXPORT) does not require the zlib target.
    target_link_libraries(webpdf PRIVATE $<BUILD_INTERFACE:${_webpdf_zlib_link}>)
    set(WEBPDF_ZLIB_FIND_DEPENDENCY "# zlib was bundled into webpdf at build time")
else()
    # zlib is a transitive dependency: static consumers must link it too, so PUBLIC.
    target_link_libraries(webpdf PUBLIC ZLIB::ZLIB)
    set(WEBPDF_ZLIB_FIND_DEPENDENCY "find_dependency(ZLIB)")
endif()

# ---------------------------------------------------------------------------
# Compiler / platform specific options
# ---------------------------------------------------------------------------
if(MSVC)
    target_compile_options(webpdf PRIVATE /W4 /utf-8 /permissive- /EHsc /Zc:__cplusplus)
    target_compile_definitions(webpdf PRIVATE _CRT_SECURE_NO_WARNINGS NOMINMAX)
else()
    target_compile_options(webpdf PRIVATE -Wall -Wextra)
endif()

if(ENABLE_ASAN)
    if(MSVC)
        target_compile_options(webpdf PUBLIC /fsanitize=address)
    else()
        target_compile_options(webpdf PUBLIC -fsanitize=address -fno-omit-frame-pointer)
        target_link_options(webpdf PUBLIC -fsanitize=address)
    endif()
endif()

# ---------------------------------------------------------------------------
# Examples (optional)
# ---------------------------------------------------------------------------
if(WEBPDF_BUILD_EXAMPLES)
    # Point examples at the in-tree font directory so they run from anywhere.
    set(WEBPDF_EXAMPLE_FONT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/example/font")

    # Emit the example binaries next to their assets (content.html, images/,
    # font/) so they can be run directly from the example directory.
    set(WEBPDF_EXAMPLE_BIN_DIR "${CMAKE_CURRENT_SOURCE_DIR}/example")

    add_executable(webpdf_hello example/hello.cpp)
    target_link_libraries(webpdf_hello PRIVATE webpdf::webpdf)
    target_compile_definitions(webpdf_hello PRIVATE
        WEBPDF_FONT_DIR="${WEBPDF_EXAMPLE_FONT_DIR}")
    set_target_properties(webpdf_hello PROPERTIES
        OUTPUT_NAME "webpdf-hello"
        RUNTIME_OUTPUT_DIRECTORY "${WEBPDF_EXAMPLE_BIN_DIR}"
        RUNTIME_OUTPUT_DIRECTORY_DEBUG "${WEBPDF_EXAMPLE_BIN_DIR}"
        RUNTIME_OUTPUT_DIRECTORY_RELEASE "${WEBPDF_EXAMPLE_BIN_DIR}")

    add_executable(webpdf_render_html example/render_html.cpp)
    target_link_libraries(webpdf_render_html PRIVATE webpdf::webpdf)
    target_compile_definitions(webpdf_render_html PRIVATE
        WEBPDF_FONT_DIR="${WEBPDF_EXAMPLE_FONT_DIR}")
    set_target_properties(webpdf_render_html PROPERTIES
        OUTPUT_NAME "webpdf-render-html"
        RUNTIME_OUTPUT_DIRECTORY "${WEBPDF_EXAMPLE_BIN_DIR}"
        RUNTIME_OUTPUT_DIRECTORY_DEBUG "${WEBPDF_EXAMPLE_BIN_DIR}"
        RUNTIME_OUTPUT_DIRECTORY_RELEASE "${WEBPDF_EXAMPLE_BIN_DIR}")

    add_executable(webpdf_cli example/webpdf.cpp)
    target_link_libraries(webpdf_cli PRIVATE webpdf::webpdf)
    set_target_properties(webpdf_cli PROPERTIES
        OUTPUT_NAME "webpdf"
        RUNTIME_OUTPUT_DIRECTORY "${WEBPDF_EXAMPLE_BIN_DIR}"
        RUNTIME_OUTPUT_DIRECTORY_DEBUG "${WEBPDF_EXAMPLE_BIN_DIR}"
        RUNTIME_OUTPUT_DIRECTORY_RELEASE "${WEBPDF_EXAMPLE_BIN_DIR}")
endif()

# ---------------------------------------------------------------------------
# Install rules + CMake package config (find_package(webpdf))
# ---------------------------------------------------------------------------
if(WEBPDF_INSTALL)
    set(WEBPDF_CMAKE_CONFIG_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/webpdf")

    install(TARGETS webpdf
        EXPORT webpdfTargets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/webpdf
    )

    install(FILES ${WEBPDF_PUBLIC_HEADERS}
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/webpdf)

    if(WEBPDF_INSTALL_FONTS)
        install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/example/font/"
            DESTINATION ${CMAKE_INSTALL_DATADIR}/webpdf/example/fonts)
    endif()

    if(WEBPDF_BUILD_EXAMPLES)
        install(TARGETS webpdf_cli
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
    endif()

    install(EXPORT webpdfTargets
        FILE webpdfTargets.cmake
        NAMESPACE webpdf::
        DESTINATION ${WEBPDF_CMAKE_CONFIG_DESTINATION})

    configure_package_config_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/webpdfConfig.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/webpdfConfig.cmake"
        INSTALL_DESTINATION ${WEBPDF_CMAKE_CONFIG_DESTINATION})

    write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/webpdfConfigVersion.cmake"
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion)

    install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/webpdfConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/webpdfConfigVersion.cmake"
        DESTINATION ${WEBPDF_CMAKE_CONFIG_DESTINATION})

    # pkg-config support for non-CMake build systems.
    configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/webpdf.pc.in"
        "${CMAKE_CURRENT_BINARY_DIR}/webpdf.pc"
        @ONLY)
    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/webpdf.pc"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)
endif()
