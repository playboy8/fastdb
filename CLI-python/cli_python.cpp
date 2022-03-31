#include "cli_python.h"
#include "../CLI++/cli++.hpp"
#include <thread>



cli_python::cli_python(std::string url, std::string database_name, std::string file_name, size_t initDatabaseSize, size_t extensionQuantum, size_t initIndexSize, size_t fileSizeLimit)
{
    cliapi = std::make_shared<cli_plusplus::cli_api<record_struct>>(url, database_name, file_name, initDatabaseSize, extensionQuantum, initIndexSize, fileSizeLimit);
}

int cli_python::open(int retry, int timeout)
{
    if(cliapi) 
        return cliapi->opendb(retry,timeout);
    else
        return open_fail;
}

int cli_python::create_statement(std::string sql, cli_field_descriptor2 field_descs[], int field_num, ParameterBinding parament_field_descs[], int parament_num)
{
    return cliapi->create_statement(sql, field_descs, field_num, parament_field_descs, parament_num);   
}

int cli_python::select(int auth, select_flag flag)
{
    return cliapi->select(auth, flag);
}

record_struct& cli_python::get_record()
{
    return cliapi->get_record();
}

int cli_python::update()
{
    return cliapi->update();
}

int cli_python::commit()
{
    return cliapi->commit();
}

int cli_python::pre_commit()
{
    return cliapi->pre_commit();
}

int cli_python::insert(record_struct record)
{
    return cliapi->insert(record);   
}

int cli_python::insert(record_struct* ptr, int num)
{
    return -1; // not support for python
}

int cli_python::insert_update(record_struct *ptr, int num)
{
    return -1;
}

int cli_python::remove(std::string table)
{
    return cliapi->remove(table);   
}

cli_python::~cli_python()
{
    return cliapi->get_record();   
}