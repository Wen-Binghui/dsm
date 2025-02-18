#
# This file is part of DSM.
#
# Copyright (C) 2019 CEIT (Universidad de Navarra) and Universidad de Zaragoza
# Developed by Jon Zubizarreta,
# for more information see <https://github.com/jzubizarreta/dsm>.
# If you use this code, please cite the respective publications as
# listed on the above website.
#
# DSM is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# DSM is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with DSM. If not, see <http://www.gnu.org/licenses/>.
#

cmake_minimum_required(VERSION 3.5)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
get_filename_component(PARENT_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)

# Build Flags
include(BuildFlags)
set(CMAKE_PREFIX_PATH "${PARENT_DIR}/cmake_modules/eigen3" ${CMAKE_PREFIX_PATH})
set(EIGEN_ROOT "${PARENT_DIR}/thirdparty/eigen")

# Compiler Flags
include(CompilerFlags)

message(STATUS "PARENT_DIR: ${PARENT_DIR}")
message(STATUS "EIGEN_ROOT ${CMAKE_MODULE_PATH}")

# Find the dependencies
find_package(Eigen3 REQUIRED)

set(EIGEN3_INCLUDE_DIR_HINTS ${EIGEN_ROOT}) # for opengv / pangolin
find_package(Eigen3 3.3.8 EXACT REQUIRED CONFIG) # find eigen with our adapted "config module", that uses our eigen source tree
set(EIGEN_INCLUDE_DIR ${EIGEN3_INCLUDE_DIR}) # for opengv / pangolin
include_directories(${EIGEN3_INCLUDE_DIR})
message(STATUS "Found Eigen headers in: ${EIGEN3_INCLUDE_DIR}")

find_package(OpenCV REQUIRED)

# find_package(Ceres REQUIRED)
set(CERES_ROOT "${PARENT_DIR}/thirdparty/build-ceres-solver")
find_package(Ceres 2.0 REQUIRED CONFIG HINTS "${CERES_ROOT}" NO_DEFAULT_PATH)
message(STATUS "CERES_ROOT: ${CERES_ROOT} + ${CERES_INCLUDE_DIRS}")
message(STATUS "---")

# Set the name of the Library
set(LIBRARY_NAME dsm)

