# NETROUTE SIMULATOR

A lightweight, beginner-friendly CLI-based Network Routing Simulator written in standard C++17. Designed specifically for college Object-Oriented Programming (OOP) and Data Structures vivas and demonstrations.

---

## 1. Project Overview

**NetRoute Simulator** models a computer network of routers connected by bidirectional, weighted communication links. It allows users to:
1. Dynamically add routers with automatically assigned integer IDs (`1, 2, 3, ...`).
2. Establish communication links between routers with custom positive costs.
3. Visualize the entire network topology, including connected and isolated routers.
4. Calculate and display the optimal shortest path and minimum total transmission cost between any two routers using **Dijkstra's Algorithm**.

The project is intentionally written without overengineering or complicated abstractions, making every line of code easy to read, understand, and defend during an oral examination (viva).

---

## 2. Menu Options

The application presents a clean and simple menu:

```text
========================================
         NETROUTE SIMULATOR
========================================
1. Add Router
2. Connect Routers
3. Display Network
4. Find Shortest Route
0. Exit
========================================
Enter your choice: 
```

---

## 3. Features & Validation Rules

- **Option 1: Add Router**
  - Adds a new router node to the network.
  - Automatically generates an incremental integer ID (`1, 2, 3...`).
  - Resizes the adjacency list to accommodate new connections.

- **Option 2: Connect Routers**
  - Prompts for two router IDs and a link cost.
  - Links are bidirectional (traffic can flow in both directions).
  - **Validation checks:**
    - Rejects IDs of routers that do not exist.
    - Rejects self-loops (connecting a router to itself).
    - Rejects non-positive link costs ($\le 0$).
    - Rejects duplicate connections between already connected routers.

- **Option 3: Display Network**
  - Displays each router along with its connected neighbors and corresponding link costs.
  - Clearly identifies isolated routers that have no connections (`[No connections (Isolated)]`).

- **Option 4: Find Shortest Route**
  - Prompts for source and destination router IDs.
  - Uses Dijkstra's algorithm to compute the path with the minimum total cost.
  - Prints the complete hop sequence (e.g., `1 -> 2 -> 3`) and total cost.
  - Detects unreachable destinations and displays `"No route exists between these routers."`.

- **Option 0: Exit**
  - Gracefully terminates the program.

---

## 4. How the Network is Represented

The network is represented as an undirected graph using an **Adjacency List**:

```cpp
struct Edge {
    int to;    // Neighbor router ID
    int cost;  // Link cost (weight)
};

int routerCount = 0;
vector<vector<Edge>> adjList;
```

### Why Adjacency List?
- **Memory Efficient:** Takes $\mathcal{O}(V + E)$ space instead of $\mathcal{O}(V^2)$ needed by an adjacency matrix.
- **Fast Traversal:** Iterating over the neighbors of a router only visits actual existing connections.

---

## 5. How Dijkstra's Algorithm Works

Dijkstra's algorithm finds the shortest path between a source router and all other routers in a weighted graph with non-negative edge weights:

1. **Initialization:**
   - Set the distance to the source router as `0`.
   - Set all other router distances to infinity (`INF = 1,000,000,000`).
   - Push `(0, source)` into a min-priority queue (`std::priority_queue` with `std::greater`).
2. **Min-Distance Selection:**
   - Extract the router `u` with the smallest tentative distance from the queue.
   - If `u` has already been visited, skip it. Otherwise, mark `u` as visited.
   - If `u` is the destination router, we can stop early.
3. **Edge Relaxation:**
   - For every neighbor `v` of `u` with link cost `w`:
     - If $\text{distance}[u] + w < \text{distance}[v]$, update $\text{distance}[v] = \text{distance}[u] + w$.
     - Set $\text{parent}[v] = u$ to remember how we reached `v`.
     - Push `(distance[v], v)` into the priority queue.
4. **Path Reconstruction:**
   - Starting from `destination`, trace backwards using the `parent` array until reaching `source`.
   - Reverse the sequence to obtain the path from source to destination.

