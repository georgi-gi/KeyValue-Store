#include "Hash.h"

Hash::Hash(char*& fileName)
{
	this->fileName = fileName;
	chains = new List<Box>[numChains];
	getData.open(fileName, std::ios::binary);
	getData.seekg(0, std::ios::beg);
	readFile();
	getData.close();
}


Hash::~Hash()
{
	delete[] chains;
}

///modular hashing function
int Hash::calculateHash(const char * value, size_t size)
{
	int hash = 0;
	for (size_t i = 0; i < size; ++i)
	{
		hash = (multiplier * hash + value[i]) % numChains;
	}

	return hash;
}

///The function writes the box received as a parameter at the end of the file.
void Hash::serializeBox(Box*& b)
{
	if (!setData.is_open())
		setData.open(fileName, std::ios::binary | std::ios::app);

	size_t size = b->getSize();

	setData.write((char*)b->getKey(), 6);
	setData.write((char*)&size, sizeof(size));
	setData.write((char*)b->getValue(), b->getSize());

	setData.close();
}

///	The function finds the positions in the file on which the box to be deleted starts and ends.
///It creates a new file and copies the information from the old one before and after the box 
///and after that deletes the old file and renames the new one with the name of the old.
void Hash::removeBoxFromFile(Box *& b)
{
	if (!getData.is_open())
		getData.open(fileName, std::ios::binary);

	unsigned long long startBoxPos = 0;
	unsigned long long endBoxPos = 0;

	while (!getData.eof())
	{
		char key[7];
		size_t size = 0;
		char* value;

		startBoxPos = getData.tellg();

		getData.read((char*)&key, 6);
		key[6] = '\0';
		getData.read((char*)&size, sizeof(size));

		value = new char[size + 1];
		getData.read((char*)value, size);
		value[size] = '\0';

		if (strcmp(b->getKey(), key) == 0)
		{
			endBoxPos = getData.tellg();
			break;
		}
	}

	getData.seekg(0, std::ios::end);
	unsigned long long endFilePos = getData.tellg();

	unsigned long long fromStartToBox = startBoxPos;
	unsigned long long fromBoxToEnd = endFilePos - endBoxPos;
	
	char* beforeBox = new char[fromStartToBox];
	char* afterBox = new char[fromBoxToEnd];

	getData.seekg(0, std::ios::beg);
	getData.read(beforeBox, fromStartToBox);

	getData.seekg(endBoxPos, std::ios::beg);
	getData.read(afterBox, fromBoxToEnd);

	getData.close();

	char* newFileName = new char[strlen(fileName) + 2];
	
	size_t i = 0;
	while (fileName[i] != '.')
	{
		newFileName[i] = fileName[i];
		++i;
	}
	size_t j = i;
	newFileName[i++] = '1';
	for (j; j < strlen(fileName); ++j, ++i)
	{
		newFileName[i] = fileName[j];
	}
	newFileName[strlen(fileName) + 1] = '\0';

	std::ofstream newFile(newFileName, std::ios::binary | std::ios::app);
	newFile.write((char*)beforeBox, fromStartToBox);
	newFile.write((char*)afterBox, fromBoxToEnd);

	newFile.close();

	std::remove(fileName);
	int isDone = std::rename(newFileName, fileName);
}

///Reads the information from the file and fills the hash table with it.
void Hash::readFile()
{
	if (!getData.good())
	{
		return;
	}
	if (!getData.is_open())
	{
		getData.open("db.dat", std::ios::binary);
		getData.seekg(0, std::ios::beg);
	}

	char d;
	getData.get(d);
	if (!getData.eof())
		getData.unget();

	while (!getData.eof())
	{
		char* key = new char[7];
		size_t size = 0;
			

		getData.read((char*)key, 6);
		key[6] = '\0';
		getData.read((char*)&size, sizeof(size));
		
		char* value = new char[size + 1];
		getData.read((char*)value, size);
		value[size] = '\0';
		storeFromFile(key, value, size);

		char c;
		getData.get(c);
		if (!getData.eof())
			getData.unget();
	}
}

///Creates a box with the info read from the file and pushes it in the hash table in the correct chain.
void Hash::storeFromFile(char * key, const char * value, size_t size)
{
	Box* b = new Box(value, key, size);

	int hash = (key[0] - 48) * 1000 + (key[1] - 48) * 100 
			 + (key[2] - 48) * 10 + (key[3] - 48);

	chains[hash].add(*b);
}

