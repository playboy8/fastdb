cmake_minimum_required(VERSION 3.0.0)

set(name fastdb_cli++)
project (${name})

set(fastdb_dir /opt/fastdb64 )


file( GLOB  sources  ${CMAKE_SOURCE_DIR} ./main.cpp ../CLI++/*.cpp    )

# -D_DEBUG  -g   
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -D_DEBUG -std=c++11  -ggdb3  -O2 -Wno-invalid-offsetof -Wall   -fno-strict-aliasing -DHAVE_CONFIG_H " ) 

include_directories(${fastdb_dir}/include  ../CLI++/ )

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR})
add_executable(  ${name}  ${sources} )

target_link_libraries(${name}   PUBLIC
                                ${fastdb_dir}/lib/libcli.so 
                                pthread m  )
                                
message(STATUS "\n\
     info :     ${CMAKE_SOURCE_DIR}\n
     sources:    ${sources} \n
")

