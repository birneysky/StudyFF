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
recorder = namedtuple('recorder',('date','open','high','close','low','volume','price_change','p_change','v_ma5','v_ma20'))
print(stocks.values)
print(stocks.dtypes.index)
'''
fig = matplotlib.pyplot.gcf()
df=ts.get_hist_data('600415',start='2015-04-01',end='2015-06-18')
with pd.plotting.plot_params.use('x_compat', 'true'):
    df.high.plot(color='r',figsize=(10,4),grid='on')
    df.low.plot(color='b',figsize=(10,4),grid='on')
    fig.savefig('./graph.png')
'''
