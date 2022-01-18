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

typedef struct varbinary { 
    void* data;
    int   size;
} varbinary;

typedef struct  person { 
    char        name[64];
    cli_int8_t  salary;

    char*       address;
    cli_real8_t weight;
    cli_int4_t  n_subordinates;
    cli_oid_t*  subordinates;
    varbinary   blob;
} person;

/*
(
name    string, 
salary  int8, 
address string, 
weight  real8, 
subordinates array of reference to persons,
blob array of int1);
*/

typedef struct Record
{
    cli_int4_t id;           // id 作为主键唯一标识
    cli_int4_t value;    
    cli_int4_t value1;
    cli_int4_t value2;        // value 作为保存值
    cli_int4_t value3;   
    cli_int4_t value4; 
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
} Record;

static cli_field_descriptor record_descriptor[] = {
    {cli_int4, cli_indexed, "id"},
    {cli_int4, 0, "value"   },
    {cli_int4, 0, "value1"  },
    {cli_int4, 0, "value2"  },
    {cli_int4, 0, "value3"  },
    {cli_int4, 0, "value4"  },
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
    {cli_int4, 0, "value20" }
};        

bool cli_column_bind(int statement, Record* p)
{
    const int lens = 22;
    int rc[lens]= {0,};
    int i = 0 ;

    rc[i++] = cli_column(statement, "id", cli_int4, NULL, &(p->id));
    rc[i++] = cli_column(statement, "value",  cli_int4, NULL, &(p->value));
    rc[i++] = cli_column(statement, "value1",  cli_int4, NULL, &(p->value1));
    rc[i++] = cli_column(statement, "value2",  cli_int4, NULL, &(p->value));
    rc[i++] = cli_column(statement, "value3",  cli_int4, NULL, &(p->value1));
    rc[i++] = cli_column(statement, "value4",  cli_int4, NULL, &(p->value1));
    rc[i++] = cli_column(statement, "value5",  cli_int4, NULL, &(p->value));
    rc[i++] = cli_column(statement, "value6",  cli_int4, NULL, &(p->value1));
    rc[i++] = cli_column(statement, "value7",  cli_int4, NULL, &(p->value1));
    rc[i++] = cli_column(statement, "value8",  cli_int4, NULL, &(p->value));
    rc[i++] = cli_column(statement, "value9",  cli_int4, NULL, &(p->value1));
    rc[i++] = cli_column(statement, "value10",  cli_int4, NULL, &(p->value1));
    rc[i++] = cli_column(statement, "value11",  cli_int4, NULL, &(p->value));
    rc[i++] = cli_column(statement, "value12",  cli_int4, NULL, &(p->value1));
    rc[i++] = cli_column(statement, "value13",  cli_int4, NULL, &(p->value1));
    rc[i++] = cli_column(statement, "value14",  cli_int4, NULL, &(p->value));
    rc[i++] = cli_column(statement, "value15",  cli_int4, NULL, &(p->value1));
    rc[i++] = cli_column(statement, "value16",  cli_int4, NULL, &(p->value1));
    rc[i++] = cli_column(statement, "value17",  cli_int4, NULL, &(p->value));
    rc[i++] = cli_column(statement, "value18",  cli_int4, NULL, &(p->value1));
    rc[i++] = cli_column(statement, "value19",  cli_int4, NULL, &(p->value1));
    rc[i++] = cli_column(statement, "value20",  cli_int4, NULL, &(p->value));

    for (size_t i = 0; i < lens; i++)
    {
        if( cli_ok != rc[i] )
        {
            fprintf(stderr, "cli_column bind failed with code %d    ,   i=%d \n", rc[i],i );
            return false;
        }
    }
    return true;
}


void free_varbinary(varbinary* vb) 
{
     if (vb->data != NULL) { 
        free(vb->data);
        vb->data = NULL;
    }
}
   
void* CLI_CALLBACK_CC set_varbinary(int var_type, void* var_ptr, int len)
{
    varbinary* vb = (varbinary*)var_ptr;
    free_varbinary(vb);
    vb->size = len;
    return vb->data = malloc(len);
}

void* CLI_CALLBACK_CC get_varbinary(int var_type, void* var_ptr, int* len)
{
    varbinary* vb = (varbinary*)var_ptr;
    *len = vb->size;
    return vb->data;
}

void* CLI_CALLBACK_CC set_subordinates(int var_type, void* var_ptr, int len)
{
    person* p = (person*)var_ptr;
    if (p->subordinates != NULL) { 
        free(p->subordinates);
    }
    p->n_subordinates = len;
    p->subordinates = (cli_oid_t*)malloc(len*sizeof(cli_oid_t));
    return p->subordinates;
}

