# Overlay port for webpdf.
#
# Usage (build webpdf as a vcpkg package):
#   vcpkg install webpdf --overlay-ports=ports
#
# When publishing to a registry, replace the SOURCE section below with
# vcpkg_from_github()/vcpkg_from_gitlab() and a real SHA512.

# --- Option A: build from a released archive (recommended for registries) ----
# vcpkg_from_github(
#     OUT_SOURCE_PATH SOURCE_PATH
#     REPO webpdf/webpdf
#     REF "v${VERSION}"
#     SHA512 0  # replace with the real archive SHA512
#     HEAD_REF main
# )

# --- Option B: build from the local checkout (overlay-port development) ------
# Points at the repository root that contains CMakeLists.txt.
get_filename_component(SOURCE_PATH "${CMAKE_CURRENT_LIST_DIR}/../.." ABSOLUTE)

vcpkg_cmake_configure(
    SOURCE_PATH "${SOURCE_PATH}"
    OPTIONS
        -DWEBPDF_BUILD_EXAMPLES=OFF
        -DWEBPDF_INSTALL=ON
        -DWEBPDF_INSTALL_FONTS=ON
)

vcpkg_cmake_install()

# Move the CMake package config into the vcpkg-standard share location.
vcpkg_cmake_config_fixup(PACKAGE_NAME webpdf CONFIG_PATH lib/cmake/webpdf)

vcpkg_fixup_pkgconfig()

# Remove duplicated debug headers.
file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/include")

file(INSTALL "${SOURCE_PATH}/LICENSE"
    DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}"
    RENAME copyright)
