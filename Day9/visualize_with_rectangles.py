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

# Create figure with two subplots
fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(18, 8))

# --- First subplot: Just the polygon ---
xs = [p[0] for p in positions]
ys = [p[1] for p in positions]

xs_closed = xs + [xs[0]]
ys_closed = ys + [ys[0]]

ax1.plot(xs_closed, ys_closed, 'b-', linewidth=2, label='Polygon')
ax1.scatter(xs, ys, c='red', s=100, zorder=5, label='Vertices')

for i, (x, y) in enumerate(positions):
    ax1.annotate(f'P{i}', (x, y), xytext=(5, 5), textcoords='offset points', fontsize=8)

ax1.set_xlabel('X')
ax1.set_ylabel('Y')
ax1.set_title('Polygon from AoC Day 9')
ax1.grid(True, alpha=0.3)
ax1.legend()
ax1.set_aspect('equal', adjustable='box')
ax1.invert_yaxis()

# --- Second subplot: Polygon with all possible rectangles (faint) and biggest highlighted ---
ax2.plot(xs_closed, ys_closed, 'b-', linewidth=2, label='Polygon')
ax2.scatter(xs, ys, c='red', s=100, zorder=5, label='Vertices')

# Draw all possible rectangles between vertices in light gray
for i in range(len(positions)):
    for j in range(i, len(positions)):
        corner_a = positions[i]
        corner_b = positions[j]
        
        x_min = min(corner_a[0], corner_b[0])
        x_max = max(corner_a[0], corner_b[0])
        y_min = min(corner_a[1], corner_b[1])
        y_max = max(corner_a[1], corner_b[1])
        
        width = x_max - x_min
        height = y_max - y_min
        
        rect = patches.Rectangle(
            (x_min, y_min), width, height,
            linewidth=0.5, edgecolor='gray', facecolor='none', alpha=0.1
        )
        ax2.add_patch(rect)

# Highlight the biggest rectangle (you can modify this if you know the dimensions)
# For now, let's find a reasonably large rectangle by area
max_area = 0
best_rect = None
for i in range(len(positions)):
    for j in range(i, len(positions)):
        corner_a = positions[i]
        corner_b = positions[j]
        
        x_min = min(corner_a[0], corner_b[0])
        x_max = max(corner_a[0], corner_b[0])
        y_min = min(corner_a[1], corner_b[1])
        y_max = max(corner_a[1], corner_b[1])
        
        width = x_max - x_min
        height = y_max - y_min
        area = width * height
        
        if area > max_area:
            max_area = area
            best_rect = (x_min, y_min, width, height)

if best_rect:
    x_min, y_min, width, height = best_rect
    rect = patches.Rectangle(
        (x_min, y_min), width, height,
        linewidth=3, edgecolor='green', facecolor='green', alpha=0.2, label=f'Largest Rect (Area: {max_area})'
    )
    ax2.add_patch(rect)

ax2.set_xlabel('X')
ax2.set_ylabel('Y')
ax2.set_title('Polygon with Rectangle Candidates')
ax2.grid(True, alpha=0.3)
ax2.legend()
ax2.set_aspect('equal', adjustable='box')
ax2.invert_yaxis()

plt.tight_layout()
plt.savefig('polygon_with_rectangles.png', dpi=150, bbox_inches='tight')
print("Saved visualization to polygon_with_rectangles.png")
plt.show()
