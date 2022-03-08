#pragma once
/*
*/

#include <stdio.h>
#include <vector>
#include <stdlib.h>
#include <string.h>
#include <fastdb/cli.h>
#include <fastdb/stdtp.h>

namespace cli_plusplus {

enum class select_flag {
    first,
    last,
    next,
    toltol_num
};

template < typename T >
class cli_api
{
private:
    /* data */
    std::string serverURL;
    std::string dbname;
    std::string filename;
    size_t init_db_size;
    size_t ext_quantum;
    size_t init_ind_size;
    size_t filesize_limit;
    T record;
    int session;
    cli_oid_t oid;
    std::vector<int> statements;

public:
    cli_api(std::string url, std::string database_name, std::string file_name, size_t initDatabaseSize, size_t extensionQuantum, size_t initIndexSize,size_t fileSizeLimit);
    ~cli_api();

    int opendb(int retry, int timeout);   
    int create_statement(std::string sql,cli_field_descriptor2 field_desc[], int field_num);
    int add_statement_parament(std::string sql,cli_field_descriptor2 field_desc[], int field_num); 
    int insert_multy(T* ptr, int num);
    int insert_multy_with_filter(T* ptr, int num);
    int insert(T& data );

    int select(int auth, select_flag flag);
};


struct dbParameterBinding { 
    union { 
        int1       i1;
        int2       i2;
        int4       i4;
        db_int8    i8;
        real4      r4;
        real8      r8;
        oid_t      oid;
        bool       b;
        char*      str;
        rectangle  rect;
    } u;
    int type;
};


}
