import pandas as pd
import matplotlib.pyplot as plt
import os
import re
import numpy as np

# File path for the CSV
file_path = os.path.join("logs", "collectedData.csv")

# Load the CSV file
new_data = pd.read_csv(file_path)

# Extract the strategy name from the "Strategy" column
strategies = ["astar", "bfs", "dfs", "dijkstra"]
new_data['Strategy'] = new_data['Strategy'].apply(lambda x: next((s for s in strategies if re.match(s, x)), 'Unknown'))

# Convert 'Elapsed Time', 'Distance Taveled', 'Right Turns', and 'Left Turns' to numeric
new_data['Elapsed Time'] = pd.to_numeric(new_data['Elapsed Time'], errors='coerce')
new_data['Distance Taveled'] = pd.to_numeric(new_data['Distance Taveled'], errors='coerce')
new_data['Right Turns'] = pd.to_numeric(new_data['Right Turns'], errors='coerce')
new_data['Left Turns'] = pd.to_numeric(new_data['Left Turns'], errors='coerce')

# Drop rows with NaN in the relevant columns
new_data = new_data.dropna(subset=['Elapsed Time', 'Distance Taveled', 'Right Turns', 'Left Turns'])

# Parse coordinates from "Start" and "End" columns
def parse_coordinates(column):
    return column.apply(lambda x: list(map(float, x.split(":"))))

new_data["Start Coordinates"] = parse_coordinates(new_data["Start"])
new_data["End Coordinates"] = parse_coordinates(new_data["End"])

# Calculate Euclidean distance between Start and End
new_data["Euclidean Distance"] = new_data.apply(lambda row: np.linalg.norm(np.array(row["Start Coordinates"]) - np.array(row["End Coordinates"])), axis=1)

# Recalculate the total elapsed time, total deliveries, and total distances for each strategy
strategy_stats = new_data.groupby('Strategy').agg({
    'Elapsed Time': ['sum', 'count', 'mean'],
    'Distance Taveled': ['sum', 'mean'],
    'Euclidean Distance': ['sum', 'mean'],
    'Right Turns': 'sum',
    'Left Turns': 'sum'
}).reset_index()

# Flatten the column names
strategy_stats.columns = [
    'Strategy', 
    'Total Time', 'Total Deliveries', 'Average Time', 
    'Total Distance Traveled', 'Average Distance', 
    'Total Euclidean Distance', 'Average Euclidean Distance',
    'Total Right Turns', 'Total Left Turns'
]

strategy_stats['Average Time per Delivery'] = strategy_stats['Total Time'] / strategy_stats['Total Deliveries']

# Function to label bars
def label_bars(ax, bars):
    for bar in bars:
        yval = bar.get_height()
        label = f"{yval:.1f}" if not yval.is_integer() else f"{int(yval)}"
        ax.text(bar.get_x() + bar.get_width() / 2, yval, label, ha='center', va='bottom')

# Plot the data for Average Time per Delivery
fig, ax = plt.subplots()
bars_1 = ax.bar(strategy_stats['Strategy'], strategy_stats['Average Time per Delivery'])
ax.set_xlabel("Strategy")
ax.set_ylabel("Average Time per Delivery")
ax.set_title("Average Time per Delivery for Each Strategy")
label_bars(ax, bars_1)

# Save the figure
output_path_1 = os.path.join("logs", "average_time_per_delivery_by_strategy.png")
plt.savefig(output_path_1)

# Plot the data for Total Deliveries
fig, ax = plt.subplots()
bars_2 = ax.bar(strategy_stats['Strategy'], strategy_stats['Total Deliveries'])
ax.set_xlabel("Strategy")
ax.set_ylabel("Total Deliveries")
ax.set_title("Total Deliveries for Each Strategy")
label_bars(ax, bars_2)

# Save the figure
output_path_2 = os.path.join("logs", "total_deliveries_by_strategy.png")
plt.savefig(output_path_2)

# Plot the data for Total Distance
fig, ax = plt.subplots()
bars_3 = ax.bar(strategy_stats['Strategy'], strategy_stats['Total Distance Traveled'])
ax.set_xlabel("Strategy")
ax.set_ylabel("Total Distance Traveled")
ax.set_title("Total Distance Traveled for Each Strategy")
label_bars(ax, bars_3)

# Save the figure
output_path_3 = os.path.join("logs", "total_distance_traveled_by_strategy.png")
plt.savefig(output_path_3)

# Plot the data for Total Euclidean Distance
fig, ax = plt.subplots()
bars_4 = ax.bar(strategy_stats['Strategy'], strategy_stats['Total Euclidean Distance'])
ax.set_xlabel("Strategy")
ax.set_ylabel("Total Euclidean Distance")
ax.set_title("Total Euclidean Distance for Each Strategy")
label_bars(ax, bars_4)

# Save the figure
output_path_4 = os.path.join("logs", "total_euclidean_distance_by_strategy.png")
plt.savefig(output_path_4)

# Plot the data for the ratio of Total Distance to Total Euclidean Distance
strategy_stats['Distance Ratio'] = strategy_stats['Total Distance Traveled'] / strategy_stats['Total Euclidean Distance']

