// FormalElement.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


struct directMappedCacheEntry {
	unsigned short int tag;
	bool valid, lru;
	unsigned char data[4];
};

struct associativeCacheEntry {
	unsigned int tag;
	bool valid;
	unsigned char data[4];
};

struct twoWayAssociativeCacheEntry {
	unsigned int tag;
	bool valid;
	unsigned char data[4];
};

int main()
{
	unsigned int cpuAddr, tag, setNumber, hitCount;
	hitCount = 0;
	twoWayAssociativeCacheEntry setAssociativeCache[100];

	for (int a = 0; a < 100; a++) {
		setAssociativeCache[a].tag = 0;
		setAssociativeCache[a].valid = false;
	}

	ifstream addressFile;
	addressFile.open("addresses.txt");
	while (!addressFile.eof()) {
		addressFile >> hex >> cpuAddr;

		tag = cpuAddr >> 16;
		setNumber = (cpuAddr & 0x0000f000) >> 12;
		setNumber = setNumber * 50;
		
		
		for (int i = setNumber; i < setNumber + 50; i++) {
			if (setAssociativeCache[i].tag == tag && setAssociativeCache[i].valid){
				hitCount++;
				cout << "Hit" << endl;
				cout << "CPU address: " << cpuAddr << endl;
				break;
			}
			else if(setAssociativeCache[i].tag == 0) {
				setAssociativeCache[i].tag = tag;
				setAssociativeCache[i].valid = true;
				break;
			}
		}

		
	}

	cout << "Total hits: " << hitCount << endl;
	return 0;

}


int AssociativeCache() {
	unsigned int cpuAddr, fullAddress, lineNumber, byteOffset, hitCount;
	hitCount = 0;
	associativeCacheEntry associativeCache[100];

	for (int a = 0; a < 100; a++) {
		associativeCache[a].tag = 0;
		associativeCache[a].valid = false;
	}

	ifstream addressFile;
	addressFile.open("addresses.txt");
	while (!addressFile.eof()) {
		addressFile >> hex >> cpuAddr;

		fullAddress = cpuAddr >> 24;
		lineNumber = cpuAddr & 0x3;

		for (int i = 0; i < lineNumber; i++) {
			if (associativeCache[i].tag == fullAddress && associativeCache[i].valid) {
				hitCount++;
				cout << "Hit" << endl;
				cout << "CPU address: " << cpuAddr << endl;
				break;
			}
			else if (associativeCache[i].tag == 0) {
				associativeCache[lineNumber].tag = fullAddress;
				associativeCache[lineNumber].valid = true;
				break;
			}
		}

	}

	cout << "Total hits: " << hitCount << endl;
	return 0;

}

int directMapping() {
	unsigned int cpuAddr, upperAddress, lowerAddress, hitCount;
	hitCount = 0;
	directMappedCacheEntry directMapCache[100];

	for (int a = 0; a < 100; a++) {
		directMapCache[a].tag = 0;
		directMapCache[a].valid = false;
	}

	ifstream addressFile;
	addressFile.open("addresses.txt");
	while (!addressFile.eof()) {
		addressFile >> hex >> cpuAddr;

		upperAddress = cpuAddr >> 24;
		lowerAddress = (cpuAddr & 0x00ff0000) >> 16;


		if (directMapCache[lowerAddress].tag == upperAddress && directMapCache[lowerAddress].valid == true) {
			hitCount++;
			cout << "Hit" << endl;
			cout << "CPU address: " << cpuAddr << endl;
		}
		else {
			directMapCache[lowerAddress].tag = upperAddress;
			directMapCache[lowerAddress].valid = true;
			directMapCache[lowerAddress].lru = true;
		}

	}


	cout << "Total hits: " << hitCount << endl;
	return 0;
}