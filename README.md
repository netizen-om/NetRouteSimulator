# NETROUTE SIMULATOR

A lightweight, clean Object-Oriented C++ Network Routing Simulator written in standard C++17. Designed specifically for college Object-Oriented Programming (OOP) and Data Structures demonstrations, vivas, and lab examinations.

---

## 1. Project Overview

**NetRoute Simulator** models a computer network of routers connected by bidirectional, weighted communication links. It allows users to:
1. Dynamically add routers with automatically assigned incremental integer IDs (`1, 2, 3, ...`).
2. Establish communication links between routers with custom positive costs.
3. Remove routers and automatically purge all associated connections across the network.
4. Visualize the entire network topology, highlighting both connected and isolated routers.
5. Calculate and display the optimal shortest path and minimum transmission cost between any two routers using **Dijkstra's Algorithm**.
6. Verify global network connectivity using **Breadth-First Search (BFS)** to determine if all routers can communicate with one another.

The project demonstrates core Object-Oriented Programming (OOP) principles—including **Classes & Objects**, **Encapsulation**, **Data Hiding**, **Friendship (`friend class`)**, **Composition**, and **Abstraction**—while remaining simple, readable, and free of unnecessary enterprise boilerplate.

---

## 2. Interactive CLI Menu

The application provides an intuitive command-line interface:

```text
========================================
          NETROUTE SIMULATOR
========================================
1. Add Router
2. Connect Routers
3. Remove Router
4. Display Network
5. Find Shortest Route
6. Check Network Connectivity
0. Exit
========================================
Enter your choice: 
```

---

## 3. Features & Validation Rules

- **Option 1: Add Router**
  - Instantiates a new `Router` object with an auto-incremented ID starting at `1`.
  - Appends the router to the network's internal collection (`vector<Router>`).
  - Confirms addition: `Router added successfully with ID: <id>`.

- **Option 2: Connect Routers**
  - Prompts for two router IDs and a link transmission cost.
  - Links are bidirectional (traffic flows both ways).
  - **Validation checks:**
    - Rejects IDs if either router does not exist (`Error: One or both router IDs do not exist.`).
    - Rejects self-loops (`Error: Cannot connect a router to itself.`).
    - Rejects non-positive link costs ($\le 0$) (`Error: Link cost must be positive.`).
    - Rejects duplicate connections between already connected routers (`Error: Connection already exists.`).

- **Option 3: Remove Router**
  - Prompts for the router ID to remove.
  - Validates that the router exists in the network.
  - Automatically cleans up and deletes all incoming and outgoing connections referencing this router across all other routers using `std::remove_if`.
  - Removes the router instance from the network.

- **Option 4: Display Network**
  - Prints the complete network topology router by router.
  - Displays each router ID and its list of connected neighbors with link costs.
  - Clearly identifies isolated routers (`[No connections (Isolated)]`).
  - Warns if the network is empty.

- **Option 5: Find Shortest Route**
  - Prompts for source and destination router IDs.
  - Validates that both router IDs exist.
  - Handles the case where source equals destination directly (Path: `source`, Cost: `0`).
  - Executes **Dijkstra's Algorithm** using an STL min-priority queue (`std::priority_queue` with `greater<pair<int, int>>`).
  - Prints the sequence of router hops (e.g., `1 -> 2 -> 3`) and the total cumulative cost.
  - Detects unreachable destinations (`No route exists between these routers.`).

- **Option 6: Check Network Connectivity**
  - Evaluates whether the entire network forms a single connected component using **Breadth-First Search (BFS)** starting from the first router.
  - Traverses all reachable routers and counts visited nodes.
  - Reports `CONNECTED` if all routers are reachable from each other, or `DISCONNECTED` if partitions/isolated subnets exist.

- **Option 0: Exit**
  - Gracefully terminates the program.

---

## 4. Object-Oriented Architecture

