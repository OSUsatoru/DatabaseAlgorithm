## Sort-merge Join
### Consider the following relations:
Dept (did (integer), dname (string), budget (double), managerid (integer))
Emp (eid (integer), ename (string), age (integer), salary (double))

* Fields of types integer, double, and string occupy 4, 8, and 40 bytes, respectively. Each block can
fit at most one tuple of an input relation. Using the provided skeleton code with this assignment,
implement the optimized sort-merge join algorithm for Dept ./Dept.managerid=Emp.eid Emp in C++.
* Each input relation is stored in a separate CSV file, i.e., each tuple is in a separate line and
fields of each record are separated by commas. The files that store relations Dept and Emp
are Dept.csv and Emp.csv, respectively. Your program must assume that the input files are
in the current working directory, i.e., the one from which your program is running. We have
included sample input CSV files with this assignment as sample test cases for your program.
Your program must join correctly other CSV files with the same fields as the sample files.
* The program must store the result in a new CSV file with the name join.csv in the current
working directory.
* During the join computation, your program may keep up to 22 blocks plus a relatively small
number of control variables, e.g., flags or counters, in main memory at any time. The
submitted solutions that use more main memory will not get any points.

### Run command
```
g++ -std=c++11 main.cpp -o main.out
main.out
```
