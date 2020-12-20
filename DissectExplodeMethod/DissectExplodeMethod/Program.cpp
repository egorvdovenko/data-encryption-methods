#include <iostream>
#include <string>

using std::string;
using std::size_t;

const int BLOCK_COUNT = 14;
const int COLUMN_COUNT = 2;
const int ROW_COUNT = 7;

int columnsKeys[COLUMN_COUNT] = {};
int rowsKeys[ROW_COUNT] = {};

string encryptionTable[BLOCK_COUNT] = {};

bool checkKey(int key, int keys[], int keyCount) {
	bool isFromRange = false;
	bool isDuplicate = false;

	for (int i = 0; i < keyCount; i++) {
		if (key == i + 1) {
			isFromRange = true;
		}
		if (key == keys[i]) {
			isDuplicate = true;
		}
	}

	return isFromRange && !isDuplicate;
}

int getBlockNumber(int i) {
	int columnIndex = i - (i / COLUMN_COUNT) * COLUMN_COUNT;
	int rowIndex = i - (i / ROW_COUNT) * ROW_COUNT;

	int columnKey = columnsKeys[columnIndex];
	int rowKey = rowsKeys[rowIndex];

	return COLUMN_COUNT * (rowKey - 1) + columnKey;
}

int getMode() {
	int mode = 0;

	while (mode != 1 && mode != 2) {
		std::cout << "First select the mode (1 - Encryption, 2 - Decryption): ";
		std::cin >> mode;

		if (mode != 1 && mode != 2) {
			std::cout << "Incorrect value! Mode must be 1 or 2.\n";
		}
	}

	return mode;
}

void fillColumnsKeys() {
	std::cout << "\nThen enter the columns keys. Key value must be: 1, 2.\n\n";

	for (int i = 0; i < COLUMN_COUNT; i++) {
		string key;
		bool keyCorrect = false;

		while (!keyCorrect) {
			std::cout << i + 1 << " column key: ";
			std::cin >> key;

			if (!checkKey(stoi(key), columnsKeys, COLUMN_COUNT)) {
				std::cout << "Incorrect value! Key value must be: 1, 2.\n";
			}
			else {
				columnsKeys[i] = stoi(key);
				keyCorrect = true;
			}
		}
	}
}

void fillRowsKeys() {
	std::cout << "\nThen enter the rows keys. Key value must be: 1, 2, 3, 4, 5, 6, 7.\n\n";

	for (int i = 0; i < ROW_COUNT; i++) {
		string key;
		bool keyCorrect = false;

		while (!keyCorrect) {
			std::cout << i + 1 << " row key: ";
			std::cin >> key;

			if (!checkKey(stoi(key), rowsKeys, ROW_COUNT)) {
				std::cout << "Incorrect value! Key value must be: 1, 2, 3, 4, 5, 6, 7.\n";
			}
			else {
				rowsKeys[i] = stoi(key);
				keyCorrect = true;
			}
		}
	}
}

void encryption() {
	string phrase = "";

	std::cout << "\nThen enter a phrase: ";
	std::cin >> phrase;

	for (int i = 0; i < phrase.length(); i++) {
		int blockNumber = getBlockNumber(i);

		encryptionTable[blockNumber - 1] += phrase[i];
	}

	std::cout << "\nEncryption table:\n\n";

	for (int i = 0; i < BLOCK_COUNT; i++) {
		std::cout << "block " << i + 1 << ": " << encryptionTable[i] << "\n";
	}
}

void decryption() {
	std::cout << "\nThen enter a encryption table values: ";

	for (int i = 0; i < BLOCK_COUNT; i++) {
		std::cout << "block " << i + 1 << ": ";
		std::cin >> encryptionTable[i];
	}

	int phraseLength = 0;

	for (int i = 0; i < BLOCK_COUNT; i++) {
		phraseLength += encryptionTable[i].length();
	}

	char* phrase = new char[static_cast<size_t>(phraseLength)]{};

	for (int i = phraseLength - 1; i >= 0; i--) {
		int blockNumber = getBlockNumber(i);
		int blockLength = encryptionTable[blockNumber - 1].length();

		phrase[i] = encryptionTable[blockNumber - 1][blockLength - 1];

		if (blockLength > 1) {
			encryptionTable[blockNumber - 1].erase(blockLength - 1, 1);
		}
	}

	std::cout << "\nDecrypted phrase:\n\n";

	for (int i = 0; i < phraseLength; i++) {
		std::cout << phrase[i];
	}
}

int main()
{
	setlocale(LC_CTYPE, "rus");

	std::cout << "This program implements a data encryption algorithm using the dissect-explode method.\n\n";

	fillColumnsKeys();
	fillRowsKeys();

	getMode() == 1
		? encryption()
		: decryption();

	std::cout << "\n\nPress any key to exit...";

	std::cin.get();

	return 0;
}