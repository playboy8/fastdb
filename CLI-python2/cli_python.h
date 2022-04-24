#pragma once
#include "cli++.h"
#include "record_struct.h"
#include <memory>

// --------------------------- 
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

using namespace cli_plusplus;
namespace py = pybind11;

class cli_python
{
private:
    cli_api  cliapi;
    record_type rec_type;
    ParameterBinding *pparament;

public:
    cli_python();
    void cli_python_init(std::string url, std::string database_name, std::string file_name="", size_t initDatabaseSize=100*1024*1024UL, size_t extensionQuantum=100*1024*1024UL, size_t initIndexSize=512*1024UL, size_t fileSizeLimit=0UL);
    int open( int retry, int timeout);
 //   record_type select_record_type(record_type type, stat_func func);
    int create_statement(record_type type, stat_func func, py::str sql);
    int create_statement(py::str sql, py::array_t<cli_field_descriptor2_py, py::array::c_style | py::array::forcecast> &field_descs, py::array_t<ParameterBinding_py, py::array::c_style | py::array::forcecast> &parament_field_descs); // , py::array::c_style | py::array::forcecast> array
    record_struct& get_record();
    cli_result_code insert(bool multy, py::array_t<snapshot, py::array::c_style | py::array::forcecast> &record);
    cli_result_code insert(bool multy, py::array_t<kline, py::array::c_style | py::array::forcecast> &record);
    cli_result_code insert(bool multy, std::vector<snapshot> &record);
    cli_result_code insert(bool multy, std::vector<kline> &record);

    cli_result_code insert_update(record_struct *ptr, int num);
    cli_result_code remove(py::str table);
    cli_result_code select(int auth, select_flag flag);
    cli_result_code update();
    cli_result_code commit();
    cli_result_code precommit();
    ~cli_python(); 
};

