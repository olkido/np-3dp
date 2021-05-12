cmake_minimum_required(VERSION 3.1)
project(np-3dp)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# Add your project files
file(GLOB SRCFILES
src/*.cpp
../stripe/src/*.cpp
)
# Remove stripe's main
list(FILTER SRCFILES EXCLUDE REGEX "../stripe/src/main.cpp$")
file(GLOB HDRFILES
include/*.hpp include/*.h
../stripe/include/*.hpp ../stripe/include/*.h ../stripe/src/*.inl
)
add_executable(${PROJECT_NAME} ${SRCFILES} ${HDRFILES})

include_directories(
include/
../stripe/include/
../stripe/src/
)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_LINK_FLAGS "-framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo -framework Accelerate -framework GLUT ")

option(REBUILD_IGL "re-build igl within folder" OFF) #OFF by default
if(REBUILD_IGL)
 # libigl
 option(LIBIGL_WITH_OPENGL            "Use OpenGL"         ON)
 option(LIBIGL_WITH_OPENGL_GLFW       "Use GLFW"           ON)
 option(LIBIGL_WITH_OPENGL_GLFW_IMGUI "Use ImGui"          ON)

 option(LIBIGL_USE_STATIC_LIBRARY "Use libigl as static library" ON)
 set(LIBIGL_INCLUDE_DIR ${LIBIGL_DIR}/include)
 find_package(LIBIGL REQUIRED QUIET)
 list(APPEND LIBRARIES igl::core igl::opengl_glfw igl::opengl igl::opengl_glfw_imgui)
else()
  # add libIGL
  # include
  list(APPEND INCLUDE_DIRS
  ${LIBIGL_DIR}/include
  )
  # libs
  list(APPEND LIBRARIES
    ${LIBIGL_DIR}/lib/libigl.a
    ${LIBIGL_DIR}/lib/libigl_opengl_glfw_imgui.a
  )

  # add Eigen
  # include
  list(APPEND INCLUDE_DIRS
  ${LIBIGL_DIR}/external/eigen
  )

  # add GLAD
  # include
  list(APPEND INCLUDE_DIRS
  ${LIBIGL_DIR}/external/glad/include
  )
  list(APPEND LIBRARIES
    ${LIBIGL_DIR}/lib/libglad.a
  )

  # add GLFW
  # include
  list(APPEND INCLUDE_DIRS
  ${LIBIGL_DIR}/external/glfw/include
  )
  list(APPEND LIBRARIES
    ${LIBIGL_DIR}/lib/libglfw3.a
  )

  # add ImGui
  # include
  list(APPEND INCLUDE_DIRS
    ${LIBIGL_DIR}/external/
    ${LIBIGL_DIR}/external/imgui/
    ${LIBIGL_DIR}/external/imgui/backends
    ${LIBIGL_DIR}/external/libigl-imgui
  )
  list(APPEND LIBRARIES
    ${LIBIGL_DIR}/lib/libimgui.a
  )

endif(REBUILD_IGL)
unset(REBUILD_IGL CACHE) # <---- this is the important!!


find_package(SuiteSparse REQUIRED)
list(APPEND INCLUDE_DIRS
${SUITESPARSE_INCLUDE_DIRS}
)
list(APPEND LIBRARIES
  ${SUITESPARSE_LIBRARIES}
)
find_package(UMFPACK REQUIRED)
list(APPEND INCLUDE_DIRS
${UMFPACK_INCLUDE_DIR}
)
list(APPEND LIBRARIES
  ${UMFPACK_LIBRARIES}
)

# set(DDG_INCLUDE_PATH
# "-I/usr/local/include/"
# )
# set(DDG_LIBRARY_PATH
# "-L/usr/local/lib/"
# )
# set(DDG_BLAS_LIBS
# "-framework Accelerate"
# )
# set(DDG_SUITESPARSE_LIBS
# "-lspqr -lumfpack -lcholmod -lmetis -lcolamd -lccolamd -lcamd -lamd -lm -lsuitesparseconfig"
# )
# set(DDG_OPENGL_LIBS
# "-framework OpenGL -framework GLUT"
# )
# set(CMAKE_CXX_FLAGS
# " ${CMAKE_CXX_FLAGS}
#  ${DDG_INCLUDE_PATH}
# ")
# set(CMAKE_CXX_LINK_FLAGS
#   " ${CMAKE_CXX_LINK_FLAGS}
#    ${DDG_LIBRARY_PATH}
#    ${DDG_BLAS_LIBS}
#    ${DDG_SUITESPARSE_LIBS}
#    ${DDG_OPENGL_LIBS}
# ")
message(${LIBRARIES})
include_directories( ${INCLUDE_DIRS} )
target_link_libraries(${PROJECT_NAME} ${LIBRARIES})
