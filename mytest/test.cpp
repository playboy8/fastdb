#include "test.h"



bool symbol_info::check()
{
    unsigned long long time = 0;
    for(auto t = data.begin(); t != data.end(); t++)
    {
        if( t->second.c_str()  > time )
            time = ;

    }

    if(time - curr_time  >  10 s)
    {
        
    }

}




future_monitor::future_monitor(/* args */)
{
}


future_monitor::~future_monitor()
{
}


void sub_process(cfg_info &info, std::unordered_map<std::string, symbol_info> &market_data)
{

    


}



bool future_monitor::start_monitor(cfg_info &info)
{
    // create thread for sub data
    for(int i = 0; i < info.thread_num; i++)
    {
    std::shared_ptr<std::thread> ptr = std::make_shared<std::thread>(sub_process,std::ref(this->market_data));
    if(ptr)
        sub_thread.emplace_back(std::move(ptr));
    }
}


bool future_monitor::stop_monitor()
{

}


bool future_monitor::sub_data(cfg_info &info)
{
}



bool future_monitor::check_data()
{
    unsigned int count_all= 0 ,  count_failed =0; 
    for(auto t = market_data.begin(); t != market_data.end(); t++)
    {
        count_all++;
        if(t->second.check())
            count_failed++;

        for(auto k = t->second.data.begin(); k != t->second.data.end(); k++ )
        {
            
        }


    }



}



