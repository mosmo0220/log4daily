# Console Command Handler Documentation

## Overview

This module provides functionalities for parsing console inputs into structured commands and filtering them based on supported commands. It consists of two primary namespaces:

- `consoleHandlers`: Handles the parsing of command-line inputs.
- `controlHandlers`: Filters the parsed commands to only include supported commands.

## Command Structure

The `Command` structure represents a command with its associated arguments.

```cpp
struct Command {
    std::string name;     // The name of the command
    std::string argument; // The argument associated with the command

    bool operator==(const Command& other) const; // Compares two Command objects for equality
};
```

## Dependencies

* vector: For dynamic arrays to store commands.
* string: For string manipulation to handle command names and arguments.
* algorithm: For using std::find to search for supported commands.
