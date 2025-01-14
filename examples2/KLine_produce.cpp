/*-< CLITEST.C >-----------------------------------------------------*--------*
 * FastDB                    Version 1.0         (c) 1999  GARRET    *     ?  *
 * (Main Memory Database Management System)                          *   /\|  *
 *                                                                   *  /  \  *
 *                          Created:     13-Jan-2000 K.A. Knizhnik   * / [] \ *
 *                          Last update: 13-Jan-2000 K.A. Knizhnik   * GARRET *
 *-------------------------------------------------------------------*--------*
 * Test for FastDB call level interface 
 * test cli_insert_multy and cli_update.
 *-------------------------------------------------------------------*--------*/

#include <fastdb/cli.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../opt/public.h"
#include <unistd.h>

int session;

#pragma pack (1)
typedef struct KLine
{
    cli_int4_t  stock_id;            //  
    cli_int8_t  market_time;         //yyyymmddhhmmssmmm   
    cli_int8_t  update_time;         //yyyymmddhhmmssmmm   
    cli_real8_t open;
    cli_real8_t high;         
    cli_real8_t low;   
    cli_real8_t close; 
    cli_int8_t  volume;   
    cli_real8_t  turnover;   
 //   cli_int1_t  value1[10];
} KLine;
#pragma pack ()

#if 0
static cli_field_descriptor KLine_descriptor[] = {
    {cli_int4         , cli_indexed, "stock_id"      },
    {cli_int8         , cli_indexed, "market_time"   },
    {cli_int8         ,           0, "update_time"   },
    {cli_real8        ,           0, "Open"          },
    {cli_real8        ,           0, "High"          },
    {cli_real8        ,           0, "Low"           },
    {cli_real8        ,           0, "Close"         },
    {cli_int8         ,           0, "volume"        },
    {cli_real8        ,           0, "turnover"      },
    {cli_array_of_int1,           0, "value1"        }
};        
#endif

