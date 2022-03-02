#pragma once

#include<vector>
#include<unordered_map>
#include<string>
#include<thread>
#include<memory>


class data_source_info
{
private:
    std::string ip;
    int port;
    std::string user;
    std::string passwd;
public:
    data_source_info(/* args */);
    ~data_source_info();
};

class cfg_info;



class symbol_info
{
public:
    std::string symbol;
    std::string newest_contract;
    std::unordered_map<std::string, std::string> data;
    unsigned long long last_time;
    unsigned long long curr_time;

    symbol_info():last_time(0),curr_time(0) {}
    ~symbol_info(){};

    void update_data( std::string str);
    bool check();
    std::string get_latest_time();
};


class future_monitor
{
public:
    std::unordered_map<std::string, symbol_info> market_data;   
    std::vector< std::shared_ptr<std::thread>> sub_thread;  
public:
    future_monitor(/* args */);
    ~future_monitor();
    bool start_monitor(cfg_info &info);
    bool stop_monitor();
    inline bool sub_data(cfg_info &info);
    inline bool check_data();
};


 