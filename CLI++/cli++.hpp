#pragma once

#include <stdio.h>
#include <vector>
#include <stdlib.h>
#include <string>
#include <fastdb/cli.h>
#include <fastdb/stdtp.h>
#include <iostream>
#include <thread>

namespace cli_plusplus {

    #ifdef _DEBUG
    #define show_db_respond(x)  \
        std::cout << x << std::endl
    #else
    #define show_db_respond(x)
    #endif


    /*********************************************************************
     * 定义select操作标记，first、next、last 对应的操作为逐条操作，
     * multy_first、multy_next、multy_last 为批量操作， 两类不能
     * 混用。批量操作必须以multy_first 开始，否则返回失败,逐条没有此限制。
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


    /*********************************************************************
     * 定义通用参数类型
     * 示例： ParameterBinding ss[2] = { 
     * {{.i1=5}, .type = cli_int2, .name = "%salary"},
     * {{.i1=5}, .type = cli_int4, .name = "%salary2"}  };
     */       

    template < typename T >
    class cli_api
    {
    private:
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

    /*********************************************************************
     *  构造函数
     * url:  远程数据地址和端口信息 （"127.0.0.1:6100"）
     * database_name:   数据库名称
     * initDatabaseSize： 指定初始索引大小
     * extensionQuantum： 内存分配位图的扩展量
     * initIndexSize： 对象索引的初始大小
     * fileSizeLimit： 文件大小限制（0 - 无限制）
     * 
     */    
    cli_api(std::string url="", std::string database_name="", std::string file_name="", size_t initDatabaseSize=100*1024*1024UL, size_t extensionQuantum=100*1024*1024UL, size_t initIndexSize=512*1024UL, size_t fileSizeLimit=0UL);


    /*********************************************************************
     */
    ~cli_api();


    /*********************************************************************
     *  初始化函数， 功能同构造函数，在构造时未传入参数时需要调用该函数初始化
     * url:  远程数据地址和端口信息 （"127.0.0.1:6100"）
     * database_name:   数据库名称
     * initDatabaseSize： 指定初始索引大小
     * extensionQuantum： 内存分配位图的扩展量
     * initIndexSize： 对象索引的初始大小
     * fileSizeLimit： 文件大小限制（0 - 无限制）
     * 
     */  

    void cli_api_init(std::string url, std::string database_name, std::string file_name="", size_t initDatabaseSize=100*1024*1024UL, size_t extensionQuantum=100*1024*1024UL, size_t initIndexSize=512*1024UL, size_t fileSizeLimit=0UL);


    /*********************************************************************
     * 打开数据库
     * retry: 重试次数
     * timeout：超时时间（秒）
     * ret： 返回成功的session id（>=0）  , 失败：(<0) , 参照 enum cli_result_code 解析操作结果。 
     * 
     */
    int opendb(int retry, int timeout);


    /*********************************************************************
     * 创建类sql语句
     * sql: 语句
     * field_desc： 字段描述数组
     * field_num： 字段描述数组大小
     * ret：参照 enum cli_result_code 解析操作结果
     * 
     */
    int create_statement(std::string sql, cli_field_descriptor2 field_desc[], int field_num);


    /*********************************************************************
     * 添加类sql语句参数信息
     * sql: 语句
     * parament_field_desc： 参数描述数组
     * parament_num ： 字段描述数组大小描述数组大小
     * ret：参照 enum cli_result_code 解析操作结果
     * 
     */
    int add_statement_parament(std::string sql,ParameterBinding parament_field_desc[], int parament_num); 


    /*********************************************************************
     * 创建类sql语句
     * sql: 语句
     * record_size：一个记录结构的大小
     * field_desc： 字段描述数组
     * field_num： 字段描述数组大小
     * parament_field_desc： 参数描述数组
     * parament_num ： 字段描述数组大小描述数组大小
     * ret：参照 enum cli_result_code 解析操作结果
     * 
     */
    int create_statement(std::string sql, int record_size, cli_field_descriptor2 field_desc[], int field_num, ParameterBinding parament_field_desc[], int parament_num);


