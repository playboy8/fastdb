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


/*
enum class select_flag 定义select操作标记，
first、next、last 对应的操作为逐条操作，不能
与 multy_first、multy_next、multy_last混用。
批量操作必须以multy_first 开始，否则返回失败，
而逐条没有此限制。
*/

enum class select_flag {
    first,          //获取第一条记录
    last,           //获取最后一条记录
    next,           //获取下一条记录
    multy_first,    //批量获取到的数据中第一条记录
    multy_last,     //本次批量获取到的数据中最后一条记录
    multy_next,     //批量获取到的数据中下一条记录
    toltol_num      //获取语句查询到的记录数
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

/*
auth: cli_view_only 或 cli_for_update
flag: 标记操作用途， 详见 enum class select_flag 定义
ret:  -1: 错误  其他：查询到的记录数量
*/
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
