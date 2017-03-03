cmake_minimum_required(VERSION 3.2)

project(usagi)

install(DIRECTORY include/usagi DESTINATION include)

option(EXAMPLES off)

set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )

if ( EXAMPLES )
  include( cmake/flags.c++.std.cmake )
  
  include_directories( include )
  
  include_directories(${CMAKE_CURRENT_BINARY_DIR}/include)
  link_directories(${CMAKE_CURRENT_BINARY_DIR}/lib)
  
  find_package( Boost REQUIRED COMPONENTS coroutine context timer filesystem system )
  
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
  
  ExternalProject_Add( picojson
    EXCLUDE_FROM_ALL  1
    PREFIX            ${CMAKE_CURRENT_BINARY_DIR}/ExternalProject
    INSTALL_DIR       ${CMAKE_CURRENT_BINARY_DIR}
    GIT_REPOSITORY    https://github.com/kazuho/picojson.git
    GIT_TAG           master
    CONFIGURE_COMMAND ""
    BUILD_COMMAND     ""
    INSTALL_COMMAND
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_BINARY_DIR}/ExternalProject/src/picojson/picojson.h
        ${CMAKE_CURRENT_BINARY_DIR}/include/picojson.h
  )
  
  add_dependencies( jsonrpc20 picojson )
  
  if(WIN32)
    
    set(LIBRESSL_VERSION 2.5.1)
    set(LIBRESSL_LIBCRYPTO_WIN32_SUFFIX -41)
    set(LIBRESSL_LIBSSL_WIN32_SUFFIX -43)
    set(LIBRESSL_LIBTLS_WIN32_SUFFIX -15)
    
    set(LIBRESSL_HASH 1b3eb8c8e5507c553dac0a85163e09669b27619396b98c4a747a9a02b9a40603)
    
    string(REPLACE / \\ CMAKE_BINARY_DIR_WIN32 ${CMAKE_BINARY_DIR})
    set(LIBRESSL_LIBCRYPTO_LIBRARY ${CMAKE_BINARY_DIR_WIN32}\\bin\\libcrypto${LIBRESSL_LIBCRYPTO_WIN32_SUFFIX}.dll)
    set(LIBRESSL_LIBSSL_LIBRARY ${CMAKE_BINARY_DIR_WIN32}\\bin\\libssl${LIBRESSL_LIBSSL_WIN32_SUFFIX}.dll)
    set(LIBRESSL_LIBTLS_LIBRARY ${CMAKE_BINARY_DIR_WIN32}\\bin\\libtls${LIBRESSL_LIBTLS_WIN32_SUFFIX}.dll)
    
    if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
      set( libressl_sub_directory x64 )
    elseif( CMAKE_SIZEOF_VOID_P EQUAL 4 )
      set( libressl_sub_directory x86 )
    else()
      message( FATAL_ERROR "unknown architeccture" )
    endif()
    
    ExternalProject_Add( libressl
      EXCLUDE_FROM_ALL 1
      PREFIX            ${CMAKE_CURRENT_BINARY_DIR}/ExternalProject
      INSTALL_DIR       ${CMAKE_CURRENT_BINARY_DIR}
      URL               http://ftp.openbsd.org/pub/OpenBSD/LibreSSL/libressl-${LIBRESSL_VERSION}-windows.zip
      URL_HASH          SHA256=${LIBRESSL_HASH}
      BUILD_BYPRODUCTS  ${LIBRESSL_LIBRARIES}
      CONFIGURE_COMMAND ""
      BUILD_COMMAND     ""
      TEST_COMMAND      ""
      INSTALL_COMMAND   COMMAND ${CMAKE_COMMAND} -E copy_directory
                          ${CMAKE_CURRENT_BINARY_DIR}/ExternalProject/src/libressl/include
                          ${CMAKE_CURRENT_BINARY_DIR}/include
                        COMMAND echo D | xcopy /Y /D ${CMAKE_BINARY_DIR_WIN32}\\ExternalProject\\src\\libressl\\${libressl_sub_directory}\\*.dll ${CMAKE_BINARY_DIR_WIN32}\\bin
                        COMMAND echo D | xcopy /Y /D ${CMAKE_BINARY_DIR_WIN32}\\ExternalProject\\src\\libressl\\${libressl_sub_directory}\\*.lib ${CMAKE_BINARY_DIR_WIN32}\\lib
    )
    
    unset( libressl_sub_directory )
    
  else()
    
    ExternalProject_Add( libressl
      EXCLUDE_FROM_ALL 1
      PREFIX            ${CMAKE_CURRENT_BINARY_DIR}/ExternalProject
      INSTALL_DIR       ${CMAKE_CURRENT_BINARY_DIR}
      GIT_REPOSITORY    git@github.com:libressl-portable/portable.git
      BUILD_IN_SOURCE   1
      CONFIGURE_COMMAND COMMAND ./autogen.sh
                        COMMAND ./configure --prefix=${CMAKE_BINARY_DIR}
      BUILD_COMMAND     make
      TEST_COMMAND      make check
      INSTALL_COMMAND   COMMAND make check
                        COMMAND make install
    )
    
    set(LIBRESSL_LIBCRYPTO_LIBRARY crypto)
    set(LIBRESSL_LIBSSL_LIBRARY ssl)
    set(LIBRESSL_LIBTLS_LIBRARY tls)
    
  endif()

  set(LIBRESSL_LIBRARIES
    ${LIBRESSL_LIBCRYPTO_LIBRARY}
    ${LIBRESSL_LIBSSL_LIBRARY}
    ${LIBRESSL_LIBTLS_LIBRARY}
  )
  
  add_executable( http_get    example/http_get.cxx )
  add_executable( http_server example/http_server.cxx )
  
  target_link_libraries( http_get    ${Boost_LIBRARIES} ${LIBRESSL_LIBRARIES} )
  target_link_libraries( http_server ${Boost_LIBRARIES} ${LIBRESSL_LIBRARIES} )
  
  add_dependencies( http_get    libressl )
  add_dependencies( http_server libressl )
  
  if(WIN32)
    target_link_libraries( http_get
      # http-client
      ws2_32
    )
    target_link_libraries( http_server
      # http-client
      ws2_32
      # http-server
      Mswsock
    )
  endif()
  
endif()
