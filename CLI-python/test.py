from select import select
import sys
import os
sys.path.append(os.path.realpath('.'))
import cli_py
import numpy as np
import time


# 全局数据--------------------------------------------
cli = cli_py.cli_python()

field =[(cli_py.cli_int8, 2, 'stock_id',    1, '', ''),
        (cli_py.cli_int8, 2, 'market_time', 1, '', ''), 
        (cli_py.cli_int8, 0, 'update_time', 1, '', ''),
        (cli_py.cli_real8, 0, 'Open',       1, '', ''),
        (cli_py.cli_real8, 0, 'High',       1, '', ''),
        (cli_py.cli_real8, 0, 'Low',        1, '', ''),
        (cli_py.cli_real8, 0, 'Close',      1, '', ''),
        (cli_py.cli_int8, 0, 'volume',      1, '', ''),
        (cli_py.cli_real8, 0, 'turnover',   1, '', '')]



# K线数据结构
class kline(object):
    #def __init__(self,stock_id, market_time, update_time, open, high, low, close, volume, turnover):
    def __init__(self,data):
        self.stock_id=    data[0] #  stock_id         #  
        self.market_time= data[1] #  market_time      #          
        self.update_time= data[2] #  update_time      #          
        self.Open=        data[3] #  open             #          
        self.High=        data[4] #  high             #      
        self.Low=         data[5] #  low              #  
        self.Close=       data[6] #  close            #  
        self.volume=      data[7] #  volume           #  
        self.turnover=    data[8] #  turnover         #  
    def show(self):
        print( self.stock_id, self.market_time, self.update_time, self.Open, self.High, self.Low, self.Close, self.volume, self.turnover )

 
# 快照数据结构
class snapshot(object):
    #def __init__(self,stock_id, market_time, update_time, open, high, low, close, volume, turnover):
    def __init__(self,data):
        self.sym=                   data[0] 
        self.szWindCode=            data[1]  
        self.nActionDay=            data[2]  
        self.nTime=                data[3] 
        self.nStatus=              data[4] 
        self.nPreClose=             data[5]  
        self.nOpen=                data[6] 
        self.nHigh=                data[7] 
        self.nLow=                 data[8]  
        self.nMatch=               data[9] 
        self.nAskPrice1=            data[10]  
        self.nAskPrice2=            data[11]  
        self.nAskPrice3=            data[12]  
        self.nAskPrice4=            data[13]  
        self.nAskPrice5=            data[14]  
        self.nAskPrice6=            data[15]  
        self.nAskPrice7=            data[16]  
        self.nAskPrice8=            data[17]  
        self.nAskPrice9=            data[18]  
        self.nAskPrice10=           data[19]  
        self.nAskVol1=             data[20] 
        self.nAskVol2=             data[21] 
        self.nAskVol3=             data[22] 
        self.nAskVol4=             data[23] 
        self.nAskVol5=             data[24] 
        self.nAskVol6=             data[25] 
        self.nAskVol7=             data[26] 
        self.nAskVol8=             data[27] 
        self.nAskVol9=             data[28] 
        self.nAskVol10=             data[29]  
        self.nBidPrice1=            data[30]  
        self.nBidPrice2=            data[31]  
        self.nBidPrice3=            data[32]  
        self.nBidPrice4=            data[33]  
        self.nBidPrice5=            data[34]  
        self.nBidPrice6=            data[35]  
        self.nBidPrice7=            data[36]  
        self.nBidPrice8=            data[37]  
        self.nBidPrice9=            data[38]  
        self.nBidPrice10=           data[39]  
        self.nBidVol1=             data[40] 
        self.nBidVol2=             data[41] 
        self.nBidVol3=             data[42] 
        self.nBidVol4=             data[43] 
        self.nBidVol5=             data[44] 
        self.nBidVol6=             data[45] 
        self.nBidVol7=             data[46] 
        self.nBidVol8=             data[47] 
        self.nBidVol9=             data[48] 
        self.nBidVol10=             data[49]  
        self.nNumTrades=            data[50]  
        self.iVolume=               data[51] 
        self.iTurnover=             data[52]  
        self.nTotalBidVol=          data[53]  
        self.nTotalAskVol=          data[54]  
        self.nWeightedAvgBidPrice=  data[55]  
        self.nWeightedAvgAskPrice=  data[56]  
        self.nIOPV=                 data[57] 
        self.nYieldToMaturity=      data[58]  
        self.nHighLimited=          data[59]  
        self.nLowLimited=           data[60]  
        self.nSyl1=                 data[61] 
        self.nSyl2=                 data[62] 
        self.nSD2=                  data[63]  

    def show(t):
        print(t.sym,t.szWindCode,t.nActionDay,t.nTime,t.nStatus,t.nPreClose,t.nOpen,t.nHigh,t.nLow,t.nMatch,t.nAskPrice1,t.nAskPrice2,t.nAskPrice3,t.nAskPrice4,t.nAskPrice5,t.nAskPrice6,t.nAskPrice7,t.nAskPrice8,t.nAskPrice9,t.nAskPrice10,t.nAskVol1,t.nAskVol2,t.nAskVol3,t.nAskVol4,t.nAskVol5,t.nAskVol6,t.nAskVol7,t.nAskVol8,t.nAskVol9,t.nAskVol10,t.nBidPrice1,t.nBidPrice2,t.nBidPrice3,t.nBidPrice4,t.nBidPrice5,t.nBidPrice6,t.nBidPrice7,t.nBidPrice8,t.nBidPrice9,t.nBidPrice10,t.nBidVol1,t.nBidVol2,t.nBidVol3,t.nBidVol4,t.nBidVol5,t.nBidVol6,t.nBidVol7,t.nBidVol8,t.nBidVol9,t.nBidVol10,t.nNumTrades,t.iVolume,t.iTurnover,t.nTotalBidVol,t.nTotalAskVol,t.nWeightedAvgBidPrice,t.nWeightedAvgAskPrice,t.nIOPV,t.nYieldToMaturity,t.nHighLimited,t.nLowLimited,t.nSyl1,t.nSyl2,t.nSD2 )

