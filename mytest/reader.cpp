

#include "../opt/public.h"
#include <fastdb/fastdb.h>
#include <stdio.h>
#include <thread>
#include <vector>


USE_FASTDB_NAMESPACE


 
class Record
{
public:
    Record(){}
    ~Record(){}
public:
    int4 id;           // id 作为主键唯一标识
    int4 value;    
    int4 value1;
    int4 value2;        // value 作为保存值
    int4 value3;   
    int4 value4; 
    int4 value5;   
    int4 value6;   
    int4 value7; 
    int4 value8;  
    int4 value9;   
    int4 value10; 
    int4 value11;  
    int4 value12;   
    int4 value13; 
    int4 value14;  
    int4 value15;   
    int4 value16; 
    int4 value17;   
    int4 value18;   
    int4 value19; 
    int4 value20;  
 
    TYPE_DESCRIPTOR( ( KEY( id, INDEXED  ),
                       FIELD( value ),                     
                      FIELD( value1) ,
                      FIELD( value2) ,        
                      FIELD( value3) ,  
                      FIELD( value4) ,
                      FIELD( value5) ,  
                      FIELD( value6) ,  
                      FIELD( value7) ,
                      FIELD( value8) , 
                      FIELD( value9) ,  
                      FIELD( value10), 
                      FIELD( value11),  
                      FIELD( value12),   
                      FIELD( value13), 
                      FIELD( value14),  
                      FIELD( value15),   
                      FIELD( value16), 
                      FIELD( value17),   
                      FIELD( value18),   
                      FIELD( value19), 
                      FIELD( value20) ) );
};
 
// 创建 Record 数据表
REGISTER(Record);
 

    #define ROW 10
    #define COL 2

//! 查询数据
void selectRecord( int  times)
{
   printf("####### selectRecord #######\n");
   
   long long sum = 0 ;
   diff_count diff;
   diff.start();

   for (size_t i = 0; i < times; i++)
   {
        dbCursor<Record> cursorRead;  // 只读游标对象
        int n = cursorRead.select();  // 查询
        sum += n;
        while (cursorRead.next())
        {
            /* code */
        }
        
   }
   
    diff.add_snap();
    int a,b;
    diff.show_diff(a,b,true);
    printf(" totole serch records: %lld\n", sum);

}

void test_insert(int test_count, int test_par[][COL], int test_result[][COL], int threadid)
{
    unsigned long initsize = 3 *1024* 1024* 1024UL;
    unsigned long extqn = 4194304UL;
    unsigned long initindexsize = 524288UL;
   dbDatabase db(dbDatabase::dbConcurrentRead, initsize,extqn,initindexsize,1, 6 );
//   printf(" will open database  threadid:%d\n ",threadid);
//   sleep(1);
    if (db.open(_T("testpar"),nullptr,3))
    {
//    printf(" opened database  threadid:%d\n ",threadid);
//    sleep(1);
        do
        {
        // 插入数据

        // 查询数据
        selectRecord(1);

        continue;   

        while (1)
        {
            sleep(100);
        }    
        }
        while (false);
    }
    else
    {
        printf(" open database failed ! ");
    }
    //std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    if(db.isOpen())
    {
        // 删除所有数据
       // removeAllRecord();
        // 关闭数据库
        db.close();
    }   
}
 

int main()
{
    // 打开数据库 testpar
    int th_count =1;

    const int test_count = 10 *10000;
    int test_par[ROW][COL];  // 批量测试参数
    int test_result[th_count][ROW][2]={0,};  // 批量测试参数
    for(int i = 0 ; i < ROW; i++)
    {
        test_par[i][0] = 2*i+1;
        test_par[i][1] = test_count/(test_par[i][0]);
    }

    std::vector<std::thread> test_th;
   
    for(int i= 0; i< th_count; i++)
    {
        std::thread  th(test_insert,test_count,test_par,test_result[i], i);
        test_th.emplace_back(std::move(th));
    }
    //test_insert(test_count, test_par, test_result);

     for(auto &t : test_th)
     {
         if(t.joinable())
            t.join();
     }
     

    
   return 0;
}


