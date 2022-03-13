#include "../CLI++/cli++.hpp"

#pragma pack (1)
typedef struct Record
{
    cli_int4_t id;           // id 作为主键唯一标识
    cli_bool_t value;    
    cli_int1_t value1;
    cli_int2_t value2;       
    cli_int4_t value3;   
    cli_int8_t value4; 
    cli_int4_t value5;   
    cli_int4_t value6;   
    cli_int4_t value7; 
    cli_int4_t value8;  
    cli_int4_t value9;   
    cli_int4_t value10; 
    cli_int4_t value11;  
    cli_int4_t value12;   
    cli_int4_t value13; 
    cli_int4_t value14;  
    cli_int4_t value15;   
    cli_int4_t value16; 
    cli_int4_t value17;   
    cli_int4_t value18;   
    cli_int4_t value19; 
    cli_int4_t value20; 
    cli_int1_t value21[10];
} Record;
#pragma pack ()

static cli_field_descriptor2 record_descriptor[] = {
    {cli_int4, cli_indexed, "id",1},
    {cli_bool, 0, "value"   ,1  },
    {cli_int1, 0, "value1"  ,1  },
    {cli_int2, 0, "value2"  ,1  },
    {cli_int4, 0, "value3"  ,1  },
    {cli_int8, 0, "value4"  ,1  },
    {cli_int4, 0, "value5"  ,1  },
    {cli_int4, 0, "value6"  ,1  },
    {cli_int4, 0, "value7"  ,1  },
    {cli_int4, 0, "value8"  ,1  },
    {cli_int4, 0, "value9"  ,1  },
    {cli_int4, 0, "value10" ,1  },
    {cli_int4, 0, "value11" ,1  },
    {cli_int4, 0, "value12" ,1  },
    {cli_int4, 0, "value13" ,1  },
    {cli_int4, 0, "value14" ,1  },
    {cli_int4, 0, "value15" ,1  },
    {cli_int4, 0, "value16" ,1  },
    {cli_int4, 0, "value17" ,1  },
    {cli_int4, 0, "value18" ,1  },
    {cli_int4, 0, "value19" ,1  },
    {cli_int4, 0, "value20" ,1  },
    {cli_array_of_int1,0,"value21", 10}
};        

static cli_plusplus::ParameterBinding paraments[2];



int main()
{
    int rc = cli_ok;
    bool empty = false;
    cli_plusplus::cli_api<Record>  dbhandle("127.0.0.1:6100", "testpar"); 
    rc = dbhandle.opendb(3, 1);
    if(cli_bad_address == rc || rc < 0 )
    {
        std::cout << "打开数据库失败 ！  错误码：" << rc << std::endl;
    }

    if(cli_ok != (rc = dbhandle.create_statement(" select * from Record;", record_descriptor, sizeof(record_descriptor)/sizeof(record_descriptor[0]), nullptr,0)))
    {
        std::cout << "创建sql 查询语句失败 ！  错误码：" << rc << std::endl;
    }

    rc = dbhandle.select(cli_view_only, cli_plusplus::select_flag::fetch);
    if(rc < 0 )
    {
        std::cout << "查询数据失败  错误码：" << rc << std::endl;
    }
    else
    {
        empty = (rc == 0);
        std::cout << "查询到数据条数 ：" << rc << std::endl;
    }

    if(!empty)
    {
        rc = dbhandle.select(cli_view_only, cli_plusplus::select_flag::first);
        if(cli_ok != rc )
        {
            std::cout << "获取第一条数据失败  错误码：" << rc << std::endl;
        }
        else
        {
            Record &p = dbhandle.get_record();
            std::cout << "获取第一条数据 ,返回码：" << rc << std::endl;
            std::cout << "\n id    \t" <<  int(p.id); 
            std::cout << "\n value \t" <<  int(p.value);    
            std::cout << "\n value1\t" <<  int(p.value1);
            std::cout << "\n value3\t" <<  p.value3;   
            std::cout << "\n value4\t" <<  p.value4; 
            std::cout << "\n value5\t" <<  p.value5;   
            std::cout << "\n value6\t" <<  p.value6;   
            std::cout << "\n value7\t" <<  p.value7; 
            std::cout << "\n value8\t" <<  p.value8;  
            std::cout << "\n value9\t" <<  p.value9;   
            std::cout << "\n value10\t" <<  p.value10; 
            std::cout << "\n value11\t" <<  p.value11;  
            std::cout << "\n value12\t" <<  p.value12;   
            std::cout << "\n value13\t" <<  p.value13; 
            std::cout << "\n value14\t" <<  p.value14;  
            std::cout << "\n value15\t" <<  p.value15;   
            std::cout << "\n value16\t" <<  p.value16; 
            std::cout << "\n value17\t" <<  p.value17;   
            std::cout << "\n value18\t" <<  p.value18;   
            std::cout << "\n value19\t" <<  p.value19; 
            std::cout << "\n value20\t" <<  p.value20; 
            std::cout << "\n value21\t" <<  (char*)(p.value21);
            std::cout << "\n";
        }
    }
    else
    {
        if(cli_ok != (rc = dbhandle.create_statement(" insert to Record;", record_descriptor, sizeof(record_descriptor)/sizeof(record_descriptor[0]), nullptr,0)))
        {
            std::cout << "创建sql 插入语句失败 ！  错误码：" << rc <<  std::endl;
        }

        int record_num = 3;
        Record record_arry[record_num]={{0,}};

        for(int i=0 ; i < record_num; i++)
        {
            record_arry[i].id = (i%record_num)/2 + 10000*i;
            record_arry[i].value = i%2 ? true: false;
            record_arry[i].value1 = i+2;
            record_arry[i].value2 = i+3;
            record_arry[i].value3 = i+4;
            record_arry[i].value4 = i+5;
            record_arry[i].value5 = i+6;
            record_arry[i].value6 = i+7;
            record_arry[i].value7 = i+8;
            record_arry[i].value8 = i+9;
            record_arry[i].value9 = i+10;
            record_arry[i].value10 = i+11;
            strcpy((char*)record_arry[i].value21,"hello ");
        }

        dbhandle.insert_update(record_arry,record_num);
    }

    


}