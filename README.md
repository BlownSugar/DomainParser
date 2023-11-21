
# Common Parser and Visualizer in C for Domain Analysis

## Overview
This repository contains two scripts written in C: a common parser and a visualizer. The parser reads data from a text file and extracts specific details to create a CSV file. The visualizer takes this CSV file and a list of specified websites to produce a visual representation in HTML format.

## Common Parser
### Description
The parser processes an input file (`input.txt`) to extract URLs, logins, and passwords, specifically filtering URLs ending with ".ru" or ".su". The results are saved in a CSV format (`output.csv`).

### Usage
1. Place your input data in `input.txt`.
2. Run the parser program.
3. Check `output.csv` for the results.

## Visualizer
### Description
The visualizer reads the generated `output.csv` file and a list of websites from `check.txt`. It then creates an HTML file (`output.html`) that visualizes the data, showing counts of specified and unspecified websites, and domain-specific information.

### Usage
1. Ensure `output.csv` and `check.txt` are populated correctly.
2. Run the visualizer program.
3. Open `output.html` in a web browser to view the visualization.

## Building and Running
### Prerequisites
- A C compiler (like GCC)
- Basic knowledge of compiling and running C programs

### Compile
```bash
gcc parser.c -o parser
gcc visualizer.c -o visualizer
```

### Run
```bash
./parser
./visualizer
```

