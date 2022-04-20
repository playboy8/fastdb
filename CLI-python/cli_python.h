#pragma once
#include "cli++.hpp"
#include "record_struct.h"
#include <memory>
#include <vector>

// --------------------------- 
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>

using namespace cli_plusplus;
namespace py = pybind11;

class cli_python
{
private:
    cli_api cliapi;
    record_type rec_type;
    ParameterBinding *pparament;

public:
    cli_python();
    void cli_python_init(std::string url, std::string database_name, std::string file_name="", size_t initDatabaseSize=100*1024*1024UL, size_t extensionQuantum=100*1024*1024UL, size_t initIndexSize=512*1024UL, size_t fileSizeLimit=0UL);
    int open( int retry, int timeout);
    int create_statement(py::str sql, py::array_t<cli_field_descriptor2_py, py::array::c_style | py::array::forcecast> &field_descs, py::array_t<ParameterBinding_py, py::array::c_style | py::array::forcecast> &parament_field_descs); // , py::array::c_style | py::array::forcecast> array
    py::list  get_record();
    int insert_one(py::list &record);
    int insert(py::list &record, int num);
    int insert_update(py::list &record , int num);
    int remove(py::str table);
    int select(int auth, select_flag flag);
    int update();
    int commit();
    int precommit();
    ~cli_python(); 

    py::list show_list(py::list list_m);

    bool convert_in(py::list &in, std::vector<char> &record);
    bool convert_out(py::list &out);
};


