# NETROUTE SIMULATOR

A clean, object-oriented CLI Network Routing Simulator written in standard C++17. Designed specifically for college Object-Oriented Programming (OOP) and Data Structures & Algorithms coursework, vivas, and practical demonstrations.

---

## 1. Project Overview & Objectives

**NetRoute Simulator** models a computer network of routers connected by bidirectional, weighted communication links. It demonstrates core Object-Oriented Programming (OOP) design patterns and graph algorithms without unnecessary overengineering.

### Primary Objectives:
1. **Object-Oriented Design**: Replace procedural code with an encapsulated, class-based architecture adhering to the Single Responsibility Principle (SRP).
2. **Decoupled Architecture**: Separate core domain network logic and shortest-path calculation from the console user interface and terminal input/output.
3. **Data Integrity & Validation**: Protect internal network state with private members and rigorous input validation against self-connections, duplicate links, non-positive costs, and out-of-range router IDs.
4. **Optimal Pathfinding**: Compute the lowest-cost transmission path using **Dijkstra's Algorithm** with an STL min-priority queue.
5. **Viva Readiness**: Structure code and documentation so that every class, method, and algorithm can be clearly explained and defended in an academic oral examination.

---

## 2. Key Features

- **Dynamic Router Management**: Automatically assigns incremental integer IDs (`1, 2, 3, ...`) upon router creation.
- **Bidirectional Links**: Connects pairs of routers with user-defined positive transmission costs.
- **Strict Validation Rules**:
  - Rejects non-existent router IDs.
  - Rejects self-loops (connecting a router to itself).
  - Rejects zero or negative link costs ($\le 0$).
  - Rejects duplicate connections between already connected routers.
  - Robustly recovers from invalid non-numeric inputs without crashing or entering infinite loops.
- **Complete Topology Visualization**: Displays every router alongside its active connections and transmission costs; highlights isolated routers (`[No connections (Isolated)]`).
- **Shortest Path Computation**: Uses Dijkstra's algorithm to compute the optimal hop sequence and minimum transmission cost.
- **Special Route Handling**:
  - Handles queries where the source equals the destination (returns 0 cost immediately).
  - Detects partitioned or disconnected sub-networks (reports `"No route exists between these routers."`).
- **Separation of Results**: Shortest path queries return a `RouteResult` object, enabling programmatic test verification.

---

## 3. Technologies & C++ Standard

