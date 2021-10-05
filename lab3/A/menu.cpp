#include "menu.h" 
#include "table.h" 

const char *FUNCS[] = {"0. Quit",
					   "1. Add element",
                       "2. Find element",
                       "3. Find information",
                       "4. Delete element",
                       "5. Print table",
                   	   "6. Refresh table"};

const int FUNCS_SIZE = sizeof(FUNCS) / sizeof(FUNCS[0]);

int dialog(const char *funcs[], int n)
{
	const char *err = "- MENU - ";
	int choice;
	do {
		std::cout << std::endl << err << std::endl;
		err = "Incorrect input. Try again...";
		for (int i = 0; i < n; i++) {
			std::cout << funcs[i] << std::endl;
		}
		std::cout << "\nMake a choice: ";
		choice = getchar() - '0';
		while (getchar() != '\n') {}
	} while (choice < 0 || choice >= n);
	return choice;
}

input get_key(int &new_key)
{
	do {
		std::cout << "\nKey: ";
		std::cin >> new_key;
		if (std::cin.bad()) { return CRASH; }
		if (std::cin.eof()) { return END_OF_FILE; }
		if (std::cin.fail()) { 
			std::cin.clear(); 
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			continue; 
		}
		std::cin.ignore(32767, '\n');
	} while (false);
	return GOOD;
}

void menu()
{
	Table table;
	int c = 0, key;
	do {
		c = dialog(FUNCS, FUNCS_SIZE);
		switch(c) {
			case 0:
				break;
			case 1:
				try {
					table.input_item(std::cout, std::cin, stdin);
				} catch(std::exception &e) {
					std::cout << std::endl << e.what() << std::endl;
				}
				break;
			case 2:
			{
				Item item;
				if (get_key(key) != GOOD) {
					std::cout << "\nEnd of file or input crash...\n";
					c = 0;
					break;
				}
				if (table.search_item(item, key) == SUCCESS) {
					std::cout << "\nItem is found!\n";
				} else {
					std::cout << "\nWrong key!\n";
				}
				break;
			}
			case 3:
			try {
				char info[N_CHAR];
				if (get_key(key) != GOOD) {
					std::cout << "\nEnd of file or input crash...\n";
					c = 0;
					break;
				}
				if (table.search_info(info, key, N_CHAR - 1) == SUCCESS) {
					std::cout << "\nInformation: " << info << std::endl;
				} else {
					std::cout << "\nWrong key!\n";
				}
				break;
			} catch(std::exception &e) {
				std::cout << e.what() << std::endl;
			}
			case 4:
				if (get_key(key) != GOOD) {
					std::cout << "\nEnd of file or input crash...\n";
					c = 0;
					break;
				}
				if (table.delete_item(key) != SUCCESS) {
					std::cout << "\nWrong key!\n";
				}
				break;
			case 5:
				table.output_table(std::cout);
				break;
			case 6:
				table.refresh();
				break;
		}
	} while (c != 0);
}