05_Hashing_Lab
==============

Implement a hash table in C++

Requirements
------------

1. `keyExists`, `find`, `remove`, and `size` should all be O(1)
2. `add` should be reasonably fast. Use linear probing to find an open slot in the hash table. This will be O(1), on average, except when `grow` is called.
3. `grow` should be O(n)
4. Do not leak memory


Reading
=======
"Open Data Structures," Chapter 5, except for 5.2.3. http://opendatastructures.org/ods-cpp/5_Hash_Tables.html

Questions
=========

#### 1. Which of the above requirements work, and which do not? For each requirement, write a brief response.

1. Works, although all of these methods except `size` use calcIndex, which needs to loop through the array. Given the nature of hash tables though, the looping should still be O(1) time. 
2. Works. Needs to do a quick loop to find the first open slot for a new item, but that should not be long, given the spaces provided i the hash table.
3. Works. `grow` loops through the old array once to transfer all the information to the new, bigger array.
4. Works. delete[] is called in only the deconstructor and grow() method, which ar ethe only places old data is being deleted (overwriting in `add` doesn't count).

#### 2. I decided to use two function (`keyExists` and `find`) to enable lookup of keys. Another option would have been to have `find` return a `T*`, which would be `NULL` if an item with matching key is not found. Which design do you think would be better? Explain your reasoning. You may notice that the designers of C++ made the same decision I did when they designed http://www.cplusplus.com/reference/unordered_map/unordered_map/

Using the two method implementation as we did here seems to be better, because keyExists is very easy to use, without having to check for any returned values of NULL.
	Also, the backingArray could contain something that is supposed to be NULL, so returning NULL makes it ambiguous if that means the key was unfound or not.

#### 3. What is one question that confused you about this exercise, or one piece of advice you would share with students next semester?

Remember that calcIndex can be used in your other functions like `remove`, `keyExists`, and `find`. Makes it much easier than just writing the same searching pattern over and over again.