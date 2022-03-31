#pragma once
#include "record_struct.h"
#include <functional>
#include <memory>

#define open_fail   -1000

class cli_python
{
private:

public:
    cli_python(std::string url, std::string database_name, std::string file_name="", size_t initDatabaseSize=100*1024*1024UL, size_t extensionQuantum=100*1024*1024UL, size_t initIndexSize=512*1024UL, size_t fileSizeLimit=0UL);
    int open(int retry, int timeout);
    int create_statement(std::string sql, cli_field_descriptor2 field_descs[], int field_num, ParameterBinding parament_field_descs[], int parament_num);
    int select(int auth, select_flag flag);
    record_struct& get_record();
    int update();
    int commit();
    int pre_commit();
    int insert(record_struct record)
    int insert(record_struct* ptr, int num)
    int insert_update(record_struct *ptr, int num);
    int remove(std::string table);
    ~cli_python();

    std::shared_ptr<cli_plusplus::cli_api<record_struct>> cliapi;
    
};

