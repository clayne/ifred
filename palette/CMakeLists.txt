cmake_minimum_required(VERSION 3.8)

include("ida-cmake/cmake/QtIDA.cmake")
include("ida-cmake/cmake/IDA.cmake")

# Some required settings
set(PYTHON_SUPPORT OFF CACHE BOOL "")
set(PYTHON_EXECUTABLE "" CACHE PATH "Python executable (x64 if >= 7.0)")

include(GenerateExportHeader)

if (${PYTHON_SUPPORT})
    set(python_binding_src src/bindings/pypalette.cpp)
    set(python_libraries pybind11::module)
    set(python_support_definitions "PYTHON_SUPPORT=1")
else ()
    set(python_binding_src)
    set(python_libraries)
    set(python_support_definitions "PYTHON_SUPPORT=0")
endif ()

set(sources src/search_services/basic_service.cpp
        "src/widgets/palette.cpp"
        "src/filter.cpp"
        "src/widgets/items.cpp"
        "src/widgets/item.cpp"
        ${python_binding_src}
        src/utils.cpp
        "src/api.cpp"
        res/theme_bundle.qrc
        "include/palette/search_services/basic_service.h"
        "include/palette/filter.h"
        "include/palette/widgets/palette.h"
        "include/palette/action.h"
        "include/palette/widgets.h"
        )

add_library(palette STATIC ${sources})
target_link_libraries(palette PRIVATE Qt5::Core Qt5::Gui Qt5::Widgets ${python_libraries})
target_compile_definitions(palette PUBLIC "QT_NAMESPACE=QT" ${python_support_definitions})

target_include_directories(palette
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        ${CMAKE_BINARY_DIR}/palette
        PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/palette>
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        )

GENERATE_EXPORT_HEADER(palette)

# IDA plugins
add_subdirectory(src/ida)