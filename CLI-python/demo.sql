open 'testpar';
create table snapshot (                
    sym                      int8,
    szWindCode               int8,
    nActionDay               int8,
    nTime                    int8,
    nStatus                  int8,
    nPreClose                int8,
    nOpen                    int8,
    nHigh                    int8,
    nLow                     int8,
    nMatch                   int8,
    nAskPrice1               int8,
    nAskPrice2               int8,
    nAskPrice3               int8,
    nAskPrice4               int8,
    nAskPrice5               int8,
    nAskPrice6               int8,
    nAskPrice7               int8,
    nAskPrice8               int8,
    nAskPrice9               int8,
    nAskPrice10              int8,
    nAskVol1                 int8,
    nAskVol2                 int8,
    nAskVol3                 int8,
    nAskVol4                 int8,
    nAskVol5                 int8,
    nAskVol6                 int8,
    nAskVol7                 int8,
    nAskVol8                 int8,
    nAskVol9                 int8,
    nAskVol10                int8,
    nBidPrice1               int8,
    nBidPrice2               int8,
    nBidPrice3               int8,
    nBidPrice4               int8,
    nBidPrice5               int8,
    nBidPrice6               int8,
    nBidPrice7               int8,
    nBidPrice8               int8,
    nBidPrice9               int8,
    nBidPrice10              int8,
    nBidVol1                 int8,
    nBidVol2                 int8,
    nBidVol3                 int8,
    nBidVol4                 int8,
    nBidVol5                 int8,
    nBidVol6                 int8,
    nBidVol7                 int8,
    nBidVol8                 int8,
    nBidVol9                 int8,
    nBidVol10                int8,
    nNumTrades               int8,
    iVolume                  int8,
    iTurnover                int8,
    nTotalBidVol             int8,
    nTotalAskVol             int8,
    nWeightedAvgBidPrice     int8,
    nWeightedAvgAskPrice     int8,
    nIOPV                    int8,
    nYieldToMaturity         int8,
    nHighLimited             int8,
    nLowLimited              int8,
    nSyl1                    int8,
    nSyl2                    int8,
    nSD2                     int8
 );
create index on snapshot.sym;
create index on snapshot.nTime;
select  from snapshot where sym =-1;


create table kline (
     stock_id       int8,
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


