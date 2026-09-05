# freebsd-c-lab

A collection of small C programs and experiments written while learning C and working directly with FreeBSD/POSIX APIs.

This is a **lab repo**, not one polished application. The earlier directories are deliberately small exercises, and the later ones start turning those pieces into more interesting programs involving sockets, IPv4/IPv6, `poll()`, signals, file I/O, and other Unix-y things.

## Start here: tcpnoise

If you're just browsing the repo, **[`07-tcpnoise`](./07-tcpnoise) is the most interesting thing here right now.**

`tcpnoise` is a small TCP network-noise monitor: point some otherwise-unused TCP ports at the public Internet and watch scanners, bots, probes, and other random garbage find them.

It currently includes:

* multiple TCP ports at once
* separate IPv4 and IPv6 listeners
* `poll()`-based listener handling
* remote IP and source-port reporting
* per-IP seen counts
* per-port connection counts
* initial payload capture and escaped binary output
* per-port log files
* optional randomized response banners
* banner send/result logging
* graceful `SIGINT` / `SIGTERM` shutdown

It's intentionally small and single-threaded. It is a learning project and network curiosity tool, not a hardened honeypot.

Example:

```sh
cd 07-tcpnoise
make
./tcpnoise 2222 2323 8080
```

Then wait for the Internet to do what the Internet does.

## Projects

| Directory                          | What it is                                                                                                                                 |
| ---------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------ |
| [`00-hello`](./00-hello)           | The obligatory first C program: `hello from FreeBSD`.                                                                                      |
| [`01-bytecount`](./01-bytecount)   | Reads a file with `fread()` and counts the bytes read.                                                                                     |
| [`02-copyfile`](./02-copyfile)     | Binary file-copy exercise with read/write error handling and same-file detection.                                                          |
| [`03-daytime`](./03-daytime)       | TCP Daytime Protocol client with IPv4/IPv6 resolution and optional conversion to local time.                                               |
| [`04-listdir`](./04-listdir)       | Lists the current directory using `getcwd()`, `opendir()`, and `readdir()`.                                                                |
| [`05-tcpclient`](./05-tcpclient)   | Small generic TCP client using `getaddrinfo()`, IPv4/IPv6, `connect()`, and `recv()`.                                                      |
| [`06-todo`](./06-todo)             | Multi-file command-line todo application supporting add, list, complete, uncomplete, and delete operations.                                |
| **[`07-tcpnoise`](./07-tcpnoise)** | **The main attraction for now:** TCP network-noise monitor with IPv4/IPv6, multiple ports, logging, payload capture, and optional banners. |
| [`08-bannergrab`](./08-bannergrab) | Work in progress. Currently handles and validates `<host> <port>` command-line arguments; networking comes next.                           |
| [`demos`](./demos)                 | Tiny throwaway examples used while learning individual C concepts.                                                                         |

## Building

There isn't one top-level build because the directories are separate exercises.

The simple programs can generally be compiled directly with `cc`, for example:

```sh
cc -Wall -Wextra -Wpedantic 01-bytecount/bytecount.c -o bytecount
```

Larger exercises such as `06-todo` and `07-tcpnoise` have their own Makefiles:

```sh
cd 07-tcpnoise
make
```

Clean a Makefile-based project with:

```sh
make clean
```

## What I'm learning here

The repo has gradually moved through things like:

* C syntax and command-line arguments
* stdio and file I/O
* error handling and return values
* directories and filesystem APIs
* splitting a program across source/header files
* TCP sockets
* DNS/address resolution with `getaddrinfo()`
* IPv4 and IPv6
* `poll()`
* signal handling
* socket timeouts
* dynamic memory
* simple logging

The code reflects that progression. Earlier exercises are intentionally small and sometimes much rougher than the newer ones.

## Platform

These programs are primarily written and tested on FreeBSD and make direct use of POSIX/BSD-style APIs.

Some of them will also compile on other Unix-like systems with little or no modification, but cross-platform portability is not the point of this repo. The point is to learn C while actually using the operating-system interfaces instead of wrapping everything in a giant abstraction layer.

That would rather defeat the purpose.
