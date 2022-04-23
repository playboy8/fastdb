#include "record_struct.h"
//#include <fastdb/stdtp.h>

//------------snapshot-------------
cli_field_descriptor2 snapshot_desc[] = {
    {cli_int8, 0, "sym"                 , 1 }, 
    {cli_int8, 0, "szWindCode"           , 1 },        
    {cli_int8, 0, "nActionDay"           , 1 },        
    {cli_int8, 0, "nTime"                , 1 },    
    {cli_int8, 0, "nStatus"              , 1 },    
    {cli_real8, 0, "nPreClose"            , 1 },        
    {cli_real8, 0, "nOpen"                , 1 },    
    {cli_real8, 0, "nHigh"                , 1 },    
    {cli_real8, 0, "nLow"                 , 1 },    
    {cli_int8, 0, "nMatch"               , 1 },    
    {cli_real8, 0, "nAskPrice1"           , 1 },        
    {cli_real8, 0, "nAskPrice2"           , 1 },        
    {cli_real8, 0, "nAskPrice3"           , 1 },        
    {cli_real8, 0, "nAskPrice4"           , 1 },        
    {cli_real8, 0, "nAskPrice5"           , 1 },        
    {cli_real8, 0, "nAskPrice6"           , 1 },        
    {cli_real8, 0, "nAskPrice7"           , 1 },        
    {cli_real8, 0, "nAskPrice8"           , 1 },        
    {cli_real8, 0, "nAskPrice9"           , 1 },        
    {cli_real8, 0, "nAskPrice10"          , 1 },        
    {cli_int8, 0, "nAskVol1"             , 1 },        
    {cli_int8, 0, "nAskVol2"             , 1 },        
    {cli_int8, 0, "nAskVol3"             , 1 },        
    {cli_int8, 0, "nAskVol4"             , 1 },        
    {cli_int8, 0, "nAskVol5"             , 1 },        
    {cli_int8, 0, "nAskVol6"             , 1 },        
    {cli_int8, 0, "nAskVol7"             , 1 },        
    {cli_int8, 0, "nAskVol8"             , 1 },        
    {cli_int8, 0, "nAskVol9"             , 1 },        
    {cli_int8, 0, "nAskVol10"            , 1 },        
    {cli_real8, 0, "nBidPrice1"           , 1 },        
    {cli_real8, 0, "nBidPrice2"           , 1 },        
    {cli_real8, 0, "nBidPrice3"           , 1 },        
    {cli_real8, 0, "nBidPrice4"           , 1 },        
    {cli_real8, 0, "nBidPrice5"           , 1 },        
    {cli_real8, 0, "nBidPrice6"           , 1 },        
    {cli_real8, 0, "nBidPrice7"           , 1 },        
    {cli_real8, 0, "nBidPrice8"           , 1 },        
    {cli_real8, 0, "nBidPrice9"           , 1 },        
    {cli_real8, 0, "nBidPrice10"          , 1 },        
    {cli_int8, 0, "nBidVol1"             , 1 },        
    {cli_int8, 0, "nBidVol2"             , 1 },        
    {cli_int8, 0, "nBidVol3"             , 1 },        
    {cli_int8, 0, "nBidVol4"             , 1 },        
    {cli_int8, 0, "nBidVol5"             , 1 },        
    {cli_int8, 0, "nBidVol6"             , 1 },        
    {cli_int8, 0, "nBidVol7"             , 1 },        
    {cli_int8, 0, "nBidVol8"             , 1 },        
    {cli_int8, 0, "nBidVol9"             , 1 },        
    {cli_int8, 0, "nBidVol10"            , 1 },        
    {cli_int8, 0, "nNumTrades"           , 1 },        
    {cli_int8, 0, "iVolume"              , 1 },    
    {cli_int8, 0, "iTurnover"            , 1 },        
    {cli_int8, 0, "nTotalBidVol"         , 1 },            
    {cli_int8, 0, "nTotalAskVol"         , 1 },            
    {cli_real8, 0, "nWeightedAvgBidPrice" , 1 },                    
    {cli_real8, 0, "nWeightedAvgAskPrice" , 1 },                    
    {cli_int8, 0, "nIOPV"                , 1 },    
    {cli_int8, 0, "nYieldToMaturity"     , 1 },                
    {cli_real8, 0, "nHighLimited"         , 1 },            
    {cli_real8, 0, "nLowLimited"          , 1 },        
    {cli_int8, 0, "nSyl1"                , 1 },    
    {cli_int8, 0, "nSyl2"                , 1 },    
    {cli_int8, 0, "nSD2"                 , 1 }   
};        

ParameterBinding snapshot_paraments[2]= {
    { {.i4=0}, .type= cli_int4, .name="%id" },
    { {.i1=2}, .type= cli_int1, .name="%value1" }
};

//-------------kline--------------   
cli_field_descriptor2 kline_desc[] = {
    {cli_int4         , cli_indexed, "stock_id"     ,1 },
    {cli_int8         , cli_indexed, "market_time"  ,1 },
    {cli_int8         ,           0, "update_time"  ,1 },
    {cli_real8        ,           0, "Open"         ,1 },
    {cli_real8        ,           0, "High"         ,1 },
    {cli_real8        ,           0, "Low"          ,1 },
    {cli_real8        ,           0, "Close"        ,1 },
    {cli_int8         ,           0, "volume"       ,1 },
    {cli_real8        ,           0, "turnover"     ,1 }
//,    {cli_array_of_int1,           0, "value1"       ,10 }
};  

ParameterBinding kline_paraments[2]= {
    { {.i4=0}, .type= cli_int4, .name="%stock_id" },
    { {.i1=2}, .type= cli_int8, .name="%market_time" }
};

//----------------------------------

cli_field_descriptor2*  field_desc_data[] = {snapshot_desc, kline_desc};
int field_desc_data_size[] = { sizeof(snapshot_desc)/sizeof(cli_field_descriptor2),  sizeof(kline_desc)/sizeof(cli_field_descriptor2) };

ParameterBinding*  par_desc_data[] = {snapshot_paraments,  kline_paraments};
int par_desc_data_size[] = { sizeof(snapshot_paraments)/sizeof(ParameterBinding),  sizeof(kline_paraments)/sizeof(ParameterBinding) };

const char * table_list[] = {"snapshot", "kline"};
const int record_size[] = { sizeof(snapshot), sizeof(kline)};

