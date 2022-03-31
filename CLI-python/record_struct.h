#pragma once
#include "../CLI++/cli++.hpp"

//------------snapshot-------------
#pragma pack (1)
typedef struct snapshot
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
} snapshot;
#pragma pack ()

static cli_field_descriptor2 snapshot_desc[] = {
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

static cli_plusplus::ParameterBinding snapshot_paraments[2]= {
    { {.i4=0}, .type= cli_int4, .name="%id" },
    { {.i1=2}, .type= cli_int1, .name="%value1" }
};


//-------------kline--------------

#pragma pack (1)
typedef struct kline
{
    cli_int4_t  stock_id;            //  
    cli_int8_t  market_time;         //yyyymmddhhmmssmmm   
    cli_int8_t  update_time;         //yyyymmddhhmmssmmm   
    cli_real8_t open;
    cli_real8_t high;         
    cli_real8_t low;   
    cli_real8_t close; 
    cli_int8_t  volume;   
    cli_real8_t  turnover;   
    cli_int1_t  value1[10];
} kline;
#pragma pack ()      

static cli_plusplus::ParameterBinding kline_paraments[2]= {
    { {.i4=0}, .type= cli_int4, .stock_id="%stock_id" },
    { {.i1=2}, .type= cli_int8, .market_time="%market_time" }
};

union record_struct
{
    snapshot snapshot_m;  
    kline   kline_m;   
};
