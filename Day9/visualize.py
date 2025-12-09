import matplotlib.pyplot as plt
import matplotlib.patches as patches
from pathlib import Path

# Read input file
input_file = Path("input.txt")
if not input_file.exists():
    print(f"Error: {input_file} not found")
    exit(1)

positions = []
with open(input_file, 'r') as f:
    for line in f:
        parts = line.strip().split(',')
        x, y = int(parts[0]), int(parts[1])
        positions.append((x, y))

if not positions:
    print("No positions found in input file")
    exit(1)

# Create figure and axis
fig, ax = plt.subplots(1, 1, figsize=(14, 10))

# Draw the polygon
xs = [p[0] for p in positions]
ys = [p[1] for p in positions]

# Close the polygon by adding the first point at the end
xs_closed = xs + [xs[0]]
ys_closed = ys + [ys[0]]

ax.plot(xs_closed, ys_closed, 'b-', linewidth=2, label='Polygon')
ax.scatter(xs, ys, c='red', s=100, zorder=5, label='Vertices')

# Add labels to vertices
for i, (x, y) in enumerate(positions):
    ax.annotate(f'P{i}', (x, y), xytext=(5, 5), textcoords='offset points', fontsize=8)

# Set labels and title
ax.set_xlabel('X')
ax.set_ylabel('Y')
ax.set_title('Polygon from AoC Day 9')
ax.grid(True, alpha=0.3)
ax.legend()
ax.set_aspect('equal', adjustable='box')

# Invert y-axis if needed (common in grid problems)
ax.invert_yaxis()

plt.tight_layout()
plt.savefig('polygon.png', dpi=150, bbox_inches='tight')
print("Saved visualization to polygon.png")
plt.show()
