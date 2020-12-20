#include <iostream>
#include <string>
#include <algorithm>
#include <iterator>
#include <iomanip>

const int SEQUENCE_LENGTH = 10;
const int CONTROL_SEQUENCE_LENGTH = 4;
const int FULL_SEQUENCE_LENGTH = SEQUENCE_LENGTH + CONTROL_SEQUENCE_LENGTH;
const int CONTROL_SEQUENCE_POSITIONS[CONTROL_SEQUENCE_LENGTH] = { 1, 2, 4, 8 };

// int originSequence[SEQUENCE_LENGTH] = { 0, 0, 1, 1, 1, 0, 1, 0, 1, 0 };
// Значения для проверки

int originSequence[SEQUENCE_LENGTH] = { 0 };
int originControlSequence[CONTROL_SEQUENCE_LENGTH] = { 0 };
int originFullSequence[FULL_SEQUENCE_LENGTH] = { 0 };

// int transmittedFullSequence[FULL_SEQUENCE_LENGTH] = { 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 0 };
// Значения для проверки

int transmittedFullSequence[FULL_SEQUENCE_LENGTH] = { 0 };

int restoredSequence[SEQUENCE_LENGTH] = { 0 };
int restoredControlSequence[CONTROL_SEQUENCE_LENGTH] = { 0 };
int restoredFullSequence[FULL_SEQUENCE_LENGTH] = { 0 };

bool isControlSequencePosition(int value)
{
	return std::find(std::begin(CONTROL_SEQUENCE_POSITIONS), std::end(CONTROL_SEQUENCE_POSITIONS), value) != std::end(CONTROL_SEQUENCE_POSITIONS);
}

int getWrongBitPosition() {
	int position = 0;

	for (int i = 0; i < CONTROL_SEQUENCE_LENGTH; i++) {
		if (transmittedFullSequence[CONTROL_SEQUENCE_POSITIONS[i] - 1] != restoredControlSequence[i]) {
			position += CONTROL_SEQUENCE_POSITIONS[i];
		}
	}

	return position;
}

void fillSequence(int sequence[]) {
	for (int i = 0; i < (sizeof(sequence) / sizeof(*sequence)); i++) {
		std::string bit;
		bool bitCorrect = false;

		while (!bitCorrect) {
			std::cout << i + 1 << " bit: ";
			std::cin >> bit;

			if (stoi(bit) != 0 && stoi(bit) != 1) {
				std::cout << "Incorrect value! Bit value must be: 0, 1.\n";
			}
			else {
				originSequence[i] = stoi(bit);
				bitCorrect = true;
			}
		}
	}
}

void calculateSequence(int fullSequence[], int sequence[]) {
	int counter = 0;

	for (int i = 1; i <= FULL_SEQUENCE_LENGTH; i++) {
		if (!isControlSequencePosition(i)) {
			sequence[counter] = fullSequence[i - 1];
			counter++;
		}
	}
}

void calculateFullSequence(int fullSequence[], int sequence[]) {
	int counter = 0;
	int controlCounter = 0;
	int controlSequence[CONTROL_SEQUENCE_LENGTH] = { 0 };

	for (int i = 1; i <= FULL_SEQUENCE_LENGTH; i++) {
		if (isControlSequencePosition(i)) {
			fullSequence[i - 1] = controlSequence[controlCounter];
			controlCounter++;
		}
		else {
			fullSequence[i - 1] = sequence[counter];
			counter++;
		}
	}
}

void calculateControlSequence(int fullSequence[], int controlSequence[]) {
	for (int i = 0; i < CONTROL_SEQUENCE_LENGTH; i++) {
		bool flag = false;
		int onesCount = 0;

		for (int j = 1; j <= FULL_SEQUENCE_LENGTH; j++) {
			if (j % CONTROL_SEQUENCE_POSITIONS[i] == 0) {
				flag = !flag;
			}

			if (flag && fullSequence[j - 1] == 1) {
				onesCount++;
			}
		}

		int controlBit = onesCount % 2 == 0 ? 0 : 1;

		controlSequence[i] = controlBit;
		fullSequence[CONTROL_SEQUENCE_POSITIONS[i] - 1] = controlBit;
	}
}

void printTable() {
	int counter = 1;

	std::cout << "\n i:";

	for (int i = 1; i <= FULL_SEQUENCE_LENGTH; i++) {
		if (isControlSequencePosition(i)) {
			std::cout << std::setw(3) << "";
		}
		else {
			std::cout << std::setw(3) << counter;
			counter++;
		}
	}

	std::cout << "\n j:";

	for (int i = 1; i <= FULL_SEQUENCE_LENGTH; i++) {
		std::cout << std::setw(3) << i;
	}

	for (int i = 0; i < CONTROL_SEQUENCE_LENGTH; i++) {
		bool flag = false;

		std::cout << "\na" << i << ":";

		for (int j = 1; j <= FULL_SEQUENCE_LENGTH; j++) {
			if (j % CONTROL_SEQUENCE_POSITIONS[i] == 0) {
				flag = !flag;
			}

			std::cout << std::setw(3) << flag ? 1 : 0;
		}
	}

	counter = 0;

	std::cout << "\n b:";

	for (int i = 1; i <= FULL_SEQUENCE_LENGTH; i++) {
		if (isControlSequencePosition(i)) {
			std::cout << std::setw(3) << "";
		}
		else {
			std::cout << std::setw(3) << originSequence[counter];
			counter++;
		}
	}

	counter = 0;

	std::cout << "\n B:";

	for (int i = 1; i <= FULL_SEQUENCE_LENGTH; i++) {
		if (isControlSequencePosition(i)) {
			std::cout << std::setw(3) << originControlSequence[counter];
			counter++;
		}
		else {
			std::cout << std::setw(3) << "";
		}
	}

	std::cout << "\nbt:";

	for (int i = 0; i < FULL_SEQUENCE_LENGTH; i++) {
		std::cout << std::setw(3) << originFullSequence[i];
	}
}

int main()
{
	std::cout << "This program implements a data encryption algorithm using the Hamming Code method.\n";
	std::cout << "Enter origin sequence bits. Bit value must be: 0, 1.\n\n";

	fillSequence(originSequence);

	calculateFullSequence(originFullSequence, originSequence);
	calculateControlSequence(originFullSequence, originControlSequence);

	printTable();

	std::cout << "\n\nThen enter transmitted full sequence bits. Bit value must be: 0, 1.\n\n";

	fillSequence(transmittedFullSequence);

	calculateSequence(transmittedFullSequence, restoredSequence);
	calculateFullSequence(restoredFullSequence, restoredSequence);
	calculateControlSequence(restoredFullSequence, restoredControlSequence);

	int wrongBitPosition = getWrongBitPosition();

	wrongBitPosition != 0
		? std::cout << "\n\nWrong bit position: " << wrongBitPosition
		: std::cout << "\n\nTransmitted sequence is correct!";

	std::cout << "\n\nPress any key to exit...";
	std::cin.get();

	return 0;
}