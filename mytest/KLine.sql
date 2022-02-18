open 'testpar2';
create table KLine (

     stock_id       int4,
     market_time    int8,
     update_time    int8,
     Open           real8,
     High           real8,
     Low            real8,
     Close          real8,
     volume         int8,
     turnover       real8,
     value1  array of int1
 );
create index on KLine.stock_id;
create index on KLine.market_time;

select  from KLine where stock_id =-1;

start server 'localhost:6300' 4

autocommit on


