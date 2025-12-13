import matplotlib.pyplot as plt
import matplotlib.patches as patches
from pathlib import Path
import subprocess
import re

# First, let's run your C program and capture the output
print("Running the C program to find the biggest rectangle...")
try:
    result = subprocess.run(['part_2_2.exe'], capture_output=True, text=True, timeout=60)
    output = result.stdout + result.stderr
    print("Program output received")
except subprocess.TimeoutExpired:
    print("Warning: Program took too long, using timeout")
    output = ""
except Exception as e:
    print(f"Could not run program: {e}")
    output = ""

# Extract the biggest rectangle coordinates from output (take the last one)
biggest_rect = None
if output:
    # Look for pattern like: Rectangle A(x1,y1) B(x2,y2) is inside polygon
    # Use findall to get all matches and take the last one
    matches = re.findall(r'Rectangle A\((\d+),(\d+)\) B\((\d+),(\d+)\)', output)
    if matches:
        x1, y1, x2, y2 = map(int, matches[-1])  # Take the last match (biggest)
        biggest_rect = (x1, y1, x2, y2)
        print(f"Found biggest rectangle: A({x1},{y1}) B({x2},{y2})")

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

# Create figure
fig, ax = plt.subplots(figsize=(14, 10))

# Draw the polygon
xs = [p[0] for p in positions]
ys = [p[1] for p in positions]

xs_closed = xs + [xs[0]]
ys_closed = ys + [ys[0]]

ax.plot(xs_closed, ys_closed, 'b-', linewidth=2.5)
ax.fill(xs_closed, ys_closed, alpha=0.1, color='blue')

# Draw the biggest rectangle if we found it
if biggest_rect:
    x1, y1, x2, y2 = biggest_rect
    x_min = min(x1, x2)
    x_max = max(x1, x2)
    y_min = min(y1, y2)
    y_max = max(y1, y2)
    
    width = x_max - x_min
    height = y_max - y_min
    area = width * height
    
    rect = patches.Rectangle(
        (x_min, y_min), width, height,
        linewidth=3, edgecolor='green', facecolor='lime', 
        alpha=0.3, label=f'Biggest Rectangle (Area: {area})'
    )
    ax.add_patch(rect)

ax.set_xlabel('X', fontsize=12)
ax.set_ylabel('Y', fontsize=12)
ax.set_title('AoC Day 9 - Polygon and Biggest Rectangle', fontsize=14, fontweight='bold')
ax.grid(True, alpha=0.3)
ax.set_aspect('equal', adjustable='box')
ax.invert_yaxis()

plt.tight_layout()
plt.savefig('polygon_final.png', dpi=200, bbox_inches='tight')
print("Saved visualization to polygon_final.png")
plt.show()
