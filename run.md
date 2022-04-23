# MS_FASTDB 使用指南

## 代码编译



- fastdb代码路径   
    url="https://172.16.0.55/ms-tech/ms-fastdb.git"

- 代码依赖   
    无

- 编译与安装   


    1. 克隆工程代码        
    `git clone $url  /home/work/ms-fastdb `  
    `cd /home/work/ms-fastdb   `
    

    2. 编译并安装  
    将执行文件、及动态库和头文件安装到 /opt/fastdb64/bin/ 目录下    
    `sudo sh build_fastdb.sh`

    
    3. 设置环境变量， 方便使用subsql、cleanupsem、forcerecovery 命令    
    `sudo  echo " PATH=$PATH:/opt/fastdb64/bin"  >> /etc/profile ;  source profile  `    
        在上一步编译安装之后会在 /opt/fastdb64/ 目录下建立对应的目录和文件
        ```
        /opt/fastdb64/
        ├── bin
        │   ├── cleanupsem
        │   ├── forcerecovery
        │   └── subsql
        ├── include
        │   ├── fastdb
        │   │   ├── ....
        │   ├── sockio.h
        │   └── stdtp.h
        └── lib
            ├── libcli.a
            ├── libcli.la
            ├── libcli.so -> libcli.so.2.0.0
            ├── libcli.so.2 -> libcli.so.2.0.0
            ├── libcli.so.2.0.0
            ├── libfastdb.a
            ├── libfastdb.la
            ├── libfastdb.so -> libfastdb.so.2.0.0
            ├── libfastdb.so.2 -> libfastdb.so.2.0.0
            └── libfastdb.so.2.0.0
        ```



## 搭建内存数据库服务    

1. 启动内存数据库   
    在终端执行 subsql 命令如下：    
    `$ subsql `
    ```
    SubSQL interactive utility for FastDB v. 3.75
    Type 'help' for more information
    >> 
    ```    
    以上进入了 subsql 命令的会话窗口，窗口中支持特定命令， 输入help会显示 命令用法, 如下：    

    `>> help`
    ```
    SubSQL commands:

    open 'database-name' ( 'database-file-name' ) ';'
    select ('*') from <table-name> where <condition> ';'
    update <table-name> set <field-name> '=' <expression> {',' <field-name> '=' <expression>} where <condition> ';'
    create table <table-name> '('<field-name> <field-type> {',' <field-name> <field-type>}')' ';' 
    alter table <table-name> '('<field-name> <field-type> {',' <field-name> <field-type>}')' ';' 
    rename <table-name> '.' <old-field-name> 'to' <new-field-name> ';' 
    delete from <table-name>
    drop table <table-name>
    drop index <table-name> {'.' <field-name>} ';'
    create index on <table-name> {'.' <field-name>} ';'
    drop hash <table-name> {'.' <field-name>};
    create hash on <table-name> {'.' <field-name>}field> ';'
    insert into <table-name> values '(' <value>{',' <value>} ')' ';'
    backup [compactify] 'file-name'
    start server URL number-of-threads
    stop server URL
    start http server URL
    stop http server
    describe <table-name>
    import 'xml-file-name'
    export 'xml-file-name'
    show
    profile
    commit
    rollback
    autocommit (on|off)
    exit
    help
    ```    
    输入 exit 命令退出会话状态。

    以上列举了subsql 中的命令，在会话窗口中逐条输入命令来完成数据库定义和创建是不方便的，可以编写 sql 脚本来批量执行命令。假设我们编写了数据库操作的脚本demo.sql, 我们使用如下命令来启动数据库服务：    

    `$ cd /home/work/ms-fastdb/example3`  
    `$ subsql demo.sql `
    ```
    Compile query for table snapshot
    Index search for field snapshot.sym: 0 probes
    No records selected

    serverURL: localhost:6100  thid=4
    Server started for URL localhost:6100
    SubSQL interactive utility for FastDB v. 3.75
    Type 'help' for more information
    >> 
    ```
    以上信息显示服务器正常运行了，服务在监听本地6100 端口， 有4个客户服务线程， fastdb版本为3.7.5 。
    输入 describe [table] 查询表的字段属性, 示例如下：

    `>> describe KLine`   

        OID=0x080004, TableName=KLine
        No Index FieldType        RefTableName     FieldName        InverseFieldName Flg
        --------------------------------------------------------------------------------
        0  T     Int4             (null)           stock_id         (null)           2
        1  T     Int8             (null)           market_time      (null)           2
        2  -     Int8             (null)           update_time      (null)           0
        3  -     Real8            (null)           Open             (null)           0
        4  -     Real8            (null)           High             (null)           0
        5  -     Real8            (null)           Low              (null)           0
        6  -     Real8            (null)           Close            (null)           0
        7  -     Int8             (null)           volume           (null)           0
        8  -     Real8            (null)           turnover         (null)           0
        9  -     Array            (null)           value1           (null)           0
        >> 

    输入 exit 将退出数据库服务。


