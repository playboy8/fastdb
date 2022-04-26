#include "cli_python.h"
#include "record_struct.hpp"
#include <thread>
#include <string>
#include <vector>
#include <stdexcept>

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

static std::vector<cli_field_descriptor2>  filed;
static std::vector<ParameterBinding>  para;
static std::vector<cli_field_descriptor2_py>  filed_py;
static std::vector<ParameterBinding_py>  para_py;

int cli_python::update_parament(py::list &t)
{
    ParameterBinding_py temp;
    if( t.size() == para.size()) 
    {
        int i = 0;
        for(auto &s : t)
        {
            std::string str = py::str(s);
            temp.convert_parament_2(para[i++],str.c_str());
        }  
        return 0;  
    } 
    return -1;
}

int cli_python::create_statement(py::str sql, py::array_t<cli_field_descriptor2_py, py::array::c_style | py::array::forcecast> &field_descs, py::array_t<ParameterBinding_py, py::array::c_style | py::array::forcecast> &parament_field_descs)
{
    py::buffer_info buf0 = field_descs.request();
    py::buffer_info buf1 = parament_field_descs.request();
    cli_field_descriptor2_py* pfield =  (cli_field_descriptor2_py*)(buf0.ptr);
    ParameterBinding_py* ppar =  (ParameterBinding_py*)(buf1.ptr);

    filed.resize(buf0.size);
    para.resize(buf1.size);
    filed_py.resize(buf0.size);
    para_py.resize(buf1.size);

    for(int i = 0; i < buf0.size; i++)
    {
        filed_py[i] = pfield[i];
        filed_py[i].convert_parament(filed[i]);
    }
    for(int i = 0; i < buf1.size; i++)
    {
        para_py[i] = ppar[i];
        para_py[i].convert_parament(para[i]);
    }

    std::cout << " buf0.size =" << buf0.size << "   buf1.size =" << buf1.size << std::endl ; 
    return cliapi.create_statement(std::string(sql),filed.data(), buf0.size , para.data(), buf1.size );   
}

cli_result_code cli_python::select(int auth, select_flag flag)
{
    return cli_result_code(cliapi.select(auth, flag));
}

py::list cli_python::get_record()
{
    int record_size =0 ;
    const char * data = (const char *)cliapi.get_record(record_size);
    const char * src = data;
    auto fields = cliapi.get_curr_field_desc();
    py::list out;
    std::string str;

    for(auto &t : fields)
    {
        char buf[t.len]={0,};
        switch (t.type)
        {
        case cli_int1:         
            memcpy(buf, src, t.len);
            str.assign(buf);
            out.append(str);
            src +=  t.len;
            break;
        case cli_int8:
            out.append(*((cli_int8_t*)src));
            src +=  sizeof(cli_int8_t);
            break; 
        case cli_real8:
            out.append(*((cli_real8_t*)src));
            src +=  sizeof(cli_real8_t);
            break;                    
        default:
            break;
        }
    }
    return out;
}

cli_result_code cli_python::update()
{
    return cli_result_code(cliapi.update());
}

cli_result_code cli_python::commit()
{
    return cli_result_code(cliapi.commit());
}

cli_result_code cli_python::precommit()
{
    return cli_result_code(cliapi.precommit());
}
 

cli_result_code cli_python::insert_one(py::list &record)
{
    std::vector<char> data= {0};
    data.resize(cliapi.get_record_size());
    if(convert_in(record, data) && data.size() > 0)
        return cli_result_code(cliapi.insert(data.data()));    
    else 
        return cli_bad_address;  
}

cli_result_code cli_python::insert(py::list &record , int num)
{
    std::vector<char> data= {0};
    auto fields = cliapi.get_curr_field_desc();
    size_t all_size = fields.size() * num;
    if( record.size()  != all_size )
    {
        std::cout << "Error : lost some fields!" << std::endl;
        return cli_bad_address;
    }
    data.resize(cliapi.get_record_size() * num);
    if(convert_in(record, data) && data.size() > 0)
        return cli_result_code(cliapi.insert(data.data(), num));    
    else 
        return cli_bad_address;  
}

