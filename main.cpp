#include <iostream>
#include "Hash.h"

void testFill(Hash& myh)
{
	for (size_t i = 1; i < 10000; ++i)
	{
		char* inp = new char[i];
		for (size_t j = 0; j < i; ++j)
		{
			inp[j] = 'b';
		}
		myh.store(inp, i);
	}
}

int main()
{
	char* fileName = "db.dat";
	Hash myh(fileName);

	//testFill(myh);

	myh.info();

	int inp = 1;

	while (inp == 1 || inp == 2 || inp == 3)
	{
		char* input;

		std::cout << "Enter: 1 - Store\n       2 - Load\n       3 - Erase\n       Anything else - exit\n";
		std::cin >> inp;
		if (inp == 1)
		{
			char buffer[100];
			std::cout << "Enter expression to store:\n";
			getchar();
			std::cin.getline(buffer, 100);
			input = new char[strlen(buffer) + 1];
			strncpy(input, buffer, strlen(buffer));
			input[strlen(buffer)] = '\0';
			char* key = myh.store(input, strlen(input));
			key[6] = '\0';
			std::cout << "The key for the entered expression is: " << key << '\n';
		}
		else if (inp == 2)
		{
			char* key = new char[7];
			std::cout << "Enter key to search for: ";
			std::cin >> key;
			key[6] = '\0';
			size_t size = 0;
			char* expr = nullptr;
			expr = myh.load(key, size);
			if (expr == nullptr)
			{
				std::cout << "No such key in the database.\n";
			}
			else
			{
				std::cout << "Expression: " << expr << "\nSize: " << size << '\n';
			}
			delete[] key;
		}
		else if (inp == 3)
		{
			char* key = new char[7];
			std::cout << "Enter key to delete: ";
			std::cin >> key;
			key[6] = '\0';
			bool isDone = myh.erase(key);
			if (isDone)
			{
				std::cout << "Expression deleted.\n";
			}
			else
			{
				std::cout << "No such key in the database. No expressions deleted.\n";
				delete[] key;
			}
			//delete only when not existing. if existing - key is deleted with the box
		}
		else
			break;
	}
	
	return 0;
}