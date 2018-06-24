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

'''
columns type <class 'str'> name
columns type <class 'str'> industry
columns type <class 'str'> area
columns type <class 'str'> pe
columns type <class 'str'> outstanding
columns type <class 'str'> totals
columns type <class 'str'> totalAssets
columns type <class 'str'> liquidAssets
columns type <class 'str'> fixedAssets
columns type <class 'str'> reserved
columns type <class 'str'> reservedPerShare
columns type <class 'str'> esp
columns type <class 'str'> bvps
columns type <class 'str'> pb
columns type <class 'str'> timeToMarket
columns type <class 'str'> undp
columns type <class 'str'> perundp
columns type <class 'str'> rev
columns type <class 'str'> profit
columns type <class 'str'> gpr
columns type <class 'str'> npr
columns type <class 'str'> holders
'''

class stock:
	def setName(self, name):
		self.name = name

	def getName(self):
		return self.name

	def setIndustry(self, industry):
		self.industry = industry

	def getIndustry(self):
		return self.industry

	def setArea(self, area):
		self.area = area

	def getArea(self):
		return self.area

	def setPe(self, pe):
		self.pe = pe

	def getPe(self):
		return self.pe

	def setOutstanding(self, outstanding):
		self.outstanding = outstanding

	def getOutstanding(self):
		return self.outstanding

	def setTotals(self,totals):
		self.totals = totals

	def getTotals(self):
		return self.totals

	def setTotalAssets(self, totalAssets):
		self.totalAssets = totalAssets

	def getTotalAssets(self):
		return self.totalAssets

	def setLiquidAssets(self,liquidAssets):
		self.liquidAssets = liquidAssets

	def getLiquidAssets(self):
		return self.liquidAssets

	def setFixedAssets(self,fixedAssets):
		self.fixedAssets = fixedAssets

	def getFixedAssets(self):
		return self.fixedAssets

	def setReserved(self,reserved):
		self.reserved = reserved

	def getReserved(self):
		return self.reserved

#获取a股所有股票的基本信息
stocks = ts.get_stock_basics()
#print('stocks type',type(stocks),stocks)
stock_infotuple = namedtuple('stock_info',('name','industry','area','pe','outstanding','totals','totalAssets','liquidAssets','fixedAssets','reserved','reservedPerShare','esp','bvps','pb','timeToMarket','undp','perundp','rev','profit','gpr','npr','holders'))
#stock_info_list = [stock_infotuple(name,industry,area,pe,outstanding,totals,totalAssets,liquidAssets,fixedAssets,reserved,reserved,PerShare,esp,bvps,pb,timeToMarket,undp,perundp,rev,profit,gpr,npr,holders) for name,industry,area,pe,outstanding,totals,totalAssets,liquidAssets,fixedAssets,reserved,reservedPerShare,esp,bvps,pb,timeToMarket,undp,perundp,rev,profit,gpr,npr,holders in stocks.values]
#print(stocks.values)
#print(type(stocks.dtypes.index.values))
#print(stocks.dtypes.index.values)
#print(stocks.dtypes)
#print('stocks.value type:',type(stocks.values))
#print('stocks type:',type(stocks))
#print('stocks.columns type:',type(stocks.columns),stocks.columns)
#print('stocks.index type:',type(stocks.index),stocks.index)
for stock in stocks:
	print('stock type',type(stock),stock)
	break
for code in stocks.index:
	print('code type:',type(code),code)
	break
for info in stocks.values:
	print('info type',type(info),info)
	break
for columns in stocks.columns:
	print('columns type',type(columns),columns)
	#stock_infotuple = namedtuple('stock_info',('index_name','value'))
	#print(type(info),type(stocks.dtypes.index.values),type([1,2,3,4]))
	#print(info.tolist(),stocks.dtypes.index.values.tolist())
	#stock_list = [stock_infotuple(index_name,value) for index_name,value in zip(stocks.dtypes.index.values.tolist(),info.tolist())]
	#print(stock_list)
	#print('😂😂😂😂😂😂😂😂😂😂😂😂😂😂😂')
for code, info in zip(stocks.index,stocks.values):
	stockDic = {}
	stockDic['code'] = code
	for index_name, value in zip(stocks.columns.tolist(),info.tolist()):
		stockDic[index_name] = value
	print('stockDic type',type(stockDic),'value:',stockDic)
	print('😂😂😂😂😂😂😂😂😂😂')
