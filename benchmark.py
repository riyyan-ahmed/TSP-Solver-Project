import subprocess, sys, time, glob, os, re

files = sys.argv[1:] or sorted(glob.glob("TSP-instances/*/*.tsp"))
os.makedirs("output/benchmark", exist_ok=True)
print("instance,n,avg_ms")

for f in files:
    n = re.search(r"DIMENSION\s*:\s*(\d+)", open(f).read()).group(1)
    out = "output/benchmark/" + os.path.basename(f) + ".tour"
    runs = []
    for _ in range(5):
        start = time.perf_counter()
        subprocess.run(["./build/TSPsolver", f, out], check=True, capture_output=True)
        runs.append((time.perf_counter() - start) * 1000)
    print(f"{os.path.basename(f)},{n},{sum(runs) / len(runs):.2f}")