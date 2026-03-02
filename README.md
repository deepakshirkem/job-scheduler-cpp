# Multithreaded Job Scheduler System (C++)

A production-style multithreaded job scheduling system built in modern C++.

## Features
- Thread-safe job queue
- Worker thread pool
- Job lifecycle state tracking
- Async execution model
- Failure handling support
- Graceful shutdown on SIGTERM/SIGINT
- Human readable job status reporting

## Architecture
Client → JobManager → ThreadSafeQueue → WorkerPool → JobExecutor → JobStateTracker

## Tech Stack
- C++17
- std::thread
- mutex
- condition_variable
- RAII

## Status
Under Development