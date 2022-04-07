#include "record_struct.h"
//#include <fastdb/stdtp.h>

//------------snapshot-------------
cli_field_descriptor2 snapshot_desc[] = {
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

