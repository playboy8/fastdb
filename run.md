# MS_FASTDB 使用指南

## 代码编译

- 代码路径
```
    url="https://172.16.0.55/ms-tech/ms-fastdb.git"
```
- 代码依赖
    无

- 编译与安装


```
    #编译和安装服务工具

    git clone $URL  /home/work/ms-fastdb
    cd /home/work/ms-fastdb
    sudo sh build_fastdb.sh



    #编译CLI测试程序
    cd /home/work/ms-fastdb/example2
    sh build.sh

```

## 内存数据库服务搭建
```
1.  在上一步编译安装之后会在 /opt/fastdb64/ 目录下建立对应的目录和文件

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

    执行 sudo  echo " PATH=$PATH:/opt/fastdb64/bin"  >> /etc/profile ;  source profile  设置环境变量。
    在终端执行 subsql 命令后出现以下表示服务安装完成：
        $ subsql
        SubSQL interactive utility for FastDB v. 3.75
        Type 'help' for more information
        >> 

    输入help 查询使用方法，输入exit 退出。



2. 使用subsql语句创建数据库服务脚本 

    #创建数据库服务示例:

    open 'clitest';  
    create table persons (  name    string, 
                            salary  int8, 
                            address string, 
                            weight  real8, 
                            subordinates array of reference to persons,
                            blob array of int1  );
    create index on persons.salary;
    create hash on persons.name;
    start server 'localhost:6100' 8

    以上语句说明：创建或打开 clitest 数据库，创建 persons 表， 将salary、name 字段设置为索引，开启网络服务。

    创建数据库服务请参考 /home/work/ms-fastdb/examples2/demo.sql 脚本。
  

3. 启动数据库服务

    执行  subsql  /home/work/ms-fastdb/examples2/demo.sql  来启动数据库服务


        $ subsql examples2/demo.sql 

        No records selected

        serverURL: localhost:6100  thid=4
        Server started for URL localhost:6100
        SubSQL interactive utility for FastDB v. 3.75
        Type 'help' for more information
        >> 

    以上表示服务启动成功， 此时可以使用 CLI 接口远程连接到该数据库进行数据操作。

```

## subsql其它使用
```
 在subsql会话界面输入指令 show 查看当前服务上的 表信息，如下：

        >> show
        FastDB   version  :  3.75
        Database version  :  3.75
        Database file size: 4194304 Kb
        Object index size : 524800 handles
        Used part of index: 524295 handles
        Number of users   : 1
        Number of readers : 1
        Number of writers : 0
        Number of blocked readers : 0
        Number of blocked writers : 0

        TABLES:
        OID       FixedSize   Fields  Columns  TableName
        ---------------------------------------------------------
        0x080004        88       11       10   KLine
        0x080002       104       24       23   Record
        0x000001        52       17        8   Metatable
        >> 



    输入 describe [table] 查询表的属性, 如下：

        >> describe KLine

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



    此处可以输入subsql语句对对应的数据库进行操作， 对数据库备份 以及恢复 等其它管理操作。

   注意： 不要在 subsql 会话中执行全量查询输出的语句，逐条打印数据话费时间较长且会阻塞服务。

```


## CLI++ 接口

```
    1. CLI++ 接口的使用示例路径： /home/work/ms-fastdb/example3

    2. 编译CLI测试程序
    cd /home/work/ms-fastdb/example3
    sh build.sh
    
    3. 示例程序说明
    fastdb cli++ 接口测试程序  ├—————— fastdb_cli++  // CLI++ 接口的 记录插入,查询，修改，删除。 


    对于CLI++客户端的操作均包含在以上示例中，可参考示例的实现。

```

##  CLI 接口
```

    1. CLI 接口的使用示例路径： /home/work/ms-fastdb/example2 


    2. 编译CLI测试程序
    cd /home/work/ms-fastdb/example2
    sh build.sh
    
    3. 示例程序说明
    fastdb 本地接口测试程序  ├—————— fast_demo 
                            |—————— reader

    fastdb CLI接口测试程序   ├────── cliwrite    //远程逐条写入
                            ├────── cliread     //远程逐条读取
                            ├────── cliwrite2   //远程批量写入
                            ├────── cliread2    //远程批量读取
                            ├────── cliwrite2_filter //远程批量写入和更新
                            ├────── KLine_pro    //K线发布服务示例
                            ├────── KLine_recive //K线消费服务示例

    /home/work/ms-fastdb/example2/readme.txt 中列举了部分情景下接口的性能表现。

    对于CLI客户端的操作均包含在以上示例中，可参考示例的实现。

    
    
```











