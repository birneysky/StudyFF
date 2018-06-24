import tushare as ts

#获取a股所有股票的基本信息
stocks = ts.get_stock_basics()


class Stock:
	__code__ = 0               #证券代码
	__name__ = ''      	       #股票名
	__industry__ = ''          #上市公司的业务领域
	__area__ = ''              #上市公司注册地
	__pe__ = 0.0               #市盈率
	__outstanding__ = 0.0      #流通股本
	__totals__ = 0.0           #总股本(亿)
	__totalAssets__ = 0.0      #总资产(万)
	__liquidAssets__ = 0.0     #流动资产
	__fixedAssets__ = 0.0      #固定资产
	__reserved__ = 0.0         #公积金
	__reservedPerShare__ = 0.0 #每股公积金
	__esp__ = 0.0              #每股收益
	__bvps__ = 0.0             #每股净资
	__pb__ = 0.0               #市净率
	__timeToMarket__ = ''      #上市日期
	__undp__ = 0.0             #未分利润
	__perundp__ = 0.0          #每股未分配
	__rev__ = 0.0              #收入同比(%)
	__profit__ = 0.0           #利润同比(%)
	__gpr__ = 0.0              #毛利率(%)
	__npr__ = 0.0              #净利润率(%)
	__holders__ = 0.0          #股东人数

	@property
	def code(self):
		return self.__code__
	@property
	def name(self):
		return self.__name__
	@property
	def industry(self):
		return self.__industry
	@property
	def area(self):
		return self.__area
	@property
	def pe(self):
		return self.__pe
	@property
	def outstanding(self):
		return self.__outstanding
	@property
	def totals(self):
		return self.__totals
	@property
	def totalAssets(self):
		return self.__totalAssets
	@property
	def liquidAssets(self):
		return self.__liquidAssets
	@property
	def fixedAssets(self):
		return self.__fixedAssets
	@property
	def reserved(self):
		return self.__reserved
	@property
	def reservedPerShare(self):
		return self.__reservedPerShare

	@property
	def esp(self):
		return self.__esp		
	@property
	def bvps(self):
		return self.__bvps
	@property
	def pb(self):
		return self.__pb
	@property
	def timeToMarket(self):
		return self.__timeToMarket
	@property
	def undp(self):
		return self.__undp
	@property
	def perundp(self):
		return self.__perundp
	@property
	def rev(self):
		return self.__rev
	@property
	def profit(self):
		return self.__profit
	@property
	def gpr(self):
		return self.__gpr
	@property
	def npr(self):
		return self.__npr
	@property
	def holders(self):
		return self.__holders


	def __init__(self, **entries):
		self.__dict__.update(entries)
		for key,value in entries.items():
			self.key = value


	def __str__(self):
		return 'name:{},\nindustry:{},\narea:{},\npe:{:.2f},\noutstanding:{:.2f},\
				\ntotals:{:.2f},\ntotalAssets:{:.2f},\nliquidAssets:{:.2f},\
				\nfixedAssets:{:.2f},\nreserved:{:.2f},\nreservedPerShare:{:.2f},\
				\nesp:{:.2f},\nbvps:{:.2f},\npb:{:.2f},\ntimeToMarket:{},\
				\nundp:{:.2f},\nperundp:{:.2f},\nrev:{:.2f},\nprofit:{:.2f},\
				\ngpr:{:.2f},\nnpr:{:.2f},\nholders:{:.2f}'\
				.format(self.__name__,self.__industry__,self.__area__,self.__pe__,
					self.__outstanding__,self.__totals__,self.__totalAssets__,
					self.__liquidAssets__,self.__fixedAssets__,self.__reserved__,
					self.__reservedPerShare__,self.__esp__,self.__bvps__,self.__pb__,
					self.__timeToMarket__,self.__undp__,self.__perundp__,
					self.__rev__,self.__profit__,self.__gpr__,self.__npr__,
					self.__holders__)
for code, info in zip(stocks.index,stocks.values):
	stockDic = {}
	stockDic['__code__'] = code
	for index_name, value in zip(stocks.columns.tolist(),info.tolist()):
		index_name = '__' + index_name + '__'
		stockDic[index_name] = value
	#print('stockDic type',type(stockDic),'value:',stockDic)
	print('😂😂😂😂😂😂😂😂😂😂')
	stock = Stock(**stockDic)
	print(stock)