The project is modularized into dedicated header, implementation, and test input files:

```text
NetRouteSimulator/
├── Router.h                 # Edge struct and Router class declaration
├── Router.cpp               # Router class implementation
├── Network.h                # Network class declaration
├── Network.cpp              # Network class implementation (graph logic & algorithms)
├── main.cpp                 # CLI driver and interactive menu loop
├── OOP_DESIGN.md            # Detailed OOP architectural specification
├── README.md                # Comprehensive project documentation & viva guide
└── test_inputs/             # Predefined automated CLI input test files
    ├── basic.txt            # Basic demonstration scenario
    ├── all_operations.txt   # Comprehensive coverage of every menu option
    ├── edge_cases.txt       # Robustness, boundary conditions, and validation errors
    ├── connectivity.txt     # Graph connectivity transitions (connected/partitioned)
    └── remove_router.txt    # Router deletion, edge cleanup, and dynamic rerouting
```

### Class Diagram:

```mermaid
classDiagram
    direction TB

    class Edge {
        +int to
        +int cost
    }

    class Router {
        -int routerID
        -vector~Edge~ connections
        +Router(int id)
        +int getRouterID() const
        +const vector~Edge~& getConnections() const
    }

    class Network {
        -vector~Router~ routers
        -int nextRouterID
        -Router* findRouter(int id)
        +Network()
        +void addRouter()
        +void connectRouters()
        +void removeRouter()
        +void displayNetwork()
        +void findShortestRoute()
        +void checkConnectivity()
    }

    Router *-- Edge : "stores outgoing edges"
    Network *-- Router : "manages collection of routers"
    Network ..> Router : "friend class (direct connection access)"
```

### Component Breakdown:

1. **`struct Edge`** (`Router.h`):
   - Represents a directed connection endpoint with `to` (neighbor router ID) and `cost` (link weight).
   - Designed as a lightweight Plain Old Data (POD) struct.

2. **`class Router`** (`Router.h`, `Router.cpp`):
   - Encapsulates an individual router entity.
   - Private attributes: `routerID` and `connections` (`vector<Edge>`).
   - Public accessors: `getRouterID()` and `getConnections()`.
   - Grants friendship to `Network` (`friend class Network;`), allowing the network controller to manage links directly while shielding them from external modification.

3. **`class Network`** (`Network.h`, `Network.cpp`):
   - Manages the overall network state and graph operations.
   - Private attributes: `routers` (`vector<Router>`) and `nextRouterID`.
   - Private helper: `Router* findRouter(int id)` for ID lookup.
   - Public methods: `addRouter()`, `connectRouters()`, `removeRouter()`, `displayNetwork()`, `findShortestRoute()`, and `checkConnectivity()`.

4. **`main()`** (`main.cpp`):
   - Creates a single `Network` instance and drives the user interface via a `do-while` menu loop.

---

## 5. OOP Principles Explained

| Principle | Implementation in NetRoute Simulator | Code Location |
| :--- | :--- | :--- |
| **Classes & Objects** | Dedicated `Router` and `Network` classes model physical network entities and network management. | `Router.h`, `Network.h` |
| **Encapsulation** | State variables (`routerID`, `connections`, `routers`, `nextRouterID`) are `private`. External access occurs strictly through public methods. | `Router.h`, `Network.h` |
| **Data Hiding** | Internal representation (vectors, pointers, algorithmic state) is concealed from outside callers. | `Router.h`, `Network.h` |
| **Friend Class** | `Router` declares `friend class Network;`. This permits `Network` to modify connection lists directly during link creation and deletion without exposing public mutators to the rest of the application. | `Router.h` |
| **Composition** | `Network` contains a collection of `Router` objects (`vector<Router>`), and each `Router` contains a collection of `Edge` objects (`vector<Edge>`). | `Network.h`, `Router.h` |
| **Abstraction** | `main()` interacts solely through clean, high-level commands (`network.addRouter()`, `network.findShortestRoute()`, etc.) without knowing Dijkstra or BFS implementation details. | `main.cpp` |
| **Separation of Concerns** | `main()` handles I/O dispatch, `Network` handles topology and algorithms, and `Router` encapsulates router-specific data. | `main.cpp`, `Network.cpp`, `Router.cpp` |

