#include "cli_python.h"
#include "record_struct.hpp"
#include <thread>


cli_python::cli_python()
{

}

void cli_python::cli_python_init(std::string url, std::string database_name, std::string file_name, size_t initDatabaseSize, size_t extensionQuantum, size_t initIndexSize, size_t fileSizeLimit)
{
    cliapi.cli_api_init(url, database_name, file_name, initDatabaseSize, extensionQuantum, initIndexSize, fileSizeLimit);
}

int cli_python::open(int retry, int timeout)
{
    return cliapi.opendb(retry,timeout);
}

record_type cli_python::select_record_type(record_type type)
{
    rec_type = type;
    return rec_type;
}


int cli_python::create_statement(py::str sql, py::array_t<cli_field_descriptor2, py::array::c_style | py::array::forcecast> field_descs, py::array_t<ParameterBinding_py, py::array::c_style | py::array::forcecast> parament_field_descs)
{
    std::vector<ParameterBinding>  para;
    py::buffer_info buf0 = field_descs.request();
    py::buffer_info buf1 = parament_field_descs.request();
    ParameterBinding_py* ppar =  (ParameterBinding_py*)(buf1.ptr);
    cli_field_descriptor2* pfield =  (cli_field_descriptor2*)(buf0.ptr);
    for(int i =0; i< buf1.size; i++)
    {
        ppar[i].convert_parament(para[i]);
    }
    return cliapi.create_statement(sql, pfield, buf0.size , para.data(), para.size());   
}

int cli_python::select(int auth, select_flag flag)
{
    return cliapi.select(auth, flag);
}

record_struct& cli_python::get_record()
{
    return cliapi.get_record();
}

int cli_python::update()
{
    return cliapi.update();
}

int cli_python::commit()
{
    return cliapi.commit();
}

int cli_python::precommit()
{
    return cliapi.precommit();
}
 
int cli_python::insert(py::array_t<snapshot> record)
{
    py::buffer_info buf1 = record.request();
    
    if( buf1.size == 1)
        return cliapi.insert(*((record_struct*)(buf1.ptr)));   
    else  if( buf1.size > 1)
        return cliapi.insert((record_struct*)(buf1.ptr), buf1.size );  
    else return -1;  
}

int cli_python::insert(py::array_t<kline> record)
{
    py::buffer_info buf1 = record.request();
    if( buf1.size == 1)
        return cliapi.insert(*((record_struct*)(buf1.ptr)));   
    else  if( buf1.size > 1)
        return cliapi.insert((record_struct*)(buf1.ptr), buf1.size ); 
    else return -1;
}

int cli_python::insert_update(record_struct *ptr, int num)
{
    return -1;
}

int cli_python::remove(py::str table)
{
    return cliapi.remove(table);   
}

cli_python::~cli_python()
{

}


///---------------------------------------------------------
///---------------------------------------------------------
///--------------------for python api ----------------------




PYBIND11_MODULE(cli_py, m) {

// define enum record_type
py::enum_<record_type>(m, "record_type")
    .value("snapshot_rec", record_type::snapshot_rec )
    .value("kline_rec", record_type::kline_rec)
    .export_values();
    
// define enum cli_type_flag 
py::enum_<cli_var_type>(m, "cli_var_type")
    .value("cli_oid", cli_var_type::cli_oid )
    .value("cli_bool", cli_var_type::cli_bool )
    .value("cli_int1", cli_var_type::cli_int1 )
    .value("cli_int2", cli_var_type::cli_int2 )
    .value("cli_int4", cli_var_type::cli_int4 )
    .value("cli_int8", cli_var_type::cli_int8 )
    .value("cli_real4", cli_var_type::cli_real4 )
    .value("cli_real8", cli_var_type::cli_real8 )
    .value("cli_decimal", cli_var_type::cli_decimal )
    .value("cli_asciiz", cli_var_type::cli_asciiz  )
    .value("cli_pasciiz", cli_var_type::cli_pasciiz )
    .value("cli_cstring", cli_var_type::cli_cstring )
    .value("cli_array_of_oid", cli_var_type::cli_array_of_oid )
    .value("cli_array_of_bool", cli_var_type::cli_array_of_bool )
    .value("cli_array_of_int1", cli_var_type::cli_array_of_int1 )
    .value("cli_array_of_int2", cli_var_type::cli_array_of_int2 )
    .value("cli_array_of_int4", cli_var_type::cli_array_of_int4 )
    .value("cli_array_of_int8", cli_var_type::cli_array_of_int8 )
    .value("cli_array_of_real4", cli_var_type::cli_array_of_real4 )
    .value("cli_array_of_real8", cli_var_type::cli_array_of_real8 )
    .value("cli_array_of_decimal", cli_var_type::cli_array_of_decimal )
    .value("cli_array_of_string", cli_var_type::cli_array_of_string )
    .value("cli_any", cli_var_type::cli_any )
    .value("cli_datetime", cli_var_type::cli_datetime )
    .value("cli_autoincrement", cli_var_type::cli_autoincrement )
    .value("cli_rectangle", cli_var_type::cli_rectangle )
    .value("cli_wstring", cli_var_type::cli_wstring )
    .value("cli_pwstring", cli_var_type::cli_pwstring )
    .value("cli_array_of_wstring", cli_var_type::cli_array_of_wstring )
    .value("cli_unknown", cli_var_type::cli_unknown )
    .export_values();


    //  depends on numpy:
    try { py::module::import("numpy"); }
    catch (...) { return; }


// define record struct
PYBIND11_NUMPY_DTYPE(snapshot, id, value, value1);
PYBIND11_NUMPY_DTYPE(kline, stock_id, market_time, update_time, open, high, low, close, volume, turnover );
//
//// define field describe struct
PYBIND11_NUMPY_DTYPE(cli_field_descriptor2, type, flags, name, len, refTableName, inverseRefFieldName );

//// define parament struct 
PYBIND11_NUMPY_DTYPE(ParameterBinding_py, u, type, name);
py::class_<ParameterBinding_py>(m, "ParameterBinding_py")
.def_readwrite("u", &ParameterBinding_py::u)
.def_readwrite("type", &ParameterBinding_py::type)
.def_readwrite("name", &ParameterBinding_py::name);


py::class_<record_struct>(m, "record_struct")
    .def(py::init<>())
    .def_readwrite("snapshot_m", &record_struct::snapshot_m)
    .def_readwrite("kline", &record_struct::kline_m);

// define export class 
py::class_<cli_python>(m, "cli_python")
    .def(py::init<>())
    .def("cli_python_init", &cli_python::cli_python_init)
    .def("open", &cli_python::open)
    .def("select_record_type", &cli_python::select_record_type)
    .def("create_statement", &cli_python::create_statement)
    .def("get_record", &cli_python::get_record, py::return_value_policy::reference_internal)
//    .def("get_record", static_cast<kline& (cli_python::*)()>(&cli_python::get_record), " get kline record reference")
    .def("insert",  static_cast<int (cli_python::*)(py::array_t<snapshot>)>(&cli_python::insert), "insert snapshot record to db" )
    .def("insert",  static_cast<int (cli_python::*)(py::array_t<kline>)>(&cli_python::insert), "insert kline record to db" )
    .def("insert_update", &cli_python::insert_update)
    .def("remove", &cli_python::remove)
    .def("select", &cli_python::select)
    .def("update", &cli_python::update)
    .def("commit", &cli_python::commit)    
    .def("precommit", &cli_python::precommit);  

}