# Current library
set(root ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Define the header and source files of the library. Also for Qt based projects
# define the form and resource files of the project
set(HEADER_FILES
	${root}/Utils/sse.h
	${root}/Utils/Settings.h
	${root}/Utils/Interpolation.h
	${root}/Utils/UtilFunctions.h
	${root}/Utils/LossFunction.h
	${root}/Utils/GlobalCalibration.h
	${root}/Utils/Projection.h
	${root}/Utils/EigenTypes.h
	${root}/Utils/Undistorter.h
	${root}/Utils/EurocReader.h
	${root}/Utils/TUMReader.h
	${root}/Utils/Kernel.h
	${root}/Thread/WorkerThread.h
	${root}/Thread/WorkerThreadPool.h
	${root}/Memory/Buffer.h
	${root}/Memory/BufferPool.h
	${root}/DataStructures/AffineLight.h
	${root}/DataStructures/ImagePyramid.h
	${root}/DataStructures/GradientPyramid.h
	${root}/DataStructures/Frame.h
	${root}/DataStructures/CandidatePoint.h
	${root}/DataStructures/ActivePoint.h
	${root}/DataStructures/Pattern.h
	${root}/DataStructures/CovisibilityGraph.h
	${root}/DataStructures/Visibility.h
	${root}/Statistics/NelderMead.h
	${root}/Statistics/Statistics.h
	${root}/Statistics/Tail.h
	${root}/Statistics/BetaFunction.h
	${root}/Statistics/IDistribution.h
	${root}/Statistics/RobustNormalDistribution.h
	${root}/Statistics/TDistribution.h
	${root}/Tracking/FrameTracker.h
	${root}/Tracking/FrameTrackerProblem.h
	${root}/Tracking/FrameTrackerReference.h
	${root}/LeastSquares/IParameterBlock.h
	${root}/LeastSquares/IResidualBlock.h
	${root}/LeastSquares/LeastSquaresProblem.h
	${root}/Optimization/PointParameterBlock.h
	${root}/Optimization/PhotometricResidual.h
	${root}/Optimization/PhotometricBAIterationCallback.h
	${root}/Optimization/PhotometricBA.h
	${root}/Optimization/PhotometricBAConfig.h
	${root}/Optimization/FrameParameterBlock.h
	${root}/OptimizationStructures/MatrixBlock.h
	${root}/OptimizationStructures/MatrixBlock.hpp
	${root}/OptimizationStructures/MatrixBlock_8_8.hpp
	${root}/OptimizationStructures/VectorBlock.h
	${root}/OptimizationStructures/VectorBlock.hpp
	${root}/OptimizationStructures/VectorBlock_8.hpp
	${root}/FullSystem/PointDetector.h
	${root}/FullSystem/DistanceTransform.h
	${root}/FullSystem/LMCW.h
	${root}/FullSystem/FullSystem.h
	${root}/FullSystem/Log.h
	${root}/FullSystem/DSMLib.h
	${root}/Initializer/MonoInitializer.h
	${root}/Initializer/ModelSelector.h
	${root}/Visualizer/IVisualizer.h
)

set(SOURCE_FILES
	${root}/Utils/Settings.cpp
	${root}/Utils/GlobalCalibration.cpp
	${root}/Utils/Undistorter.cpp
	${root}/Utils/EurocReader.cpp
	${root}/Utils/TUMReader.cpp
	${root}/DataStructures/AffineLight.cpp
	${root}/DataStructures/Frame.cpp
	${root}/DataStructures/CandidatePoint.cpp
	${root}/DataStructures/ActivePoint.cpp
	${root}/DataStructures/Pattern.cpp
	${root}/DataStructures/CovisibilityGraph.cpp
	${root}/Statistics/NelderMead.cpp
	${root}/Statistics/BetaFunction.cpp
	${root}/Statistics/IDistribution.cpp
	${root}/Statistics/RobustNormalDistribution.cpp
	${root}/Statistics/TDistribution.cpp
	${root}/Tracking/FrameTracker.cpp
	${root}/Tracking/FrameTrackerProblem.cpp
	${root}/Tracking/FrameTrackerReference.cpp
	${root}/LeastSquares/IResidualBlock.cpp
	${root}/LeastSquares/LeastSquaresProblem.cpp
	${root}/Optimization/PointParameterBlock.cpp
	${root}/Optimization/PhotometricResidual.cpp
	${root}/Optimization/PhotometricBAIterationCallback.cpp
	${root}/Optimization/PhotometricBA.cpp
	${root}/Optimization/PhotometricBAConfig.cpp
	${root}/Optimization/FrameParameterBlock.cpp
	${root}/FullSystem/PointDetector.cpp
	${root}/FullSystem/DistanceTransform.cpp
	${root}/FullSystem/LMCW.cpp
	${root}/FullSystem/FullSystem.cpp
	${root}/FullSystem/Log.cpp
	${root}/FullSystem/DSMLib.cpp
	${root}/Initializer/MonoInitializer.cpp
	${root}/Initializer/ModelSelector.cpp
	${root}/Visualizer/IVisualizer.cpp
)

# Organize the source and header files into groups
source_group("Headers" FILES ${HEADER_FILES})
source_group("Source" FILES ${SOURCE_FILES})
source_group("Forms" FILES ${FORM_FILES})
source_group("Resources" FILES ${RESOURCE_FILES})

if(CMAKE_VERSION VERSION_GREATER 3.8)
	source_group(TREE ${root} FILES ${HEADER_FILES} ${SOURCE_FILES})
endif()

# Define the folder containing the header files for this library
set(DSM_LIB_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src PARENT_SCOPE)

# Define the include directory search file for this project
include_directories(${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_BINARY_DIR} # required to include "BuildFlags.h"
	${CMAKE_CURRENT_SOURCE_DIR}/src
	${CMAKE_SOURCE_DIR}/thirdparty/Sophus
	${EIGEN3_INCLUDE_DIR}
	${CERES_INCLUDE_DIRS}
)

# Add the library
add_library(${LIBRARY_NAME} SHARED
	${SOURCE_FILES}
	${HEADER_FILES}
	${FORM_HEADERS}
	${HEADERS_MOC}
	${RESOURCES_RCC})

# Define the link libraries used in the project
target_link_libraries(${LIBRARY_NAME}
	${OpenCV_LIBS}
	${CERES_LIBRARIES})

set_target_properties(${LIBRARY_NAME} PROPERTIES FOLDER "DSM")
