// Test.cpp : Defines the entry point for the console application.
//

#include <iomanip>
#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <conio.h>
#include "DiffieHellmanKeysExchanger.h"

void PrintBuffer(const std::vector<char>& buffer)
{
    for (size_t i = 0; i < buffer.size(); ++i)
    {
        std::cout << "0x" << std::setbase(16) << std::setw(2) << std::setfill('0') << (int)(unsigned char)buffer[i] << " ";
        if ((i + 1) % 8 == 0)
        {
            std::cout << std::endl;
        }
    }
}

int _tmain(int argc, _TCHAR* argv[])
{
    std::cout << "Diffie-Hellman key exchange alogrithm example:\n";
	const size_t PModuleLength = 32;

	// Module P (The bytes representation of number 0x24357685944363427687549776543601)
	unsigned char buffer[PModuleLength] = {0xEE, 0x38, 0x6B, 0xFB, 
                                           0x5A, 0x89, 0x9F, 0xA5, 
                                           0xAE, 0x9F, 0x24, 0x11, 
                                           0x7C, 0x4B, 0x1F, 0xE6,
                                           0x49, 0x28, 0x66, 0x51, 
                                           0xEC, 0xE6, 0x53, 0x81, 
                                           0xFF, 0xFF, 0xFF, 0xFF, 
                                           0xFF, 0xFF, 0xFF, 0xFF
                                           };
    std::vector<char> cryptoPModule((char*)buffer, (char*)buffer + PModuleLength);
	
    // Module G
    unsigned long cryptoGModule = 0x02;

    // Keys exchanger for Alice
	DiffieHellmanLib::DiffieHellmanKeysExchanger<PModuleLength> aliceExchanger(cryptoPModule, cryptoGModule);

    // Keys exchanger for Bob
	DiffieHellmanLib::DiffieHellmanKeysExchanger<PModuleLength> bobExchanger(cryptoPModule, cryptoGModule);

    // Generation of exchange keys for Alice and Bob
    std::cout << "\nGeneration of exchange key for Alice...\n";
    std::vector<char> aliceExchangeKey;
	aliceExchanger.GenerateExchangeData(aliceExchangeKey);
    std::cout << "Exchange key for Alice is equal:\n";
    PrintBuffer(aliceExchangeKey);

    std::cout << "\nGeneration of exchange key for Bob...\n";
	std::vector<char> bobExchangeKey;
	bobExchanger.GenerateExchangeData(bobExchangeKey);
    std::cout << "Exchange key for Bob is equal:\n";
    PrintBuffer(bobExchangeKey);

    // Calculation if shared keys for Alice and Bob
    std::cout << "\nCalculation of shared key for Alice...\n";
	std::vector<char> aliceSharedKey;
	aliceExchanger.CompleteExchangeData(bobExchangeKey, aliceSharedKey);
    std::cout << "Shared key for Alice is equal:\n";
    PrintBuffer(aliceSharedKey);

    std::cout << "\nCalculation of shared key for Bob...\n";
	std::vector<char> bobSharedKey;
	bobExchanger.CompleteExchangeData(aliceExchangeKey, bobSharedKey);
    std::cout << "Shared key for Bob is equal:\n";
    PrintBuffer(bobSharedKey);

    if (aliceSharedKey == bobSharedKey)
    {
        std::cout << "\nOperation finished successfully: shared keys for Alice and Bob are equal.";
    }
    else
    {
        std::cout << "\nOperation failed: shared keys for Alice and Bob are different.";
    }

    _getch();

	return 0;
}

