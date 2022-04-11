#pragma once
#include <fastdb/cli.h>
#include <pybind11/pybind11.h>
#include <string>
#include <stdlib.h>
namespace py = pybind11;


//------------snapshot-------------
#pragma pack (1)
typedef struct snapshot
{
    cli_int4_t sym;
    cli_int4_t szWindCode;
    cli_int4_t nActionDay;
    cli_int4_t nTime;
    cli_int4_t nStatus;
    cli_int4_t nPreClose;
    cli_int4_t nOpen;
    cli_int4_t nHigh;
    cli_int4_t nLow;
    cli_int4_t nMatch;
    cli_int4_t nAskPrice1;
    cli_int4_t nAskPrice2;
    cli_int4_t nAskPrice3;
    cli_int4_t nAskPrice4;
    cli_int4_t nAskPrice5;
    cli_int4_t nAskPrice6;
    cli_int4_t nAskPrice7;
    cli_int4_t nAskPrice8;
    cli_int4_t nAskPrice9;
    cli_int4_t nAskPrice10;
    cli_int4_t nAskVol1;
    cli_int4_t nAskVol2;
    cli_int4_t nAskVol3;
    cli_int4_t nAskVol4;
    cli_int4_t nAskVol5;
    cli_int4_t nAskVol6;
    cli_int4_t nAskVol7;
    cli_int4_t nAskVol8;
    cli_int4_t nAskVol9;
    cli_int4_t nAskVol10;
    cli_int4_t nBidPrice1;
    cli_int4_t nBidPrice2;
    cli_int4_t nBidPrice3;
    cli_int4_t nBidPrice4;
    cli_int4_t nBidPrice5;
    cli_int4_t nBidPrice6;
    cli_int4_t nBidPrice7;
    cli_int4_t nBidPrice8;
    cli_int4_t nBidPrice9;
    cli_int4_t nBidPrice10;
    cli_int4_t nBidVol1;
    cli_int4_t nBidVol2;
    cli_int4_t nBidVol3;
    cli_int4_t nBidVol4;
    cli_int4_t nBidVol5;
    cli_int4_t nBidVol6;
    cli_int4_t nBidVol7;
    cli_int4_t nBidVol8;
    cli_int4_t nBidVol9;
    cli_int4_t nBidVol10;
    cli_int4_t nNumTrades;
    cli_int8_t iVolume;
    cli_int8_t iTurnover;
    cli_int4_t nTotalBidVol;
    cli_int4_t nTotalAskVol;
    cli_int4_t nWeightedAvgBidPrice;
    cli_int4_t nWeightedAvgAskPrice;
    cli_int4_t nIOPV;
    cli_int4_t nYieldToMaturity;
    cli_int4_t nHighLimited;
    cli_int4_t nLowLimited;
    cli_int4_t nSyl1;
    cli_int4_t nSyl2;
    cli_int4_t nSD2;
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
//    cli_int1_t value1[10];
//    kline()
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

enum stat_func {
    stat_for_insert = 0,
    stat_for_select_all,
    stat_for_select_spec,
    stat_for_update
};


typedef struct cli_field_descriptor2_py { 
    enum cli_var_type type;
    int               flags;
    char        name[64];
    int               len;
    char        refTableName[64];
    char        inverseRefFieldName[64];

    bool convert_parament(cli_field_descriptor2 &field)
    {
        field.type = cli_var_type(type);
        field.flags = flags;
        field.name = name;
        field.len = len;
        field.refTableName = refTableName;
        field.inverseRefFieldName = inverseRefFieldName;
        return true;
    }
    
} cli_field_descriptor2_py;


struct ParameterBinding_py { 
    char u[64];
    int type;
    char name[64];

    bool convert_parament(ParameterBinding &para)
    { // u 的值必须 按照 python 方式转化成str， 在c++中再还原成原类型   
        bool ret = true;
        switch (type)
        {
        case cli_oid : 
            para.u.oid = atoi(u);     std::cout << " para.u = " << para.u.oid << std::endl;    
                break;
        case cli_bool : 
            para.u.b = atoi(u);         std::cout << " para.u = " << para.u.b << std::endl;   
                break;
        case cli_int1 : 
            para.u.i1 = atoi(u);        std::cout << " para.u = " << para.u.i1 << std::endl;  
                break;
        case cli_int2 :
            para.u.i2 = atoi(u);     std::cout << " para.u = " << para.u.i2 << std::endl;
                break;
        case cli_int4 : 
            para.u.i4 = atoi(u);     std::cout << " para.u = " << para.u.i4 << std::endl;
                break;
        case cli_int8 : 
            para.u.i8 = atol(u);     std::cout << " para.u = " << para.u.i8 << std::endl;
                break;
        case cli_real4 : 
            para.u.r4 = atof(u);      std::cout << " para.u = " << para.u.r4 << std::endl;
                break;
        case cli_real8 : 
            para.u.r8 = atof(u);      std::cout << " para.u = " << para.u.r8 << std::endl;
                break;
        case cli_array_of_int1 : 
        case cli_rectangle : 
        default:
            ret = false;
            break;
        }
        para.type = type;
        para.name = name;
        return ret;
    }
};   
