import matplotlib.pyplot as plt

# Load data from the file
energy, cross_section = [], []
with open('proton_cross_section_vs_energy.txt', 'r') as f:
    next(f)  # Skip header
    for line in f:
        e, cs = map(float, line.split())
        energy.append(e)
        cross_section.append(cs)

# Plot the data
plt.figure(figsize=(10, 6))
plt.plot(energy, cross_section, marker='o', linestyle='-', color='b')
plt.xlabel('Proton Energy (MeV)')
plt.ylabel('Cross-Section (cm²)')
plt.title('Proton Cross-Section vs Energy')
plt.grid(True)
plt.show()

