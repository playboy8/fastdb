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
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}   -std=c++11  -D_DEBUG  -g -ggdb3   -O1  -Wno-invalid-offsetof -Wall   -fno-strict-aliasing -DHAVE_CONFIG_H    " ) 

#g++ -DHAVE_CONFIG_H -I. -I../inc -I../inc    -Wno-invalid-offsetof -Wall -DNDEBUG -O2 -fno-strict-aliasing -MT guess.o -MD -MP -MF .deps/guess.Tpo -c -o guess.o guess.cpp
#/bin/sh ../libtool --tag=CXX   --mode=link g++  -Wno-invalid-offsetof -Wall -DNDEBUG -O2 -fno-strict-aliasing   -o guess guess.o ../src/libfastdb.la -lpthread -lm 
#g++ -Wno-invalid-offsetof -Wall -DNDEBUG -O2 -fno-strict-aliasing -o .libs/guess guess.o  ../src/.libs/libfastdb.so -lpthread -lm  -Wl,--rpath -Wl,/opt/fastdb64/lib

#link_directories(${vendor_dir}/lib)

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

add_executable( cliread  ${sources} ${CMAKE_SOURCE_DIR}/cliread.cpp )
target_link_libraries(cliread   PUBLIC
                                ${vendor_dir}/lib/libcli.so 
                                pthread m  )

add_executable( cliread2  ${sources} ${CMAKE_SOURCE_DIR}/cliread2.cpp )
target_link_libraries(cliread2   PUBLIC
                                ${vendor_dir}/lib/libcli.so 
                                pthread m  )
message(STATUS "\n\
     info :     ${CMAKE_SOURCE_DIR}\n
     sources:    ${sources} \n
")

