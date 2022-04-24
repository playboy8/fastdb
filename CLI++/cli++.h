#pragma once
#include <stdio.h>
#include <vector>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <fastdb/cli.h>
#include <fastdb/stdtp.h>

namespace cli_plusplus {

    #if 1 //TRUE //def _DEBUG
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
        fetch = 0,      //每次查询数据时最先查询记录条数
        first,          //获取第一条记录
        last,           //获取最后一条记录
        next,           //获取下一条记录
        multy_get,      //批量获取数据，并定位至当前批次第一条
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
        std::vector<char> record;
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
     * initDatabaseSize： 指定数据库初始大小
     * extensionQuantum： 数据库扩展量
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
     * field_desc： 字段描述数组
     * field_num： 字段描述数组大小
     * parament_field_desc： 参数描述数组
     * parament_num ： 字段描述数组大小描述数组大小
     * ret：参照 enum cli_result_code 解析操作结果
     * 
     */
    int create_statement(std::string sql, cli_field_descriptor2 field_desc[], int field_num, ParameterBinding parament_field_desc[], int parament_num);


    /*********************************************************************
     * 批量插入记录
     * ptr： 记录数据地址
     * num：记录个数
     * ret： 成功返回oid（大于等于0）  失败： 小于0
     * 
     */
    int insert(void* ptr, int num);

    /*********************************************************************
     * 批量插入记录并更新库中重复记录
     * ptr： 记录数据地址
     * num：记录个数
     * ret： 成功返回oid（大于等于0）  失败： 小于0
     * 
     */
    int insert_update(void* ptr, int num);


    /*********************************************************************
     * 插入一条记录
     * data： 记录数据引用
     * ret： 成功返回oid（大于等于0）  失败： 小于0
     * 
     */
    int insert(void* data );
    

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
     * ret： 返回记录的指针
     * record_size: 用获取记录的字节长度值，调用后被设置
     */    
    void* get_record(int& record_size);


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
     * 删除表中的当前数据
     *  删除的数据为 select语句游标定位到的语句， 仅仅适用于 逐条查询操作， 注意此场景下fetch操作的update 标记必须为true
     * ret: 参照 enum cli_result_code 解析操作结果
     */
    int remove_curr_record();


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

    /*********************************************************************
     * 获取当前记录的数据字段描述信息 
     * ret: cli_field_descriptor2 数组
     * 
     */
    std::vector<cli_field_descriptor2> get_curr_field_desc();

    /*********************************************************************
     * 获取当前记录的字节大小 
     * ret: cli_field_descriptor2 数组
     * 
     */
    unsigned int get_record_size();

    };
}
