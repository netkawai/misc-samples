import matplotlib.pyplot as plt
import numpy as np
import urllib
import matplotlib.dates as mdates

def graph_data(stock):
  # http://www.google.com/finance/historical?q=NASDAQ:ADBE&startdate=Jan+01%2C+2009&enddate=Aug+2%2C+2012&output=csv
  stock_price_url = 'http://www.google.com/finance/historical?q=NASDAQ:' + stock + '&startdate=Jan+01%2C+2015&enddate=Nov+2%2C+2017&output=csv'

  source_code = urllib.urlopen(stock_price_url).read()
  stock_data = []
  split_source = source_code.split('\n')
  for line in split_source:
      split_line = line.split(',')
      if len(split_line) == 6:
          if 'Volume' not in line:
              stock_data.append(line)

  date, closep, highp, lowp, openp, volume = np.loadtxt(stock_data,
                                                        delimiter=',',
                                                        unpack=True,
                                                        converters={0:mdates.strpdate2num('%d-%b-%y')})
  fit = np.polyfit(date,closep,1)
  fit_fn = np.poly1d(fit)
  # fit_fn is now a function which takes in x and returns an estimate for y

  plt.plot_date(date,closep,'-')
  plt.plot_date(date, fit_fn(date))
  plt.xlabel('Date')
  plt.ylabel('Price')
  plt.title('Show Graph from Internet data')
  plt.legend()
  plt.show()

graph_data('ADBE')