void* CLI_CALLBACK_CC get_subordinates(int var_type, void* var_ptr, int* len)
{
    person* p = (person*)var_ptr;
    *len = p->n_subordinates;
    return p->subordinates;
}

static cli_field_descriptor person_descriptor[] = {
    {cli_asciiz, cli_hashed, "name"},
    {cli_int8, cli_indexed, "salary"},
    {cli_pasciiz, 0, "address"}, 
    {cli_real8, 0, "weight"}, 
    {cli_array_of_oid, 0, "subordinates", "persons"},
    {cli_array_of_int1, 0, "blob"}
}; 


int main()
{
    char* serverURL = "127.0.0.1:6100";
    char_t* databaseName = _T("testpar");
    char_t* filePath = nullptr;
    int session, statement, statement2, rc, len;
    int i, n, salary;
    int table_created = 0;
    char name[256];
    char address[256];
    cli_oid_t oid;
    Record p;

    session = cli_open(serverURL, 10, 1);
    if (session == cli_bad_address) { 
        printf(" create local database ...\n");
	    session = cli_create(databaseName, filePath, 0, 0, 100*1024*1024, 100*1024*1024, 512*1024, 0); 
    }
    if (session < 0) { 
        fprintf(stderr, "cli_open failed with code %d\n", session);
        return EXIT_FAILURE;
    }

#if 1
    rc = cli_create_table(session, "Record", sizeof(record_descriptor)/sizeof(cli_field_descriptor),  record_descriptor);

    if (rc == cli_ok) { 
	table_created = 1;
    } else if (rc != cli_table_already_exists && rc != cli_not_implemented) { 
	fprintf(stderr, "cli_create_table failed with code %d\n", rc);
	return EXIT_FAILURE;
    } 
#endif
    statement = cli_statement(session, "select * from Record");
    if (statement < 0) { 
        fprintf(stderr, "cli_statement failed with code %d\n", statement);
        return EXIT_FAILURE;
    }

    if(false == cli_column_bind(statement,&p))
    {
        fprintf(stderr, "cli_column 2 failed with code %d\n", rc);
        return EXIT_FAILURE;
    }

    int a , b ;
   diff_count diff;
   diff.start();

    int count = 1;
    int count_num = count;
    long long sum_select = 0;
    while (count > 0)
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
        
       // rc = cli_get_multy(statement);
        //rc = cli_get_first(statement);
       // std::cout << " rc:" <<  rc << std::endl;


        while ((rc =  cli_get_multy(statement)) == cli_ok)
        {
            if(cli_ok == rc )
            {
            //    std::cout << "p.id:" << p.id << "\t p.value:" << p.value << std::endl; 
            }
        } 
          std::cout << " rc:" <<  rc << std::endl;        
    }




    diff.add_snap();
    diff.show_diff(a,b, true);
    std::cout << "IPS:" << sum_select*1.0 * 1000  /  a  << "   totle_select_count:" << sum_select << std::endl ;


    rc = cli_free(statement);
    if (rc != cli_ok) { 
        fprintf(stderr, "cli_free failed with code %d\n", rc);
        return EXIT_FAILURE;
    }

   #if 0 

    statement = cli_statement(session, 
                              "select * from persons where "
                              "length(subordinates) < %subordinates and salary > %salary");
    if (statement < 0) { 
        fprintf(stderr, "cli_statement 2 failed with code %d\n", rc);
        return EXIT_FAILURE;
    }
    p.address = address;
    len = sizeof(address);
    if ((rc=cli_column(statement, "name", cli_asciiz, NULL, p.name)) != cli_ok
        || (rc=cli_column(statement, "salary", cli_int8, NULL, &p.salary)) != cli_ok
        || (rc=cli_column(statement, "address", cli_pasciiz, &len, &p.address)) != cli_ok
        || (rc=cli_column(statement, "weight", cli_real8, NULL, &p.weight)) != cli_ok
        || (rc=cli_array_column(statement, "subordinates", cli_array_of_oid, &p, 
                                set_subordinates, get_subordinates)) != cli_ok
        || (rc=cli_array_column(statement, "blob", cli_array_of_int1, &p.blob, set_varbinary, get_varbinary)) != cli_ok)
    {
        fprintf(stderr, "cli_column 2 failed with code %d\n", rc);
        return EXIT_FAILURE;
    }
    if ((rc = cli_parameter(statement, "%subordinates", cli_int4, &n)) != cli_ok 
        || (rc = cli_parameter(statement, "%salary", cli_int4, &salary)) != cli_ok)
    {
        fprintf(stderr, "cli_parameter failed with code %d\n", rc);
        return EXIT_FAILURE;
    }
    n = 2;
    salary = 90000;
    rc = cli_fetch(statement, cli_view_only);
    if (rc != 1) { 
        fprintf(stderr, "cli_fetch 1 returns %d instead of 1\n", rc);
        return EXIT_FAILURE;
    }
    n = 10;
    salary = 50000;
    rc = cli_fetch(statement, cli_for_update);
    if (rc != 2) { 
        fprintf(stderr, "cli_fetch 2 returns %d instead of 2\n", rc);
        return EXIT_FAILURE;
    }
    statement2 = cli_statement(session, "select * from persons where current = %oid");
    if (statement2 < 0) { 
        fprintf(stderr, "cli_statement 3 failed with code %d\n", rc);
        return EXIT_FAILURE;
    }
 
    if ((rc=cli_column(statement2, "name", cli_asciiz, NULL, name)) != cli_ok) { 
        fprintf(stderr, "cli_column 3 failed with code %d\n", rc);
        return EXIT_FAILURE;
    }
    if ((rc = cli_parameter(statement2, "%oid", cli_oid, &oid)) != cli_ok) { 
        fprintf(stderr, "cli_parameter 3 failed with code %d\n", rc);
        return EXIT_FAILURE;
    }	

    while ((rc = cli_get_next(statement)) == cli_ok) { 
        printf("%s\t%ld\t%f\t%s\t%d*%d\n", p.name, (long)p.salary, p.weight, p.address, *(char*)p.blob.data, p.blob.size);
        if (p.n_subordinates > 0) { 
            printf("Manages:\n");
            for (i = 0; i < p.n_subordinates; i++) { 
                oid = p.subordinates[i];
                rc = cli_fetch(statement2, cli_view_only); 
                if (rc != 1) { 
                    fprintf(stderr, "cli_fetch by oid failed with code %d\n", rc);
                    return EXIT_FAILURE;
                }	
                if ((rc = cli_get_first(statement2)) != cli_ok) { 
                    fprintf(stderr, "cli_get_first failed with code %d\n", rc);
                    return EXIT_FAILURE;
                }
                printf("\t%s\n", name);
            }
        }
        len = sizeof(address);
        p.salary = p.salary*90/100;
        rc = cli_update(statement);
        if (rc != cli_ok) { 
            fprintf(stderr, "cli_update failed with code %d\n", rc);
            return EXIT_FAILURE;
        }
    }
    if (rc != cli_not_found) { 
        fprintf(stderr, "cli_get_next failed with code %d\n", rc);
        return EXIT_FAILURE;
    }
    free_varbinary(&p.blob);
    if ((rc = cli_free(statement)) != cli_ok ||
        (rc = cli_free(statement2)) != cli_ok)
    {
        fprintf(stderr, "cli_free 2 failed with code %d\n", rc);
        return EXIT_FAILURE;	
    }


    if ((rc = cli_commit(session)) != cli_ok) { 
        fprintf(stderr, "cli_commit failed with code %d\n", rc);
        return EXIT_FAILURE;	
    }

    statement = cli_statement(session, "select * from persons order by salary");
    if (statement < 0) { 
        fprintf(stderr, "cli_statement 4 failed with code %d\n", rc);
        return EXIT_FAILURE;
    }	
    if ((rc=cli_column(statement, "salary", cli_int4, NULL, &salary)) != cli_ok) { 
        fprintf(stderr, "cli_column 4 failed with code %d\n", rc);
        return EXIT_FAILURE;
    }
    rc = cli_fetch(statement, cli_for_update);
    if (rc != 2) { 
        fprintf(stderr, "cli_fetch 4 failed with code %d\n", rc);
        return EXIT_FAILURE;
    }	
    printf("New salaries:\n");
    while ((rc = cli_get_prev(statement)) == cli_ok) { 
        printf("\t%d\n", salary);
    }

    if (rc != cli_not_found) { 
        fprintf(stderr, "cli_get_prev failed with code %d\n", rc);
        return EXIT_FAILURE;
    }
    if ((rc = cli_remove(statement)) != cli_ok) {
        fprintf(stderr, "cli_remove failed with code %d\n", rc);
        return EXIT_FAILURE;
    }
    if ((rc = cli_free(statement)) != cli_ok) { 
        fprintf(stderr, "cli_free 3 failed with code %d\n", rc);
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
	