- **Language**: C++17
- **Standard Library Components**:
  - `std::vector` (Dynamic contiguous storage for the adjacency list and path sequences)
  - `std::priority_queue` with `std::greater` (Min-heap for Dijkstra's node selection)
  - `std::pair` (Queue elements holding tentative distance and router ID)
  - `std::algorithm` (`std::reverse` for backtracking reconstructed paths)
  - `std::iostream`, `std::string` (Formatted terminal I/O)
- **Compiler Requirements**: Any standard C++17 compliant compiler (`g++`, `clang++`, or MSVC).
- **External Dependencies**: **None** (Zero third-party libraries required).

---

## 4. Project Architecture & Class Responsibilities

The codebase is organized into modular components with clear separation of responsibilities:

```
NetRouteSimulator/
├── Network.h          # Domain model declarations (Edge, RouteResult, Network class)
├── Network.cpp        # Domain model implementation (Network logic & Dijkstra's algorithm)
├── main.cpp           # Presentation layer (NetworkApp class & main entry point)
├── tests.cpp          # Automated unit test suite verifying all 12 operational scenarios
├── OOP_DESIGN.md      # In-depth OOP architecture document with Mermaid diagrams
└── README.md          # Comprehensive project documentation and viva preparation guide
```

### Component Responsibilities:

| Component | Type | Responsibility |
| :--- | :--- | :--- |
| [`Edge`](file:///f:/DEV/NetRouteSimulator/Network.h) | `struct` | Lightweight Plain Old Data (POD) holder for destination router ID and link cost. |
| [`ConnectionResult`](file:///f:/DEV/NetRouteSimulator/Network.h) | `enum class` | Strongly-typed status returned when connecting routers (`Success`, `InvalidRouterId`, etc.). |
| [`RouteResult`](file:///f:/DEV/NetRouteSimulator/Network.h) | `struct` | Encapsulates shortest-path query outcome (existence, total cost, hop list). |
| [`Network`](file:///f:/DEV/NetRouteSimulator/Network.h) | `class` | **Core Domain Model**: Encapsulates router count and adjacency list. Handles topology changes and Dijkstra's algorithm. Has **no** console I/O dependencies. |
| [`NetworkApp`](file:///f:/DEV/NetRouteSimulator/main.cpp) | `class` | **Presentation / Controller**: Manages the interactive menu loop, input validation, and formatted console output. Composes a `Network` instance. |
| [`main()`](file:///f:/DEV/NetRouteSimulator/main.cpp) | function | Program entry point; instantiates `NetworkApp` and invokes `app.run()`. |

---

## 5. Object-Oriented Principles Implemented

### A. Encapsulation & Data Hiding
All internal data structures representing the network state are declared `private` within `class Network`:

```cpp
class Network {
private:
    int routerCount;                        // Guarded router counter
    std::vector<std::vector<Edge>> adjList; // Adjacency list: adjList[u] contains edges from u
    // ...
};
```
External code cannot directly alter `routerCount` or push malformed edges into `adjList`. All modifications occur strictly through validated public member functions (`addRouter()`, `connectRouters()`).

### B. Class-Based Abstraction
`Network` provides a high-level public interface:
- `addRouter()`
- `connectRouters(router1, router2, cost)`
- `findShortestRoute(source, destination)`

Consumers (such as `NetworkApp` or automated tests) do not need to know whether the graph uses an adjacency list, an adjacency matrix, or how the min-priority queue relaxes edges during Dijkstra's algorithm.

Furthermore, C++ header/implementation separation (`Network.h` vs `Network.cpp`) strictly separates the public contract from the internal algorithm.

### C. Composition
`NetworkApp` demonstrates the **"has-a"** relationship by owning a private instance of `Network`:
```cpp
class NetworkApp {
private:
    Network network; // Composition: NetworkApp owns Network
    // ...
};
```

### D. Why Inheritance and Polymorphism Are Omitted
- **Inheritance** models hierarchical subtyping ("is-a"). In this simulator, all routers are homogeneous, and all communication links follow identical bidirectional rules. Adding artificial parent classes (e.g., `BaseRouter`) would introduce unnecessary cognitive overhead without modeling real variations.
- **Polymorphism** (virtual functions and vtables) enables dynamic runtime method selection. Since there is a single concrete routing algorithm (Dijkstra) and one graph model, static dispatch is faster, cleaner, and adheres to the **YAGNI** (*You Aren't Gonna Need It*) principle.

---

## 6. How the Network is Represented

The network is modeled as an undirected, weighted graph using an **Adjacency List**:

```cpp
struct Edge {
    int to;    // Neighbor router ID
    int cost;  // Non-negative link weight
};

std::vector<std::vector<Edge>> adjList;
```

### Why an Adjacency List?
- **Space Efficiency**: Consumes $\mathcal{O}(V + E)$ memory compared to $\mathcal{O}(V^2)$ required by an adjacency matrix. This makes it optimal for real-world sparse network topologies.
- **Fast Traversal**: During edge relaxation in Dijkstra's algorithm, the algorithm iterates only over existing links (`adjList[u]`) rather than checking all $V$ potential vertices.
- **Natural 1-Based Indexing**: `adjList` is sized to `routerCount + 1`, leaving index 0 unused so that router IDs `1..N` map directly to vector indices without offset subtractions.

---

## 7. How Dijkstra's Algorithm Works

Dijkstra's algorithm computes the shortest path from a single source router to a destination router:

```
[Source] -> (Push tentative cost 0 into Min-Heap)
   │
   ▼
[Extract Min] -> Pop router with minimum tentative distance
   │
   ├─► Already visited? -> Skip
   ├─► Destination reached? -> Early exit
   │
   ▼
[Relax Neighbors] -> For each neighbor:
   │  If (currentCost + linkCost < knownDistance):
   │     Update knownDistance
   │     Record predecessor in parent[]
   │     Push (newDistance, neighbor) into Min-Heap
   ▼
[Reconstruct Path] -> Backtrack from destination to source using parent[]
```

### Min-Priority Queue Implementation:
In C++, `std::priority_queue` is a max-heap by default. We configure it with `std::greater` to create a min-heap:
```cpp
std::priority_queue<std::pair<int, int>,
                    std::vector<std::pair<int, int>>,
                    std::greater<std::pair<int, int>>> pq;
```

---

## 8. Time and Space Complexity

| Operation | Time Complexity | Space Complexity | Explanation |
| :--- | :--- | :--- | :--- |
| **Add Router** | $\mathcal{O}(1)$ amortized | $\mathcal{O}(1)$ | Resizes the outer vector of the adjacency list. |
| **Connect Routers** | $\mathcal{O}(\text{deg}(u))$ | $\mathcal{O}(1)$ | Checks for duplicate connections across existing neighbor edges, then pushes two `Edge` structs. |
| **Display Network** | $\mathcal{O}(V + E)$ | $\mathcal{O}(1)$ auxiliary | Iterates across all vertices and their incident edges. |
| **Find Shortest Route** | $\mathcal{O}((V + E) \log V)$ | $\mathcal{O}(V)$ auxiliary | Standard Dijkstra with min-heap priority queue; maintains `distance`, `visited`, and `parent` arrays. |

*$V$ = number of routers, $E$ = number of links, $\text{deg}(u)$ = degree (number of connections) of router $u$.*

---

## 9. Compilation & Execution Instructions

The project uses only standard C++17 libraries. You can compile using GCC / Clang on Windows, Linux, or macOS.

### Compile the Application:
```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic main.cpp Network.cpp -o NetRouteSimulator
```

### Run the Application:
- **Windows (PowerShell / Command Prompt)**:
  ```powershell
  .\NetRouteSimulator.exe
  ```
- **Linux / macOS**:
  ```bash
  ./NetRouteSimulator
  ```

### Compile & Run the Automated Tests:
```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic tests.cpp Network.cpp -o tests
```
- **Windows**:
  ```powershell
  .\tests.exe
  ```
- **Linux / macOS**:
  ```bash
  ./tests
  ```

---

## 10. Menu Options & Usage Examples

### Main Menu Interface:
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

### Walkthrough Scenario:
1. **Add Routers**:
   - Option 1 $\rightarrow$ Router 1 added.
   - Option 1 $\rightarrow$ Router 2 added.
   - Option 1 $\rightarrow$ Router 3 added.
2. **Connect Links**:
   - Connect 1 and 2 with cost 4.
   - Connect 2 and 3 with cost 5.
   - Connect 1 and 3 with cost 12.
3. **Display Topology (Option 3)**:
   ```text
   ========================================
              NETWORK TOPOLOGY
   ========================================
   Router 1 -> Router 2 (Cost: 4), Router 3 (Cost: 12)
   Router 2 -> Router 1 (Cost: 4), Router 3 (Cost: 5)
   Router 3 -> Router 2 (Cost: 5), Router 1 (Cost: 12)
   ========================================
   ```
4. **Compute Shortest Path from Router 1 to Router 3 (Option 4)**:
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
   *(Note: Dijkstra correctly selects the multi-hop path $1 \rightarrow 2 \rightarrow 3$ with cost 9 instead of the direct link of cost 12).*

---

## 11. Input Validation & Error Handling

- **Non-Numeric Console Input**: If a user enters non-numeric text (e.g., `abc`), `NetworkApp::readInt` detects `std::cin.fail()`, clears the stream flag, consumes the invalid token, and warns the user without crashing or entering an infinite loop.
- **Out-of-Range Router IDs**: Attempting to connect or query router IDs $< 1$ or $> \text{routerCount}$ produces:
  `Error: One or both router IDs do not exist.`
- **Self-Loops**: Attempting to connect a router to itself yields:
  `Error: Cannot connect a router to itself.`
- **Non-Positive Cost**: Negative numbers or zero are rejected:
  `Error: Link cost must be a positive integer.`
- **Duplicate Connections**: Attempting to add an edge between two already connected routers outputs:
  `Error: A connection already exists between router X and router Y.`
- **Unreachable Routes**: If routers are in disconnected components, the simulator outputs:
  `No route exists between these routers.`

---

## 12. Automated Verification & Testing

The project includes an automated test harness in [`tests.cpp`](file:///f:/DEV/NetRouteSimulator/tests.cpp) covering all critical scenarios:

1. **Add Multiple Routers**: Validates incremental ID assignment and router count tracking.
2. **Valid Bidirectional Connections**: Confirms adjacency symmetric entries for both endpoints.
3. **Out-of-Range Router Rejection**: Confirms rejection for ID 0 and large IDs.
4. **Self-Loop Prevention**: Verifies self-connections are blocked.
5. **Non-Positive Cost Rejection**: Verifies costs of 0 and negative values are rejected.
6. **Duplicate Connection Prevention**: Verifies both forward and reverse duplicate connection attempts are rejected.
7. **Isolated Router Detection**: Checks routers without connections are correctly identified.
8. **Multi-Hop Shortest Path**: Verifies correct hop sequence and cost across intermediate routers.
9. **Alternative Path Optimization**: Confirms lower-cost multi-hop paths are preferred over direct higher-cost links.
10. **Trivial Path Handling**: Verifies 0 cost when source equals destination.
11. **Partitioned / Disconnected Routers**: Verifies safe handling and empty path when no route exists.
12. **Invalid Query Handling**: Verifies safe rejection of invalid source/destination IDs.

---

## 13. Limitations & Future Improvements

- **Dynamic Link Removal**: Currently, connections cannot be removed or updated after creation. A future enhancement could introduce `disconnectRouters(u, v)` and `updateLinkCost(u, v, newCost)`.
- **Directed Links**: The current model assumes symmetric bidirectional links. Support for directed / asymmetric links (e.g., upload vs download speeds) could be added.
- **Negative Edge Weights**: Dijkstra's algorithm does not support negative weights. Integrating the Bellman-Ford algorithm would allow handling graphs with negative edge costs.
- **Dynamic Routing Protocols**: Modeling real-world protocols such as OSPF (Open Shortest Path First) or BGP (Border Gateway Protocol) with dynamic link-state announcements.

---

## 14. College Viva Questions & Answers

### Q1: What makes this implementation Object-Oriented compared to the original code?
**A:** The original code relied on global variables (`routerCount`, `adjList`) and procedural functions that intermingled graph algorithms with console printing. The refactored code encapsulates network state inside the `Network` class with `private` members, exposes a clean public interface, uses typed result objects (`RouteResult`, `ConnectionResult`), and separates UI concerns into `NetworkApp`.

### Q2: Why is `Edge` defined as a `struct` instead of a `class`?
**A:** In C++, `struct` members are public by default. `Edge` is a Plain Old Data (POD) record holding only destination ID and cost with no invariants to protect. Wrapping it in a `class` with private fields and trivial getters/setters would add boilerplate without providing any encapsulation benefit.

### Q3: Why didn't you create a separate `Router` class?
**A:** In this simulation, routers have no unique behavior or state other than an integer ID (`1, 2, ..., N`). Creating a `class Router` would either be a pointless one-line wrapper around an integer or an unnecessary heap-allocated container that harms cache locality without adding any architectural benefit.

### Q4: Why are inheritance and polymorphism not used?
**A:** Inheritance models "is-a" relationships where distinct subtypes exist. In this simulator, all routers and links are homogeneous. Adding abstract classes or virtual methods would introduce vtable overhead and artificial complexity, violating the YAGNI principle.

### Q5: How does Dijkstra's algorithm work with an STL priority queue?
**A:** C++ `std::priority_queue` is a max-heap by default. By declaring it as `std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<pair<int, int>>>`, it operates as a min-heap, allowing $\mathcal{O}(\log V)$ extraction of the unvisited router with the minimum tentative distance.

### Q6: What is edge relaxation in Dijkstra's algorithm?
**A:** Relaxation is the step where we check if traversing through the current router $u$ provides a shorter path to neighbor $v$ than previously known:
$$\text{if } (\text{distance}[u] + \text{cost} < \text{distance}[v]) \implies \text{distance}[v] = \text{distance}[u] + \text{cost}$$

### Q7: How is the shortest path reconstructed after Dijkstra finishes?
**A:** We maintain a `parent` array where `parent[v] = u` records that router $u$ is the immediate predecessor of router $v$ on the shortest path. Once the destination is reached, we backtrack from destination to source using `parent`, and then reverse the vector using `std::reverse`.

### Q8: What is the time and space complexity of Dijkstra's algorithm in this project?
**A:** Time complexity is $\mathcal{O}((V + E) \log V)$ using a binary min-priority queue. Space complexity is $\mathcal{O}(V + E)$ for the adjacency list and $\mathcal{O}(V)$ auxiliary memory for the `distance`, `visited`, and `parent` arrays.

### Q9: How is invalid input handled when a user enters non-numeric characters?
**A:** `NetworkApp::readInt` checks `std::cin >> value`. If it fails, `cin.fail()` is triggered, `std::cin.clear()` resets the stream error state, and a dummy string consumes the invalid characters, preventing terminal lockup.

### Q10: How does the system detect unreachable routers?
**A:** All distances are initialized to a sentinel infinity value (`INF = 1,000,000,000`). If `distance[destination]` remains `INF` after Dijkstra's algorithm finishes, no connected path exists between source and destination, and `RouteResult.routeExists` is set to `false`.
