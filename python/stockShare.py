import tushare as ts
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

history_data = ts.get_hist_data('601360')
print(history_data)
print(type(history_data))
#获取a股所有股票的基本信息
print(ts.get_stock_basics())