### Why Inheritance and Polymorphism Are Not Needed
- **Inheritance** models an "is-a" relationship (e.g., `CoreRouter` vs `EdgeRouter`). In this simulator, all routers adhere to identical rules and behavior.
- **Polymorphism** (virtual functions) enables varying runtime behavior for different subclasses. Because there is a single routing algorithm (Dijkstra) and uniform router nodes, virtual tables would introduce unnecessary complexity without functional value.

---

## 6. Algorithms Used

### A. Dijkstra's Algorithm (Shortest Route)
Used in `Network::findShortestRoute()` to compute the minimum transmission cost path between two routers:
1. **Initialization:** Tentative distances to all routers are initialized to infinity (`INF = 1,000,000,000`), with `distance[source] = 0`.
2. **Min-Priority Queue:** A min-heap (`priority_queue` with `std::greater`) stores `(currentCost, routerID)` pairs.
3. **Relaxation:** The router with the lowest tentative distance is extracted. For each outgoing edge to neighbor `v` with cost `w`:
   $$\text{if } \text{distance}[u] + w < \text{distance}[v] \implies \text{distance}[v] = \text{distance}[u] + w$$
4. **Path Reconstruction:** A `parent` vector tracks predecessor nodes. Once the destination is reached, the path is reconstructed by backtracking from destination to source and reversing the result.

### B. Breadth-First Search (Network Connectivity)
Used in `Network::checkConnectivity()` to verify graph completeness:
1. Starts from the first router in the `routers` list.
2. Uses a `std::queue<int>` and a `visited` vector to traverse all reachable routers level by level.
3. Counts total reached routers; if $\text{count} == \text{routers.size()}$, the network is reported as **CONNECTED**; otherwise, it is **DISCONNECTED**.

---

## 7. Time and Space Complexity

| Operation | Time Complexity | Auxiliary Space Complexity | Explanation |
| :--- | :--- | :--- | :--- |
| **Add Router** | $\mathcal{O}(1)$ amortized | $\mathcal{O}(1)$ | Vector push-back with integer ID assignment. |
| **Connect Routers** | $\mathcal{O}(\text{deg}(u) + \text{deg}(v))$ | $\mathcal{O}(1)$ | ID lookups and linear scan to prevent duplicate links. |
| **Remove Router** | $\mathcal{O}(V + E)$ | $\mathcal{O}(1)$ | Purges links referencing the ID from all routers, then erases the router. |
| **Display Network** | $\mathcal{O}(V + E)$ | $\mathcal{O}(1)$ | Traverses every router and every outgoing edge once. |
| **Find Shortest Route** | $\mathcal{O}((V + E) \log V)$ | $\mathcal{O}(V)$ | Min-priority queue edge relaxation (Dijkstra). |
| **Check Connectivity** | $\mathcal{O}(V + E)$ | $\mathcal{O}(V)$ | Standard queue-based BFS traversal. |

*$V$ = number of routers, $E$ = number of bidirectional links, $\text{deg}(u)$ = degree (number of neighbors) of router $u$.*

---

## 8. Compilation & Execution Instructions

The project uses standard C++17 and standard library headers.

### Compile:
```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic main.cpp Network.cpp Router.cpp -o NetRouteSimulator
```

### Run Interactively on Windows:
```powershell
.\NetRouteSimulator.exe
```

### Run Interactively on Linux / macOS:
```bash
./NetRouteSimulator
```

---

## 9. Automated Testing & Verification

### Running Predefined Test Scenarios

