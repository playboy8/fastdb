#pragma once
#include <fastdb/cli.h>
#include <pybind11/pybind11.h>
namespace py = pybind11;


//------------snapshot-------------
#pragma pack (1)
typedef struct snapshot
{
    cli_int4_t id;           // id 作为主键唯一标识
    cli_bool_t value;    
    cli_int1_t value1;
 //   cli_int2_t value2;       
 //   cli_int4_t value3;   
 //   cli_int8_t value4; 
 //   cli_int4_t value5;   
 //   cli_int4_t value6;   
 //   cli_int4_t value7; 
 //   cli_int4_t value8;  
 //   cli_int4_t value9;   
 //   cli_int4_t value10; 
 //   cli_int4_t value11;  
 //   cli_int4_t value12;   
 //   cli_int4_t value13; 
 //   cli_int4_t value14;  
 //   cli_int4_t value15;   
 //   cli_int4_t value16; 
 //   cli_int4_t value17;   
 //   cli_int4_t value18;   
 //   cli_int4_t value19; 
 //   cli_int4_t value20; 
 //   cli_int1_t value21[10];
} snapshot;


//-------------kline--------------

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
} kline;
#pragma pack ()      



union record_struct
{
    snapshot snapshot_m;  
    kline   kline_m;   
};

enum record_type
{
    snapshot_rec =0,
    kline_rec
};


struct ParameterBinding_py { 
    char const* u;
    int type;
    char const* name;

    bool convert_parament(ParameterBinding &para)
    {
        py::object obj = py::str(u);

        bool ret = true;
        switch (type)
        {
        case cli_oid : 
            para.u.oid = obj.cast<size_t>();       
                break;
        case cli_bool : 
            para.u.b = obj.cast<bool>();     
                break;
        case cli_int1 : 
            para.u.i1 = obj.cast<char>();   
                break;
        case cli_int2 :
            para.u.i2 = obj.cast<int16_t>(); 
                break;
        case cli_int4 : 
            para.u.i4 = obj.cast<int32_t>();
                break;
        case cli_int8 : 
            para.u.i8 = obj.cast<int64_t>();
                break;
        case cli_real4 : 
            para.u.r4 = obj.cast<float>();
                break;
        case cli_real8 : 
            para.u.r8 = obj.cast<double>();
                break;
        case cli_array_of_int1 : 
        case cli_rectangle : 
        default:
            ret = false;
            break;
        }

        return ret;
    }
};   