cli_result_code cli_python::insert_update(py::list &record , int num)
{
    std::vector<char> data= {0};
    auto fields = cliapi.get_curr_field_desc();
    if( record.size()  != fields.size() * num)
    {
        std::cout << "Error : lost some fields!" << std::endl;
        return cli_bad_address;
    }
    data.resize(cliapi.get_record_size() * num);
    if(convert_in(record, data) && data.size() > 0)
        return cli_result_code(cliapi.insert_update(data.data(), num));    
    else 
        return cli_bad_address; 
    
}

cli_result_code cli_python::remove_all(py::str table)
{
    return cli_result_code(cliapi.remove(table));   
}

cli_result_code cli_python::remove_curr()
{
    return cli_result_code(cliapi.remove_curr_record());   
}

cli_python::~cli_python()
{

}

static cli_var_type convet_python_type(std::string str)
{
    if(str == "<class 'int'>" )
        return cli_int8;
    else if(str == "<class 'float'>" )
        return cli_real8;
    else if(str == "<class 'str'>" )
        return cli_int1;
    else 
        return cli_unknown;
}

// 注意 table 定义只运行三种类型  （int8， real8，  int1数组）
bool cli_python::convert_in(py::list &in, std::vector<char> &record)
{
    size_t i = 0, r_id = 0;
    char *pdst = record.data();
    static std::vector<unsigned int> filed_pos;
    auto fields = cliapi.get_curr_field_desc();
    std::string strdata;
    size_t filed_num = fields.size();
    if( 0 != (in.size() % filed_num) )
    {
        std::cout << " Error, unmatched input data"  << std::endl;
        return false;
    }

    for(auto &t : in)
    {
        std::string str =  py::str(py::type::of(t));
        cli_var_type type = convet_python_type(str);
        if(fields[i].type != type )
        {
            std::cout << " Error, this type not support:" <<  str << "  field name:" << fields[i].name << " record id =" << r_id <<  std::endl;
            return false;
        }
        if(cli_int1 == type)
        {
            strdata = py::str(t);
            if((strdata.size()+1) > size_t(fields[i].len))
            {
                std::cout << " Error, this str too long to store ,field name:" <<  fields[i].name << ", totle size limit: " << fields[i].len << std::endl;
                return false;           
            }
        }
        switch (type)
        {
        case cli_var_type::cli_int1 :     
            strcpy(pdst, strdata.c_str());
            pdst += fields[i].len ;  
            std::cout << strdata << std::endl;
            break;

        case cli_var_type::cli_int8 :
            *((cli_int8_t*)pdst) = t.cast<cli_int8_t>();
            pdst += sizeof(cli_int8_t);
             std::cout << t.cast<cli_int8_t>() << std::endl;
            break;
        case cli_var_type::cli_real8 :
            *((cli_real8_t*)pdst) = t.cast<cli_real8_t>();
            pdst += sizeof(cli_real8_t);
            std::cout <<  t.cast<cli_real8_t>() << std::endl;
            break;                
        default:
            std::cout << " Error,   error type  in " << fields[i].name << std::endl;
            return false;  
            break;
        }        
        if( i >= filed_num-1)
        {
            i = 0; 
            r_id += 1;
        }
        else 
            i++;
    }
    return true;
}

