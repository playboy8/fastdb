open 'testpar';
create table Record (

     id       int4,
     value    int4,
     value1   int4,
     value2   int4,
     value3   int4,
     value4   int4,
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
     value20  int4	
 );
create index on Record.id;
select  from Record where id =-1;
start server 'localhost:6100' 3