    /*********************************************************************
     * 批量插入记录
     * ptr： 记录数据地址
     * num：记录个数
     * ret： 成功返回oid（大于等于0）  失败： 小于0
     * 
     */
    int insert(T* ptr, int num);

    /*********************************************************************
     * 批量插入记录并更新库中重复记录
     * ptr： 记录数据地址
     * num：记录个数
     * ret： 成功返回oid（大于等于0）  失败： 小于0
     * 
     */
    int insert_update(T* ptr, int num);


    /*********************************************************************
     * 插入一条记录
     * data： 记录数据引用
     * ret： 成功返回oid（大于等于0）  失败： 小于0
     * 
     */
    int insert(T& data );
    

    /*********************************************************************
     * 查询数据
     * auth: 查询操作的权限参数 cli_view_only 或 cli_for_update
     * flag：标记操作用途， 详见 enum class select_flag 定义
     * ret: 错误： 小于0，  其他值：返回的记录总数 或者成功 
     * 当falg= select_flag::fetch ， 返回值为记录总数
     * 当falg= 其他值时 返回当次操作是否成功，查询的数据通过 get_record()获取
     * 
     */
    int select(int auth, select_flag flag);


    /*********************************************************************
     * 获取当前操作记录的引用， 通过读取和更新此引用来读取更新记录值
     * ret： 返回记录的引用
     *  
     */    
    T& get_record();


    /*********************************************************************
     * 更新记录
     * ret: 参照 enum cli_result_code 解析操作结果
     * note：更改记录时 select()中auth必须是cli_for_update.
     */
    int update();


    /*********************************************************************
     * 删除表中的全部数据
     * table： 数据表名称
     * ret: 参照 enum cli_result_code 解析操作结果
     */
    int remove(std::string table);


    /*********************************************************************
     * precommit操作
     * ret: 参照 enum cli_result_code 解析操作结果
     * 
     * 
     */
    int precommit();


    /*********************************************************************
     * commit操作 
     * ret: 参照 enum cli_result_code 解析操作结果
     * 
     */
    int commit();

