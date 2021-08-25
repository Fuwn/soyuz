add_executable(${PROJECT_NAME} WIN32
  tray.cc
  library.cc
  ../resource/resource.rc
  soyuz.cc
  windows.cc
)

target_include_directories(${PROJECT_NAME} PRIVATE
  ${PROJECT_SOURCE_DIR}/include
  ${PROJECT_SOURCE_DIR}/deps/include
  ${PROJECT_SOURCE_DIR}/include/soyuz
)

set_target_properties(${PROJECT_NAME} PROPERTIES
  CXX_STANDARD 20
  CXX_STANDARD_REQUIRED ON
  CMAKE_CXX_EXTENSIONS OFF
)
target_compile_features(${PROJECT_NAME} PUBLIC
  cxx_std_20
  cxx_return_type_deduction
)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  target_compile_options(${PROJECT_NAME} PUBLIC /W4 /WX) # /Wall
else()
  target_compile_options(${PROJECT_NAME} PUBLIC -Wall -Wextra -pedantic -Werror)
endif()
add_compile_options(-stdlib=libc++)

find_package(fmt)
target_link_libraries(${PROJECT_NAME} PRIVATE fmt)