---

## 6. Step-by-Step Example Walkthrough

### Scenario:
1. Add 3 routers: Router 1, Router 2, Router 3.
2. Connect Router 1 and Router 2 with cost 4.
3. Connect Router 2 and Router 3 with cost 5.
4. Connect Router 1 and Router 3 with cost 12.

### Graph Diagram:
```text
         (4)
    [1]-------[2]
      \       /
  (12) \     / (5)
        \   /
         [3]
```

### Finding Shortest Route from Router 1 to Router 3:
- Direct path: $1 \rightarrow 3$ (Cost = 12).
- Indirect path via Router 2: $1 \rightarrow 2 \rightarrow 3$ (Cost = $4 + 5 = 9$).
- Dijkstra's algorithm selects $1 \rightarrow 2 \rightarrow 3$ because $9 < 12$.
- Output:
  ```text
  ========================================
              SHORTEST ROUTE
  ========================================
  Source Router: 1
  Destination Router: 3
  Shortest Path: 1 -> 2 -> 3
  Total Cost: 9
  ========================================
  ```

---

## 7. Compilation & Execution

The entire project is self-contained in `main.cpp` using only the C++ Standard Library. No external build systems or dependencies are required.

### Compile with strict flags:
```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic main.cpp -o NetRouteSimulator
```

### Run on Windows:
```powershell
.\NetRouteSimulator.exe
```

### Run on Linux / macOS:
```bash
./NetRouteSimulator
```

---

## 8. Viva Questions and Answers

1. **Q: What data structure is used to represent the network?**
   - **A:** An Adjacency List using `vector<vector<Edge>>`, where each router ID indexes a vector of `Edge` structs containing the neighbor ID and link cost.

2. **Q: Why is an adjacency list preferred over an adjacency matrix?**
   - **A:** An adjacency list uses $\mathcal{O}(V + E)$ space, which is optimal for sparse graphs, whereas an adjacency matrix uses $\mathcal{O}(V^2)$ space regardless of the number of links.

3. **Q: What is the purpose of Dijkstra's algorithm?**
   - **A:** It finds the shortest (lowest total cost) path from a single source node to all other nodes in a graph with non-negative edge weights.

4. **Q: Why do we use `priority_queue` with `greater`?**
   - **A:** By default, C++'s `std::priority_queue` is a max-heap. Using `greater<pair<int, int>>` turns it into a min-heap so that the router with the smallest tentative distance is popped first.

5. **Q: What is "relaxation" in Dijkstra's algorithm?**
   - **A:** Relaxation is the process of checking whether traveling through the current router `u` gives a shorter path to neighbor `v` than previously recorded, i.e., `if (distance[u] + cost < distance[v])`.

6. **Q: How is the shortest path reconstructed?**
   - **A:** We maintain a `parent` array. When relaxing an edge to router `v` from router `u`, we record `parent[v] = u`. After Dijkstra finishes, we backtrack from destination to source using `parent`, and reverse the list.

7. **Q: Can Dijkstra's algorithm handle negative link costs?**
   - **A:** No. Dijkstra greedily assumes that once a node is visited, its shortest distance is finalized. Negative weights violate this assumption. For negative weights, the Bellman-Ford algorithm must be used.

8. **Q: What is the time complexity of this Dijkstra implementation?**
   - **A:** $\mathcal{O}((V + E) \log V)$ using a binary min-priority queue, where $V$ is the number of routers and $E$ is the number of links.

9. **Q: How does the program detect that a destination is unreachable?**
   - **A:** All distances are initialized to infinity (`INF = 1000000000`). If `distance[destination]` remains `INF` after Dijkstra's algorithm terminates, no path connects the source to the destination.

10. **Q: Why are connections bidirectional in this project?**
    - **A:** When connecting routers $A$ and $B$, communication can travel both ways. We add $B$ to $A$'s adjacency list, and $A$ to $B$'s adjacency list.
