open 'testpar';
create table snapshot (                
    sym                      int4,
    szWindCode               int4,
    nActionDay               int4,
    nTime                    int4,
    nStatus                  int4,
    nPreClose                int4,
    nOpen                    int4,
    nHigh                    int4,
    nLow                     int4,
    nMatch                   int4,
    nAskPrice1               int4,
    nAskPrice2               int4,
    nAskPrice3               int4,
    nAskPrice4               int4,
    nAskPrice5               int4,
    nAskPrice6               int4,
    nAskPrice7               int4,
    nAskPrice8               int4,
    nAskPrice9               int4,
    nAskPrice10              int4,
    nAskVol1                 int4,
    nAskVol2                 int4,
    nAskVol3                 int4,
    nAskVol4                 int4,
    nAskVol5                 int4,
    nAskVol6                 int4,
    nAskVol7                 int4,
    nAskVol8                 int4,
    nAskVol9                 int4,
    nAskVol10                int4,
    nBidPrice1               int4,
    nBidPrice2               int4,
    nBidPrice3               int4,
    nBidPrice4               int4,
    nBidPrice5               int4,
    nBidPrice6               int4,
    nBidPrice7               int4,
    nBidPrice8               int4,
    nBidPrice9               int4,
    nBidPrice10              int4,
    nBidVol1                 int4,
    nBidVol2                 int4,
    nBidVol3                 int4,
    nBidVol4                 int4,
    nBidVol5                 int4,
    nBidVol6                 int4,
    nBidVol7                 int4,
    nBidVol8                 int4,
    nBidVol9                 int4,
    nBidVol10                int4,
    nNumTrades               int4,
    iVolume                  int8,
    iTurnover                int8,
    nTotalBidVol             int4,
    nTotalAskVol             int4,
    nWeightedAvgBidPrice     int4,
    nWeightedAvgAskPrice     int4,
    nIOPV                    int4,
    nYieldToMaturity         int4,
    nHighLimited             int4,
    nLowLimited              int4,
    nSyl1                    int4,
    nSyl2                    int4,
    nSD2                     int4
 );
create index on snapshot.sym;
create index on snapshot.nTime;
select  from snapshot where sym =-1;


create table kline (
     stock_id       int4,
     market_time    int8,
     update_time    int8,
     Open           real8,
     High           real8,
     Low            real8,
     Close          real8,
     volume         int8,
     turnover       real8
 );
create index on kline.stock_id;
create index on kline.market_time;


start server 'localhost:6100' 4
autocommit on


