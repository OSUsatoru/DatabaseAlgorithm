## Hash Indexing

* Assume that we have a relation Employee(id, name, bio, manager-id). The values of id and
manager-id are integers each with the fixed size of 8 bytes. The values of name and bio are
character strings and take at most 200 and 500 bytes, respectively. Note that as opposed to the
values of id and manager-id, the sizes of the values of name (bio) are not fixed and are between 1
to 200 (500) bytes. The size of each block is 4096 bytes (4KB). The size of each record is less than
the size of a block. Using the provided skeleton code with this assignment, write a C++ program
that creates a hash index file for relation Employee using attribute id. Your program must also
enable users to search the created index by providing the id of a record.
    - Your program must first read an input Employee relation and build a linear hash index for
the relation using attribute id. The input relation is stored in a CSV file, i.e., each tuple is
in a separate line and fields of each record are separated by commas. Your program must
assume that the input CSV file is in the current working directory, i.e., the one from which
your program is running, and its name is Employee.csv. We have included an input CSV file
with this assignment as a sample test case for your program. Your program must create and
search hash indexes correctly for other CSV files with the same fields as the sample file.
    - Your program must store the hash index in a file with the name EmployeeIndex on the
    current working directory. You may use one of the methods explained for storing
    variable-length records and the method described on storing blocks (pages) of
    variable-length records in our lectures on storage management to store records and blocks in
    the index file. They are also explained in Sections 9.7.2 and 9.6.2 of Cow Book, respectively.
    - During the index creation, your program may keep up to three blocks plus the directory of
    the hash index in main memory at any time. The submitted solutions that use more main
    memory will not get any points.
    - You may use hash function h = id mod 216. Your program must increment the value of n if
the average number of records per each block exceeds 70% of the block capacity.
    - After finishing the index creation, your program should accept an Employee id in its
    command line and search the index file for all records of the given id. Like index creation,
    your program may use up to three blocks plus the directory of the hash index in main
    memory at any time. The submitted solutions that use more main memory will not get any
    points for implementing lookup operation. The user of your program may search for records
    of multiple ids, one id at a time.


### Run command
```
g++ -std=c++11 main.cpp -o main.out
main.out
```
