CMAKE_MINIMUM_REQUIRED(VERSION 3.19.0)

# Set the project name
SET( PROJECT_NAME DXViewer )
PROJECT(${PROJECT_NAME})

# Define character set as Unicode
add_definitions(-DUNICODE -D_UNICODE)

# Collect shader files
FILE( GLOB VS_SHD shader/vertexShader.hlsl )
FILE( GLOB PS_SHD shader/fragShader.hlsl )

# Set the shader type of files
set_property(SOURCE ${VS_SHD} PROPERTY VS_SHADER_TYPE Vertex)
set_property(SOURCE ${PS_SHD} PROPERTY VS_SHADER_TYPE Pixel)

# Collect source files
FILE( GLOB SRC src/*.cpp)
FILE( GLOB HDR src/*.h )
FILE( GLOB SHD ${VS_SHD} ${PS_SHD} )

# Link Source files ('WIN32' keyword is used for win32 project)
ADD_EXECUTABLE( ${PROJECT_NAME} WIN32 ${SRC} ${HDR} ${SHD} )