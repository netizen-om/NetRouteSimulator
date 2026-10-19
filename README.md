# NetRoute Simulator

### CLI-Based Network Routing Simulator in C++

---

## Table of Contents
1. [Project Overview](#project-overview)
2. [Project Objectives](#project-objectives)
3. [Features](#features)
4. [Technologies Used](#technologies-used)
5. [Project Directory Structure](#project-directory-structure)
6. [Architecture & Class Responsibilities](#architecture--class-responsibilities)
7. [How the Network is Represented](#how-the-network-is-represented)
8. [How Dijkstra's Algorithm Works](#how-dijkstras-algorithm-works)
9. [Packet Simulation](#packet-simulation)
10. [Requirements & Installation](#requirements--installation)
11. [Compilation Instructions](#compilation-instructions)
12. [Running the Application](#running-the-application)
13. [Usage Walkthrough](#usage-walkthrough)
14. [Example Output](#example-output)
15. [STL Containers Used](#stl-containers-used)
16. [Complexity Analysis](#complexity-analysis)
17. [Testing](#testing)
18. [Limitations](#limitations)
19. [Future Improvements](#future-improvements)
20. [Author & Academic Use](#author--academic-use)

---

## Project Overview

**NetRoute Simulator** is an interactive, command-line computer network simulation developed in modern C++17. It models a network topology consisting of routers connected by weighted bidirectional links, computes optimal forwarding paths using Dijkstra's shortest-path algorithm, and simulates hop-by-hop packet transmissions.

> **Important Note:** This software is a logical discrete-event simulation designed for educational purposes. It does not open raw network sockets, interact with operating system routing tables, or transmit physical packets over the internet.

---

## Project Objectives

This project was developed for an undergraduate Object-Oriented Programming (OOP) curriculum to demonstrate:
- **Object-Oriented Design (OOD):** Designing discrete, single-responsibility classes (`Router`, `Network`, `RoutingEngine`, `Packet`, `PacketSimulator`, `NetworkSimulator`) with strict encapsulation and clean abstractions.
- **Graph Modeling:** Modeling dynamic networks using adjacency lists backed by standard associative containers.
- **Algorithm Implementation:** Applying Dijkstra's algorithm with a min-priority queue and predecessor mapping to solve single-source shortest path problems.
- **Standard Template Library (STL):** Utilizing modern C++ containers (`std::vector`, `std::unordered_map`, `std::priority_queue`, `std::pair`) and algorithms (`std::sort`, `std::remove_if`, `std::reverse`).
- **Modular Software Engineering:** Structuring a multi-file project with separated interfaces (`include/*.h`) and implementations (`src/*.cpp`), proper header inclusion rules, and zero circular dependencies.

---

## Features

- **Router Management:**
  - Add routers with unique alphanumeric identifiers.
  - Remove routers with automatic cascading cleanup of all incident links (no dangling links).
  - Toggle router operational status (`ACTIVE` vs. `INACTIVE`) to model hardware/interface downtime.
- **Connection & Topology Control:**
  - Establish bidirectional weighted links with configurable positive integer costs.
  - Reject invalid operations: self-links ($A \leftrightarrow A$), non-positive costs ($\le 0$), duplicate links, and links referencing nonexistent routers.
  - Disconnect existing links with validation.
  - Display alphabetical, human-readable adjacency lists showing neighbors, link costs, and operational states.
- **Dijkstra Shortest-Path Routing:**
  - Compute the lowest-cost path between any two active routers.
  - Track complete router-by-router trajectory, total accumulated cost, and edge hop count.
  - Automatic rerouting around inactive nodes or broken connections.
  - Graceful reporting of unreachable destinations and nonexistent endpoints.
- **Single-Source Routing Table Generation:**
  - Compute minimum costs and immediate next hops from a selected source router to all other nodes in the network in a single Dijkstra traversal.
  - Clearly display `Local (Self)`, `Reachable`, or `UNREACHABLE` status.
- **Packet Transmission Simulation:**
  - Generate simulated packets with monotonic, auto-incrementing tracking IDs starting at `1001`.
  - Simulate hop-by-hop forwarding visualization (`A -> C`, `C -> B`, `B -> D`).
  - Track packet states across their lifecycle: `CREATED`, `IN_TRANSIT`, `DELIVERED`, and `FAILED`.
  - Record the actual path followed and failure diagnostics (e.g., `"No route available"`).
- **Packet History & Metrics:**
  - Chronological history of all delivered and failed transmissions.
  - Comprehensive network statistics: router count, unique link count, packet delivery success rate, and average hop count with division-by-zero protection.
- **Demonstration Benchmark Network:**
  - Built-in option to load a standardized 4-node topology ($A, B, C, D$) to demonstrate cost versus hop-count optimization.

---

## Technologies Used

- **Language:** Standard C++17 (ISO/IEC 14882:2017)
- **Standard Library:** Containers (`<vector>`, `<unordered_map>`, `<queue>`, `<string>`, `<utility>`), Algorithms (`<algorithm>`), I/O & Formatting (`<iostream>`, `<iomanip>`), and Limits (`<limits>`)
- **Compiler:** `g++` (GCC) with `-Wall -Wextra -pedantic` flags
- **Interface:** Interactive Console / Command-Line Interface (CLI)

---

## Project Directory Structure

```
NetRouteSimulator/
├── include/
│   ├── Network.h           # Network class declaration and OperationResult struct
│   ├── NetworkSimulator.h  # Application controller and CLI menu coordinator
│   ├── Packet.h            # Packet class declaration and PacketStatus enum
│   ├── PacketSimulator.h   # Packet lifecycle, forwarding engine, and metrics
│   ├── Router.h            # Router node class declaration
│   └── RoutingEngine.h     # RoutingEngine declaration and RouteResult struct
├── src/
│   ├── main.cpp            # Minimal entry point (instantiates and runs controller)
│   ├── Network.cpp         # Graph storage, connection validation, topology display
│   ├── NetworkSimulator.cpp# Menu loop, user I/O, and input validation utilities
│   ├── Packet.cpp          # Packet member definitions and status-to-string conversion
│   ├── PacketSimulator.cpp # Packet forwarding simulation, history, and statistics
│   ├── Router.cpp          # Router accessors and state mutators
│   └── RoutingEngine.cpp   # Dijkstra's algorithm and routing table generation
├── README.md               # Complete project documentation
└── .gitignore              # Git ignore rules for build artifacts and executables
```

### File Responsibilities

| File | Type | Responsibility |
| :--- | :--- | :--- |
| `include/Router.h` / `src/Router.cpp` | Class | Represents a network node; encapsulates router ID and operational state (`active_`). |
| `include/Network.h` / `src/Network.cpp` | Class | Maintains graph topology (nodes and adjacency list); enforces link validation and graph integrity. |
| `include/RoutingEngine.h` / `src/RoutingEngine.cpp` | Class | Algorithmic engine; executes Dijkstra's shortest-path search and single-source routing table generation. |
| `include/Packet.h` / `src/Packet.cpp` | Class | Entity modeling a data packet; tracks ID, endpoints, actual path, status, and cost. |
| `include/PacketSimulator.h` / `src/PacketSimulator.cpp` | Class | Manages packet generation, hop-by-hop transmission simulation, history storage, and metrics. |
| `include/NetworkSimulator.h` / `src/NetworkSimulator.cpp` | Class | Top-level coordinator; handles menu display, input parsing, error handling, and delegation. |
| `src/main.cpp` | Entry | Minimal executable entry point containing only `main()`. |

---

## Architecture & Class Responsibilities

The application follows the **Single Responsibility Principle (SRP)**. Subsystems are loosely coupled through clean public interfaces:

```
                      ┌──────────────────────┐
                      │     src/main.cpp     │
                      └──────────┬───────────┘
                                 │
                                 ▼
                     ┌────────────────────────┐
                     │    NetworkSimulator    │  (UI Controller & Menu Loop)
                     └───┬────────┬────────┬──┘
                         │        │        │
         ┌───────────────┘        │        └──────────────┐
         ▼                        ▼                       ▼
  ┌─────────────┐       ┌───────────────────┐    ┌─────────────────┐
  │   Network   │       │   RoutingEngine   │    │ PacketSimulator │
  └──────┬──────┘       └─────────┬─────────┘    └────────┬────────┘
         │                        │                       │
         ▼                        │                       ▼
  ┌─────────────┐                 │                 ┌───────────┐
  │   Router    │                 │                 │  Packet   │
  └─────────────┘                 │                 └───────────┘
         ▲                        │
         └────────────────────────┴───────────────────────┘
           (RoutingEngine & PacketSimulator query Network)
```

### Key OOP Principles Demonstrated
- **Encapsulation:** Class attributes (`routers_`, `adjacencyList_`, `packetHistory_`, `id_`, `active_`) are `private`. External classes cannot mutate data structures directly; all mutations occur through validated member methods (`addRouter`, `connectRouters`, `recordHop`).
- **Separation of Concerns:**
  - `Network` knows *how to store and validate the graph*, but has no awareness of console menus, Dijkstra's algorithm, or packet forwarding.
  - `RoutingEngine` knows *how to compute shortest paths*, but does not store graph data or print menus.
  - `PacketSimulator` knows *how to simulate transmission and log statistics*, but delegates route discovery to `RoutingEngine`.
  - `NetworkSimulator` manages *user interaction and input sanitization*, delegating work to domain classes.

---

## How the Network is Represented

The network is modeled as a weighted undirected graph using an **Adjacency List** stored in:
```cpp
std::unordered_map<std::string, std::vector<std::pair<std::string, int>>> adjacencyList_;
```

### Structure:
- **Key (`std::string`):** The unique ID of the source router (e.g., `"A"`).
- **Value (`std::vector<std::pair<std::string, int>>`):** A list of neighbor connections. Each `pair` stores:
  - `first` (`std::string`): Neighbor router ID.
  - `second` (`int`): Link traversal cost / weight.

### Example Graph:
```
       (4)
  [A]-------[B]
   |  \     /|
(2)|   (1) / |(3)
   |    \ /  |
  [C]----+  [D]
   |         |
   +---(7)---+
```

### Representation in Memory:
```
Router A -> [ (B, 4), (C, 2) ]
Router B -> [ (A, 4), (C, 1), (D, 3) ]
Router C -> [ (A, 2), (B, 1), (D, 7) ]
Router D -> [ (B, 3), (C, 7) ]
```

When connecting $A$ and $B$ with cost $4$, bidirectional entries are created: $(B, 4)$ is added to $A$'s list, and $(A, 4)$ is added to $B$'s list.

---

## How Dijkstra's Algorithm Works

Dijkstra's algorithm finds the shortest path between a source router and destinations in a weighted graph with non-negative edge weights.

### Step-by-Step Procedure:
1. **Endpoint Validation:** Verify that both source and destination routers exist in the network and are currently `ACTIVE`.
2. **Initialization:**
   - Initialize tentative distances: $\text{dist}[u] = \infty$ for all $u \in V$.
   - Set $\text{dist}[\text{source}] = 0$.
   - Initialize an empty predecessor map: $\text{predecessor}[u] = \text{None}$.
3. **Priority Queue Population:**
   - Push $(\text{cost} = 0, \text{node} = \text{source})$ into a min-priority queue (`std::priority_queue` configured with `std::greater`).
4. **Iterative Extraction & Relaxation:**
   - Pop the router $u$ with the lowest tentative cost $(d, u)$.
   - **Stale Entry Check:** If $d > \text{dist}[u]$, discard this entry (handles duplicate queue insertions in $O(1)$).
   - **Early Exit:** If $u == \text{destination}$, stop further exploration.
   - For each neighbor $v$ of $u$ with link cost $w$:
     - Check if $v$ is `ACTIVE`; skip if inactive.
     - Calculate $\text{newDist} = \text{dist}[u] + w$.
     - If $\text{newDist} < \text{dist}[v]$:
       - Update $\text{dist}[v] = \text{newDist}$.
       - Update $\text{predecessor}[v] = u$.
       - Push $(\text{newDist}, v)$ into the priority queue.
5. **Path Reconstruction:**
   - If $\text{dist}[\text{destination}] == \infty$, return `found = false`.
   - Otherwise, trace backward from $\text{destination}$ through $\text{predecessor}$ until $\text{source}$ is reached.
   - Reverse the sequence to produce the source-to-destination path.
   - Hop count is calculated as $\text{path.size}() - 1$.

### Why Non-Negative Weights Are Required
Dijkstra's algorithm assumes that once a node is extracted from the min-priority queue, its shortest distance is finalized. A negative edge weight would violate this greedy invariant because a subsequent path could reduce an already "finalized" cost. NetRoute Simulator strictly validates that all link costs are positive integers ($> 0$).

### Time Complexity
Using an adjacency list and a binary min-heap (`std::priority_queue`):
$$\mathcal{O}((V + E) \log V)$$
where $V$ is the number of routers and $E$ is the number of links.

---

## Packet Simulation

Packet transmission is simulated logically through discrete state transitions:

```
[Packet Created: ID 1001]
          │
          ▼
   Query RoutingEngine
          │
     ┌────┴────────────────────────┐
     ▼                             ▼
[Route Found]              [No Route Exists]
     │                             │
     ▼                             ▼
Status: IN_TRANSIT           Status: FAILED
Simulate Hops:               Reason: "No route available"
  A -> C                     Log to History
  C -> B                     Increment packetsFailed_
  B -> D
Record actual path
     │
     ▼
Status: DELIVERED
Set total cost & hops
Log to History
Increment packetsDelivered_
```

---

## Requirements & Installation

### Requirements
- **Compiler:** `g++` supporting C++17 or later (GCC 7+).
- **Environment:** Windows (MinGW-w64 / MSYS2), Linux (Ubuntu, Debian, Fedora), or macOS (Clang with C++17 support).
- **Shell:** Command Prompt, PowerShell, or Bash.

### Verification
Check compiler support by running:
```bash
g++ --version
```

---

## Compilation Instructions

The project uses standard C++ header/source file organization and compiles directly with `g++` without requiring CMake, Makefiles, or third-party tools.

### Compilation Command:
```bash
g++ -std=c++17 -Wall -Wextra -pedantic -Iinclude src/main.cpp src/Router.cpp src/Network.cpp src/RoutingEngine.cpp src/Packet.cpp src/PacketSimulator.cpp src/NetworkSimulator.cpp -o NetRouteSimulator
```

### Compiler Flag Explanations:
- `-std=c++17`: Enables ISO C++17 standard features.
- `-Wall -Wextra -pedantic`: Enables strict compiler diagnostic warnings.
- `-Iinclude`: Adds the `include/` directory to the compiler's header search path.
- `src/*.cpp`: Compiles each implementation translation unit.
- `-o NetRouteSimulator`: Sets the output binary name.

---

## Running the Application

### Windows (PowerShell / Command Prompt):
```powershell
.\NetRouteSimulator.exe
```

### Linux / macOS / WSL:
```bash
./NetRouteSimulator
```

---

## Usage Walkthrough

1. **Launch the simulator:** Run `./NetRouteSimulator` or `.\NetRouteSimulator.exe`.
2. **Load sample network or add routers manually:**
   - Select Option `12` to load the standard sample topology ($A, B, C, D$).
   - Or select Option `1` to add routers $A, B, C, D$ manually.
3. **Connect routers (if creating manually):**
   - Option `3` $\rightarrow$ Connect $A \leftrightarrow B$ (Cost: 4)
   - Option `3` $\rightarrow$ Connect $A \leftrightarrow C$ (Cost: 2)
   - Option `3` $\rightarrow$ Connect $C \leftrightarrow B$ (Cost: 1)
   - Option `3` $\rightarrow$ Connect $B \leftrightarrow D$ (Cost: 3)
   - Option `3` $\rightarrow$ Connect $C \leftrightarrow D$ (Cost: 7)
4. **Display topology:**
   - Option `5` displays all nodes, active states, and neighbors.
5. **Find shortest route:**
   - Option `6` $\rightarrow$ Enter Source `A` and Destination `D`.
   - Output shows $A \rightarrow C \rightarrow B \rightarrow D$ with Total Cost 6 and 3 Hops.
6. **Display routing table:**
   - Option `7` $\rightarrow$ Enter Source `A` to view optimal next hops to all destinations.
7. **Send simulated packet:**
   - Option `8` $\rightarrow$ Source `A`, Destination `D`.
   - Observes hop-by-hop forwarding visualization.
8. **View history and statistics:**
   - Option `9` views logged packet history.
   - Option `10` views network delivery percentage and average hops.
9. **Exit:** Option `0` exits cleanly.

---

## Example Output

### Finding Shortest Route (Option 6):
```
========================================
         SHORTEST ROUTE
========================================
Source Router: A
Destination Router: D

Shortest Route: A -> C -> B -> D
Total Cost: 6
Number of Hops: 3
Status: Route Found
========================================
```

### Single-Source Routing Table (Option 7):
```
======================================================================
               ROUTING TABLE FOR ROUTER [A]
======================================================================
Destination    | Total Cost     | Next Hop       | Status
----------------------------------------------------------------------
A              | 0              | A              | Local (Self)
B              | 3              | C              | Reachable
C              | 2              | C              | Reachable
D              | 6              | C              | Reachable
======================================================================
```

### Packet Simulation (Option 8):
```
========================================
           PACKET SIMULATION
========================================
Packet ID: 1001
Source: A
Destination: D

Forwarding:
A -> C
C -> B
B -> D

Actual Path: A -> C -> B -> D
Total Cost: 6
Number of Hops: 3
Status: DELIVERED
========================================
```

### Network Statistics (Option 10):
```
========================================
         NETWORK STATISTICS
========================================
Total Routers               : 4
Total Bidirectional Links   : 5
Total Packets Created       : 1
Successfully Delivered      : 1
Failed Packets              : 0
Delivery Success Rate       : 100.00%
Average Delivered Hops      : 3.00
========================================
```

---

## STL Containers Used

| Container | Header | Purpose in Project | Rationale |
| :--- | :--- | :--- | :--- |
| `std::unordered_map` | `<unordered_map>` | Router lookup table (`routers_`), Adjacency list (`adjacencyList_`), Distance table (`dist`), Predecessor map (`predecessor`) | Provides average $\mathcal{O}(1)$ key-value lookups and insertions by router ID string. |
| `std::vector` | `<vector>` | Neighbor list (`std::vector<std::pair<...>>`), Path reconstruction (`path_`), Packet history (`packetHistory_`) | Provides dynamic contiguous memory with $\mathcal{O}(1)$ amortized append and sequential cache locality. |
| `std::priority_queue` | `<queue>` | Dijkstra min-priority queue (`std::greater` comparator) | Allows $\mathcal{O}(\log V)$ extraction of the node with the minimum tentative distance. |
| `std::pair` | `<utility>` | Adjacency edge `(neighborId, cost)`, Dijkstra heap element `(cost, routerId)` | Lightweight tuple pairing two related values without defining custom boilerplate structs. |
| `std::string` | `<string>` | Router IDs, messages, endpoints | Safe dynamic character string handling. |

---

## Complexity Analysis

| Operation | Time Complexity | Space Complexity | Description |
| :--- | :--- | :--- | :--- |
| **Add Router** | $\mathcal{O}(1)$ avg | $\mathcal{O}(1)$ | Hash map insertion into `routers_` and `adjacencyList_`. |
| **Remove Router** | $\mathcal{O}(V + E)$ | $\mathcal{O}(1)$ aux | Erases router and searches adjacency lists to remove incident links. |
| **Connect Routers** | $\mathcal{O}(\text{deg}(u))$ | $\mathcal{O}(1)$ | Checks for duplicate link and appends to neighbor vectors. |
| **Disconnect Routers**| $\mathcal{O}(\text{deg}(u) + \text{deg}(v))$ | $\mathcal{O}(1)$ | Finds and erases edge from neighbor vectors. |
| **Display Topology** | $\mathcal{O}(V \log V + E \log E)$ | $\mathcal{O}(V + E)$ | Sorts router IDs and neighbor lists alphabetically for display. |
| **Shortest Route (Dijkstra)** | $\mathcal{O}((V + E) \log V)$ | $\mathcal{O}(V)$ | Min-heap Dijkstra traversal with edge relaxation. |
| **Path Reconstruction** | $\mathcal{O}(H)$ | $\mathcal{O}(H)$ | Traces predecessor chain backward ($H$ = hop count $\le V$). |
| **Routing Table** | $\mathcal{O}((V + E) \log V + V \cdot H)$ | $\mathcal{O}(V)$ | Single Dijkstra pass followed by predecessor next-hop traces. |
| **Packet Forwarding**| $\mathcal{O}((V + E) \log V + H)$ | $\mathcal{O}(H)$ | Computes path, prints forwarding hops, and records visited nodes. |
| **Packet History Display** | $\mathcal{O}(P \cdot H)$ | $\mathcal{O}(1)$ aux | Iterates over $P$ stored packets and prints their path sequences. |

---

## Testing

### Automated CLI Verification Suite
The project was tested using automated command sequences piped into standard input:
- **Router Lifecycle:** Added routers $A, B, C, D$; tested duplicate router rejection ($A$ already exists); tested empty string rejection.
- **Connection Validation:** Connected $A \leftrightarrow B$ (cost 4); rejected duplicate link creation; rejected self-links ($A \leftrightarrow A$); rejected non-positive weights ($\le 0$).
- **Graph Invariant Testing:** Connected multiple links to router $B$; deleted router $B$; verified that no dangling links to $B$ remained in $A$, $C$, or $D$.
- **Pathfinding Correctness:** Verified that the lowest-cost route from $A$ to $D$ ($A \rightarrow C \rightarrow B \rightarrow D$, Cost 6, 3 Hops) was selected over the fewer-hop route ($A \rightarrow B \rightarrow D$, Cost 7, 2 Hops).
- **Network Resilience:** Set intermediate router $C$ to `INACTIVE`; verified that Dijkstra automatically rerouted packets via $B$ ($A \rightarrow B \rightarrow D$).
- **Error Handling:** Tested packet transmission to unreachable disconnected nodes; verified `Status: FAILED` and `Reason: No route available`. Tested invalid alphanumeric menu inputs; verified clean recovery without infinite loops or crashes.

---

## Limitations

- **Logical Simulation Only:** Does not bind network sockets, parse physical Ethernet frames, or interface with real network cards.
- **In-Memory Volatility:** Network topology, packet history, and statistics are stored in RAM and reset when the application terminates (no file persistence/database).
- **Single-Threaded:** Packet forwarding is simulated sequentially; concurrent packet collisions, queue delays, and bandwidth throttling are not modeled.
- **Non-Negative Link Costs:** Relies on Dijkstra's algorithm; graphs with negative edge weights are not supported (and prevented by input validation).

---

## Future Improvements

- **Serialization & Persistence:** Save and load network topologies and routing configurations to JSON or binary files.
- **Dynamic Link Congestion:** Simulate link latency and queuing delay based on real-time traffic volume.
- **Alternative Routing Protocols:** Implement Bellman-Ford (distance-vector) and Open Shortest Path First (OSPF) link-state broadcast simulations.
- **Packet Loss Simulation:** Introduce configurable link drop probabilities and checksum corruption to model transmission errors.
- **Graphical User Interface (GUI):** Add a visual network canvas using Qt or WebAssembly to animate packet traversals visually.

---

## Author & Academic Use

- **Project:** NetRoute Simulator
- **Course:** Object-Oriented Programming (OOP) / Data Structures & Algorithms
- **Purpose:** Developed strictly for academic and educational purposes to demonstrate object-oriented software engineering, graph theory, and algorithmic pathfinding in C++.
