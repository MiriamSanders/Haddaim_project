#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>
enum FILE_STATUS { ERROR, CLOSED, OPEN_R, OPEN_W };
using namespace std;
int splitFile(string inputFile,string locationOutputFiles)
{
    int finalCountFiles = 1;
       // string inputFile = "../textfiles/logs.txt"; 
        ifstream inFile(inputFile);
        // Check if file is open
        if (!inFile) {
            cerr << "Error opening file " << inputFile << endl;
            return 0;//change this
        }

        string line;
        int lineCount = 0;
        int fileCount = 1;
        fstream iofile;

        // Read through the file line by line
        while (!inFile.eof()) {
            // If we've reached 500 lines, close the current output file and create a new one
            if (lineCount % 500 == 0) {
                if (iofile.is_open()) {
                    iofile.close();
                }
                // Create a new output file with a numbered suffix
               // string fileName = "../textfiles/segmentedTextFiles/log_segment" + to_string(fileCount) + ".txt";
                string fileName = locationOutputFiles+"/log_segment" + to_string(fileCount) + ".txt";
                iofile.open(fileName, ios::out);
                finalCountFiles++;
                fileCount++;
            }
            getline(inFile, line);
            // Write the line to the current output file
            iofile << line << endl;
            lineCount++;
        }

        // Close the last output file
        if (iofile.is_open()) {
            iofile.close();
        }

        inFile.close();

        cout << "File split completed successfully." << endl;
        return finalCountFiles;

}
unordered_map<string, int> countErrors(const string& filename) {
    ifstream file(filename);
    unordered_map<string, int> errorCounts;
    string line;

    if (!file.is_open()) {
        cerr << "Could not open the file!" << endl;
        return errorCounts; // Return empty map if file cannot be opened
    }

    while (getline(file, line)) {
        size_t errorPos = line.find("Error: ");
        if (errorPos != string::npos) {
            string error = line.substr(errorPos + 7); // Extract error type
            error.pop_back();
            errorCounts[error]++; // Count directly
        }
    }
    file.close();
    return errorCounts; // Return the map with error counts
}
unordered_map<string, int> mergeErrorCounts(int fileCount) {
  
    unordered_map<string, int> merged = countErrors("../textfiles/segmentedTextFiles/log_segment1.txt"); // Start with the first map
    for (int i = 2; i <= fileCount; i++)
    {
        string fileName = "../textfiles/segmentedTextFiles/log_segment"+ to_string(i) + ".txt";
        unordered_map<string, int> temp = countErrors(fileName);
        for (const auto& pair : temp) {
            merged[pair.first] += pair.second; // Merge counts
        }
    }
    return merged;
}
vector<pair<string, int>> getNLargest(const unordered_map<string, int>& errorCounts, int N) {
    vector<pair<string, int>> sortedErrors(errorCounts.begin(), errorCounts.end());

    // Sort the vector by value in descending order
    sort(sortedErrors.begin(), sortedErrors.end(), [](const pair<string, int>& a, const pair<string, int>& b) {
        return a.second > b.second; // Sort by count (highest first)
        });

    // Return only the top N elements
    if (N > sortedErrors.size()) N = sortedErrors.size(); // Prevent out-of-bounds
    return vector<pair<string, int>>(sortedErrors.begin(), sortedErrors.begin() + N);
}
void printNlargest(vector<pair<string, int>>& vec)
{
    for (const auto& pair : vec) {
        cout << pair.first << ": " << pair.second << endl;
    }
}
int main() {
    cout << "finding errors:"<<endl;
    int count=splitFile("../textfiles/logs.txt","../textfiles/segmentedTextFiles");
    unordered_map<string, int> merged= mergeErrorCounts(count);
    vector<pair<string, int>> Nlargest = getNLargest(merged, 3);
    printNlargest(Nlargest);
	return 0;
}
//run-time complexity: o(n) space complexity o(n) - need to explain why...