// test local api

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
    bool value;    
    int1 value1;
    int2 value2;        
    int4 value3;   
    int8 value4; 
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
    dbArray<int1> value21;


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
                      FIELD( value20),
                      FIELD( value21)  ) );
};
 
REGISTER(Record);
 
void insertRecord(int size, int id)
{
    for ( int4 i = 0; i < size; i++ )
    { 
        Record rec;
        rec.id = i;
        rec.value =  i;
        rec.value1 = i;
        rec.value2 = i;
        rec.value3 = i;
        rec.value4 = i;
        rec.value5 = i;
        rec.value6 = i;
        rec.value7 = i;
        rec.value8 = i;
        rec.value9 = i;
        rec.value10 = i;
        rec.value11 = i;
        rec.value12 = i;
        rec.value13 = i;
        rec.value14 = i;
        rec.value15 = i;
        rec.value16 = i;
        rec.value17 = i;
        rec.value18 = i;
        rec.value19 = i;
        rec.value20 = i;
        insert( rec );                         
    }
}
 
//! 更新数据
void updateRecord()
{
    printf("####### updateRecord #######\n");
   
    dbCursor<Record> cursorWrite(dbCursorForUpdate);  
    int n = cursorWrite.select();                    
    if(0 < n)
    {
        do
        {
            cursorWrite->value = cursorWrite->value + 1;
            cursorWrite.update();
        } while(cursorWrite.next());  // next() 游标向后滚
   }
    
}
 
void removeRecord(const int id)
{
    printf("####### removeRecord #######\n");
    dbQuery q;                                        
    dbCursor<Record> cursorWrite(dbCursorForUpdate);  
   
    q = "id =", id;
    int n = cursorWrite.select(q);   
    if(0 < n)
    {
        do
        {
            cursorWrite.removeAllSelected();        
        } while(cursorWrite.next()); 
    }
}
 
void removeAllRecord()
{

    dbCursor<Record> cursorWrite(dbCursorForUpdate);  
    cursorWrite.removeAll();                   
}
 

void selectRecord()
{
   printf("####### selectRecord #######\n");
   
   diff_count diff;
   diff.start();
    dbCursor<Record> cursorRead;  
    int n = cursorRead.select();  
    diff.add_snap();
    int a,b;
    diff.show_diff(a,b,true);
    if(0 < n)
    {
        int cnt = 1;
        do
        {
            printf("%d---%d---%d --- size= %d\n", n, cursorRead->id, cursorRead->value,n);
            cnt-- ;
        }while(cursorRead.next() && cnt > 0); 
    }
}


#define ROW 10
#define COL 2

void test_insert(int test_count, int test_par[][COL], int test_result[][COL], int threadid)
{
    unsigned long initsize = 3 *1024* 1024* 1024UL;
    unsigned long extqn = 4194304UL;
    unsigned long initindexsize = 524288UL;
   dbDatabase db(dbDatabase::dbAllAccess, initsize,extqn,initindexsize,1, 6 );
    if (db.open(_T("testpar"),nullptr,3))
    {
        do
        {

        for(int i = 0 ; i < ROW; i++)
        {
            diff_count diff;
            diff.start();
            for(int j = 0; j < test_par[i][1]; j++)
            {
                
                insertRecord(test_par[i][0], threadid);         
                db.precommit();      
            }
            diff.stop();
            db.commit();
            diff.show_diff(test_result[i][0],test_result[i][1]);           
        }

        selectRecord();
        continue;   
    
        updateRecord();                
    
        db.commit();     
   
        selectRecord();      

        removeRecord(1);
 
        db.commit();     
 
        selectRecord();     

        removeAllRecord();

        db.commit();   

        selectRecord();
        }
        while (false);
    }
    else
    {
        printf(" open database failed ! ");
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    if(db.isOpen())
    {
        removeAllRecord();
        db.close();
    }   
}
 
int main()
{

    int th_count =1;
    const int test_count = 1 *10;
    int test_par[ROW][COL];  // 批量测试参数
    int test_result[th_count][ROW][2];  // 批量测试参数
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

     for(auto &t : test_th)
     {
         if(t.joinable())
            t.join();
     }
     
    for(int j= 0; j< th_count; j++)
    {
        printf("\n %s\t\t %s\t %s\t %s\t\t %s\t %s\t\t\n" , "总条数", "每次发送条数" ,"发送线程数" ,"延时ms" , "仅发送延迟" , "IPS");
        for(int i = 0 ; i < ROW; i++)
        {
           printf("%16d %8d %8d %16d %16d \t %16f \n" ,  test_par[i][0]*test_par[i][1] ,  test_par[i][0], 1 , test_result[j][i][0], test_result[j][i][1],   (test_count*1000 *1.0 )/ (test_result[j][i][0] *1.0)   );
        }
    }
   return 0;
}
