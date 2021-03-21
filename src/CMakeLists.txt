file(GLOB_RECURSE ProjectSRC "*.h" "*.cpp")

add_executable(CPU_PathTracer ${ProjectSRC})
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT CPU_PathTracer)

target_link_libraries(CPU_PathTracer PRIVATE glfw GLAD glm::glm assimp)
target_include_directories(CPU_PathTracer PRIVATE ${glm_SOURCE_DIR})

set_target_properties(CPU_PathTracer PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "bin")
set_target_properties(CPU_PathTracer PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE "bin")
set_property(TARGET CPU_PathTracer PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "bin")