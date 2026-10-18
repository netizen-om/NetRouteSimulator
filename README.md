# NetRoute Simulator

A beginner-friendly, CLI-based Network Routing Simulator in C++17 designed for Object-Oriented Programming (OOP).

## Project Structure

The project is structured into modular `.cpp` files without separate `.h`/`.hpp` header files, using `ProjectDeclarations.cpp` as the shared declaration include:

```
NetRouteSimulator/
├── main.cpp                # Application entry point; creates and runs NetworkSimulator
├── ProjectDeclarations.cpp # Shared declarations (enums, data structures, and class definitions)
├── Router.cpp              # Implementation of Router state and accessors
├── Network.cpp             # Implementation of Network topology, links, and adjacency lists
├── RoutingEngine.cpp       # Implementation of Dijkstra shortest-path algorithm and routing tables
├── Packet.cpp              # Implementation of Packet data and status conversion
├── PacketSimulator.cpp     # Implementation of PacketSimulator (history and statistics)
├── NetworkSimulator.cpp    # Implementation of NetworkSimulator (CLI menu and input validation)
└── README.md               # Project documentation and build instructions
```

## Compilation

Compile the project using the standard C++17 toolchain:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp Router.cpp Network.cpp RoutingEngine.cpp Packet.cpp PacketSimulator.cpp NetworkSimulator.cpp -o NetRouteSimulator
```

> **Note:** `ProjectDeclarations.cpp` is included as a declaration file across implementation translation units and is not compiled as an independent object file to avoid duplicate symbol definitions.

## Execution

Run the compiled executable:

```powershell
.\NetRouteSimulator.exe
```

## Supported Features

- **Router Management:** Add and remove routers with duplicate and empty-ID validation; toggle active/inactive status.
- **Connection Management:** Create bidirectional weighted links, prevent self-links and duplicate connections, and validate link costs.
- **Dangling Link Prevention:** Automatically removes links associated with deleted routers.
- **Topology Display:** Displays network nodes and connections sorted alphabetically with active/inactive indicators.
- **Dijkstra Shortest-Path Routing:** Computes the lowest-cost path between routers using a min-priority queue and predecessor tracing.
- **Single-Source Routing Table:** Generates next-hop forwarding tables for any source node in a single Dijkstra traversal.
- **Sample Demonstration Network:** Option 12 sets up the standard 4-node benchmark topology (A, B, C, D) showcasing cost vs. hop optimization.
- **Safe Input Processing:** Guards against empty strings, non-numeric values, and EOF conditions without crashing.
