# P2P File Sharing System

A peer-to-peer file sharing system built in C++, similar to BitTorrent. Peers can share and download files directly from each other. A central server acts as a tracker — it only stores metadata (who has what), never the actual file data.

## Architecture

```
[Server - Tracker]       ← maintains file registry only
      ↑        ↑
  [Peer 1]  [Peer 2]     ← transfer files directly between each other
```

- **Server**: Tracks which peers have which files/chunks. Stores nothing on disk.
- **Client (Peer)**: Shares local files and downloads files directly from other peers.

## How It Works

1. A peer registers its files with the server (`share` command).
2. Another peer asks the server for available files (`show` command).
3. The server responds with peer IPs and chunk locations.
4. The downloading peer connects **directly** to source peers and downloads chunks **in parallel**.
5. Chunks are reassembled into the final file locally.

Files are split into **1024-byte chunks**. Different chunks can be downloaded simultaneously from different peers, improving speed.

## Requirements

- g++ with C++11 support
- POSIX-compatible OS (Linux / macOS)

## Build

```bash
git clone <repo-url>
cd p2p-file-sharing
make
```

This produces two binaries: `server` and `client`.

## Running

### Start the server (Terminal 1)
```bash
./server
```
Output: `Server started on port 8080`

### Start a peer (Terminal 2)
```bash
./client 8082
```

### Start another peer (Terminal 3)
```bash
./client 8083
```

> Each peer must use a different port number.

## Peer Commands

| Command | Description |
|---|---|
| `show` | List all files available to download |
| `share N file1 file2 ...` | Share N files from the current directory |
| `download filename` | Download a file from other peers |

### Example

**Peer 1** — share a file:
```
share 1 notes.pdf
```

**Peer 2** — list and download:
```
show
download notes.pdf
```

## Notes

- The file you want to share must be in the same directory from which you run `./client`.
- Server data is in-memory only — restarting the server clears all registered files. Peers must re-register their files after a server restart.
- Downloaded files are saved to disk permanently and are not affected by server restarts.
- If a download is interrupted, a `.log` file tracks completed chunks so the download can resume.

## Deploying on Multiple Machines

Edit `SERVER_IP` in both [server.hpp](server.hpp) and [peers.hpp](peers.hpp):

```cpp
#define SERVER_IP "your.server.ip"
```

Then recompile and run `./server` on the server machine and `./client <port>` on each peer machine.

Peers must have publicly reachable IPs (or be on the same LAN) since file transfers are direct peer-to-peer — they do not go through the server.

## Protocol

| Request | Description |
|---|---|
| Register Request | Peer tells server which files it wants to share |
| File List Request | Peer asks server for all available files |
| File Locations Request | Peer asks server for IPs/ports of peers that have a specific file |
| Chunk Register Request | Peer notifies server after receiving a new chunk |
| File Chunk Request | Peer requests a specific chunk directly from another peer |

## Cleanup

```bash
make clean
```