    /*********************************************************************
     * 关闭数据库 
     * ret: 参照 enum cli_result_code 解析操作结果
     * note：对象析构会自动调用
     */
    int closedb();

    };

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
        active_stat = -1;
        session = -1;
    }

    template < typename T >
    void cli_api<T>::cli_api_init(std::string url, std::string database_name, std::string file_name, size_t initDatabaseSize, size_t extensionQuantum, size_t initIndexSize,size_t fileSizeLimit)
    {
        serverURL =url;
        dbname = database_name;
        filename = file_name;
        init_db_size = initDatabaseSize;
        ext_quantum = extensionQuantum;
        init_ind_size = initIndexSize;
        filesize_limit = fileSizeLimit;
        active_stat = -1;
        session = -1;
    }

    template < typename T >
    cli_api<T>::~cli_api()
    {
        closedb();
    }

    template < typename T >
    int cli_api<T>::opendb(int retry, int timeout)
    {
        session = cli_open(serverURL.c_str(), retry, timeout);
        show_db_respond("opendb(" << serverURL  << ") " << "   with sessionid= " <<  session );
        return session;
    }

    template < typename T >
    int cli_api<T>::create_statement(std::string sql, cli_field_descriptor2 field_descs[], int field_num)
    {
        int  statement = cli_statement(session, sql.c_str());
        if(statement >= 0)
        {
            statements.push_back(statement);          
            if( 0 != cli_column_autobind(statement,&record, sizeof(record),field_descs,field_num))
            {
                statements.pop_back();
                return -1;
            }
            if(active_stat >=0 )
                cli_precommit(active_stat);

            active_stat = statement;
            if(0 == field_desc.size())
            {
                for(int i = 0 ; i < field_num; i++)
                {
                    field_desc.push_back(field_desc[i]);
                }
            }
        }
        return statement;
    }

    template < typename T >
    int cli_api<T>::add_statement_parament(std::string sql, ParameterBinding parament_field_desc[], int parament_num)
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

    template < typename T >
    int cli_api<T>::create_statement(std::string sql, int record_size, cli_field_descriptor2 field_descs[], int field_num, ParameterBinding parament_field_descs[], int parament_num)
    {
        int rc;
        if(active_stat >=0 )
            cli_precommit(active_stat);

        int statement = cli_statement(session, sql.c_str());
        if(statement >= 0)
        {
            statements.push_back(statement);    
            if( cli_ok != (rc = cli_column_autobind(statement, &record, record_size, field_descs, field_num)))
            {
                statements.pop_back();
                show_db_respond("create_statement(" << sql << " ) failed ,  error code: " << rc );
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
            if(parament_num > 0 )
            {
                int rc = cli_ok;
                for(int i = 0; i < parament_num; i++)
                {
                    rc = cli_parameter(active_stat, parament_field_descs[i].name, parament_field_descs[i].type, &(parament_field_descs[i].u));
                    if(cli_ok != rc)
                    {
                        show_db_respond("cli_parameter(" << parament_field_descs[i].name << ", " << parament_field_descs[i].type <<  " ) failed ,  error code: " << rc );
                        return rc;
                    }
                }
                show_db_respond("cli_parameter(" << sql << " )  succes ,  statement : " << statement );
            }
            return rc;
        }
        return statement;
    }

    template < typename T >
    int cli_api<T>::insert(T* ptr, int num)
    {
        int rc = cli_insert_multy(active_stat, ptr, num, &oid); 
        return rc;
    }

    template < typename T >
    int cli_api<T>::insert_update(T* ptr, int num)
    {
        int rc = cli_insert_multy_with_filter(active_stat, ptr, num, &oid);
        return rc;
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
        int rc2 = cli_precommit(session);
        return (rc == cli_ok)? rc2 : rc;
    }

    template < typename T >
    int cli_api<T>::remove(std::string table)
    {
        return remove_all(table);
    }

    template < typename T >
    int cli_api<T>::precommit()
    {
        return cli_precommit(session);
    }

    template < typename T >
    int cli_api<T>::commit()
    {   
        return cli_commit(session);
    }

    template < typename T >
    int cli_api<T>::closedb()
    {
        if(session>=0)
        {
            int rc = cli_commit(session);
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
            statements.clear();
            field_desc.clear();
            rc = cli_close(session);
            if(cli_ok != rc)
            {
                show_db_respond("cli_close(" << session << ") failed with errcode:" << rc );
            }
            return rc;
        }
        return 0;
    }

    template < typename T >
    inline int cli_api<T>::remove_all(std::string table)
    {
        int rc;
        std::string sql = "select * from " + table;
        int sql_statement = cli_statement(session, sql.c_str());
        if (sql_statement < 0) {
            show_db_respond("cli_statement(" << session << "," << sql << " ) failed with errcode:" << sql_statement );
            return -1;
        }

        T p;
        if( field_desc.size() > 0 &&  0 != (rc = cli_column_autobind(sql_statement, &p, sizeof(p), (cli_field_descriptor2*)(field_desc.data()), field_desc.size())))
        {            
            show_db_respond("cli_column_autobind(" << sql_statement << "," << sql << " ) failed with errcode:" << rc );
            cli_free(sql_statement);
            return -2;
        }
        rc = cli_fetch(sql_statement, cli_for_update);
        if (rc < 0) {
            
            show_db_respond("cli_fetch(" << session << "," << sql << " ) failed with errcode:" << rc );
            cli_free(sql_statement);
            return -3;
        }
        
        int count = rc;
        if (count == 0)
        {
            cli_free(sql_statement);
            return 0;
        }

        rc = cli_get_first(sql_statement);
        if (rc != cli_ok) {
            show_db_respond("cli_get_first(" << sql_statement  << " ) failed with errcode:" << rc );
            cli_free(sql_statement);
            return -4;
        }

        rc = cli_remove(sql_statement);
        if(cli_ok != rc)
        {
            show_db_respond("cli_get_first(" << sql_statement  << " ) failed with errcode:" << rc );
            return rc;
        }

        rc = cli_free(sql_statement);
        if (rc != cli_ok) 
        {
            show_db_respond("cli_get_first(" << sql_statement  << " ) failed with errcode:" << rc );
            return rc;
        }

        rc = cli_commit(session);
        show_db_respond("cli_commit(" << session  << " ) for remove failed with errcode:" << rc );
            return rc;
    }


}