2. 使用sql语句创建数据库服务脚本 

    在上面用到了demo.sql 文件，对sql脚本创建并启动数据库示例如下, 脚本中不支持符号#的注释，以下注释仅用作说明：
    #创建数据库服务示例: 
    ```
    #打开或创建本地数据库
    open 'clitest';      

    #创建本地数据表person和其成员   
    create table persons (  name    string, 
                            salary  int8, 
                            address string, 
                            weight  real8, 
                            subordinates array of reference to persons,
                            blob array of int1  );
    #设置索引字段/键                        
    create index on persons.salary;

    #设置索引字段/键
    create hash on persons.name;

    #启动网络服务，包含端口和服务线程数
    start server 'localhost:6100' 8
    ```

    注意： 不要在 subsql 会话中执行全量查询输出的语句，逐条打印数据话费时间较长且会阻塞服务。    
    更多内容请参考下文 'subsql语句语法规则' 部分。    
 




## CLI-pytho接口

1. 编译CLI-python包     
    说明：需要安装python、 python-dev ;  安装 numpy      
    `cd /home/work/ms-fastdb/CLI-python`     
    `sh build.sh`

    编译后build目录生成python运行的依赖库 cli_py.cpython-36m-x86_64-linux-gnu.so 

2. 运行本地数据库服务和测试程序      
    - 运行本地数据库服务       
    `subsql ../demo.sql`      

    - 运行python程序      
    新开一个终端，然后      
    `cd /home/work/ms-fastdb/CLI-python/build`     
    `python3 test.py`     

    测试程序将 插入数据到数据库 ， 然后 查询数据库内容， 最后删除所有数据, 运行后终端正常打印出读到的数据，并显示删除数据完成，则运行成功。
 
3. CLI-pytho接口说明  

    [返回类型]函数名[参数]
```
    cli_python()

    #数据库初始化
    void cli_python_init(std::string url, std::string database_name, std::string file_name="", size_t initDatabaseSize=100*1024*1024UL, size_t 
    extensionQuantum=100*1024*1024UL, size_t initIndexSize=512*1024UL, size_t fileSizeLimit=0UL)

    #打开数据库
    int open( int retry, int timeout)

    #创建操作语句
    int create_statement(py::str sql,  numpy.array field_descs, numpy.array parament_field_descs)

    #获取查询到的数据
    list  get_record()

    #逐条插入数据
    int insert_one(list &record);

    #批量插入多条数据
    int insert(list &record, int num);

    #插入并更新重复数据
    int insert_update(list &record , int num)

    #删除全部数据
    int remove_all(str table)

    #删除当前数据
    int remove_curr()

    #使用游标选择数据
    int select(int auth, select_flag flag)

    #更新数据
    int update()

    #提交数据
    int commit()

    #预提交数据
    int precommit()
```
更多使用细节参考 /home/work/ms-fastdb/CLI-python/test.py





## CLI++ 接口

### 接口介绍

### 测试用例

```
    1. CLI++ 接口的使用示例路径： /home/work/ms-fastdb/example3

    2. 编译CLI测试程序
    cd /home/work/ms-fastdb/example3
    sh build.sh
    
    3. 示例程序说明
    fastdb cli++ 接口测试程序  ├—————— fastdb_cli++  // CLI++ 接口的 记录插入,查询，修改，删除。 


    对于CLI++客户端的操作均包含在以上示例中，可参考示例的实现。

```


