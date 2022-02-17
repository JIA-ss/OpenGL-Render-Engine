# Install script for directory: F:/StudyProj/openGLStudy/review/libs/assimp-master/code

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/Assimp")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "F:/StudyProj/openGLStudy/review/libs/assimp-master/lib/Debug/assimp-vc142-mtd.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "F:/StudyProj/openGLStudy/review/libs/assimp-master/lib/Release/assimp-vc142-mt.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "F:/StudyProj/openGLStudy/review/libs/assimp-master/lib/MinSizeRel/assimp-vc142-mt.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "F:/StudyProj/openGLStudy/review/libs/assimp-master/lib/RelWithDebInfo/assimp-vc142-mt.lib")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "F:/StudyProj/openGLStudy/review/libs/assimp-master/bin/Debug/assimp-vc142-mtd.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "F:/StudyProj/openGLStudy/review/libs/assimp-master/bin/Release/assimp-vc142-mt.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "F:/StudyProj/openGLStudy/review/libs/assimp-master/bin/MinSizeRel/assimp-vc142-mt.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "F:/StudyProj/openGLStudy/review/libs/assimp-master/bin/RelWithDebInfo/assimp-vc142-mt.dll")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xassimp-devx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/assimp" TYPE FILE FILES
    "F:/StudyProj/openGLStudy/review/libs/assimp-master/code/../include/assimp/anim.h"
    "F:/StudyProj/openGLStudy/review/libs/assimp-master/code/../include/assimp/aabb.h"
    "F:/StudyProj/openGLStudy/review/libs/assimp-master/code/../include/assimp/ai_assert.h"
    "F:/StudyProj/openGLStudy/review/libs/assimp-master/code/../include/assimp/camera.h"
    "F:/StudyProj/openGLStudy/review/libs/assimp-master/code/../include/assimp/color4.h"
    "F:/StudyProj/openGLStudy/review/libs/assimp-master/code/../include/assimp/color4.inl"
    "F:/StudyProj/openGLStudy/review/libs/assimp-master/code/../include/assimp/config.h"
    "F:/StudyProj/openGLStudy/review/libs/assimp-master/code/../include/assimp/ColladaMetaData.h"
    "F:/StudyProj/openGLStudy/review/libs/assimp-master/code/../include/assimp/commonMetaData.h"
    "F:/StudyProj/openGLStudy/review/libs/assimp-master/code/../include/assimp/defs.h"
    "F:/StudyProj/openGLStudy/review/libs/assimp-master/code/../include/assimp/cfileio.h"
    "F:/StudyProj/openGLStudy/review/libs/assimp-master/code/../include/assimp/light.h"
    "F:/StudyProj/openGLStudy/review/libs/assimp-master/code/../include/assimp/material.h"
    "F:/StudyProj/openGLStudy/review/libs/assimp-master/code/../include/assimp/material.inl"
    "F:/StudyProj/openGLStudy/review/libs/assimp-master/code/../include/assimp/matrix3x3.h"
    "F:/StudyProj/openGLStudy/review/libs/assimp-master/code/../include/assimp/matrix3x3.inl"
    "F:/StudyProj/openGLStudy/review/libs/assimp-master/code/../include/assimp/matrix4x4.h"
    "F:/StudyProj/openGLStudy/review/libs/assimp-master/code/../include/assimp/matrix4x4.inl"
    "F:/StudyProj/openGLStudy/review/libs/assimp-master/code/../include/assimp/mesh.h"
    "F:/StudyProj/openGLStudy/review/libs/assimp-master/code/../include/assimp/pbrmaterial.h"
    "F:/StudyProj/openGLStudy/review/libs/assimp-master/code/../include/assimp/GltfMaterial.h"
    "F:/StudyProj/openGLStudy/review/libs/assimp-master/code/../include/assimp/postprocess.h"
    "F:/StudyProj/openGLStudy/review/libs/assimp-master/code/../include/assimp/quaternion.h"
    "F:/StudyProj/openGLStudy/review/libs/assimp-master/code/../include/assimp/quaternion.inl"
    "F:/StudyProj/openGLStudy/review/libs/assimp-master/code/../include/assimp/scene.h"
    "F:/StudyProj/openGLStudy/review/libs/assimp-master/code/../include/assimp/metadata.h"
    "F:/StudyProj/openGLStudy/review/libs/assimp-master/code/../include/assimp/texture.h"
    "F:/StudyProj/openGLStudy/review/libs/assimp-master/code/../include/assimp/types.h"
    "F:/StudyProj/openGLStudy/review/libs/assimp-master/code/../include/assimp/vector2.h"
    "F:/StudyProj/openGLStudy/review/libs/assimp-master/code/../include/assimp/vector2.inl"
    "F:/StudyProj/openGLStudy/review/libs/assimp-master/code/../include/assimp/vector3.h"
    "F:/StudyProj/openGLStudy/review/libs/assimp-master/code/../include/assimp/vector3.inl"
    "F:/StudyProj/openGLStudy/review/libs/assimp-master/code/../include/assimp/version.h"
    "F:/StudyProj/openGLStudy/review/libs/assimp-master/code/../include/assimp/cimport.h"
    "F:/StudyProj/openGLStudy/review/libs/assimp-master/code/../include/assimp/importerdesc.h"
    "F:/StudyProj/openGLStudy/review/libs/assimp-master/code/../include/assimp/Importer.hpp"
    "F:/StudyProj/openGLStudy/review/libs/assimp-master/code/../include/assimp/DefaultLogger.hpp"
    "F:/StudyProj/openGLStudy/review/libs/assimp-master/code/../include/assimp/ProgressHandler.hpp"
    "F:/StudyProj/openGLStudy/review/libs/assimp-master/code/../include/assimp/IOStream.hpp"
    "F:/StudyProj/openGLStudy/review/libs/assimp-master/code/../include/assimp/IOSystem.hpp"
    "F:/StudyProj/openGLStudy/review/libs/assimp-master/code/../include/assimp/Logger.hpp"
    "F:/StudyProj/openGLStudy/review/libs/assimp-master/code/../include/assimp/LogStream.hpp"
    "F:/StudyProj/openGLStudy/review/libs/assimp-master/code/../include/assimp/NullLogger.hpp"
    "F:/StudyProj/openGLStudy/review/libs/assimp-master/code/../include/assimp/cexport.h"
    "F:/StudyProj/openGLStudy/review/libs/assimp-master/code/../include/assimp/Exporter.hpp"
    "F:/StudyProj/openGLStudy/review/libs/assimp-master/code/../include/assimp/DefaultIOStream.h"
    "F:/StudyProj/openGLStudy/review/libs/assimp-master/code/../include/assimp/DefaultIOSystem.h"
    "F:/StudyProj/openGLStudy/review/libs/assimp-master/code/../include/assimp/ZipArchiveIOSystem.h"
    "F:/StudyProj/openGLStudy/review/libs/assimp-master/code/../include/assimp/SceneCombiner.h"
    "F:/StudyProj/openGLStudy/review/libs/assimp-master/code/../include/assimp/fast_atof.h"
    "F:/StudyProj/openGLStudy/review/libs/assimp-master/code/../include/assimp/qnan.h"
    "F:/StudyProj/openGLStudy/review/libs/assimp-master/code/../include/assimp/BaseImporter.h"
    "F:/StudyProj/openGLStudy/review/libs/assimp-master/code/../include/assimp/Hash.h"
    "F:/StudyProj/openGLStudy/review/libs/assimp-master/code/../include/assimp/MemoryIOWrapper.h"
    "F:/StudyProj/openGLStudy/review/libs/assimp-master/code/../include/assimp/ParsingUtils.h"
    "F:/StudyProj/openGLStudy/review/libs/assimp-master/code/../include/assimp/StreamReader.h"
    "F:/StudyProj/openGLStudy/review/libs/assimp-master/code/../include/assimp/StreamWriter.h"
    "F:/StudyProj/openGLStudy/review/libs/assimp-master/code/../include/assimp/StringComparison.h"
    "F:/StudyProj/openGLStudy/review/libs/assimp-master/code/../include/assimp/StringUtils.h"
    "F:/StudyProj/openGLStudy/review/libs/assimp-master/code/../include/assimp/SGSpatialSort.h"
    "F:/StudyProj/openGLStudy/review/libs/assimp-master/code/../include/assimp/GenericProperty.h"
    "F:/StudyProj/openGLStudy/review/libs/assimp-master/code/../include/assimp/SpatialSort.h"
    "F:/StudyProj/openGLStudy/review/libs/assimp-master/code/../include/assimp/SkeletonMeshBuilder.h"
    "F:/StudyProj/openGLStudy/review/libs/assimp-master/code/../include/assimp/SmallVector.h"
    "F:/StudyProj/openGLStudy/review/libs/assimp-master/code/../include/assimp/SmoothingGroups.h"
    "F:/StudyProj/openGLStudy/review/libs/assimp-master/code/../include/assimp/SmoothingGroups.inl"
    "F:/StudyProj/openGLStudy/review/libs/assimp-master/code/../include/assimp/StandardShapes.h"
    "F:/StudyProj/openGLStudy/review/libs/assimp-master/code/../include/assimp/RemoveComments.h"
    "F:/StudyProj/openGLStudy/review/libs/assimp-master/code/../include/assimp/Subdivision.h"
    "F:/StudyProj/openGLStudy/review/libs/assimp-master/code/../include/assimp/Vertex.h"
    "F:/StudyProj/openGLStudy/review/libs/assimp-master/code/../include/assimp/LineSplitter.h"
    "F:/StudyProj/openGLStudy/review/libs/assimp-master/code/../include/assimp/TinyFormatter.h"
    "F:/StudyProj/openGLStudy/review/libs/assimp-master/code/../include/assimp/Profiler.h"
    "F:/StudyProj/openGLStudy/review/libs/assimp-master/code/../include/assimp/LogAux.h"
    "F:/StudyProj/openGLStudy/review/libs/assimp-master/code/../include/assimp/Bitmap.h"
    "F:/StudyProj/openGLStudy/review/libs/assimp-master/code/../include/assimp/XMLTools.h"
    "F:/StudyProj/openGLStudy/review/libs/assimp-master/code/../include/assimp/IOStreamBuffer.h"
    "F:/StudyProj/openGLStudy/review/libs/assimp-master/code/../include/assimp/CreateAnimMesh.h"
    "F:/StudyProj/openGLStudy/review/libs/assimp-master/code/../include/assimp/XmlParser.h"
    "F:/StudyProj/openGLStudy/review/libs/assimp-master/code/../include/assimp/BlobIOSystem.h"
    "F:/StudyProj/openGLStudy/review/libs/assimp-master/code/../include/assimp/MathFunctions.h"
    "F:/StudyProj/openGLStudy/review/libs/assimp-master/code/../include/assimp/Exceptional.h"
    "F:/StudyProj/openGLStudy/review/libs/assimp-master/code/../include/assimp/ByteSwapper.h"
    "F:/StudyProj/openGLStudy/review/libs/assimp-master/code/../include/assimp/Base64.hpp"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xassimp-devx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/assimp/Compiler" TYPE FILE FILES
    "F:/StudyProj/openGLStudy/review/libs/assimp-master/code/../include/assimp/Compiler/pushpack1.h"
    "F:/StudyProj/openGLStudy/review/libs/assimp-master/code/../include/assimp/Compiler/poppack1.h"
    "F:/StudyProj/openGLStudy/review/libs/assimp-master/code/../include/assimp/Compiler/pstdint.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES "F:/StudyProj/openGLStudy/review/libs/assimp-master/code/Debug/assimp-vc142-mtd.pdb")
  endif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES "F:/StudyProj/openGLStudy/review/libs/assimp-master/code/RelWithDebInfo/assimp-vc142-mt.pdb")
  endif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
endif()

