open 'testpar';
create table Record (

     id       int4,
     value    bool,
     value1   int1,
     value2   int2,
     value3   int4,
     value4   int8,
     value5   int4,
     value6   int4,
     value7   int4,
     value8   int4,
     value9   int4,
     value10  int4,
     value11  int4,
     value12  int4,
     value13  int4,
     value14  int4,
     value15  int4,
     value16  int4,
     value17  int4,
     value18  int4,
     value19  int4,
     value20  int4,
     value21  array of int1
 );
create index on Record.id;
select  from Record where id =-1;


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


start server 'localhost:6100' 4
autocommit on


