import csv
import matplotlib.pyplot as plt
import numpy as np

def read_csv(file_path):
    x_values = []
    y_values = []
    with open(file_path, 'r') as file:
        reader = csv.reader(file)
        for i, row in enumerate(reader):
            x_values.append(i)
            y_value = float(row[0])  # Assuming one column in the CSV file
            y_values.append(max(0, min(y_value, 255)))  # Ensure y-value is in the range [0, 255]
    return x_values, y_values

def plot_data(x_values, y_values):
    plt.plot(x_values, y_values)
    plt.xlabel('Time')
    plt.ylabel('Percentage of Reading')
    plt.title('Plot of Soil Sensor Reading')

    # Calculate statistics
    mean_y = np.mean(y_values)
    median_y = np.median(y_values)
    max_y = np.max(y_values)
    min_y = np.min(y_values)
    std_y = np.std(y_values)


    # Plot statistics
    plt.axhline(y=mean_y, color='r', linestyle='--', label=f'Mean: {mean_y:.2f}')
    plt.axhline(y=median_y, color='c', linestyle='--', label=f'Median: {median_y:.2f}')
    plt.axhline(y=max_y, color='g', linestyle='--', label=f'Max: {max_y:.2f}')
    plt.axhline(y=min_y, color='b', linestyle='--', label=f'Min: {min_y:.2f}')
    plt.axhline(y=mean_y + std_y, color='m', linestyle='--', label=f'Std Deviation: {std_y:.2f}')
    plt.axhline(y=mean_y - std_y, color='m', linestyle='--')

   
    # Show legend
    plt.legend()

    plt.show()

if __name__ == "__main__":
    file_path = 'data.csv'  # Replace 'your_csv_file.csv' with the path to your CSV file
    x_values, y_values = read_csv(file_path)
    plot_data(x_values, y_values)
