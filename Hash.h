#pragma once
#include <fstream>
#include "List.h"

//separate chaining hash
class Hash
{
private:
	class Box
	{
	private:
		const char* value;
		const char* key;
		size_t size;
	public:
		Box(const char*& value, char*& key, size_t size);
		Box();
		~Box();
	public:
		const char* getKey();
		const char* getValue();
		size_t getSize();
	};

private:
	size_t numChains = 6833;
	size_t multiplier = 31;
	List<Box>* chains;
	std::ifstream getData;
	std::ofstream setData;
	char* fileName;
public:
	Hash(char*& fileName);
	~Hash();
private:
	int calculateHash(const char* value, size_t size);
	void serializeBox(Box*& b);
	void removeBoxFromFile(Box*& b);
	void readFile();
	void storeFromFile(char* key, const char* value, size_t size);
public:
	char* store(const char* value, size_t size);
	char* load(char* key, size_t& size);
	bool erase(char* key);
	void info();
};

