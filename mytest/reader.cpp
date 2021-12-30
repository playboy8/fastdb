
#include "../opt/public.h"



#include <fastdb/fastdb.h>
#include <stdio.h>

USE_FASTDB_NAMESPACE

 
 
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
 
    TYPE_DESCRIPTOR( ( KEY( id, HASHED  ),
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
 
int main()
{

return 0;
}
