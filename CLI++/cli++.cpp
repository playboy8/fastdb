#include "cli++.h"
#include <thread>

namespace cli_plusplus {
    
    cli_api::cli_api(std::string url, std::string database_name, std::string file_name, size_t initDatabaseSize, size_t extensionQuantum, size_t initIndexSize,size_t fileSizeLimit)
    {
        serverURL =url;
        dbname = database_name;
        filename = file_name;
        init_db_size = initDatabaseSize;
        ext_quantum = extensionQuantum;
        init_ind_size = initIndexSize;
        filesize_limit = fileSizeLimit;
        active_stat = -1;
        session = -1;
    }

    
    void cli_api::cli_api_init(std::string url, std::string database_name, std::string file_name, size_t initDatabaseSize, size_t extensionQuantum, size_t initIndexSize,size_t fileSizeLimit)
    {
        serverURL =url;
        dbname = database_name;
        filename = file_name;
        init_db_size = initDatabaseSize;
        ext_quantum = extensionQuantum;
        init_ind_size = initIndexSize;
        filesize_limit = fileSizeLimit;
        active_stat = -1;
        session = -1;
    }

    
    cli_api::~cli_api()
    {
        closedb();
    }

    
    int cli_api::opendb(int retry, int timeout)
    {
        session = cli_open(serverURL.c_str(), retry, timeout);
        show_db_respond("opendb(" << serverURL  << ") " << "   with sessionid= " <<  session );
        return session;
    }

    
    int cli_api::create_statement(std::string sql, cli_field_descriptor2 field_descs[], int field_num)
    {
        int  statement = cli_statement(session, sql.c_str());
        if(statement >= 0)
        {
            if(active_stat >=0 )
            {
                cli_precommit(active_stat);
                cli_free(active_stat);
                statements.pop_back();
            }

            statements.push_back(statement);     
            long unsigned record_size = cli_cal_record_size(field_descs,field_num);
            if( record.size() < record_size )
                record.resize(record_size);     
            if( cli_ok != cli_column_autobind(statement,record.data(), record_size,field_descs,field_num))
            {
                statements.pop_back();
                return cli_bad_descriptor;
            }

            active_stat = statement;
            if(0 == field_desc.size())
            {
                for(int i = 0 ; i < field_num; i++)
                {
                    field_desc.push_back(field_desc[i]);
                }
            }
        }
        return statement;
    }

    
    int cli_api::add_statement_parament(std::string sql, ParameterBinding parament_field_desc[], int parament_num)
    {
        int rc = cli_ok;
        for(int i = 0; i < parament_num; i++)
        {
            rc = cli_parameter(active_stat, parament_field_desc[i].name, parament_field_desc[i].type, &(parament_field_desc[i].u));
            if(cli_ok != rc)
            return rc;
        }
        return rc;
    }

    
    int cli_api::create_statement(std::string sql, cli_field_descriptor2 field_descs[], int field_num, ParameterBinding parament_field_descs[], int parament_num)
    {
        int rc;
        int statement = cli_statement(session, sql.c_str());
        if(statement >= 0)
        {
            if(active_stat >=0 )
            {
                cli_precommit(active_stat);
                cli_free(active_stat);
                statements.pop_back();
            }
            
            int record_size = cli_cal_record_size(field_descs,field_num);
            record.resize(record_size);   
            statements.push_back(statement);    
            if( cli_ok != (rc = cli_column_autobind(statement, record.data(), record_size, field_descs, field_num)))
            {
                statements.pop_back();
                show_db_respond("create_statement(" << sql << " ) failed ,  error code: " << rc );
                return rc;
            }

            active_stat = statement;
            field_desc.clear();
            if(0 == field_desc.size())
            {
                for(int i = 0 ; i < field_num; i++)
                {
                    field_desc.push_back(field_descs[i]);
                }
            }
            if(parament_num > 0 )
            {
                int rc = cli_ok;
                for(int i = 0; i < parament_num; i++)
                {
                    rc = cli_parameter(active_stat, parament_field_descs[i].name, parament_field_descs[i].type, &(parament_field_descs[i].u));
                    if(cli_ok != rc)
                    {
                        show_db_respond("cli_parameter(" << parament_field_descs[i].name << ", " << parament_field_descs[i].type <<  " ) failed ,  error code: " << rc );
                        return rc;
                    }
                }
                show_db_respond("cli_parameter(" << sql << " )  succes ,  statement : " << statement );
            }
            return rc;
        }
        return statement;
    }

    
    int cli_api::insert(void* ptr, int num)
    {
        return cli_insert_multy(active_stat, ptr, num, &oid); 
    }

    
    int cli_api::insert_update(void* ptr, int num)
    {
        return cli_insert_multy_with_filter(active_stat, ptr, num, &oid);
    }

    
    int cli_api::insert(void* data )
    {
        memcpy(record.data(), data, record.size());
        return cli_insert(active_stat, &oid);   
    }

