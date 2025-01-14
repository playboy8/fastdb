/*-< CLITEST.C >-----------------------------------------------------*--------*
 * FastDB                    Version 1.0         (c) 1999  GARRET    *     ?  *
 * (Main Memory Database Management System)                          *   /\|  *
 *                                                                   *  /  \  *
 *                          Created:     13-Jan-2000 K.A. Knizhnik   * / [] \ *
 *                          Last update: 13-Jan-2000 K.A. Knizhnik   * GARRET *
 *-------------------------------------------------------------------*--------*
 * Test for FastDB call level interface 
 * test cli_insert_multy_with_filter .
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
#if 0
bool cli_column_autobind(int statement, Record* p, cli_field_descriptor arr[])
{
    static std::unordered_map<std::string,int> len_map;
    int arr_len  = sizeof(arr)/sizeof(cli_field_descriptor);
    int rc[arr_len]= {0,};
    char* p_dst = (char*)p;
    for(int i = 0 ; i < arr_len; i++)
    {
        cli_field_descriptor* curr = arr+i;
        int curr_size = 0;      

        if( curr->type >= cli_array_of_oid && curr->type <= cli_array_of_string )
        {
            curr_size = sizeof_type[curr->type-cli_array_of_oid];
            curr_size *= curr->len;
            rc[i++] = cli_column2(statement, curr->name, curr->type, &curr->len, p_dst);
        }
        else
        {
            rc[i++] = cli_column2(statement, curr->name, curr->type, NULL, p_dst);
            curr_size = sizeof_type[curr->type];
        }

        p_dst += curr_size;     
    }

    assert(p_dst == (p+1));

    for (size_t i = 0; i < lens; i++)
    {
        if( cli_ok != rc[i] )
        {
        //    fprintf(stderr, "cli_column2 bind failed with code %d    ,   i=%d \n", rc[i],i );
            return false;
        }
    }
    return true;
}
#endif

// 删除所有数据
int test_cli_fastdb_remove_all(int db_handle)
{
	int rc;
	int sql_statement = cli_statement(db_handle, "select * from Record");
	if (sql_statement < 0) {
		printf("Error: cli_statement failed with code %d\n", sql_statement);
		return -1;
	}
	Record p;
	if (false == cli_column2_bind(sql_statement, &p)) // 设置查询的字段
	{
		printf("Error: cli_column2 2 failed with code %d\n", rc);
		cli_free(sql_statement);
		cli_abort(db_handle);
		return -2;
	}
	rc = cli_fetch(sql_statement, cli_for_update);
	if (rc < 0) {
		printf("Error: cli_fetch failed with code %d\n", rc);
		cli_free(sql_statement);
		cli_abort(db_handle);
		return -3;
	}
	
	int count = rc;
	if (count == 0)
	{
		printf("can not remove, no record exist\n");
		cli_free(sql_statement);
		cli_abort(db_handle);
		return 0;
	}

	// 备注: cli_get_prev这一步不能少,否则cli_remove报错-13:cli_not_found
	//while ((rc = cli_get_first(sql_statement)) == cli_ok) {
	//	printf("id=%d\n", p.id);
	//}
    rc = cli_get_first(sql_statement);

	if (rc != cli_not_found) {
		printf("Error: cli_get_prev failed with code %d\n", rc);
		cli_free(sql_statement);
		cli_abort(db_handle);
		return -4;
	}

	rc = cli_remove(sql_statement);
	{
		printf("cli_remove rc=[%d]\n", rc);
		//return -4;
	}

	rc = cli_free(sql_statement);
	if (rc != cli_ok) {
		printf("Error: cli_free failed with code %d\n", rc);

		//return -2;
	}

	rc = cli_commit(db_handle);
	{
		printf("cli_commit rc=[%d]\n", rc);
		//return -5;
	}

	return count;
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

//    rc = cli_create_table(session, "Record", sizeof(record_descriptor)/sizeof(cli_field_descriptor), 
//			  record_descriptor);
//    rc == cli_table_already_exists;
//    if (rc == cli_ok) { 
//	table_created = 1;
//    } else if (rc != cli_table_already_exists && rc != cli_not_implemented) { 
//	fprintf(stderr, "cli_create_table failed with code %d\n", rc);
//	return EXIT_FAILURE;
//    } 

    statement = cli_statement(session, "insert into Record");
    if (statement < 0) { 
        fprintf(stderr, "cli_statement failed with code %d\n", statement);
        return EXIT_FAILURE;
    }

    
    if( 0 != cli_column_autobind(statement,&p, sizeof(p), record_descriptor, sizeof(record_descriptor)/sizeof(cli_field_descriptor2)))
    {
        fprintf(stderr, "cli_column2 2 failed with code %d\n", rc);
        return EXIT_FAILURE;
    }

    int paramen[] = {20,50,100,200,500,1000 };
    int totle_send = 1000;
    for(long unsigned int j=0; j < sizeof(paramen)/sizeof(paramen[0]); j++)
    {
        u_int16_t record_num = paramen[j];
        Record record_arry[record_num];

        memset(record_arry, 0, sizeof(record_arry));
        for(int i=0 ; i < record_num; i++)
        {
            record_arry[i].id = (i%record_num)/2 + 1000000*j;
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

        int a , b ;
        diff_count diff;
        diff.start();

        int count = totle_send/record_num ;
        int count_num = count;
        while (count > 0)
        { 
            {
                for(int k =0; k < record_num; k++)
                {
                    record_arry[k].id +=  record_num*(count_num-count);
                   // fprintf(stderr,  "%lld  ", record_arry[k].id);
                }
            } 

            rc = cli_insert_multy_with_filter(statement, record_arry, record_num, &oid);
            if (rc != cli_ok) { 
                fprintf(stderr, "cli_insert failed with code %d\n", rc);
                return EXIT_FAILURE;
            } 
            count--; 
        }
        diff.add_snap();
        diff.show_diff(a,b, true);

        cli_commit(session);
        printf(" IPS:  %8f      totle_insert_count:%d     record_num:%d  \n", record_num* count_num*1.0 * 1000  /  a ,record_num* count_num , record_num );
      //  test_cli_fastdb_remove_all(statement);
    }  

    fprintf(stderr, "\n\n------test finished------\n");  
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
	

