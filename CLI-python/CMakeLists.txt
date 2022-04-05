cmake_minimum_required(VERSION 2.8.12)

set(name cli_py)
project (${name})

set(fastdb_dir /opt/fastdb64 )

set(python_dir /usr/include/python3.6m)
set(pybind11_dir ${CMAKE_SOURCE_DIR}/include)

file( GLOB  sources  ${CMAKE_SOURCE_DIR} ./*.cpp  )
# -D_DEBUG  -g   
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}   -std=c++11  -ggdb3  -O2 -Wno-invalid-offsetof -Wall   -fno-strict-aliasing -DHAVE_CONFIG_H " ) 

add_subdirectory(pybind11)
pybind11_add_module(${name}  ${sources})


#add_library( ${name} SHARED ${sources})
#set_target_properties(${name} PROPERTIES SUFFIX ".pyd")


include_directories(${fastdb_dir}/include  ../CLI++/   ./  ${pybind11_dir} ${python_dir}  )

target_link_libraries(${name}   PUBLIC
                                ${fastdb_dir}/lib/libcli.so 
                                pthread m  )



                                
message(STATUS "\n\
     info :     ${CMAKE_SOURCE_DIR}\n
     sources:    ${sources} \n
")