    int(*func[7])(int )= {
    cli_get_first, 
    cli_get_last,
    cli_get_next,
    cli_get_multy,
    cli_parser_first,
    cli_parser_last,
    cli_parser_next };

    
    int cli_api::select(int auth, select_flag flag)
    {
        int statement = active_stat;
        if(select_flag::fetch == flag)
        {
            return cli_fetch(statement, auth);
        }
        else
        {
            int ret = (func[int(flag) -1])(statement);
            if(select_flag::multy_next == flag && ret < 0 )
            {
                show_db_respond("select(" << session << ") 批量获取下一批记录 :" << ret << " flag=" << int(flag) );
                ret = cli_get_multy(statement);
            }
            return ret;
        }
    }

    
    void* cli_api::get_record(int& record_size)
    {
        record_size = record.size();
        return record.data();
    }
    
    
    int cli_api::update()
    {
        int rc = cli_update(active_stat);
        int rc2 = cli_precommit(session);
        return (rc == cli_ok)? rc2 : rc;
    }

    
    int cli_api::remove(std::string table)
    {
        return remove_all(table);
    }

    int cli_api::remove_curr_record()
    {
        return cli_remove_current(active_stat);
    }

    
    int cli_api::precommit()
    {
        return cli_precommit(session);
    }

    
    int cli_api::commit()
    {   
        return cli_commit(session);
    }

    
    int cli_api::closedb()
    {
        if(session>=0)
        {
            int rc = cli_commit(session);
            if(cli_ok != rc)
            {
                show_db_respond("cli_commit(" << session << ") failed with errcode:" << rc );
            }
        
            for(auto t : statements)
            {
                rc = cli_free(t);
                if (rc != cli_ok) { 
                    show_db_respond("cli_free(" << t << ") failed with errcode:" << rc );
                }
            }
            statements.clear();
            field_desc.clear();
            rc = cli_close(session);
            if(cli_ok != rc)
            {
                show_db_respond("cli_close(" << session << ") failed with errcode:" << rc );
            }
            return rc;
        }
        return 0;
    }

    std::vector<cli_field_descriptor2> cli_api::get_curr_field_desc()
    {
        return field_desc;
    }

    unsigned int cli_api::get_record_size()
    {
        return record.size();
    }
    
    inline int cli_api::remove_all(std::string table)
    {
        int rc;
        std::string sql = "select * from " + table;
        int sql_statement = cli_statement(session, sql.c_str());
        if (sql_statement < 0) {
            show_db_respond("cli_statement(" << session << "," << sql << " ) failed with errcode:" << sql_statement );
            return -1;
        }

        int record_size = cli_cal_record_size(field_desc.data(), field_desc.size());
        std::vector<char> p;
        p.resize(record_size);
        if( field_desc.size() > 0 &&  0 != (rc = cli_column_autobind(sql_statement, p.data(), record_size, (cli_field_descriptor2*)(field_desc.data()), field_desc.size())))
        {            
            show_db_respond("cli_column_autobind(" << sql_statement << "," << sql << " ) failed with errcode:" << rc );
            cli_free(sql_statement);
            return -2;
        }
        rc = cli_fetch(sql_statement, cli_for_update);
        if (rc < 0) {
            
            show_db_respond("cli_fetch(" << session << "," << sql << " ) failed with errcode:" << rc );
            cli_free(sql_statement);
            return -3;
        }
        
        int count = rc;
        if (count == 0)
        {
            cli_free(sql_statement);
            return 0;
        }

        rc = cli_get_first(sql_statement);
        if (rc != cli_ok) {
            show_db_respond("cli_get_first(" << sql_statement  << " ) failed with errcode:" << rc );
            cli_free(sql_statement);
            return -4;
        }

        rc = cli_remove(sql_statement);
        if(cli_ok != rc)
        {
            show_db_respond("cli_get_first(" << sql_statement  << " ) failed with errcode:" << rc );
            return rc;
        }

        rc = cli_free(sql_statement);
        if (rc != cli_ok) 
        {
            show_db_respond("cli_get_first(" << sql_statement  << " ) failed with errcode:" << rc );
            return rc;
        }

        rc = cli_commit(session);
        show_db_respond("cli_commit(" << session  << " ) for remove failed with errcode:" << rc );
            return rc;
    }


}
