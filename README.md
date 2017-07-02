# KeyValue-Store

KeyValue Store is an application, developed during the Data Structures and Algorithms course
in Sofia University. It stores data and returns a unique key with which the data can
be obtained later and can also permanently delete an entry when given a key.

The purpose is accomplished with a separate chaining hash (an array of linked lists with boxes
where each box contains the needed information such as the stored data and the unique
key). All the data is stored permanently in a file and when the application is started
all the file is being read and the info is stored in the memory for easier usage. 

The three main options are to store, load and erase an entry.

The used hashing function is modular hashing. The number of chains is a constant value - 6833. 
It is a prime number and separate chaining hashing with a prime number of chains is proved to
have much better performance than one with a number that is not prime.

When deleting a box from the file, the program finds the position in the file in which the box starts,
creates a new file, copies all the data before the box and after the box to the new file and deletes 
the old one.

When adding a box in the store, the data is appended at the end of the file and a file copy is needed
only when deleting one.

The Hash class has the function info() as an attribute. It prints information about the hash - minimum,
maximum and average number of entries in all the chains.
