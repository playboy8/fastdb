from select import select
import sys
import os
sys.path.append(os.path.realpath('.'))
import cli_py
import numpy as np


def show_kline( t):
    print( t.stock_id, t.market_time, t.update_time, t.open, t.high, t.low, t.close, t.volume, t.turnover )

def show_snapshot(t):
    print(t.sym,t.szWindCode,t.nActionDay,t.nTime,t.nStatus,t.nPreClose,t.nOpen,t.nHigh,t.nLow,t.nMatch,t.nAskPrice1,t.nAskPrice2,t.nAskPrice3,t.nAskPrice4,t.nAskPrice5,t.nAskPrice6,t.nAskPrice7,t.nAskPrice8,t.nAskPrice9,t.nAskPrice10,t.nAskVol1,t.nAskVol2,t.nAskVol3,t.nAskVol4,t.nAskVol5,t.nAskVol6,t.nAskVol7,t.nAskVol8,t.nAskVol9,t.nAskVol10,t.nBidPrice1,t.nBidPrice2,t.nBidPrice3,t.nBidPrice4,t.nBidPrice5,t.nBidPrice6,t.nBidPrice7,t.nBidPrice8,t.nBidPrice9,t.nBidPrice10,t.nBidVol1,t.nBidVol2,t.nBidVol3,t.nBidVol4,t.nBidVol5,t.nBidVol6,t.nBidVol7,t.nBidVol8,t.nBidVol9,t.nBidVol10,t.nNumTrades,t.iVolume,t.iTurnover,t.nTotalBidVol,t.nTotalAskVol,t.nWeightedAvgBidPrice,t.nWeightedAvgAskPrice,t.nIOPV,t.nYieldToMaturity,t.nHighLimited,t.nLowLimited,t.nSyl1,t.nSyl2,t.nSD2)



cli = cli_py.cli_python()
print(cli)
print('---------------')
cli.cli_python_init('127.0.0.1:6100','testpar', '', 100*1024*1024,100*1024*1024,100*1024*1024, 0)
rc = cli.open(10,1)
print(rc)




def test_select():
    print('---------------')
    stmt = cli.create_statement( cli_py.record_type.kline_rec, cli_py.stat_for_select_all, 'select * from kline')
    print('stmt=',stmt)

    data = cli.get_record()
    print('-----逐条读取----------')
    select_rc = cli.select(0,cli_py.select_flag.fetch)
    select_rc = cli.select(0,cli_py.select_flag.first)
    for i in range(5):
        if select_rc < 0 :
            break
        show_kline(data.kline)
        select_rc = cli.select(0,cli_py.select_flag.next)
    cli.commit()

    print('-----批量读取----------')
    select_rc = cli.select(0,cli_py.select_flag.fetch)
    print(' fetch = ', select_rc )
    i = 0
    if select_rc > 0 :
        select_rc = cli.select(0,cli_py.select_flag.multy_get )
        print(' multy_get = ', select_rc )
        while select_rc == 0 :
            show_kline(data.kline) 
            i = i+1 
            select_rc = cli.select(0,cli_py.select_flag.multy_next)
            print('select_rc',select_rc)
            if select_rc < 0 :
                break
    cli.commit()
    print('totle kline num: ' , i )


#逐条插入
print('---------------逐条插入----')
stmt2 = cli.create_statement( cli_py.record_type.kline_rec, cli_py.stat_for_insert, 'insert into kline')
#print('stmt2=',stmt2)

data = cli_py.kline()
#data = {227552000.0, 600519, 202206060980400, 26000, 202206060980, 86.66, 87.52, 88.0, 90.24}
data = np.array([227552000.0, 600519, 202206060980400, 26000, 202206060980, 86.66, 87.52, 88.0, 90.24], dtype=complex,  copy=False )

#np.array(data, dtype=None, copy=True, order=’K’, subok=False, ndmin=0)

#// numpy.ndarray[cli_py.snapshot])

#data = {600519,202206060980,202206060980400,88.0,90.24,86.66,87.52,26000,227552000.0}

inser_rc =cli.insert(data)
print('inser_rc=',inser_rc)






     