/*-< CLITEST.C >-----------------------------------------------------*--------*
 * FastDB                    Version 1.0         (c) 1999  GARRET    *     ?  *
 * (Main Memory Database Management System)                          *   /\|  *
 *                                                                   *  /  \  *
 *                          Created:     13-Jan-2000 K.A. Knizhnik   * / [] \ *
 *                          Last update: 13-Jan-2000 K.A. Knizhnik   * GARRET *
 *-------------------------------------------------------------------*--------*
 * Test for FastDB call level interface 
 * test cli_insert_multy .
 *-------------------------------------------------------------------*--------*/

#include <fastdb/cli.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../opt/public.h"

#pragma pack (1)
typedef struct Record
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
} Record;
#pragma pack ()

static cli_field_descriptor record_descriptor[] = {
    {cli_int4, cli_indexed, "id"},
    {cli_bool, 0, "value"   },
    {cli_int1, 0, "value1"  },
    {cli_int2, 0, "value2"  },
    {cli_int4, 0, "value3"  },
    {cli_int8, 0, "value4"  },
    {cli_int4, 0, "value5"  },
    {cli_int4, 0, "value6"  },
    {cli_int4, 0, "value7"  },
    {cli_int4, 0, "value8"  },
    {cli_int4, 0, "value9"  },
    {cli_int4, 0, "value10" },
    {cli_int4, 0, "value11" },
    {cli_int4, 0, "value12" },
    {cli_int4, 0, "value13" },
    {cli_int4, 0, "value14" },
    {cli_int4, 0, "value15" },
    {cli_int4, 0, "value16" },
    {cli_int4, 0, "value17" },
    {cli_int4, 0, "value18" },
    {cli_int4, 0, "value19" },
    {cli_int4, 0, "value20" },
    {cli_array_of_int1,0,"value21"}
};        


bool cli_column2_bind(int statement, Record* p)
{
    const int lens = 23;
    int rc[lens]= {0,};
    int i = 0 ;
    static int len_val21 = sizeof(p->value21)/sizeof(p->value21[0]);

    rc[i++] = cli_column2(statement, "id", cli_int4, NULL, &(p->id));
    rc[i++] = cli_column2(statement, "value",  cli_bool, NULL, &(p->value));
    rc[i++] = cli_column2(statement, "value1",  cli_int1, NULL, &(p->value1));
    rc[i++] = cli_column2(statement, "value2",  cli_int2, NULL, &(p->value2));
    rc[i++] = cli_column2(statement, "value3",  cli_int4, NULL, &(p->value3));
    rc[i++] = cli_column2(statement, "value4",  cli_int8, NULL, &(p->value4));
    rc[i++] = cli_column2(statement, "value5",  cli_int4, NULL, &(p->value5));
    rc[i++] = cli_column2(statement, "value6",  cli_int4, NULL, &(p->value6));
    rc[i++] = cli_column2(statement, "value7",  cli_int4, NULL, &(p->value7));
    rc[i++] = cli_column2(statement, "value8",  cli_int4, NULL, &(p->value8));
    rc[i++] = cli_column2(statement, "value9",  cli_int4, NULL, &(p->value9));
    rc[i++] = cli_column2(statement, "value10",  cli_int4, NULL, &(p->value10));
    rc[i++] = cli_column2(statement, "value11",  cli_int4, NULL, &(p->value11));
    rc[i++] = cli_column2(statement, "value12",  cli_int4, NULL, &(p->value12));
    rc[i++] = cli_column2(statement, "value13",  cli_int4, NULL, &(p->value13));
    rc[i++] = cli_column2(statement, "value14",  cli_int4, NULL, &(p->value14));
    rc[i++] = cli_column2(statement, "value15",  cli_int4, NULL, &(p->value15));
    rc[i++] = cli_column2(statement, "value16",  cli_int4, NULL, &(p->value16));
    rc[i++] = cli_column2(statement, "value17",  cli_int4, NULL, &(p->value17));
    rc[i++] = cli_column2(statement, "value18",  cli_int4, NULL, &(p->value18));
    rc[i++] = cli_column2(statement, "value19",  cli_int4, NULL, &(p->value19));
    rc[i++] = cli_column2(statement, "value20",  cli_int4, NULL, &(p->value20));
    rc[i++] = cli_column2(statement, "value21",  cli_array_of_int1, &len_val21, &(p->value21));

    for (size_t i = 0; i < lens; i++)
    {
        if( cli_ok != rc[i] )
        {
            fprintf(stderr, "cli_column2 bind failed with code %d    ,   i=%ld \n", rc[i],i );
            return false;
        }
    }
    return true;
}

