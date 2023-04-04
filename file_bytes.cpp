#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

int main()
{
    string inputFile = "in.mp4";
    string outputFile = "out.mp4";
    ifstream infile(inputFile, ios_base::binary);


    vector <char> v((istreambuf_iterator <char> (infile)), istreambuf_iterator <char> ());

    ofstream outfile(outputFile, ios_base::binary);
    copy(v.begin(), v.end(), ostreambuf_iterator <char> (outfile));
}