///---------------------------------------------------------
///---------------------------------------------------------
///--------------------for python api ----------------------
PYBIND11_MODULE(cli_py, m) {

//  depends on numpy:
    try { py::module::import("numpy"); }
    catch (...) { return; }

// define enum record_type
py::enum_<stat_func>(m, "stat_func")
    .value("stat_for_insert", stat_func::stat_for_insert )
    .value("stat_for_select_all", stat_func::stat_for_select_all)
    .value("stat_for_select_spec", stat_func::stat_for_select_spec )
    .value("stat_for_update", stat_func::stat_for_update)
    .export_values();

// define enum select_flag 
py::enum_<select_flag>(m, "select_flag")                                        
    .value("fetch", select_flag::fetch )                        //每次查询数据时最先查询记录条数                
    .value("first", select_flag::first)                         //获取第一条记录            
    .value("last", select_flag::last )                          //获取最后一条记录            
    .value("next", select_flag::next)                           //获取下一条记录        
    .value("multy_get", select_flag::multy_get)                      //批量获取数据，并定位至当前批次第一条    
    .value("multy_first", select_flag::multy_first )            //批量获取到的数据中第一条记录                            
    .value("multy_last", select_flag::multy_last)               //本次批量获取到的数据中最后一条记录                        
    .value("multy_next", select_flag::multy_next )              //批量获取到的数据中下一条记录                        
    .value("select_flag_max", select_flag::select_flag_max)     //                                 
    .export_values();   

// define all cli operator result 
py::enum_<cli_result_code>(m, "result_code")        
    .value("cli_ok", cli_result_code::cli_ok)
    .value("cli_bad_address", cli_result_code::cli_bad_address)
    .value("cli_connection_refused", cli_result_code::cli_connection_refused)
    .value("cli_database_not_found", cli_result_code::cli_database_not_found)
    .value("cli_bad_statement", cli_result_code::cli_bad_statement)
    .value("cli_parameter_not_found", cli_result_code::cli_parameter_not_found)
    .value("cli_unbound_parameter", cli_result_code::cli_unbound_parameter)
    .value("cli_column_not_found", cli_result_code::cli_column_not_found)
    .value("cli_incompatible_type", cli_result_code::cli_incompatible_type)
    .value("cli_network_error", cli_result_code::cli_network_error)
    .value("cli_runtime_error", cli_result_code::cli_runtime_error)
    .value("cli_bad_descriptor", cli_result_code::cli_bad_descriptor)
    .value("cli_unsupported_type", cli_result_code::cli_unsupported_type)
    .value("cli_not_found", cli_result_code::cli_not_found)
    .value("cli_not_update_mode", cli_result_code::cli_not_update_mode)
    .value("cli_table_not_found", cli_result_code::cli_table_not_found)
    .value("cli_not_all_columns_specified", cli_result_code::cli_not_all_columns_specified)
    .value("cli_not_fetched", cli_result_code::cli_not_fetched)
    .value("cli_already_updated", cli_result_code::cli_already_updated)
    .value("cli_table_already_exists", cli_result_code::cli_table_already_exists)
    .value("cli_not_implemented", cli_result_code::cli_not_implemented)
    .value("cli_xml_parse_error", cli_result_code::cli_xml_parse_error)
    .value("cli_backup_failed", cli_result_code::cli_backup_failed)
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

//// define field describe struct
PYBIND11_NUMPY_DTYPE(cli_field_descriptor2_py, type, flags, name, len, refTableName, inverseRefFieldName );

//// define parament struct 
PYBIND11_NUMPY_DTYPE(ParameterBinding_py, u, type, name);
 
// define export class 
py::class_<cli_python>(m, "cli_python") // , py::array::c_style | py::array::forcecast
    .def(py::init<>())
    .def("cli_python_init", &cli_python::cli_python_init)
    .def("open", &cli_python::open)
    .def("create_statement", static_cast<int (cli_python::*)(py::str, py::array_t<cli_field_descriptor2_py, py::array::c_style | py::array::forcecast>&, py::array_t<ParameterBinding_py, py::array::c_style | py::array::forcecast>&)>(&cli_python::create_statement))
    .def("get_record", &cli_python::get_record, py::return_value_policy::reference_internal)
    .def("insert",  &cli_python::insert_one, "insert one record to db" )
    .def("insert",  &cli_python::insert, "insert kline record to db" )
    .def("insert_update", &cli_python::insert_update, "insert and update to db")
    .def("remove_all", &cli_python::remove_all) 
    .def("remove_curr", &cli_python::remove_curr)
    .def("select", &cli_python::select)
    .def("update", &cli_python::update)
    .def("commit", &cli_python::commit)  
    .def("precommit", &cli_python::precommit);  

}