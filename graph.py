import re
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.ticker import MultipleLocator

# ==================================================
# Read benchmark file
# ==================================================

orders = []
fast_times = []
normal_times = []
speedups = []

with open("BENCHMARK.txt", "r") as f:
	content = f.read()

pattern = re.compile(
	r"Order:\s*(\d+)\s*"
	r"Fast:\s*([0-9.eE+-]+)\s*"
	r"Normal:\s*([0-9.eE+-]+)\s*"
	r"Speedup:\s*([0-9.eE+-]+)"
)

for match in pattern.finditer(content):
	order = int(match.group(1))
	fast = float(match.group(2))
	normal = float(match.group(3))
	speedup = float(match.group(4))

	orders.append(order)
	fast_times.append(fast)
	normal_times.append(normal)

	# Convert x-fold speedup into percentage gain
	# 1.00x -> 0%
	# 1.50x -> 50%
	# 2.00x -> 100%
	speedups.append((speedup - 1.0) * 100.0)

orders = np.array(orders)
fast_times = np.array(fast_times)
normal_times = np.array(normal_times)
speedups = np.array(speedups)

# ==================================================
# Smooth speedup curve
# ==================================================

window = 25

kernel = np.ones(window) / window

smooth_speedup = np.convolve(
	speedups,
	kernel,
	mode="same"
)

# ==================================================
# Create figure
# ==================================================

fig, (ax1, ax2) = plt.subplots(
	2,
	1,
	figsize=(14, 10),
	sharex=True
)

# ==================================================
# Timing graph
# ==================================================

ax1.plot(
	orders,
	fast_times,
	label="Fast",
	linewidth=1,
	alpha=0.65,
	linestyle="-"
)

ax1.plot(
	orders,
	normal_times,
	label="Normal",
	linewidth=1,
	alpha=0.65,
	linestyle="--"
)

ax1.fill_between(
	orders,
	fast_times,
	normal_times,
	alpha=0.12
)

ax1.set_yscale("log")

ax1.set_title("Execution Time Comparison (Log Scale)")
ax1.set_ylabel("Time (seconds)")

ax1.grid(
	True,
	which="both",
	alpha=0.3
)

ax1.legend()

ax1.set_xlim(
	orders.min(),
	orders.max()
)

ax1.margins(x=0)

# ==================================================
# Speedup graph
# ==================================================

ax2.plot(
	orders,
	speedups,
	alpha=0.25,
	linewidth=1,
	label="Raw speedup"
)

ax2.plot(
	orders,
	smooth_speedup,
	linewidth=1,
	label=f"Moving average ({window})"
)

ax2.axhline(
	0,
	linestyle="--",
	linewidth=1
)

ax2.set_title("Fast Version Speedup")
ax2.set_xlabel("Group order")
ax2.set_ylabel("Speedup (%)")

# More y-axis values
# ax2.yaxis.set_major_locator(MultipleLocator(5))
# ax2.yaxis.set_minor_locator(MultipleLocator(1))

ymin = speedups.min()
ymax = speedups.max()

span = ymax - ymin

if span < 10:
	step = 1
elif span < 25:
	step = 2
elif span < 50:
	step = 5
else:
	step = 10

ax2.yaxis.set_major_locator(MultipleLocator(step))

ax2.grid(
	True,
	which="major",
	alpha=0.4
)

ax2.grid(
	True,
	which="minor",
	alpha=0.15
)

ax2.legend()

# ==================================================
# Show graph
# ==================================================

plt.tight_layout()
plt.show()

# ==================================================
# Statistics
# ==================================================

print()
print("========== Statistics ==========")

print(
	f"Average speedup : {np.mean(speedups):.2f}%"
)

print(
	f"Median speedup  : {np.median(speedups):.2f}%"
)

print(
	f"Maximum speedup : {np.max(speedups):.2f}%"
)

print(
	f"Minimum speedup : {np.min(speedups):.2f}%"
)

best_idx = np.argmax(speedups)
worst_idx = np.argmin(speedups)

print()

print(
	f"Best order      : {orders[best_idx]}"
	f" ({speedups[best_idx]:.2f}%)"
)

print(
	f"Worst order     : {orders[worst_idx]}"
	f" ({speedups[worst_idx]:.2f}%)"
)