cmake_minimum_required(VERSION 3.0.0)

set(name  fast_demo)
project (${name})


set(vendor_dir /opt/fastdb64 )
set(boost_inc_dir ${vendor_dir}/boost/boost_1_77_0/boost_out/lib )
set(boost_lib_dir ${vendor_dir}/boost/boost_1_77_0/boost_out/include )
set(boost_libs   )


if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set (lib "hiredis-d.so")
else()
        set (lib "hiredis.so")
endif()


file( GLOB  sources  ${CMAKE_SOURCE_DIR}     ../opt/*     )

# -D_DEBUG  -g -ggdb3 
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}   -std=c++11 -ggdb3   -O2 -Wno-invalid-offsetof -Wall   -fno-strict-aliasing -DHAVE_CONFIG_H    " ) 

include_directories(${vendor_dir}/include)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR})
add_executable(  ${name}  ${sources}  ${CMAKE_SOURCE_DIR}/main.cpp )

target_link_libraries(${name}   PUBLIC
                                ${vendor_dir}/lib/libfastdb.so 
                                pthread m  )

                              
add_executable( reader  ${sources} ${CMAKE_SOURCE_DIR}/reader.cpp )

target_link_libraries(reader   PUBLIC
                                ${vendor_dir}/lib/libfastdb.so 
                                pthread m  )


add_executable( cliwrite  ${sources} ${CMAKE_SOURCE_DIR}/cliwrite.cpp )
target_link_libraries(cliwrite   PUBLIC
                                ${vendor_dir}/lib/libcli.so 
                                pthread m  )

add_executable( cliwrite2  ${sources} ${CMAKE_SOURCE_DIR}/cliwrite2.cpp )
target_link_libraries(cliwrite2   PUBLIC
                                ${vendor_dir}/lib/libcli.so 
                                pthread m  )

add_executable( cliwrite2_filter  ${sources} ${CMAKE_SOURCE_DIR}/cliwrite2_filter.cpp )
target_link_libraries(cliwrite2_filter   PUBLIC
                                ${vendor_dir}/lib/libcli.so 
                                pthread m  )
                                

add_executable( cliread  ${sources} ${CMAKE_SOURCE_DIR}/cliread.cpp )
target_link_libraries(cliread   PUBLIC
                                ${vendor_dir}/lib/libcli.so 
                                pthread m  )

add_executable( cliread2  ${sources} ${CMAKE_SOURCE_DIR}/cliread2.cpp )
target_link_libraries(cliread2   PUBLIC
                                ${vendor_dir}/lib/libcli.so 
                                pthread m  )

add_executable( KLine_pro  ${sources} ${CMAKE_SOURCE_DIR}/KLine_produce.cpp )
target_link_libraries(KLine_pro   PUBLIC
                                ${vendor_dir}/lib/libcli.so 
                                pthread m  )
                                
add_executable( KLine_recive  ${sources} ${CMAKE_SOURCE_DIR}/KLine_recive.cpp )
target_link_libraries(KLine_recive   PUBLIC
                                ${vendor_dir}/lib/libcli.so 
                                pthread m  )

                                
message(STATUS "\n\
     info :     ${CMAKE_SOURCE_DIR}\n
     sources:    ${sources} \n
")

