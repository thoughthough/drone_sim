#! /bin/python3
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv('logs/collectedData.csv')
# print(df.columns)
# print(df.info())
# print(df.head())

# Csv format: Strategy, Start Coordinates, End Coordinates, Elapsed Time
# get number of dfs, bfs, dijkstra, astar
print((df['Strategy'] == "dfs").sum())

# #split coordinates by : into [x, y, z]
# df['Start coordinates'] = df['Start Coordinates'].str.split(':')
# df['End'] = df['End Coordinates'].str.split(':')
# for i in range(len(df['End'])):
#     df['End'][i] = [float(j) for j in df['End'][i]]
#
#
# df['Start coordinates'] = df['Start coordinates'].apply(lambda x: [float(i) for i in x])
# df['x'] = df['End coordinates'].apply(lambda x : float(x[0]))
# df['y'] = df['End coordinates'].apply(lambda x : float(x[2]))
#
# # print(df['End coordinates'])
# #
# fig, ax = plt.subplots()
#
# #plot end  x, z coordinates on a grid. The x is between -1400.0 and 1500.0, z is between 800.0 and -800.0
# # color  by Strategy, astar, dijkstra, bfs, dfs,
# colors = plt.cm.jet(np.linspace(0, 1, len(df['Strategy'].unique())))
# for (strategy, color) in zip(df['Strategy'].unique(), colors):
#     subset = df[df['Strategy'] == strategy]
#     ax.scatter(subset['End coordinates'], subset['End coordinates'][2], color=color, label=strategy)





# plt.xlabel('X')
# plt.ylabel('Z')
# plt.legend()
# plt.show()




