CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(runTimePostProcessing)

#FIND_PACKAGE(VTK COMPONENTS
#    vtkCommonCore
#    vtkFiltersGeneral
#    vtkRenderingOpenGL
#)

FIND_PACKAGE(ParaView REQUIRED)

INCLUDE(${VTK_USE_FILE})


LINK_DIRECTORIES(
    $ENV{FOAM_LIBBIN}
    $ENV{FOAM_EXT_LIBBIN}
)

INCLUDE_DIRECTORIES(
    $ENV{WM_PROJECT_DIR}/src/OpenFOAM/lnInclude
    $ENV{WM_PROJECT_DIR}/src/OSspecific/$ENV{WM_OSTYPE}/lnInclude
    $ENV{WM_PROJECT_DIR}/src/triSurface/lnInclude
    $ENV{WM_PROJECT_DIR}/src/finiteVolume/lnInclude
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)

ADD_DEFINITIONS(
    -DWM_$ENV{WM_PRECISION_OPTION}
)

# Set output library destination to plugin directory
SET(
    LIBRARY_OUTPUT_PATH $ENV{FOAM_USER_LIBBIN}
    CACHE INTERNAL
    ""
)

ADD_DEFINITIONS(
    -DWM_$ENV{WM_PRECISION_OPTION}
    -DNoRepository
)

FILE(GLOB SOURCE_FILES
    fieldVisualisationBase.C
    functionObjectLine.C
    functionObjectSurface.C
    geometryBase.C
    geometrySurface.C
    pathline.C
    runTimePostProcessing.C
    runTimePostProcessingFunctionObject.C
    scene.C
    surface.C
    text.C
)

SET(OPENFOAM_LIBRARIES
    OpenFOAM
    triSurface
    finiteVolume
)

ADD_LIBRARY(
    runTimePostProcessing
    SHARED
    ${SOURCE_FILES}
)

TARGET_LINK_LIBRARIES(
    runTimePostProcessing
    ${VTK_LIBRARIES}
    ${OPENFOAM_LIBRARIES}
)

SET(CMAKE_BUILD_TYPE Debug)
SET(CMAKE_CXX_FLAGS_DEBUG
     "-g -O0 -Wall -Wextra -Wno-unused-parameter -Wnon-virtual-dtor -Wno-overloaded-virtual -Wno-unused-comparison"
)


#-----------------------------------------------------------------------------