The repository includes a dedicated `test_inputs/` directory containing predefined CLI test files. These allow you to execute, verify, and demonstrate all simulator features during viva examinations and demonstrations with a single command, eliminating the need to type inputs manually.

#### How Input Redirection Works
The standard C++ `cin >>` statement reads data sequentially from standard input (`stdin`). By using **input redirection** (`<` in Command Prompt / Linux, or pipe `Get-Content ... |` in PowerShell), the operating system feeds the content of a text file directly into `stdin` just as if a user typed it at the keyboard. The program consumes each menu option and its corresponding parameters, executes the logic, and terminates cleanly when it encounters choice `0` (Exit).

> **Note:** Input redirection requires **zero modifications** to the C++ code. The existing implementation is tested in its exact, original form.

#### What `test_inputs/` Contains

| Test File | Scenario Description | Features & Behavior Verified |
| :--- | :--- | :--- |
| **`basic.txt`** | Basic viva demonstration | Adds 3 routers, creates bidirectional links (costs 4, 5, 12), displays topology, calculates shortest path between 1 and 3 ($1 \to 2 \to 3$ with cost 9 instead of direct 12), checks network connectivity (`CONNECTED`), and exits cleanly. |
| **`all_operations.txt`** | Full menu coverage | Systematically tests every menu choice ($1, 2, 3, 4, 5, 6, 0$). Builds a 4-router chain, verifies route and connectivity, removes router 3, displays updated topology with isolated router 4, checks connectivity transition to `DISCONNECTED`, and verifies that path from 1 to 4 is now unreachable. |
| **`edge_cases.txt`** | Error handling & boundaries | Validates error recovery on: empty network queries (display, connectivity, removal, route), invalid menu choices, non-existent router IDs, self-loops ($1 \to 1$), zero link cost ($\le 0$), negative link cost ($-5$), duplicate link creation, reverse duplicate link creation, identical source/destination shortest path ($1 \to 1$, cost 0), and unreachable disconnected router paths. |
| **`connectivity.txt`** | Connectivity lifecycle (BFS) | Tests BFS graph connectivity transitions: empty network $\to$ single router (`CONNECTED`) $\to$ two isolated routers (`DISCONNECTED`) $\to$ linked pair (`CONNECTED`) $\to$ two partitioned subnets `{1,2}` and `{3,4}` (`DISCONNECTED`) $\to$ bridge link between components (`CONNECTED`). |
| **`remove_router.txt`** | Node removal & dynamic rerouting | Builds a 4-router mesh topology, computes baseline shortest path $1 \to 2 \to 3$ (cost 7), removes transit router 2, verifies edge purging and dynamic rerouting via router 4 ($1 \to 4 \to 3$, cost 12), then removes router 4 to verify isolated nodes and path failure. |

#### How to Compile the Project
Before running any test scenario, compile the program once from the project root:

```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic main.cpp Network.cpp Router.cpp -o NetRouteSimulator
```

#### How to Run Each Scenario Using a Single Command

**Windows PowerShell (VS Code Terminal / PowerShell):**
```powershell
Get-Content test_inputs/basic.txt | .\NetRouteSimulator.exe
Get-Content test_inputs/all_operations.txt | .\NetRouteSimulator.exe
Get-Content test_inputs/edge_cases.txt | .\NetRouteSimulator.exe
Get-Content test_inputs/connectivity.txt | .\NetRouteSimulator.exe
Get-Content test_inputs/remove_router.txt | .\NetRouteSimulator.exe
```

**Windows Command Prompt (`cmd.exe`):**
```cmd
NetRouteSimulator.exe < test_inputs\basic.txt
NetRouteSimulator.exe < test_inputs\all_operations.txt
NetRouteSimulator.exe < test_inputs\edge_cases.txt
NetRouteSimulator.exe < test_inputs\connectivity.txt
NetRouteSimulator.exe < test_inputs\remove_router.txt
```