int main(int arg, char **argv)
{
    const char_t* databaseName = _T("testpar");
    const char_t* filePath = nullptr;
    int session, statement, rc;
    int table_created = 0;
    cli_oid_t oid;
    Record p;
    const char* serverURL ;
    
    if(arg == 2 &&  0 == strcmp(argv[1],"cli"))
    {
        serverURL = "192.168.5.191:6100" ;
        std::cout << " CLI mode , IP: " << serverURL << "\n";
    }    
    else
    {
        serverURL = "127.0.0.1:6100";
        std::cout << " Local mode , IP: " << serverURL << "\n";
    }

    session = cli_open(serverURL, 10, 1);
    if (session == cli_bad_address) { 
        printf(" create local database ...\n");
	    session = cli_create(databaseName, filePath, 0, 0, 100*1024*1024, 100*1024*1024, 512*1024, 0); 
    }
    if (session < 0) { 
        fprintf(stderr, "cli_open failed with code %d\n", session);
        return EXIT_FAILURE;
    }

    rc = cli_create_table(session, "Record", sizeof(record_descriptor)/sizeof(cli_field_descriptor), 
			  record_descriptor);
    if (rc == cli_ok) { 
	table_created = 1;
    } else if (rc != cli_table_already_exists && rc != cli_not_implemented) { 
	fprintf(stderr, "cli_create_table failed with code %d\n", rc);
	return EXIT_FAILURE;
    } 

    statement = cli_statement(session, "insert into Record");
    if (statement < 0) { 
        fprintf(stderr, "cli_statement failed with code %d\n", statement);
        return EXIT_FAILURE;
    }

    if(false == cli_column2_bind(statement,&p))
    {
        fprintf(stderr, "cli_column2 2 failed with code %d\n", rc);
        return EXIT_FAILURE;
    }

    u_int16_t record_num = 300;
    Record record_arry[record_num];

    memset(record_arry, 0, sizeof(record_arry));
    for(int i=0 ; i < record_num; i++)
    {
        record_arry[i].id = i+10;
        record_arry[i].value = i%2 ? true: false;
        record_arry[i].value1 = i+2;
        record_arry[i].value2 = i+3;
        record_arry[i].value3 = i+4;
        record_arry[i].value4 = i+5;
        record_arry[i].value5 = i+6;
        record_arry[i].value6 = i+7;
        record_arry[i].value7 = i+8;
        record_arry[i].value8 = i+9;
        record_arry[i].value9 = i+10;
        record_arry[i].value10 = i+11;
        strcpy((char*)record_arry[i].value21,"hello ");
    }

    fprintf(stderr, " record_len:%ld,   record_num:%d,  \n", sizeof(record_arry[0]), record_num);

    int a , b ;
    diff_count diff;
    diff.start();

    int count = 1000 ;
    int count_num = count;
    while (count-- > 0)
    { 
        rc = cli_insert_multy(statement,record_arry, record_num, &oid);
        if (rc != cli_ok) { 
            fprintf(stderr, "cli_insert failed with code %d\n", rc);
            return EXIT_FAILURE;
        }     
    }
    diff.add_snap();
    diff.show_diff(a,b, true);
    cli_commit(session);

    printf(" IPS:  %8f      totle_insert_count:%d    \n", record_num* count_num*1.0 * 1000  /  a ,record_num* count_num );

    rc = cli_free(statement);
    if (rc != cli_ok) { 
        fprintf(stderr, "cli_free failed with code %d\n", rc);    
        return EXIT_FAILURE;
    }

    if (table_created) { 
	rc = cli_drop_table(session, "Record");
	if (rc != cli_ok) { 
	    fprintf(stderr, "cli_drop_table failed with code %d\n", rc);
	    return EXIT_FAILURE;
	}
    }    

    if ((rc = cli_close(session)) != cli_ok) { 
        fprintf(stderr, "cli_close failed with code %d\n", rc);
        return EXIT_FAILURE;	
    }
    printf("*** CLI test sucessfully passed!\n");
    return EXIT_SUCCESS;
}
	