## SUBSQL 语句

### subsql语句语法规则

```
    subsql 是 sql语句的一个子集，语句语法说明如下 ： 

    directive ::= 
        select (*) from table-name select-condition ;
    | insert into table-name values values-list ;
    | create index on on table-name.field-name ;
    | create table table-name (field-descriptor {, field-descriptor}) ;
    | alter table table-name (field-descriptor {, field-descriptor}) ;
    | update table-name set field-name = expression {, field-name = expression} where condition ;
    | drop index table-name.field-name ;
    | drop table table-name
    | open database-name ( database-file-name ) ;
    | delete from table-name
    | backup file-name
    | start server server-URL number-of-threads
    | stop server server-URL
    | start http server server-URL 
    | stop http server server-URL
    | export xml-file-name
    | import xml-file-name
    | commit
    | rollback
    | autocommit (on | off)
    | exit
    | show
    | help

    table-name ::= identifier
    values-list ::= tuple { , tuple }
    tuple ::= ( value { , value } )
    value ::= number | string | true | false 
                | tuple
    index ::= index | hash
    field-descriptor ::= field-name field-type (inverse field-name)
    field-name ::= identifier { . identifier }
    database-name ::= string
    database-file-name ::= string
    xml-file-name ::= string
    file-name ::= string
    server-URL ::= 'HOST:PORT'
```

## 查询语句

### 查询语句语法规则

select-condition ::= ( expression ) ( traverse ) ( order )
expression ::= disjunction
disjunction ::= conjunction 
        | conjunction or disjunction
conjunction ::= comparison 
        | comparison and conjunction
comparison ::= operand = operand 
        | operand != operand 
        | operand <> operand 
        | operand < operand 
        | operand <= operand 
        | operand > operand 
        | operand >= operand 
        | operand (not) like operand 
        | operand (not) like operand escape string
        | operand (not) match operand
        | operand (not) in operand
        | operand (not) in expressions-list
        | operand (not) between operand and operand
	| operand is (not) null
operand ::= addition
additions ::= multiplication 
        | addition +  multiplication
        | addition || multiplication
        | addition -  multiplication
multiplication ::= power 
        | multiplication * power
        | multiplication / power
power ::= term
        | term ^ power
term ::= identifier | number | string 
        | true | false | null 
	| current | first | last
	| ( expression ) 
        | not comparison
	| - term
	| term [ expression ] 
	| identifier . term 
	| function term
        | exists identifier : term
function ::= abs | length | lower | upper
        | integer | real | string | user-function
string ::= ' { { any-character-except-quote } ('') } '
expressions-list ::= ( expression { , expression } )
order ::= order by sort-list
sort-list ::= field-order { , field-order }
field-order ::= [length] field (asc | desc)
field ::= identifier { . identifier }
traverse ::= start from field ( follow by fields-list )
fields-list ::=  field { , field }
user-function ::= identifier
Identifiers are case sensitive, begin with a a-z, A-Z, '_' or '$' character, contain only a-z, A-Z, 0-9, '_' or '$' characters, and do not duplicate a SQL reserved word.

### 查询语句内置函数

abs	and	asc	between	by
current	desc	escape	exists	false
first	follow	from	in	integer
is	length	like	last	lower
match	not	null	or	real
start	string	true	upper

ANSI-standard comments may also be used. All characters after a double-hyphen up to the end of the line are ignored.



### 查询语句示例

```
select * from TestTable where price >=  25.66      
select * from TestTable where name = 'xyz'  
select * from TestTable where name = 'xyz' and id = 555 order by name

select from Supplier,Shipment,Detail where 
                 Supplier.SID = Shipment.SID and Shipment.DID = Detail.DID 
		 and Supplier.company like ? and Supplier.address like ?
		 and Detail.name like ? 

select from TestTable where  detail.name like",name," and supplier.company like",company,
	         " and supplier.address like",address,"order by price

```