**Linux / macOS (Terminal):**
```bash
./NetRouteSimulator < test_inputs/basic.txt
./NetRouteSimulator < test_inputs/all_operations.txt
./NetRouteSimulator < test_inputs/edge_cases.txt
./NetRouteSimulator < test_inputs/connectivity.txt
./NetRouteSimulator < test_inputs/remove_router.txt
```

#### How to Create Additional Test Files
To create custom test scenarios for specific viva questions or test cases:
1. Create a new `.txt` file inside `test_inputs/` (e.g., `test_inputs/custom_scenario.txt`).
2. Add numbers matching the menu sequence:
   - To add a router, enter `1`.
   - To connect routers, enter `2`, followed by `<router1> <router2> <cost>` (e.g., `1 2 5`).
   - To remove a router, enter `3`, followed by `<routerID>` (e.g., `2`).
   - To display the network, enter `4`.
   - To query the shortest route, enter `5`, followed by `<sourceID> <destID>` (e.g., `1 3`).
   - To check connectivity, enter `6`.
   - Conclude every test file with `0` so the program exits cleanly without hanging.
3. Execute your scenario:
   ```powershell
   Get-Content test_inputs/custom_scenario.txt | .\NetRouteSimulator.exe
   ```

---

## 10. Viva Questions & Answers

1. **Q: What OOP concepts are implemented in this project?**
   - **A:** 
     - **Classes & Objects:** Separate `Router` and `Network` classes.
     - **Encapsulation & Data Hiding:** Data members (`routerID`, `connections`, `routers`, `nextRouterID`) are `private`.
     - **Friendship:** `Router` declares `friend class Network;` so `Network` can modify connections directly while protecting them from external misuse.
     - **Composition:** `Network` owns `Router`s, and `Router` owns `Edge`s.
     - **Abstraction:** `main()` interacts solely through high-level methods.

2. **Q: Why is `Edge` defined as a `struct` while `Router` is a `class`?**
   - **A:** `Edge` is a passive Plain Old Data (POD) record with just two integer fields (`to` and `cost`) and no internal invariants. `Router` encapsulates state (`routerID` and internal connection list), requires access control, and maintains invariants, justifying a full C++ `class`.

3. **Q: What is the purpose of `friend class Network;` in `Router`?**
   - **A:** It grants `Network` controlled access to modify `Router`'s private `connections` vector (e.g., during `connectRouters()` and `removeRouter()`) without exposing public setter methods that could compromise encapsulation.

4. **Q: How does `removeRouter()` prevent dangling edges?**
   - **A:** Before deleting the router object itself from `routers`, `removeRouter()` iterates over all remaining routers and uses `std::remove_if` to erase any edge where `edge.to == id`. This ensures no other router retains a connection to the removed router.

5. **Q: How does the network connectivity check work?**
   - **A:** It performs a Breadth-First Search (BFS) starting from the first router. If the count of reachable nodes equals the total number of routers in `routers`, the graph is fully connected. If it is less, the network contains partitioned or isolated components.

6. **Q: Why is `priority_queue` configured with `greater<pair<int, int>>` in Dijkstra's algorithm?**
   - **A:** By default, C++ `std::priority_queue` is a max-heap (extracts the largest element). Passing `greater<pair<int, int>>` transforms it into a min-heap so the node with the smallest tentative path cost is extracted first.

7. **Q: Why are inheritance and virtual functions not used in this project?**
   - **A:** All routers share the same behavior, attributes, and routing logic. Adding inheritance or polymorphism would introduce unnecessary complexity and overhead without any design benefit, adhering to the YAGNI (*You Aren't Gonna Need It*) principle.

8. **Q: How does the program detect that no path exists between two routers?**
   - **A:** All router tentative distances are initialized to infinity (`INF = 1000000000`). If `distance[destination]` remains `INF` after Dijkstra's algorithm completes, no connecting route exists.