bool cli_column2_bind(int statement, KLine* p)
{
    const int lens = 10;
    int rc[lens]= {0,};
    int i = 0 ;
//    static int len_val21 = sizeof(p->value1)/sizeof(p->value1[0]);

    rc[i++] = cli_column2(statement, "stock_id"   , cli_int4,          NULL, &(p->stock_id   ));
    rc[i++] = cli_column2(statement, "market_time", cli_int8,          NULL, &(p->market_time));
    rc[i++] = cli_column2(statement, "update_time", cli_int8,          NULL, &(p->update_time));
    rc[i++] = cli_column2(statement, "Open"       , cli_real8   ,      NULL, &(p->open       ));
    rc[i++] = cli_column2(statement, "High"       , cli_real8   ,      NULL, &(p->high       ));
    rc[i++] = cli_column2(statement, "Low"        , cli_real8   ,      NULL, &(p->low        ));
    rc[i++] = cli_column2(statement, "Close"      , cli_real8   ,      NULL, &(p->close      ));
    rc[i++] = cli_column2(statement, "volume"     , cli_int8    ,      NULL, &(p->volume     ));
    rc[i++] = cli_column2(statement, "turnover"   , cli_real8    ,     NULL, &(p->turnover   ));
//    rc[i++] = cli_column2(statement, "value1"    , cli_array_of_int1,  &len_val21, &(p->value1    ));

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

void mock_data(KLine record_arry[] , int record_num, bool new_or_update, cli_int4_t* start_stockid, cli_int8_t*  curr_time)
{
    static cli_int4_t start_stock = 600519;
    static cli_int8_t start_time = 202206060930;
    if(new_or_update) start_time += 1;
    *curr_time = start_time;

    memset(record_arry, 0, sizeof(KLine)*record_num);
    for(int i=0 ; i < record_num; i++)
    {
       record_arry[i].stock_id= start_stock+i;

       record_arry[i].market_time = start_time;
       record_arry[i].update_time = start_time*1000+200;
       
       record_arry[i].open = 88.0;
       record_arry[i].high =90.24;
       record_arry[i].low = 86.66;
       record_arry[i].close = 87.52;
       record_arry[i].volume = 26000;
       record_arry[i].turnover = 227552000;
//       strcpy((char*)record_arry[i].value1,"reserved");
    }

    *start_stockid = start_stock ;   
}

int fetch_data(int statement,int mode=cli_view_only)
{
    int rc = cli_fetch(statement, mode);
    if(rc>0)
    {
        int rc = cli_get_last(statement);
        if(0 == rc)
            return 1;
        else
            return -1;
    }
    return rc;
}

int  update_or_insert(int stat_update, int stat_insert, KLine* record_arry,  KLine* dst, cli_oid_t* oid)
{
    int rc;
    if(fetch_data(stat_update,cli_for_update)>0) //cli_view_only
    { 
        cli_int8_t old_time = dst->update_time;
        dst->update_time += 200;
        rc = cli_update(stat_update);
        cli_precommit(session);

        if (rc != cli_ok) { 
            fprintf(stderr, "cli_update failed with code %d\n", rc);
            return EXIT_FAILURE;
        }

        fprintf(stderr, " found last record := %ld,  now update to: %ld, id:%d, market_time:%ld \n", old_time, dst->update_time, dst->stock_id,dst->market_time  );   
    }
    else
    {
        cli_precommit(session);
        rc = cli_insert_multy(stat_insert,record_arry, 1, oid);
        if (rc != cli_ok) { 
            fprintf(stderr, "cli_insert failed with code %d\n", rc);
            return EXIT_FAILURE;
        }
        else
            fprintf(stderr, " add new record, time: %ld, id:%d, market_time:%ld \n", record_arry->update_time, record_arry->stock_id,record_arry->market_time  );   
         
    }
    return 0;
}

int main(int arg, char **argv)
{
    const char_t* databaseName = _T("testpar");
    int  statement, statement2, rc;
    int table_created = 0;
    cli_oid_t oid;
    KLine p;
    cli_int4_t start_stockid;
    cli_int8_t curr_time;
    const char* serverURL;

    if(arg == 2 &&  0 == strcmp(argv[1],"cli"))
    {
        serverURL = "192.168.5.191:6300" ;
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
	    session = cli_create(databaseName, nullptr, 0, 0, 100*1024*1024, 100*1024*1024, 512*1024, 0); 
    }
    if (session < 0) { 
        fprintf(stderr, "cli_open failed with code %d\n", session);
        return EXIT_FAILURE;
    }

    statement = cli_statement(session, "insert into kline");
    if (statement < 0) { 
        fprintf(stderr, "cli_statement failed with code %d\n", statement);
        return EXIT_FAILURE;
    }

    if(false == cli_column2_bind(statement,&p))
    {
        fprintf(stderr, "cli_column2 2 failed with code %d\n", rc);
        return EXIT_FAILURE;
    }

    statement2 = cli_statement(session, "select * from kline where stock_id=%stock_id and market_time=%market_time");
    if (statement2 < 0) { 
        fprintf(stderr, "cli_statement failed with code %d\n", statement );
        return EXIT_FAILURE;
    }
    if(false == cli_column2_bind(statement2,&p))
    {
        fprintf(stderr, " cli_column2 bind statement2   failed with code %d\n", rc);
        return EXIT_FAILURE;
    }

    if ((rc = cli_parameter(statement2, "%stock_id", cli_int4, &start_stockid)) != cli_ok 
        || (rc = cli_parameter(statement2, "%market_time", cli_int8, &curr_time)) != cli_ok)
    {
        fprintf(stderr, "cli_parameter statement2 failed with code %d\n", rc);
        return EXIT_FAILURE;
    }    

    u_int16_t record_num = 2;
    KLine record_arry[record_num];

    for(int i = 0; i < 50; i++ )
    {
        mock_data(record_arry,record_num,true,&start_stockid,&curr_time);
        update_or_insert(statement2, statement, &record_arry[0], &p, &oid);
        
        sleep(1);
        mock_data(record_arry,record_num,false,&start_stockid,&curr_time);
   
        update_or_insert(statement2, statement, &record_arry[0], &p, &oid);
        cli_commit(session);
        sleep(1);

    }

    rc = cli_free(statement);
    if (rc != cli_ok) { 
        fprintf(stderr, "cli_free failed with code %d\n", rc);
        
        return EXIT_FAILURE;
    }

    rc = cli_free(statement2);
    if (rc != cli_ok) { 
        fprintf(stderr, "cli_free failed with code %d\n", rc);
        
        return EXIT_FAILURE;
    }

    if (table_created) { 
	rc = cli_drop_table(session, "kline");
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
	

