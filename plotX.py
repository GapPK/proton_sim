import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

# Read the data from the file
data = np.loadtxt('scattering_data_run_0.txt', skiprows=2)

# Extract columns
events = data[:, 0]
energies = data[:, 1]
positions_x = data[:, 2]
positions_y = data[:, 3]
positions_z = data[:, 4]

# Create a figure with two subplots
fig = plt.figure(figsize=(16, 8))

# Plot 1: Histogram of energies
ax1 = fig.add_subplot(121)
ax1.hist(energies, bins=50, edgecolor='black')
ax1.set_xlabel('Energy (MeV)')
ax1.set_ylabel('Count')
ax1.set_title('Energy Distribution of Scattered Protons')

# Plot 2: 3D scatter plot of positions
ax2 = fig.add_subplot(122, projection='3d')
scatter = ax2.scatter(positions_x, positions_y, positions_z, c=energies, cmap='viridis')
ax2.set_xlabel('X Position (mm)')
ax2.set_ylabel('Y Position (mm)')
ax2.set_zlabel('Z Position (mm)')
ax2.set_title('3D Scatter Plot of Proton Positions')

# Add a colorbar
cbar = fig.colorbar(scatter, ax=ax2, label='Energy (MeV)')

plt.tight_layout()
plt.show()
