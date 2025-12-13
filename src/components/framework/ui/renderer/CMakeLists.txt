cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

set(target qor_renderer)

add_library(${target} SHARED qor_renderer.cpp
common/math.cpp
common/colour.cpp
common/compressor.cpp
common/str.cpp
sw/stroke.cpp
sw/shape.cpp
sw/rle.cpp
sw/fill.cpp
sw/image.cpp
sw/math.cpp
sw/posteffect.cpp
sw/raster.cpp
sw/renderer.cpp
sw/mempool.cpp
paint.cpp
shape.cpp
render.cpp
picture.cpp
scene.cpp
text.cpp
fill.cpp
canvas.cpp
animation.cpp
accessor.cpp
taskscheduler.cpp
rawloader.cpp
loader.cpp
gl/effect.cpp
gl/geometry.cpp
gl/gl.cpp
gl/gpubuffer.cpp
gl/program.cpp
gl/renderer.cpp
gl/renderpass.cpp
gl/rendertask.cpp
gl/rendertarget.cpp
gl/shader.cpp
gl/shadersrc.cpp
gl/tessellator.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE QOR_RENDERER)
target_link_libraries(${target} qor_module qor_datastructures qor_objectcontext qor_role qor_application qor_workflow qor_signals qor_error
qor_property)

include ("${CMAKE_SOURCE_DIR}/cmake/postbuild.cmake")

