cmake_minimum_required(VERSION 3.10)

project(WRF-POST-PROCESSING-ADIOS2 VERSION 1.0 LANGUAGES C CXX)

find_package(ADIOS2 REQUIRED)
find_package(MPI REQUIRED COMPONENTS C CXX)

add_executable(postprocessing main.cpp)

target_include_directories(postprocessing PRIVATE
        ${ADIOS2_INCLUDE_DIRS}
        ${MPI_CXX_INCLUDE_PATH})
target_link_libraries(postprocessing PRIVATE
        ${ADIOS2_LIBRARIES}
        ${MPI_CXX_LIBRARIES}
        MPI::MPI_CXX)
