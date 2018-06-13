import tushare as ts
import pandas as pd
import matplotlib
import matplotlib.pyplot
from collections import namedtuple
#获取某只股票历史交易数据
'''
date 日期
open 开盘价
high 最高价
close 最低价
low 最低价
volume 成交量
price_change 价格变动
p_change 涨跌幅
v_ma5: 5日均量
v_ma20: 20日均量
'''
'''
history_data = ts.get_hist_data('601360')
print(history_data)
print(type(history_data))
print(history_data.dtypes)
print(history_data.dtypes.index)
data_array = history_data.as_matrix()
print(data_array)
print(history_data.values)
for item in history_data:
	print(type(item))
	print(item)
'''

#获取a股所有股票的基本信息
stocks = ts.get_stock_basics()
print(stocks)
stock_infotuple = namedtuple('stock_info',('name','industry','area','pe','outstanding','totals','totalAssets','liquidAssets','fixedAssets','reserved','reservedPerShare','esp','bvps','pb','timeToMarket','undp','perundp','rev','profit','gpr','npr','holders'))
#stock_info_list = [stock_infotuple(name,industry,area,pe,outstanding,totals,totalAssets,liquidAssets,fixedAssets,reserved,reserved,PerShare,esp,bvps,pb,timeToMarket,undp,perundp,rev,profit,gpr,npr,holders) for name,industry,area,pe,outstanding,totals,totalAssets,liquidAssets,fixedAssets,reserved,reservedPerShare,esp,bvps,pb,timeToMarket,undp,perundp,rev,profit,gpr,npr,holders in stocks.values]
print(stocks.values)
print(type(stocks.dtypes.index.values))
print(stocks.dtypes.index.values)
print(stocks.dtypes)
print('stocks.value type:',type(stocks.values))
print('stocks type:',type(stocks))
print('stocks.columns type:',type(stocks.columns),stocks.columns)
print('stocks.index type:',type(stocks.index),stocks.index)
for info in stocks.values:
	stock_infotuple = namedtuple('stock_info',('index_name','value'))
	#print(type(info),type(stocks.dtypes.index.values),type([1,2,3,4]))
	#print(info.tolist(),stocks.dtypes.index.values.tolist())
	stock_list = [stock_infotuple(index_name,value) for index_name,value in zip(stocks.dtypes.index.values.tolist(),info.tolist())]
	print(stock_list)
	print('😂😂😂😂😂😂😂😂😂😂😂😂😂😂😂')
names = ['joy','jack','toney','blink']
ages = [23,22,34,68]
for name,age in zip(names,ages):
	print('name:',name,'age:',age)
