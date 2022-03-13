#pragma once
/*
*/

#include <stdio.h>
#include <vector>
#include <stdlib.h>
#include <string>
#include <fastdb/cli.h>
#include <fastdb/stdtp.h>
#include <iostream>

namespace cli_plusplus {

#define show_db_respond(x)  \
    std::cout << x << std::endl


/*
enum class select_flag 定义select操作标记，
first、next、last 对应的操作为逐条操作，不能
与 multy_first、multy_next、multy_last混用。
批量操作必须以multy_first 开始，否则返回失败，
而逐条没有此限制。
*/

enum class select_flag {
    fetch = 0,          //每次查询数据时最先查询记录条数
    first,          //获取第一条记录
    last,           //获取最后一条记录
    next,           //获取下一条记录
    multy_first,    //批量获取到的数据中第一条记录
    multy_last,     //本次批量获取到的数据中最后一条记录
    multy_next,     //批量获取到的数据中下一条记录
    select_flag_max
};

struct ParameterBinding { 
    union { 
        cli_int1_t       i1;
        cli_int2_t       i2;
        cli_int4_t       i4;
        cli_int8_t       i8;
        cli_real4_t      r4;
        cli_real8_t      r8;
        cli_oid_t       oid;
        bool       b;
        char*      str;
        cli_rectangle_t  rect;
    } u;
    int type;
    char const*    name;
};

ParameterBinding ss[2] = {
                    {{.i1=5}, .type = cli_int2, .name = "%salary"},
                    {{.i1=5}, .type = cli_int4, .name = "%salary2"}  };

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
    int active_stat;
    std::vector<cli_field_descriptor2> field_desc;
    inline int remove_all(std::string table);

public:
    cli_api(std::string url, std::string database_name, std::string file_name="", size_t initDatabaseSize=100*1024*1024UL, size_t extensionQuantum=100*1024*1024UL, size_t initIndexSize=512*1024UL, size_t fileSizeLimit=0UL);
    ~cli_api();

    int opendb(int retry, int timeout);
#if 0
    int create_statement(std::string sql, cli_field_descriptor2 field_desc[], int field_num);
    int add_statement_parament(std::string sql,ParameterBinding parament_field_desc[], int parament_num); 
#endif

    int create_statement(std::string sql, cli_field_descriptor2 field_desc[], int field_num, ParameterBinding parament_field_desc[], int parament_num);

    int insert(T* ptr, int num);
    int insert_update(T* ptr, int num);
    int insert(T& data );
    
    /*
    auth: cli_view_only 或 cli_for_update
    flag: 标记操作用途， 详见 enum class select_flag 定义
    ret:  -1: 错误  其他：查询到的记录数量
    */
    int select(int auth, select_flag flag);
    T& get_record();

    /*
    更改 get_record()得到数据后更新到数据库，select()中auth必须是cli_for_update，
    */
    int update();

    int remove(std::string table);

};



///////////////////////////////////////////////
///////////////////////////////////////////////


    template < typename T >
    cli_api<T>::cli_api(std::string url, std::string database_name, std::string file_name, size_t initDatabaseSize, size_t extensionQuantum, size_t initIndexSize,size_t fileSizeLimit)
    {
        serverURL =url;
        dbname = database_name;
        filename = file_name;
        init_db_size = initDatabaseSize;
        ext_quantum = extensionQuantum;
        init_ind_size = initIndexSize;
        filesize_limit = fileSizeLimit;
        active_stat = 0;
    }

    template < typename T >
    cli_api<T>::~cli_api()
    {
        int rc;
        rc = cli_commit(session);
        if(cli_ok != rc)
        {
            show_db_respond("cli_commit(" << session << ") failed with errcode:" << rc );
        }
        for(auto t : statements)
        {
            rc = cli_free(t);
            if (rc != cli_ok) { 
                show_db_respond("cli_free(" << t << ") failed with errcode:" << rc );
            }
        }
        rc = cli_close(session);
        if(cli_ok != rc)
        {
            show_db_respond("cli_close(" << session << ") failed with errcode:" << rc );
        }
    }

    template < typename T >
    int cli_api<T>::opendb(int retry, int timeout)
    {
        show_db_respond("opendb(" << serverURL    );
        return cli_open(serverURL.c_str(), retry, timeout);
    }