# k线查询参数
parament = [('600520', cli_py.cli_int4, '%stock_id'),('202206060980100', cli_py.cli_int8, '%market_time')]


# 函数--------------------------------------------

def show_kline( t):
    print( t.stock_id, t.market_time, t.update_time, t.open, t.high, t.low, t.close, t.volume, t.turnover )

def show_snapshot(t):
    print(t.sym,t.szWindCode,t.nActionDay,t.nTime,t.nStatus,t.nPreClose,t.nOpen,t.nHigh,t.nLow,t.nMatch,t.nAskPrice1,t.nAskPrice2,t.nAskPrice3,t.nAskPrice4,t.nAskPrice5,t.nAskPrice6,t.nAskPrice7,t.nAskPrice8,t.nAskPrice9,t.nAskPrice10,t.nAskVol1,t.nAskVol2,t.nAskVol3,t.nAskVol4,t.nAskVol5,t.nAskVol6,t.nAskVol7,t.nAskVol8,t.nAskVol9,t.nAskVol10,t.nBidPrice1,t.nBidPrice2,t.nBidPrice3,t.nBidPrice4,t.nBidPrice5,t.nBidPrice6,t.nBidPrice7,t.nBidPrice8,t.nBidPrice9,t.nBidPrice10,t.nBidVol1,t.nBidVol2,t.nBidVol3,t.nBidVol4,t.nBidVol5,t.nBidVol6,t.nBidVol7,t.nBidVol8,t.nBidVol9,t.nBidVol10,t.nNumTrades,t.iVolume,t.iTurnover,t.nTotalBidVol,t.nTotalAskVol,t.nWeightedAvgBidPrice,t.nWeightedAvgAskPrice,t.nIOPV,t.nYieldToMaturity,t.nHighLimited,t.nLowLimited,t.nSyl1,t.nSyl2,t.nSD2)


