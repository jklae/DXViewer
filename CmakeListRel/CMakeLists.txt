CMAKE_MINIMUM_REQUIRED(VERSION 3.19.0)

# Set the project name
SET( PROJECT_NAME DXViewerRel )
PROJECT(${PROJECT_NAME})

# Define character set as Unicode
add_definitions(-DUNICODE -D_UNICODE)

# Collect shader files
FILE( GLOB VS_SHD ${CMAKE_SOURCE_DIR}/shader/vertexShader.hlsl )
FILE( GLOB PS_SHD ${CMAKE_SOURCE_DIR}/shader/fragShader.hlsl )

# Set the shader type of files
set_property(SOURCE ${VS_SHD} PROPERTY VS_SHADER_TYPE Vertex)
set_property(SOURCE ${PS_SHD} PROPERTY VS_SHADER_TYPE Pixel)

# Set the header directory
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/DXViewer-1.0.0/include)

# Collect source files
FILE( GLOB SRC ${CMAKE_SOURCE_DIR}/src/*.cpp )
FILE( GLOB HDR ${CMAKE_SOURCE_DIR}/DXViewer-1.0.0/include/*.h )
FILE( GLOB SHD ${VS_SHD} ${PS_SHD} )

# Link Source files
ADD_LIBRARY( ${PROJECT_NAME} SHARED ${SRC} ${HDR} ${SHD} )