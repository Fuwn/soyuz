cmake_minimum_required(VERSION 3.20)
project(soyuz)

option(DOWNLOAD_EXTERNAL_DEPENDENCIES "Download external dependencies" ON)

if (DOWNLOAD_EXTERNAL_DEPENDENCIES)
  find_package(Git)
  if (Git_FOUND)
    include(build_tools/cmake/DownloadProject.cmake)

    download_project(
      PROJ                fmt
      GIT_REPOSITORY      https://github.com/fmtlib/fmt.git
      GIT_TAG             master
      SOURCE_DIR          ${PROJECT_SOURCE_DIR}/deps/fmt
      UPDATE_DISCONNECTED 1
    )
  else ()
    string(CONCAT GIT_NOT_FOUND_WARNING
      "Git was not found on your system: skipping downloading external dependencies"
      "This will likely lead to Soyuz being unable to compile"
      "Consider installing or fixing Git"
    )
    message(WARING GIT_NOT_FOUND_WARNING)
  endif ()
else ()
  string(CONCAT DOWNLOAD_EXTERNAL_DEPENDENCIES_WARNING
    "Skipping downloading external dependencies"
    "This will likely lead to Soyuz being unable to compile"
    "Consider turning DOWNLOAD_EXTERNAL_DEPENDENCIES ON"
  )
  message(WARING DOWNLOAD_EXTERNAL_DEPENDENCIES_WARNING)
endif ()

add_subdirectory(${PROJECT_NAME})
