## Sorting on external storage

### Consider a file with records of following structure:

Emp (eid (integer), ename (string), age (integer), salary (double))

Fields of types integer, double, and string occupy 4, 8, and 40 bytes, respectively. Assume that
each (I/O) block can fit at most one record (tuple) of the input file. Implement the two-pass
multi-way sorting for the file Emp.csv in C/C++ using the skeleton code posted with this
assignment. The sorting should be based on the attribute eid. There are at most 22 blocks
available to the sort algorithm in the main memory, i.e., the size of the buffer is 22.

* The input relation is stored in a CSV file, i.e., each tuple is in a separate line and fields of
each record are separated by commas.
* The result of the sort must be stored in a new CSV file. The file that stores the relation
Emp are Emp.csv.
* Your program must assume that the input file is in the current working directory, i.e., the
one from which your program is running.
* The program must store the result in a new CSV file with the name EmpSorted.csv in the
current working directory.

### Run command
```
g++ -std=c++11 main.cpp -o main.out
main.out
```