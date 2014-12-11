//DO NOT CHANGE THIS FILE
//Author: Bo Brinkman
//Date: 2013/07/24
#include "USet.h"

/*
 * Note: Just above your template declaration when you use this class, you
 * must define method called "hash" that takes a Key as input, and returns
 * an unsigned long (which is the hash value)
 *
 * For example, you might do:
 * unsigned long hash(char c){ return 10*((unsigned long)c)%13; }
 * HashTable<char,int> mySillyTable;
 *
 * If you don't define an appropriate hash function, the class won't compile.
 */

template <class Key, class T>
class HashTable : public USet <Key, T> {
 private:
  class HashRecord {
  public:
    Key k;
    T x;

    //If the slot in the hash table is totally empty, set this to true.
    bool isNull;

    //If the slot used to have something in it, but doesn't now, set
    // isDel to true, and isNull to false. isNull is only for slots
    // that have never been used
    bool isDel;

    HashRecord() { isNull = true; isDel = false; };
  };

 public:
  //See USet.h for documentation of these methods
  virtual unsigned long size();
  virtual void add(Key k, T x);
  virtual void remove(Key k);
  virtual T find(Key k);
  virtual bool keyExists(Key k);

  //Initialize all private member variables.
  HashTable();
  //Delete any dynamically allocated memory.
  virtual ~HashTable();

private:
  //A pointer to the array that holds the hash table data
  HashRecord* backingArray;

  //Whenever numItems + numRemoved >= backingArraySize/2, call
  // grow(). grow() should make a new backing array that is twice the
  // size of the old one, similar to what we did in the ArrayQueue
  // lab.
  //Note: You cannot just loop through the old array and copy it to the
  // new one! Since the backing array size has changed, each item will likely
  // map to a different slot in the array. You may just want to use add()
  // after initializing the new array.
  void grow();

  //This helper method should take a key, and return the index for that
  // item within the hash table. If the item already exists, return the
  // index of the existing item. If the item doesn't exist, return the index
  // where it OUGHT to be. This function can then be used as a helper method in
  // your other methods.
  unsigned long calcIndex(Key k);

  unsigned long numItems; //Number of items in the hash table

  unsigned long hashPrimesIndex;

  //Note: Ordinarily, these OUGHT to be private. In this case I have
  // made them public for easy of testing.
 public:
  unsigned long numRemoved; //Number of slots that have been removed but not re-used. Those that have isDel == true
  unsigned long backingArraySize;
};

//#include "HashTable.ipp"

//You will need this so you can make a string to throw in
// remove
#include <string>

template <class Key, class T>
HashTable<Key,T>::HashTable(){
	hashPrimesIndex = 0;
	backingArray = new HashRecord[hashPrimes[hashPrimesIndex]];
	backingArraySize = 53;
	numItems = 0;
	numRemoved = 0;
}

template <class Key, class T>
HashTable<Key,T>::~HashTable() {
	delete[] backingArray;
}

template <class Key, class T>
unsigned long HashTable<Key,T>::calcIndex(Key k){
	//find starting index (with hash()
	//then linearly probe to find the actual index
	unsigned long index = hash(k) % backingArraySize;
	//if(keyExists(k)){
		while(!backingArray[index].isNull) //if backingArray[index] is not empty
		{
			if(!backingArray[index].isDel && backingArray[index].k == k)
				return index;
			index = (index + 1) % backingArraySize;
		}
	//}
	return numItems;
  //return numItems; //This indicates failure, since it is an impossible value
}

template <class Key, class T>
void HashTable<Key,T>::add(Key k, T x){
	
	if (keyExists(k)) {
		throw std::string("Error: Can't add a key that already exists.");
	}
	
	if ( ((numItems + numRemoved + 1) * 2) > backingArraySize) {
		grow();
	}

	unsigned int i = hash(k) % backingArraySize;

	while(!backingArray[i].isNull && !backingArray[i].isDel)
	{
		i = (i + 1) % backingArraySize;
	}

	if(backingArray[i].isDel)
	{
		numRemoved--;
	}

	if(backingArray[i].isNull)
	{
		numItems++;
	}

	backingArray[i].k = k;
	backingArray[i].x = x;
	backingArray[i].isNull = false;
	backingArray[i].isDel = false;
}

template <class Key, class T>
void HashTable<Key,T>::remove(Key k){
	if(keyExists(k)){
		
		unsigned long i = calcIndex(k);

		if( numItems != i) //making sure k is actually in the table
		{
			backingArray[i].isDel = true;
			numItems--;
			numRemoved++;
		}


	}
}

template <class Key, class T>
T HashTable<Key,T>::find(Key k){

	unsigned long i = calcIndex(k);

	if(i != numItems)
	{
		return backingArray[i].x;
	}

	throw std::string("In find(), that key doesn't exist.");
}

template <class Key, class T>
bool HashTable<Key,T>::keyExists(Key k){

	unsigned long i = calcIndex(k);

	if(i == numItems) //key doesn't exist
	{
		return false;
	}

	return true;
}

template <class Key, class T>
unsigned long HashTable<Key,T>::size(){
  return numItems;
}

template <class Key, class T>
void HashTable<Key,T>::grow(){
	hashPrimesIndex++;
	
	HashRecord* oldArray = backingArray;
	unsigned long oldArraySize = backingArraySize;

	backingArray = new HashRecord[hashPrimes[hashPrimesIndex]];
	backingArraySize = hashPrimes[hashPrimesIndex];

	numItems = 0;
	numRemoved = 0;

	for(unsigned int i = 0; i < oldArraySize; i++)
	{
		if((oldArray[i].isNull != true) && (oldArray[i].isDel != true))
		{
			add(oldArray[i].k, oldArray[i].x);
		}
	}

	delete[] oldArray;
}
