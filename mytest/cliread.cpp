/*-< CLITEST.C >-----------------------------------------------------*--------*
 * FastDB                    Version 1.0         (c) 1999  GARRET    *     ?  *
 * (Main Memory Database Management System)                          *   /\|  *
 *                                                                   *  /  \  *
 *                          Created:     13-Jan-2000 K.A. Knizhnik   * / [] \ *
 *                          Last update: 13-Jan-2000 K.A. Knizhnik   * GARRET *
 *-------------------------------------------------------------------*--------*
 * Test for FastDB call level interface 
 * Spawn "subsql  clitest.sql" to start CLI server. 
 *-------------------------------------------------------------------*--------*/

#include <fastdb/cli.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../opt/public.h"
#include <unistd.h>
 

#pragma pack (1)
typedef struct Record
{
    cli_int4_t id;           // id 作为主键唯一标识
    cli_bool_t value;    
    cli_int1_t value1;
    cli_int2_t value2;        // value 作为保存值
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
            fprintf(stderr, "cli_column2 bind failed with code %d    ,   i=%d \n", rc[i],i );
            return false;
        }
    }
    return true;
}

int main(int arg, char **argv)
{      
    char_t* databaseName = _T("testpar");
    char_t* filePath = nullptr;
    int session, statement, statement2, rc, len;
    int table_created = 0;
    cli_oid_t oid;
    Record p;
    char* serverURL ;

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

   if(false == cli_column2_bind(statement,&p))
   {
       fprintf(stderr, "cli_column 2 failed with code %d\n", rc);
       return EXIT_FAILURE;
   }

  
#if 1
    int count = 2;
    int count_num = count;
    long long sum_select = 0;
 
    rc = cli_fetch(statement, cli_view_only);// cli_view_only);
    if (rc < 0 ) { 
        fprintf(stderr, "cli_fetch failed with code %d\n", rc);
        return EXIT_FAILURE;
    }  
    else
    {          
        sum_select += rc;
    }   

    std::cout << "fitech data nums :" <<  rc << "\n";
    if ((rc = cli_get_last(statement)) == cli_ok)
    {
        // std::cout << "p.id:" << p.id << "\t p.value:" << p.value << "\t p.value1:" << p.value1 << std::endl; 
        // printf("\t%d   %d     %d     %d   \n", p.id,   p.value ,  p.value1 ,  p.value2 );
        std::cout << "last record:\n id    \t" <<  int(p.id); 
        std::cout << "\n value \t" <<  int(p.value);    
        std::cout << "\n value1\t" <<  int(p.value1);
        std::cout << "\n value3\t" <<  p.value3;   
        std::cout << "\n value4\t" <<  p.value4; 
        std::cout << "\n value5\t" <<  p.value5;   
        std::cout << "\n value6\t" <<  p.value6;   
        std::cout << "\n value7\t" <<  p.value7; 
        std::cout << "\n value8\t" <<  p.value8;  
        std::cout << "\n value9\t" <<  p.value9;   
        std::cout << "\n value10\t" <<  p.value10; 
        std::cout << "\n value11\t" <<  p.value11;  
        std::cout << "\n value12\t" <<  p.value12;   
        std::cout << "\n value13\t" <<  p.value13; 
        std::cout << "\n value14\t" <<  p.value14;  
        std::cout << "\n value15\t" <<  p.value15;   
        std::cout << "\n value16\t" <<  p.value16; 
        std::cout << "\n value17\t" <<  p.value17;   
        std::cout << "\n value18\t" <<  p.value18;   
        std::cout << "\n value19\t" <<  p.value19; 
        std::cout << "\n value20\t" <<  p.value20; 
        std::cout << "\n value21\t" <<  (char*)(p.value21);
        std::cout << "\n";
    } 
    rc =cli_precommit(session);  
    //sleep(6);     

#endif 

#if 1
    rc = cli_free(statement);
    if (rc != cli_ok) { 
        fprintf(stderr, "cli_free failed with code %d\n", rc);
        return EXIT_FAILURE;
    }
#endif
 
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
	

