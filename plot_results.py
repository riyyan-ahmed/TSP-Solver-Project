# Written by Claude (Claude Opus 5.5), 2026-11-15 : Guided by us that tells it which types of graphs we want for what purposes
import csv, pathlib
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt

OUT = pathlib.Path("output/figures")
METHODS = ["trivial", "held-karp", "mst-2opt", "nn-2opt"]
COLOR = {"trivial": "#3d3d3d", "held-karp": "#b35806", "mst-2opt": "#1f4e9c", "nn-2opt": "#1b7a4a"}
MARKER = {"trivial": "o", "held-karp": "s", "mst-2opt": "^", "nn-2opt": "D"}

plt.rcParams.update({"font.size": 9, "axes.spines.top": False, "axes.spines.right": False})


def read(name):
    with open(pathlib.Path("output") / name) as f:
        return list(csv.DictReader(f))


def scaling_chart(rows):
    """Time of each method against the number of cities, both axes in log scale."""
    fig, ax = plt.subplots(figsize=(6.0, 3.6))
    for method in METHODS:
        points = [(int(r["n"]), float(r["time_ms"])) for r in rows if r["method"] == method]
        ax.plot(*zip(*points), color=COLOR[method], marker=MARKER[method], markersize=5,
                linewidth=2, label=method)
    ax.set_xscale("log")
    ax.set_yscale("log")
    ax.set_xticks([8, 12, 16, 22, 200, 500, 2000], labels=["8", "12", "16", "22", "200", "500", "2000"])
    ax.set_yticks([1, 2, 5, 10, 20, 50, 100, 200, 500], labels=["1", "2", "5", "10", "20", "50", "100", "200", "500"])
    ax.minorticks_off()
    ax.set_xlabel("number of cities n (log scale)")
    ax.set_ylabel("time (ms, log scale)")
    ax.grid(True, color="#dddddd", linewidth=0.8)
    ax.legend(frameon=False, loc="lower center", bbox_to_anchor=(0.5, 1.0), ncol=4)
    fig.tight_layout()
    fig.savefig(OUT / "scaling_time.png", dpi=200)
    plt.close(fig)


def gap_chart(rows):
    """Gap to the optimum of the two 2-opt methods on every provided instance."""
    instances = list(dict.fromkeys(r["instance"] for r in rows))
    gap = {(r["instance"], r["method"]): float(r["gap_percent"]) for r in rows}
    fig, ax = plt.subplots(figsize=(7.0, 3.4))
    for k, method in enumerate(["mst-2opt", "nn-2opt"]):
        xs = [i + (k - 0.5) * 0.36 for i in range(len(instances))]
        values = [gap[(name, method)] for name in instances]
        ax.bar(xs, values, width=0.33, color=COLOR[method], label=method)
        for x, value in zip(xs, values):
            if value == 0:              
                ax.text(x, 0.15, "0", ha="center", va="bottom", fontsize=7)
    ax.set_xticks(range(len(instances)), labels=instances, rotation=45, ha="right")
    ax.set_ylabel("gap to the optimum (%)")
    ax.grid(True, axis="y", color="#dddddd", linewidth=0.8)
    ax.set_axisbelow(True)
    ax.legend(frameon=False, loc="upper left")
    fig.tight_layout()
    fig.savefig(OUT / "provided_gap.png", dpi=200)
    plt.close(fig)


def tour_chart(rows, instance="burma14"):
    """The tour of each method on one small instance (GEO: latitude and longitude)."""
    points, inside = {}, False
    for line in pathlib.Path(f"TSP-instances/coord/{instance}.tsp").read_text().splitlines():
        line = line.strip()
        if line == "NODE_COORD_SECTION":
            inside = True
        elif line == "EOF":
            break
        elif inside and line:
            i, latitude, longitude = line.split()
            points[int(i)] = (float(longitude), float(latitude))
    result = {r["method"]: r for r in rows if r["instance"] == instance}

    fig, axes = plt.subplots(1, 4, figsize=(9.0, 2.8))
    for ax, method in zip(axes, METHODS):
        lines = (pathlib.Path("output") / f"{method.replace('-', '_')}_solution" / f"{instance}.tour").read_text().split("\n")
        ids = [int(x) for x in lines[lines.index("TOUR_SECTION") + 1:lines.index("-1")]]
        cycle = ids + ids[:1]
        ax.plot([points[i][0] for i in cycle], [points[i][1] for i in cycle], color=COLOR[method], linewidth=1.8)
        ax.scatter([p[0] for p in points.values()], [p[1] for p in points.values()], s=16, color="black", zorder=3)
        r = result[method]
        ax.set_title(f"{method}\n{r['tour_length']} (+{float(r['gap_percent']):.1f} %)")
        ax.set_aspect("equal")
        ax.axis("off")
    fig.tight_layout()
    fig.savefig(OUT / f"tours_{instance}.png", dpi=200)
    plt.close(fig)


if __name__ == "__main__":
    OUT.mkdir(exist_ok=True)
    provided = read("results_provided.csv")
    scaling_chart(read("results_scaling.csv"))
    gap_chart(provided)
    tour_chart(provided)
    print("figures written to", OUT)