///The function gets the information needed to store a box. It generates a unique key, pushes the box
///in the correct chain and updates the file with the information for that box (serializeBox()).
char* Hash::store(const char * value, size_t size)
{
	char* key = new char[7];
	int hash = calculateHash(value, size);

	itoa(hash / 1000, &key[0], 10);
	itoa((hash / 100) % 10, &key[1], 10);
	itoa((hash / 10) % 10, &key[2], 10);
	itoa(hash % 10, &key[3], 10);
	
	int lastNum = 0;

	if (chains[hash].getSize() == 0)
		lastNum = 0;
	else
		lastNum = (chains[hash].getTail().getKey()[4] - 48) * 10 
				+ (chains[hash].getTail().getKey()[5] - 48) + 1;

	itoa(lastNum / 10, &key[4], 10);
	itoa(lastNum % 10, &key[5], 10);
	key[6] = '\0';

	Box* b = new Box(value, key, size);

	chains[hash].add(*b);

	serializeBox(b);

	return key;
}

///The function gets a key and gets the hash from it. If there is an existing item with that key,
///it is returned (value) and the parameter size is changed. If there isn't an entry with that key
///the function returns nullptr.
char* Hash::load(char* key, size_t & size)
{
	int hash = (key[0] - 48) * 1000 + (key[1] - 48) * 100 
			 + (key[2] - 48) * 10 + (key[3] - 48);

	if (hash < 0 || hash >= numChains)
	{
		size = NULL;
		return nullptr;
	}
	else if (chains[hash].getSize() == 0)
	{
		size = NULL;
		return nullptr;
	}
	else
	{
		char* value = nullptr;
		for (Iterator<Hash::Box> it = chains[hash].getIterator(); !it.isEnd(); it.moveNext())
		{
			if (strcmp(it.getCurrent().getKey(), key) == 0)
			{
				value = new char[it.getCurrent().getSize()];
				strcpy(value, it.getCurrent().getValue());
				size = it.getCurrent().getSize();
			}
		}
		return value;
	}
}

///Gets the hash from the key and if there is an element with this key, the program iterates 
///through the chain to see on which position it is. After that calls the function
///removeBoxFromFile() and then removes the box from the chain.
bool Hash::erase(char* key)
{
	int hash = (key[0] - 48) * 1000 + (key[1] - 48) * 100 
			 + (key[2] - 48) * 10 + (key[3] - 48);

	if (hash < 0 || hash >= numChains)
		return false;
	else
	{
		bool doesExist = false;
		Box* toBeRemoved;
		size_t ind = 0;
		for (Iterator<Hash::Box> it = chains[hash].getIterator(); !it.isEnd(); it.moveNext())
		{
			if (strcmp(it.getCurrent().getKey(), key) != 0)
			{
				++ind;
			}
			else
			{
				toBeRemoved = &it.getCurrent();
				doesExist = true;
				break;
			}
		}
		if (doesExist)
		{
			removeBoxFromFile(toBeRemoved);
			chains[hash].removeAt(ind);
			return true;
		}
		return false;
	}
}

///The function prints information about the hashtable - average, minimum and 
///maximum entries on all chains.
void Hash::info()
{
	size_t cntZero = 0;
	size_t cntElements = 0;
	int average = 0;
	int min = 9999;
	int max = 0;
	for (size_t i = 0; i < numChains; ++i)
	{
		if (chains[i].getSize() < min)
			min = chains[i].getSize();
		if (chains[i].getSize() > max)
			max = chains[i].getSize();
		average += chains[i].getSize();
		if (chains[i].getSize() == 0)
			cntZero++;
		cntElements += chains[i].getSize();
	}
	average /= numChains;
	std::cout << "Chains: " << numChains 
			  << "\nElements: " << cntElements 
			  << "\nMin: " << min << "\nMax: " << max << "\nAverage: " << average
			  << "\nZeroes: " << cntZero << '\n';
}

Hash::Box::Box(const char *& value, char*& key, size_t size)
{
	this->value = value;
	this->key = key;
	this->size = size;
}

Hash::Box::Box()
{
	this->size = 0;
	this->value = nullptr;
	this->key = nullptr;
}

Hash::Box::~Box()
{
	delete[] value;
	delete[] key;
}

const char * Hash::Box::getKey()
{
	return this->key;
}

const char * Hash::Box::getValue()
{
	return this->value;
}

size_t Hash::Box::getSize()
{
	return this->size;
}