#if 0 
    template < typename T >
    int cli_api<T>::create_statement(std::string sql, cli_field_descriptor2 field_desc[], int field_num)
    {
        int  statement = cli_statement(session, sql.c_str());
        if(statement >= 0)
        {
            statements.push_back(statement);          
            if( 0 != cli_column_autobind(statement,&record,field_desc,field_num))
            {
                statements.pop_back();
                return -1;
            }
            active_stat = statement;
            if(0 == field_desc.szie())
            {
                for(int i = 0 ; i < field_num; i++)
                {
                    field_desc.push_back(field_desc[i]);
                }
            }
        }
        return statement;
    }

    int cli_api::add_statement_parament<T>(std::string sql, ParameterBinding parament_field_desc[], int parament_num)
    {
        int rc = cli_ok;
        for(int i = 0; i < parament_num; i++)
        {
            rc = cli_parameter(active_stat, parament_field_desc[i].name, parament_field_desc[i].type, &(parament_field_desc[i].u));
            if(cli_ok != rc)
            return rc;
        }
        return rc;
    }
#endif

    template < typename T >
    int cli_api<T>::create_statement(std::string sql, cli_field_descriptor2 field_descs[], int field_num, ParameterBinding parament_field_descs[], int parament_num)
    {
        int rc;
        int statement = cli_statement(session, sql.c_str());
        if(statement >= 0)
        {
            statements.push_back(statement);          
            if( cli_ok != (rc= cli_column_autobind(statement, &record, field_descs, field_num)))
            {
                statements.pop_back();
                return rc;
            }
            active_stat = statement;
            if(0 == field_desc.size())
            {
                for(int i = 0 ; i < field_num; i++)
                {
                    field_desc.push_back(field_descs[i]);
                }
            }

            {
                int rc = cli_ok;
                for(int i = 0; i < parament_num; i++)
                {
                    rc = cli_parameter(active_stat, parament_field_descs[i].name, parament_field_descs[i].type, &(parament_field_descs[i].u));
                    if(cli_ok != rc)
                    return rc;
                }
            }
            return rc;
        }
        return statement;
    }

    template < typename T >
    int cli_api<T>::insert(T* ptr, int num)
    {
        int rc = cli_insert_multy(active_stat, ptr, num, &oid);
        if (rc != cli_ok) { 
            return 0;
        } 
        return -1;
    }

    template < typename T >
    int cli_api<T>::insert_update(T* ptr, int num)
    {
        int rc = cli_insert_multy_with_filter(active_stat, ptr, num, &oid);
        if (rc != cli_ok) { 
            return 0;
        } 
        return -1;
    }

    template < typename T >
    int cli_api<T>::insert(T& data )
    {
        record = data;
        int rc = cli_insert(active_stat, &oid);
        if (rc != cli_ok) { 
            return 0;
        } 
        return -1;       
    }

    //typedef int (select_func)(int) ;
    int(*func[7])(int )= {
    cli_get_first, 
    cli_get_last,
    cli_get_next,
    cli_get_multy,
    cli_parser_first,
    cli_parser_last,
    cli_parser_next };

    template < typename T >
    int cli_api<T>::select(int auth, select_flag flag)
    {
        int rc =0;
        int statement = active_stat;

        if(select_flag::fetch == flag)
        {
            return cli_fetch(statement, auth);
        }
        else
        {


            int ret = (func[int(flag) -1])(statement);
            if(select_flag::multy_next == flag && ret < 0 )
            {
                ret = cli_get_multy(statement);
            }
            return ret;
        }
    }

    template < typename T >
    T& cli_api<T>::get_record()
    {
        return record;
    }
    
    template < typename T >
    int cli_api<T>::update()
    {
        int rc = cli_update(active_stat);
        cli_precommit(session);
        if (rc != cli_ok) { 
            return -1;
        }
        return 0;
    }

    template < typename T >
    int cli_api<T>::remove(std::string table)
    {
        return remove_all(table);
    }

    template < typename T >
    inline int cli_api<T>::remove_all(std::string table)
    {
        int rc;
        std::string sql = "select * from " + table;
        int sql_statement = cli_statement(session, sql.c_str());
        if (sql_statement < 0) {
            return -1;
        }

        T p;
        if( field_desc.size() > 0 &&  0 != cli_column_autobind(sql_statement, &p, field_desc, field_desc.size()))
        {
            cli_free(sql_statement);
            cli_abort(session);
            return -2;
        }
        rc = cli_fetch(sql_statement, cli_for_update);
        if (rc < 0) {
            cli_free(sql_statement);
            cli_abort(session);
            return -3;
        }
        
        int count = rc;
        if (count == 0)
        {
            cli_free(sql_statement);
            cli_abort(session);
            return 0;
        }

        rc = cli_get_first(sql_statement);
        if (rc != cli_not_found) {
            cli_free(sql_statement);
            cli_abort(session);
            return -4;
        }

        rc = cli_remove(sql_statement);
        if(cli_ok != rc)
            return rc;

        rc = cli_free(sql_statement);
        if (rc != cli_ok) 
            return rc;

        rc = cli_commit(session);
            return rc;
    }


}
