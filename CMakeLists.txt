cmake_minimum_required(VERSION 3.2)
project(usagi)
install(DIRECTORY include/usagi DESTINATION include)
option(EXAMPLES off)
if ( EXAMPLES )
  include( cmake/flags.c++.std.cmake )
  
  include_directories( include )
  
  include_directories(${CMAKE_CURRENT_BINARY_DIR}/include)
  link_directories(${CMAKE_CURRENT_BINARY_DIR}/lib)
  
  find_package( Boost REQUIRED COMPONENTS coroutine context timer )
  
  add_executable( easy_logger           example/easy_logger.cxx )
  
  add_executable( mutex                 example/mutex.cxx )
  find_package( Threads REQUIRED )
  target_link_libraries( mutex ${CMAKE_THREAD_LIBS_INIT} )
  
  add_executable( circular_tile         example/circular_tile.cxx )
  
  add_executable( wake_ptr              example/wake_ptr.cxx )
  
  add_executable( yieldable_task_system example/yieldable_task_system.cxx )
  include_directories( ${Boost_INCLUDE_DIRS} )
  target_link_libraries( yieldable_task_system ${Boost_LIBRARIES} )
  
  add_executable( jsonrpc20             example/jsonrpc20.cxx )
  include( ExternalProject )
  ExternalProject_Add( external_picojson
    PREFIX            ${CMAKE_CURRENT_BINARY_DIR}/external/picojson
    INSTALL_DIR       ${CMAKE_CURRENT_BINARY_DIR}
    GIT_REPOSITORY    https://github.com/kazuho/picojson.git
    GIT_TAG           master
    CONFIGURE_COMMAND ""
    BUILD_COMMAND     ""
    INSTALL_COMMAND
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_BINARY_DIR}/external/picojson/src/external_picojson/picojson.h
        ${CMAKE_CURRENT_BINARY_DIR}/include/picojson.h
  )
  add_dependencies( jsonrpc20 external_picojson )
endif()