def open():
    print('---------------')
    cli.cli_python_init('127.0.0.1:6100','testpar', '', 100*1024*1024,100*1024*1024,100*1024*1024, 0)
    rc = cli.open(10,1)
    print('open result:',rc)


def create_selsect_statement(table):
    print('---------------')
    t =np.array([])
    stmt = cli.create_statement('select * from '+table,  field, t )
    print('stmt=',stmt)

def create_cond_selsect_statement(table):
    print('---------------')
    stmt = cli.create_statement('select * from '+ table +' where stock_id=%stock_id and market_time=%market_time', field, parament )
    print('stmt:',stmt)

def test_del_curr_record():
    print('-----逐条读取----------')
    select_rc = cli.select(0,cli_py.select_flag.fetch)
    print('ftech select_rc : ',select_rc)
    if select_rc > 0 :
        select_rc = cli.select(0,cli_py.select_flag.first)
        print('get first ret: ',select_rc)
    else :
        return
    print(cli.get_record())    

def test_kline_select():

    print('-----逐条读取----------')
    select_rc = cli.select(0,cli_py.select_flag.fetch)
    print('ftech select_rc : ',select_rc)
    if select_rc > 0 :
        select_rc = cli.select(0,cli_py.select_flag.first)
        print('get first ret: ',select_rc)
    else :
        return
    data = kline(cli.get_record())
    data.show()
    #print(cli.get_record())
    cli.commit()
    return 
    
def perftest_kline_multy_select(limit): 
    create_selsect_statement('kline')
    print('-----批量读取----------')
    select_rc = cli.select(0,cli_py.select_flag.fetch)
    print(' fetch = ', select_rc )
    i = 0
    t0 = time.perf_counter()
    if select_rc > 0 : 
        while i <  limit :
            select_rc = cli.select(0,cli_py.select_flag.fetch)
            select_rc = cli.select(0,cli_py.select_flag.multy_get )
            print(' multy_get = ', select_rc )
            while select_rc == 0 :
                #print(cli.get_record())
                i = i+1 
                select_rc = cli.select(0,cli_py.select_flag.multy_next)
                #print('select_rc',select_rc)
                if select_rc < 0 :
                    break
            cli.precommit()  
    t1 =time.perf_counter()
    elapsed = t1 -t0
    cli.commit()
    print('select totle kline num: ' , i , ', elapsed = ',elapsed , '  IPS: ' , limit/elapsed   )


def tset_kline_cond_select():
    create_cond_selsect_statement('kline')
    test_kline_select()

def test_kline_select_all():
    create_selsect_statement('kline')
    test_kline_select()   


def test_insert():
#逐条插入
    print('---------------逐条插入----')
    stmt2 = cli.create_statement( cli_py.record_type.kline_rec, cli_py.stat_for_insert, 'insert into kline')
    print('insert stmt=',stmt2)
    if stmt2 == cli_py.cli_result_code.cli_ok :
        data1 = [(600519,202206060980100,202206060980400,88.0,90.24,86.66,87.52,26000,227552000.0)]
        inser_rc =cli.insert(False, data1)
        cli.commit()
        print('单条插入结束， inser_rc=', inser_rc)
    cli.commit()


def test_kline_multy_insert():
#逐条插入
    print('---------------批量插入----')
    stmt2 = cli.create_statement('insert into kline', field, np.array([]) )
    print('insert stmt=',stmt2)
    if stmt2 == cli_py.cli_result_code.cli_ok :   
        data2 = [600520,202206060980100,202206060980400,88.0,90.24,86.66,87.52,26000,227552000.0,600521,202206060980100,202206060980400,88.0,90.24,86.66,87.52,26000,227552000.0]
        inser_rc =cli.insert(data2, 2)
        print('批量插入结束， inser_rc=', inser_rc)
        cli.commit()


#test_insert()
#test_multy_insert()
open()
test_kline_select_all()
perftest_kline_multy_select(500)
#test_kline_multy_insert()

     