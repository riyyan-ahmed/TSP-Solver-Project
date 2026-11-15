# Written with help of AI : by Claude (Claude Opus 5.5), 2026-11-11: to extract the relevant tsplib optimum data and run solver
import csv, itertools, math, pathlib, statistics, subprocess, tempfile, time

SOLVER = "./build/TSPsolver"
METHODS = ["trivial", "held-karp", "mst-2opt", "nn-2opt"]
# HeldKarpSolver::maxCities
HELD_KARP_LIMIT = 24        
RUNS = 5

# optimal tour lengths published with TSPLIB
TSPLIB_OPTIMUM = {"att48": 10628, "berlin52": 7542, "burma14": 3323, "ch130": 6110, "eil76": 538, "pr1002": 259045, "pr299": 48191, "pr76": 108159, "st70": 675, "ulysses16": 6859,"dantzig42": 699, "gr17": 2085, "gr21": 2707, "gr24": 1272, "gr48": 5046}


def read_instance(path):
    """Returns the header, the sorted city ids and a distance function on 0-based indices (TSPLIB formulas)."""
    header, data, section = {}, [], None
    for line in path.read_text().splitlines():
        line = line.strip()
        if not line or line == "EOF":
            continue
        if line in ("NODE_COORD_SECTION", "EDGE_WEIGHT_SECTION", "DISPLAY_DATA_SECTION"):
            section = line
        elif section is None:
            key, value = line.split(":", 1)
            header[key.strip()] = value.strip()
        elif section != "DISPLAY_DATA_SECTION":
            data.extend(line.split())

    n = int(header["DIMENSION"])
    kind = header["EDGE_WEIGHT_TYPE"]
    if kind == "EXPLICIT":                       
        w = [[0] * n for _ in range(n)]
        values = iter(int(x) for x in data)
        for i in range(n):
            for j in range(i + 1):
                w[i][j] = w[j][i] = next(values)
        return header, list(range(1, n + 1)), lambda a, b: w[a][b]

    rows = [data[k:k + 3] for k in range(0, len(data), 3)]
    ids = sorted(int(r[0]) for r in rows)
    points = {int(r[0]): (float(r[1]), float(r[2])) for r in rows}
    p = [points[ids[0] + k] for k in range(n)]
    if kind == "EUC_2D":
        return header, ids, lambda a, b: int(math.dist(p[a], p[b]) + 0.5)
    if kind == "ATT":
        def att(a, b):
            r = math.sqrt(((p[a][0] - p[b][0]) ** 2 + (p[a][1] - p[b][1]) ** 2) / 10.0)
            t = int(r + 0.5)
            return t + 1 if t < r else t
        return header, ids, att
    if kind == "GEO":
        def radians(x):
            degrees = int(x)
            return 3.141592 * (degrees + 5.0 * (x - degrees) / 3.0) / 180.0
        r = [(radians(x), radians(y)) for x, y in p]
        def geo(a, b):
            if a == b:
                return 0
            q1 = math.cos(r[a][1] - r[b][1])
            q2 = math.cos(r[a][0] - r[b][0])
            q3 = math.cos(r[a][0] + r[b][0])
            return int(6378.388 * math.acos(min(1.0, 0.5 * ((1 + q1) * q2 - (1 - q1) * q3))) + 1.0)
        return header, ids, geo
    raise ValueError(f"unsupported EDGE_WEIGHT_TYPE {kind}")


def read_tour(path, header, ids):
    """Checks the exact format and returns the tour as 0-based indices."""
    name = header["NAME"]
    name = (name[:-4] if name.endswith(".tsp") else name) + ".tour"
    expected_head = [f"NAME : {name}", "TYPE : TOUR", f"DIMENSION : {len(ids)}", "TOUR_SECTION"]
    lines = path.read_text().split("\n")
    if lines[:4] != expected_head or lines[-3:] != ["-1", "EOF", ""]:
        raise ValueError(f"{path}: wrong header or ending")
    visited = [int(x) for x in lines[4:-3]]
    if sorted(visited) != ids:
        raise ValueError(f"{path}: not a permutation of the city ids")
    return [v - ids[0] for v in visited]


def tour_length(tour, d):
    return sum(d(tour[k], tour[(k + 1) % len(tour)]) for k in range(len(tour)))


def mean_time_ms(instance, method, out):
    """Mean wall-clock time of RUNS complete runs of the solver, after one warm-up run."""
    times = []
    for _ in range(RUNS + 1):
        start = time.perf_counter()
        subprocess.run([SOLVER, str(instance), str(out), "-m", method], check=True, capture_output=True)
        times.append(1000 * (time.perf_counter() - start))
    return round(statistics.mean(times[1:]), 2)


def methods_for(n):
    return [m for m in METHODS if m != "held-karp" or n <= HELD_KARP_LIMIT]


def provided_results():
    """Time and tour quality on the provided instances; the tours are kept in"""
    rows = []
    instances = sorted(pathlib.Path("TSP-instances").glob("*/*.tsp"),
                       key=lambda p: (int(read_instance(p)[0]["DIMENSION"]), p.stem))
    for instance in instances:
        header, ids, d = read_instance(instance)
        n = len(ids)
        optimum = TSPLIB_OPTIMUM.get(instance.stem)
        if optimum is None and n <= 10:         
            optimum = min(tour_length((0,) + order, d) for order in itertools.permutations(range(1, n)))
        for method in methods_for(n):
            out = pathlib.Path("output") / f"{method.replace('-', '_')}_solution" / f"{instance.stem}.tour"
            time_ms = mean_time_ms(instance, method, out)
            length = tour_length(read_tour(out, header, ids), d)
            if length < optimum or (method == "held-karp" and length != optimum):
                raise ValueError(f"{instance.stem} {method}: length {length}, optimum {optimum}")
            rows.append({"instance": instance.stem, "n": n, "method": method, "time_ms": time_ms,
                         "tour_length": length, "optimum": optimum,
                         "gap_percent": round(100 * (length - optimum) / optimum, 1)})
            print(instance.stem, method, time_ms, "ms", flush=True)
    return rows


def scaling_results():
    """Time against the number of cities, on the generated instances."""
    rows = []
    instances = sorted(pathlib.Path("output/scaling").glob("*.tsp"), key=lambda p: int(p.stem[1:]))
    with tempfile.TemporaryDirectory() as tmp:
        for instance in instances:
            header, ids, _ = read_instance(instance)
            for method in methods_for(len(ids)):
                out = pathlib.Path(tmp) / "tour.tour"
                time_ms = mean_time_ms(instance, method, out)
                read_tour(out, header, ids)
                rows.append({"n": len(ids), "method": method, "time_ms": time_ms})
                print(instance.stem, method, time_ms, "ms", flush=True)
    return rows


def write_csv(path, rows):
    with open(path, "w", newline="") as f:
        writer = csv.DictWriter(f, fieldnames=list(rows[0]))
        writer.writeheader()
        writer.writerows(rows)
    print("wrote", path)


if __name__ == "__main__":
    write_csv("output/results_provided.csv", provided_results())
    write_csv("output/results_scaling.csv", scaling_results())
