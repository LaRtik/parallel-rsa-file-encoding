#include <iostream>
#include <cmath>
#include <omp.h>
#include <cstring>
#include <cstdlib>
#include <fstream>

using namespace std;

#define MESSAGE_SIZE 100000


// calculate co-prime numbers
unsigned int gcd(unsigned int a, unsigned int b)
{
	unsigned int x;
	while (b)
	{
		x = a % b;
		a = b;
		b = x;
    }
    return a;
}

int main(int argc, char *argv[])
{
	unsigned int message[MESSAGE_SIZE];
	long double encryptedMessage[MESSAGE_SIZE];


    ifstream fInput("file.txt");
    int thread_count = 10;

    // Checking the input file exists in the directory or not
    if (!fInput)
	{
		cout << "File does not exists.";
		return -1;
	}

	unsigned int iP, iQ, iD, iE, iN, iTotientN;

	fInput >> iP >> iQ;

	int i = 0;
	char cItem;

	while(fInput >> cItem)
	{
		message[i] = cItem - '0';
		i++;
	}

	// Done reading from the file
	fInput.close();


	cout << "P: " << iP << endl;
	cout << "Q: " << iQ << endl;

	//Compute the coefficients
	iN = iP * iQ;
	iP -= 1;
	iQ -= 1;
	iTotientN = iP * iQ;

	iE = 7;
	while(i < (iN - 1))
	{
		if(gcd(iE, iTotientN) == 1)
			break;

		iE++;
	}

	i = 1;
	while(i < iN)
	{
		iD = (iE * i - 1) % iTotientN;
		if(!iD)
		{
			iD = i;
			break;
		}
		i++;
	}

	// RSA Encryption
	cout << "\n------ Encrypted message -------" << endl;
	cout << "E: " << iE << endl;
	cout << "N: " << iN << endl;

	// Assign partition variables
	int iChunk = iE / thread_count;
    int iChunkRemainder = iE % thread_count;
    int startIndex, endIndex;
    long double dTemp;

	for(i = 0; i < MESSAGE_SIZE; i++)
	{
		encryptedMessage[i] = 1;

		// Getting into the parallel
		#pragma omp parallel firstprivate(startIndex, endIndex) num_threads(thread_count)
		{
			int myRank = omp_get_thread_num();

			dTemp = 1;

	        startIndex = myRank * iChunk;
	        if(iChunkRemainder && (startIndex + iChunk) > iE)
	            endIndex = iChunkRemainder;
	        else if (iChunkRemainder && myRank == thread_count - 1 && (startIndex + iChunk) < iE)
	        	endIndex = startIndex + iChunk + iChunkRemainder;
	        else
	            endIndex = startIndex + iChunk;

	        for(int index = startIndex; index < endIndex; index++)
	        	dTemp = dTemp * message[i];


	        encryptedMessage[i] = encryptedMessage[i] * dTemp;
	    }

	    encryptedMessage[i] = fmod(encryptedMessage[i], iN);

	    cout << encryptedMessage[i] << " ";
	}

	// RSA Decryption
	cout << "\n\n------ Decrypted message -------" << endl;
	cout << "D: " << iD << endl;
	cout << "N: " << iN << endl;

	// Computing partition variables
	iChunk = iD / thread_count;
    iChunkRemainder = iD % thread_count;

	for(i = 0; i < MESSAGE_SIZE; i++)
	{
		long double decryptedText = 1;

		//Getting parallel region
		#pragma omp parallel firstprivate(startIndex, endIndex) num_threads(thread_count)
		{
			int myRank = omp_get_thread_num();

			dTemp = 1;

			// Computing two varibles to buffer through the array which are derivatives from myRank variable
	        startIndex = myRank * iChunk;
	        if(iChunkRemainder && (startIndex + iChunk) > iD)
	            endIndex = iChunkRemainder;
	        else if (iChunkRemainder && myRank == thread_count - 1 && (startIndex + iChunk) < iD)
	        	endIndex = startIndex + iChunk + iChunkRemainder;
	        else
	            endIndex = startIndex + iChunk;

	        for(int index = startIndex; index < endIndex; index++)
	        	dTemp = dTemp * encryptedMessage[i];

	        /*if(i < 2)
	        	cout << "Start Index: " << startIndex << " | End Index: " << endIndex << " | Process: " << myRank << " | Result: " << dTemp << endl;*/

	        decryptedText = decryptedText * dTemp;
	    }

	    decryptedText = fmod(decryptedText, iN);

	    cout << decryptedText << " ";
  	}
    return 0;
}