fig, ax = plt.subplots()
bars_5 = ax.bar(strategy_stats['Strategy'], strategy_stats['Distance Ratio'])
ax.set_xlabel("Strategy")
ax.set_ylabel("Distance Ratio")
ax.set_title("Ratio of Total Distance to\nTotal Euclidean Distance for Each Strategy")
label_bars(ax, bars_5)

# Save the figure
output_path_5 = os.path.join("logs", "distance_ratio_by_strategy.png")
plt.savefig(output_path_5)

# Plot the data for the ratio of Total Euclidean Distance to Total Time
strategy_stats['Efficiency'] = strategy_stats['Total Euclidean Distance'] / strategy_stats['Total Time']

fig, ax = plt.subplots()
bars_6 = ax.bar(strategy_stats['Strategy'], strategy_stats['Efficiency'])
ax.set_xlabel("Strategy")
ax.set_ylabel("Efficiency (Euclidean Distance / Time)")
ax.set_title("Efficiency of Each Strategy")
label_bars(ax, bars_6)

# Save the figure
output_path_6 = os.path.join("logs", "efficiency_by_strategy.png")
plt.savefig(output_path_6)

# Plot the data for the ratio of Euclidean Distance to Total Deliveries
strategy_stats['Euclidean Distance per Delivery'] = strategy_stats['Total Euclidean Distance'] / strategy_stats['Total Deliveries']

fig, ax = plt.subplots()
bars_7 = ax.bar(strategy_stats['Strategy'], strategy_stats['Euclidean Distance per Delivery'])
ax.set_xlabel("Strategy")
ax.set_ylabel("Euclidean Distance per Delivery")
ax.set_title("Euclidean Distance per Delivery for Each Strategy")
label_bars(ax, bars_7)

# Save the figure
output_path_7 = os.path.join("logs", "euclidean_distance_per_delivery_by_strategy.png")
plt.savefig(output_path_7)

# Plot the data for the ratio of Total Distance to Total Deliveries
strategy_stats['Distance per Delivery'] = strategy_stats['Total Distance Traveled'] / strategy_stats['Total Deliveries']

fig, ax = plt.subplots()
bars_8 = ax.bar(strategy_stats['Strategy'], strategy_stats['Distance per Delivery'])
ax.set_xlabel("Strategy")
ax.set_ylabel("Distance per Delivery")
ax.set_title("Distance per Delivery for Each Strategy")
label_bars(ax, bars_8)

# Save the figure
output_path_8 = os.path.join("logs", "distance_per_delivery_by_strategy.png")
plt.savefig(output_path_8)

# Plot the data for Total Right Turns
fig, ax = plt.subplots()
bars_9 = ax.bar(strategy_stats['Strategy'], strategy_stats['Total Right Turns'])
ax.set_xlabel("Strategy")
ax.set_ylabel("Total Right Turns")
ax.set_title("Total Right Turns for Each Strategy")
label_bars(ax, bars_9)

# Save the figure
output_path_9 = os.path.join("logs", "total_right_turns_by_strategy.png")
plt.savefig(output_path_9)

# Plot the data for Total Left Turns
fig, ax = plt.subplots()
bars_10 = ax.bar(strategy_stats['Strategy'], strategy_stats['Total Left Turns'])
ax.set_xlabel("Strategy")
ax.set_ylabel("Total Left Turns")
ax.set_title("Total Left Turns for Each Strategy")
label_bars(ax, bars_10)

# Save the figure
output_path_10 = os.path.join("logs", "total_left_turns_by_strategy.png")
plt.savefig(output_path_10)

# Plot the data for the ratio of Total Distance to Total Time
strategy_stats['Speed (Total Distance/Total Time)'] = strategy_stats['Total Distance Traveled'] / strategy_stats['Total Time']

fig, ax = plt.subplots()
bars_11 = ax.bar(strategy_stats['Strategy'], strategy_stats['Speed (Total Distance/Total Time)'])
ax.set_xlabel("Strategy")
ax.set_ylabel("Speed (Total Distance/Total Time)")
ax.set_title("Speed of Each Strategy (Total Distance/Total Time)")
label_bars(ax, bars_11)

# Save the figure
output_path_11 = os.path.join("logs", "speed_total_distance_per_total_time_by_strategy.png")
plt.savefig(output_path_11)

# Plot the data for the ratio of Average Distance to Average Time
strategy_stats['Speed (Average Distance/Average Time)'] = strategy_stats['Average Distance'] / strategy_stats['Average Time']

fig, ax = plt.subplots()
bars_12 = ax.bar(strategy_stats['Strategy'], strategy_stats['Speed (Average Distance/Average Time)'])
ax.set_xlabel("Strategy")
ax.set_ylabel("Speed (Average Distance/Average Time)")
ax.set_title("Speed of Each Strategy (Average Distance/Average Time)")
label_bars(ax, bars_12)

# Save the figure
output_path_12 = os.path.join("logs", "speed_average_distance_per_average_time_by_strategy.png")
plt.savefig(output_path_12)

# Optional: Show the plots (useful for debugging)
# plt.show()
