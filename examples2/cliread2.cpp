/*-< CLITEST.C >-----------------------------------------------------*--------*
 * FastDB                    Version 1.0         (c) 1999  GARRET    *     ?  *
 * (Main Memory Database Management System)                          *   /\|  *
 *                                                                   *  /  \  *
 *                          Created:     13-Jan-2000 K.A. Knizhnik   * / [] \ *
 *                          Last update: 13-Jan-2000 K.A. Knizhnik   * GARRET *
 *-------------------------------------------------------------------*--------*
 * Test for FastDB call level interface 
 * test cli_get_multy .
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

    for (int i = 0; i < lens; i++)
    {
        if( cli_ok != rc[i] )
        {
            fprintf(stderr, "cli_column2 bind failed with code %d    ,   i=%d \n", rc[i],i );
            return false;
        }
    }
    return true;
}


static cli_field_descriptor2 record_descriptor[] = {
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



int main(int arg, char **argv)
{
    const char_t* databaseName = _T("testpar");
    const char_t* filePath = nullptr;
    int session, statement, rc;
    int table_created = 0;
    Record p;
    const char* serverURL = NULL;
    
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

    statement = cli_statement(session, "select * from Record");
    if (statement < 0) { 
        fprintf(stderr, "cli_statement failed with code %d\n", statement);
        return EXIT_FAILURE;
    }

    
    if( 0 != cli_column_autobind(statement,&p, sizeof(p), record_descriptor, sizeof(record_descriptor)/sizeof(cli_field_descriptor2)))
    {
        fprintf(stderr, "cli_column2 2 failed with code %d\n", rc);
        return EXIT_FAILURE;
    }

    int a , b ;
    diff_count diff;
    diff.start();

    int count = 1,sum_select = 0;
    long long read_count = 0;
    while (read_count < count)
    { 
        rc = cli_fetch(statement, cli_view_only);
        count--;
        if (rc < 0 ) { 
            fprintf(stderr, "cli_fetch failed with code %d\n", rc);
            return EXIT_FAILURE;
        }  
        else
        {          
            sum_select += rc;
        }   

        while ((rc =  cli_get_multy(statement)) == cli_ok)
        {            
                read_count++;
                while (rc = cli_parser_next(statement)== cli_ok)
                {
                    read_count++;              
                }            
        } 
       if(rc < 0 ) std::cout << " cli_get_multy failed, rc:" <<  rc << std::endl;        
    }

    diff.add_snap();
    diff.show_diff(a,b, true);
    printf(" IPS:  %8f      totle_select_count:%lld  ,  read_count=%lld  \n", sum_select*1.0 * 1000  /  a ,  sum_select, read_count );

    cli_commit(session);
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
	

