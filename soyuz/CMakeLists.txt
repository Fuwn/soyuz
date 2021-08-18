add_executable(${PROJECT_NAME}
  library.cc
  soyuz.cc
  windows.cc
)

target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/include)

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
  target_compile_options(${PROJECT_NAME} PUBLIC -W3)
else()
  target_compile_options(${PROJECT_NAME} PUBLIC -Wall)
endif()
add_compile_options(-stdlib=libc++)
