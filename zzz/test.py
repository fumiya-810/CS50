from pandas_datareader import data
import pandas as pd
import matplotlib.pyplot as plt

start = '2019-06-01'
end = '2020-06-01'

df = data.DataReader('^N225', 'yahoo', start, end)
df.head(10)
