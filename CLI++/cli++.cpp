#include "cli_api.h"
#include <fastdb/cli.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

namespace cli_plusplus {

    template < typename T >
    cli_api::cli_api<T>(std::string url, std::string database_name, std::string file_name="", size_t initDatabaseSize=100*1024*1024UL, size_t extensionQuantum=100*1024*1024UL, size_t initIndexSize=512*1024UL, size_t fileSizeLimit=0UL)
    {
        serverURL =url;
        dbname = database_name;
        filename = file_name;
        init_db_size = initDatabaseSize;
        ext_quantum = extensionQuantum;
        init_ind_size = initIndexSize;
        filesize_limit = fileSizeLimit;
    }

    template < typename T >
    int cli_api::opendb<T>(int retry, int timeout)
    {
        return cli_open(serverURL, retry, timeout);
    }

    template < typename T >
    int cli_api::create_statement<T>(std::string sql, cli_field_descriptor2 field_desc[], int field_num)
    {
        int  statement = cli_statement(session, sql.c_str());
        if(statement >= 0)
        {
            statements.push_back(statement);
            if( 0 != cli_column_autobind(statement,&record,field_desc,field_num))
            {
                statements.pop_back();
                return -1;
            }
        }
        return statement;
    }

    template < typename T >
    int cli_api::insert_multy(T* ptr, int num)
    {
        int rc = cli_insert_multy(statement, ptr, num, &oid);
        if (rc != cli_ok) { 
            return 0;
        } 
        return -1;
    }

    template < typename T >
    int cli_api::insert_multy_with_filter(T* ptr, int num)
    {
        int rc = cli_insert_multy_with_filter(statement, ptr, num, &oid);
        if (rc != cli_ok) { 
            return 0;
        } 
        return -1;
    }

    template < typename T >
    int cli_api::insert(T& data )
    {
        record = data;
        int rc = cli_insert(statement,&oid);
        if (rc != cli_ok) { 
            return 0;
        } 
        return -1;       
    }



typedef void* select_func(int) ;

    template < typename T >
    int cli_api::select(int auth, select_flag flag)
    {
        select_func func[2] = {
            cli_get_first, 
            cli_get_last,
            cli_get_next,
            cli_get_multy,
            cli_parser_first,
            cli_parser_last,
            cli_parser_next };

        int rc =0;
        int statement = statements.back();

        rc = cli_fetch(statement, auth);
        if(select_flag::toltol_num == flag) {
            return rc;
        }           
        else{

            int ret = select_func[int(flag)](statement);

            if(multy_next == flag)
            {
                if(ret < 0)
                ret = cli_get_multy(statement);
            }

            if(rc > 0 && cli_ok == ret )
                return rc;
            else
                return -1
        }
    }
}
