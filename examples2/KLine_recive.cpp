/*-< CLITEST.C >-----------------------------------------------------*--------*
 * FastDB                    Version 1.0         (c) 1999  GARRET    *     ?  *
 * (Main Memory Database Management System)                          *   /\|  *
 *                                                                   *  /  \  *
 *                          Created:     13-Jan-2000 K.A. Knizhnik   * / [] \ *
 *                          Last update: 13-Jan-2000 K.A. Knizhnik   * GARRET *
 *-------------------------------------------------------------------*--------*
 * Test for FastDB call level interface 
 * test cli_get . 
 *-------------------------------------------------------------------*--------*/

#include <fastdb/cli.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../opt/public.h"
#include <unistd.h>

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
    cli_int1_t  value1[10];
} KLine;
#pragma pack ()

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

bool cli_column2_bind(int statement, KLine* p)
{
    const int lens = 10;
    int rc[lens]= {0,};
    int i = 0 ;
    static int len_val21 = sizeof(p->value1)/sizeof(p->value1[0]);

    rc[i++] = cli_column2(statement, "stock_id"   , cli_int4,          NULL, &(p->stock_id   ));
    rc[i++] = cli_column2(statement, "market_time", cli_int8,          NULL, &(p->market_time));
    rc[i++] = cli_column2(statement, "update_time", cli_int8,          NULL, &(p->update_time));
    rc[i++] = cli_column2(statement, "Open"       , cli_real8   ,      NULL, &(p->open       ));
    rc[i++] = cli_column2(statement, "High"       , cli_real8   ,      NULL, &(p->high       ));
    rc[i++] = cli_column2(statement, "Low"        , cli_real8   ,      NULL, &(p->low        ));
    rc[i++] = cli_column2(statement, "Close"      , cli_real8   ,      NULL, &(p->close      ));
    rc[i++] = cli_column2(statement, "volume"     , cli_int8    ,      NULL, &(p->volume     ));
    rc[i++] = cli_column2(statement, "turnover"   , cli_real8    ,     NULL, &(p->turnover   ));
    rc[i++] = cli_column2(statement, "value1"    , cli_array_of_int1,  &len_val21, &(p->value1    ));

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

void show_data(KLine *p)
{
    fprintf(stderr, "recive record :  %8d, %12lld, %12lld, %8f, %8f, %8f , %8f , %lld, %8f \n", p->stock_id,p->market_time, p->update_time, p->open, p->high, p->low, p->close, p->volume, p->turnover);
}

cli_oid_t fetch_all_data(int statement,int session, KLine *p)
{
    int rc = cli_fetch(statement, cli_view_only);
    while(true )
    {   
        rc = cli_fetch(statement, cli_view_only);
        if(rc > 0)
        {
            while( (rc = cli_get_multy(statement)) == cli_ok)
            {           
                show_data(p);
                while ((rc = cli_parser_next(statement)) == cli_ok)                
                    show_data(p);
            }
        }
        else if( rc == -13 )
        {
            std::cout << " fetch data failed , rc=" << int(rc) << "     no data. \n" ;
            cli_precommit(session);
            sleep(1);
            continue;
        }
        else
        {
            std::cout << " fetch data failed , rc=" << int(rc) << " \n" ;
            sleep(1);
            continue;
        }

        cli_precommit(session);
        return  cli_get_oid(statement);
    }
     return  cli_get_oid(statement);
}

int fetch_new_data(int statement, int session ,KLine *p, cli_oid_t oid , cli_int4_t* id, cli_int8_t* time)
{
    int count = 500;
    int rc=0 ;
    int old_rc=0;
    {
        while( count-- >0)   
        {
            rc = cli_fetch(statement, cli_view_only);   
            if(rc > 0 && rc > old_rc)
            {
                old_rc = rc;
                if( (rc = cli_get_last(statement)) == cli_ok )
                {
                     std::cout << "Found new record : "<<   std::endl; 
                     show_data(p);
                }   
                else
                     std::cout << "cli_get_last error,  rc:" << int(rc) <<   std::endl;                      
            }
            else
            {
              if( old_rc == rc)
                 std::cout << "fetech data: server has no update, rc:" << int(rc)   <<  std::endl;
              else if( rc < 0) std::cout << "seek error \n" << std::endl;
            }
            cli_precommit(session);
            sleep(1);
        }
    }   
    return rc;
}


int main(int arg, char **argv)
{
    char_t* databaseName = _T("testpar2");
    int session, statement, statement2, rc ;

    cli_oid_t oid;
    KLine p;
    cli_int4_t start_stockid;
    cli_int8_t update_time; 
    char* serverURL ;

    if(arg == 2 &&  0 == strcmp(argv[1],"cli"))
    {
        serverURL = "192.168.5.191:6300" ;
        std::cout << " CLI mode , IP: " << serverURL << "\n";
    }    
    else
    {
        serverURL = "127.0.0.1:6300";
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

    statement = cli_statement(session, "select * from KLine");
    if (statement < 0) { 
        fprintf(stderr, "cli_statement failed with code %d\n", statement);
        return EXIT_FAILURE;
    }

    if(false == cli_column2_bind(statement,&p))
    {
        fprintf(stderr, "cli_column2 2 failed with code %d\n", rc);
        return EXIT_FAILURE;
    }

    statement2 = cli_statement(session, "select * from KLine where stock_id = %stock_id and update_time > %update_time");
    if (statement < 0) { 
        fprintf(stderr, "cli_statement failed with code %d\n", statement);
        return EXIT_FAILURE;
    }

    if(false == cli_column2_bind(statement2,&p))
    {
        fprintf(stderr, "cli_column2 2 failed with code %d\n", rc);
        return EXIT_FAILURE;
    }

    if ((rc = cli_parameter(statement2, "%stock_id", cli_int4, &start_stockid)) != cli_ok 
        || (rc = cli_parameter(statement2, "%update_time", cli_int8, &update_time)) != cli_ok)
    {
        fprintf(stderr, "cli_parameter statement2 failed with code %d\n", rc);
        return EXIT_FAILURE;
    }  

    oid = fetch_all_data(statement, session, &p);
    fetch_new_data(statement, session ,&p, oid, &start_stockid, &update_time);

    rc = cli_free(statement);
    if (rc != cli_ok) { 
        fprintf(stderr, "cli_free failed with code %d\n", rc);
        
        return EXIT_FAILURE;
    }
  
    if ((rc = cli_close(session)) != cli_ok) { 
        fprintf(stderr, "cli_close failed with code %d\n", rc);
        return EXIT_FAILURE;	
    }
    printf("*** CLI test sucessfully passed!\n");
    return EXIT_SUCCESS;
}
	

