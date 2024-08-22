import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import os

# File path for the CSV
file_path = os.path.join("logs", "collectedData.csv")

# Load the CSV file
new_data = pd.read_csv(file_path)

# Check if 'Euclidean Distance' column already exists
if 'Euclidean Distance' not in new_data.columns:
    def parse_coordinates(column):
        return column.apply(lambda x: list(map(float, x.split(":"))))
    
    new_data["Start Coordinates"] = parse_coordinates(new_data["Start"])
    new_data["End Coordinates"] = parse_coordinates(new_data["End"])

    # Calculate Euclidean distance
    new_data["Euclidean Distance"] = new_data.apply(
        lambda row: np.linalg.norm(
            np.array(row["Start Coordinates"]) - np.array(row["End Coordinates"])
        ),
        axis=1
    )

# Calculate percentiles for Euclidean Distance
lower_percentile = new_data['Euclidean Distance'].quantile(0.25)
upper_percentile = new_data['Euclidean Distance'].quantile(0.75)

# Classify each delivery as short, medium, or long
def classify_delivery(distance):
    if distance < lower_percentile:
        return 'Short'
    elif distance > upper_percentile:
        return 'Long'
    else:
        return 'Medium'

new_data['Delivery Length'] = new_data['Euclidean Distance'].apply(classify_delivery)

# Count the number of deliveries for each category and strategy
delivery_counts = new_data.groupby(['Strategy', 'Delivery Length']).size().unstack(fill_value=0)

# Plot the results
fig, ax = plt.subplots()
strategies = delivery_counts.index
bar_width = 0.25
positions = np.arange(len(strategies))
short_counts = delivery_counts.get('Short', pd.Series([0] * len(strategies)))
medium_counts = delivery_counts.get('Medium', pd.Series([0] * len(strategies)))
long_counts = delivery_counts.get('Long', pd.Series([0] * len(strategies)))

bars_short = ax.bar(positions - bar_width, short_counts, width=bar_width, label='Short')
bars_medium = ax.bar(positions, medium_counts, width=bar_width, label='Medium')
bars_long = ax.bar(positions + bar_width, long_counts, width=bar_width, label='Long')

# Add labels to the bars
def label_bars(ax, bars):
    for bar in bars:
        yval = bar.get_height()
        label = f"{yval:.1f}" if not yval.is_integer() else f"{int(yval)}"
        ax.text(bar.get_x() + bar.get_width() / 2, yval, label, ha='center', va='bottom')

for bars in [bars_short, bars_medium, bars_long]:
    label_bars(ax, bars)

ax.set_xlabel("Strategy")
ax.set_ylabel("Number of Deliveries")
ax.set_title("Number of Short, Medium, and Long Deliveries for Each Strategy")
ax.set_xticks(positions)
ax.set_xticklabels(strategies)
ax.legend()

# Save the figure
output_path = os.path.join("logs", "delivery_length_counts_by_strategy.png")
plt.savefig(output_path)

# Optional: Show the plot
# plt.show()
