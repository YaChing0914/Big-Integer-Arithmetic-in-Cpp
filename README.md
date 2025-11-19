# Big Integer Arithmetic in C++

A simple C++ program that performs big integer addition, subtraction, multiplication, and division on numbers up to 100 digits.  
All operations are implemented manually using string-based digit processing (no external libraries).
## Features

- Supports integers up to 10¹⁰⁰
- Implements +, -, \*, /
- File-based input/output using fstream
- Works without GMP or Boost


## Input Format (testbench.txt)
Each line contains two big integers:
> X Y
>

Constraints:
> 0 < X, Y < 10¹⁰⁰
>

Example Input:
> 22222222222 11111111111
> 
> 12345678901234567890 67890123456789012345
> 
> 77777777777777777777 2
> 
> 2486248624862486248624862486 1
>
> 11 1

## Output Format

For each test case, output:
> X + Y
> 
> X - Y
> 
> X * Y
> 
> X / Y

Separate test cases with one blank line. 

## How to Compile & Run
### Compile
> g++ -std=c++11 -o big_integer big_integer.cpp
### Run
> ./big_integer

Make sure testbench.txt is in the same directory.







