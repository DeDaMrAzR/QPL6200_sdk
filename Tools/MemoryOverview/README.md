# Memory Overview Script

We need to generate a report of code sizes for relevant builds within the project. This script parses map files from CHIP builds and provides a detailed memory usage overview.

## Setup

Install the `prettytable` library if required:

```sh
pip install PrettyTable
```

## Usage
To run the script, use the following command:

```shell
Usage: memoryoverview.py [OPTIONS]

Options:
  -l, --logfile TEXT          Output log file name.
  -s, --separate              Generate a log file per application. Useful for diffs.
  -a, --add-application TEXT  Point to the map file of an application to add it to the memory comparison.
  -1, --only-this TEXT        Point to the map file of an application to analyze.
  -c, --compiler TEXT         Select the compiler producing the map-file type (default: GCC).
  --help                      Show this message and exit.
```

### Example

To generate a memory overview report for a specific application map file:

```shell
python memoryoverview.py -1 path/to/application.map
```

To generate a memory overview report and save it to a log file:

```shell
python memoryoverview.py -1 path/to/application.map -l memory_report.log
```

## Interpreting Memory Usage Percentages

The memory usage percentages in the report indicate how much of each type of memory (Flash, RAM, Stack, Heap) is being used by the application. Here's a breakdown of how to interpret these percentages:

1. **Flash Usage**: This section shows the usage of flash memory, which is typically used for storing the program code and read-only data. The percentage indicates how much of the available flash memory is being used.

2. **RAM Usage**: This section shows the usage of RAM, which is used for storing variables and dynamic data during the execution of the program. The percentage indicates how much of the available RAM is being used.

3. **Stack Usage**: This section shows the usage of stack memory, which is used for function call management, including local variables and return addresses. The percentage indicates how much of the allocated stack memory is being used.

4. **Heap Usage**: This section shows the usage of heap memory, which is used for dynamic memory allocation during the program's execution. The percentage indicates how much of the allocated heap memory is being used.

Each section includes a table with the following columns:

- **Section Name**: The name of the memory section (e.g., `.text`, `.data`, `.bss`).
- **Size**: The size of the memory section in bytes.
- **Percentage**: The percentage of the total memory used by this section.

For example, if the Flash Usage section shows that the `.text` section is using 50% of the flash memory, it means that half of the available flash memory is occupied by the program code.

### Example Output

```text
Memory Overview Report
=======================

Flash Usage
-----------
| Section Name | Size (bytes) | Percentage |
|--------------|---------------|------------|
| .text        | 10240         | 50%        |
| .rodata      | 2048          | 10%        |
| .data        | 512           | 2.5%       |
| .bss         | 0             | 0%         |
| .heap        | 5120          | 25%        |
| .stack       | 1024          | 5%         |
|--------------|---------------|------------|
| **Total**    | **18944**     | **92.5%**  |

RAM Usage
---------
| Section Name | Size (bytes) | Percentage |
|--------------|---------------|------------|
| .data        | 512           | 5%         |
| .bss         | 4096          | 40%        |
| .heap        | 5120          | 50%        |
| .stack       | 1024          | 10%        |
|--------------|---------------|------------|
| **Total**    | **10752**     | **105%**   |

Stack Usage
-----------
| Section Name | Size (bytes) | Percentage |
|--------------|---------------|------------|
| .stack       | 1024          | 100%       |
|--------------|---------------|------------|
| **Total**    | **1024**      | **100%**   |

Heap Usage
----------
| Section Name | Size (bytes) | Percentage |
|--------------|---------------|------------|
| .heap        | 5120          | 100%       |
|--------------|---------------|------------|
| **Total**    | **5120**      | **100%**   |
```
