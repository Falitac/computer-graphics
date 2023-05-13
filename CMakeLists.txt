cmake_minimum_required(VERSION 3.22.1)

project(CG1)

set(SOURCES 
  "src/Main.cpp"
  "src/App.cpp"
  "src/gfx/Shader.cpp"
  "imgui/"
  "imgui/imgui.cpp"
  "imgui/imgui_demo.cpp"
  "imgui/imgui_draw.cpp"
  "imgui/backends/imgui_impl_glfw.cpp"
  "imgui/backends/imgui_impl_opengl3.cpp"
  "imgui/imgui_tables.cpp"
  "imgui/imgui_widgets.cpp"
)

add_executable(CG1 ${SOURCES})

target_include_directories(CG1 PRIVATE "include/")
target_include_directories(CG1 PRIVATE "imgui/")
target_include_directories(CG1 PRIVATE "imgui/backends")

find_package(fmt)
find_package(glfw3)
# find_package(glew)


target_link_libraries(CG1 fmt::fmt)

if(UNIX)
  target_link_libraries(CG1 GLEW)
  target_link_libraries(CG1 GL)
  target_link_libraries(CG1 glfw)
else()
  target_link_libraries(CG1 glfw3)
  target_link_libraries(CG1 glew32)
  target_link_libraries(CG1 opengl32)
endif()