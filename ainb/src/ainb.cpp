#include "nin-io/ainb/Ainb.h"
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

AINB::AINB()
{
	isBigEndian = false;
}

AINB::~AINB()
{

}

void AINB::load(fstream& file)
{
	cout << "Loading AINB" << endl << endl;

	file.seekg(0, ios::beg);

	// Reading Header Data
	char type[4];
	file.read(type, 3);
	type[3] = '\0';

	// Check if file is AINB
	if (strcmp(type, "AIB") != 0) {
		cout << "File is not AINB" << endl;
		return;
	}

	int headerDataStart;
	readIntFromStream(file, isBigEndian, 1, headerDataStart);

	// find number of type A blocks
	file.seekg(0xC, ios::beg);
	int a_BlockCount;
	readIntFromStream(file, isBigEndian, 4, a_BlockCount);
	cout << "Type A Block Count: " << a_BlockCount << endl;

	// find number of type B blocks
	int b_BlockCount;
	readIntFromStream(file, isBigEndian, 4, b_BlockCount);
	cout << "Type B Block Count: " << b_BlockCount << endl;

	cout << endl;

	// find end address of block section
	file.seekg(0x20, ios::beg);
	int blockEndAddress;
	readIntFromStream(file, isBigEndian, 4, blockEndAddress);

	// find start of string list
	int stringListStartAddress;
	readIntFromStream(file, isBigEndian, 4, stringListStartAddress);


	// load footer
	loadFooter(file);

	// find file type (sequence, logic, etc)
	file.seekg(0x64, ios::beg);
	int fileType;
	readIntFromStream(file, isBigEndian, 4, fileType);

	if (fileType == 2) {
		cout << "File Type: Sequence" << endl;
	}
	else if (fileType == 1) {
		cout << "File Type: Logic" << endl;
	}
	else {
		cout << "File Type: Unknown" << endl;
	}

	cout << endl;

	// Start Parsing DataBlocks
	cout << "Loading Type A Blocks" << endl;
	file.seekg(0x74, ios::beg);

	vector<A_Block> aBlocks;
	for (int i = 0; i < a_BlockCount; i++) {
		A_Block aBlock = A_Block::load(file);
		aBlocks.push_back(aBlock);
	}

	cout << "Finished Loading Type A Blocks" << endl << endl;

	cout << "Loading Type B Blocks" << endl;

	vector<B_Block> bBlocks;
	for (int i = 0; i < b_BlockCount; i++) {
		B_Block bBlock = B_Block::load(file);
		bBlocks.push_back(bBlock);
	}

	cout << "Finished Loading Type B Blocks" << endl << endl;

	if (b_BlockCount != bBlocks.size()) {
		cout << "Error: Type B Block Count Mismatch" << endl;
		cout << "Expected: " << to_string(b_BlockCount) << endl;
		cout << "Actual: " << to_string(bBlocks.size()) << endl << endl;
		return;
	}


}

void AINB::loadFooter(fstream& file)
{
	int initialPosition = file.tellg();

	file.seekg(0x28, ios::beg);

	cout << "Loading Footer Address Data" << endl;

	// find end of footer
	int footerEndAddress;
	readIntFromStream(file, isBigEndian, 4, footerEndAddress);

	// find start of footer
	// also start of section one
	int footerStartAddress;
	readIntFromStream(file, isBigEndian, 4, footerStartAddress);

	// find start of section three
	int sectionThreeStartAddress;
	readIntFromStream(file, isBigEndian, 4, sectionThreeStartAddress);

	// find start of section two
	int sectionTwoStartAddress;
	readIntFromStream(file, isBigEndian, 4, sectionTwoStartAddress);
	// now at pos 0x38

	// find start of section four
	file.seekg(0x5c, ios::beg);
	int sectionFourStartAddress;
	readIntFromStream(file, isBigEndian, 4, sectionFourStartAddress);

	// find start of section five
	file.seekg(0x70, ios::beg);
	int sectionFiveStartAddress;
	readIntFromStream(file, isBigEndian, 4, sectionFiveStartAddress);

	file.seekg(initialPosition, ios::beg);

	cout << "Finished Loading Footer Address Data" << endl << endl;
	cout << "Section One: " << hex << footerStartAddress << " - " << sectionTwoStartAddress << endl;
	cout << "Section Two: " << hex << sectionTwoStartAddress << " - " << sectionThreeStartAddress << endl;
	cout << "Section Three: " << hex << sectionThreeStartAddress << " - " << sectionFourStartAddress << endl;
	cout << "Section Four: " << hex << sectionFourStartAddress << " - " << sectionFiveStartAddress << endl;
	cout << "Section Five: " << hex << sectionFiveStartAddress << " - " << footerEndAddress << endl;

	cout << endl;

	cout << "Loading Section One" << endl;
	file.seekg(footerStartAddress, ios::beg);

	struct SectionOneAddresses {
		int addressOne;
		int addressTwo;
		int addressThree;
		int addressFour;
		int addressFive;
		int addressSix;
	};

	SectionOneAddresses sectionOneAddresses;

	readIntFromStream(file, isBigEndian, 4, sectionOneAddresses.addressOne);
	readIntFromStream(file, isBigEndian, 4, sectionOneAddresses.addressTwo);
	readIntFromStream(file, isBigEndian, 4, sectionOneAddresses.addressThree);
	readIntFromStream(file, isBigEndian, 4, sectionOneAddresses.addressFour);
	readIntFromStream(file, isBigEndian, 4, sectionOneAddresses.addressFive);
	readIntFromStream(file, isBigEndian, 4, sectionOneAddresses.addressSix);

	cout << "Finished Loading Section One" << endl << endl;

	cout << "Loading Section Two" << endl;


}


