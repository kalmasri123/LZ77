#include <iostream>
#include <vector>
#include <cstddef>
#include <fstream>
#include <string>

using namespace std;

struct stringTuple
{
    string foundChar;
    int offset;
    int length;
    bool found;
};

struct searchQuery
{

    int offset;
    int length;
    bool found;
    string queryString;
};
// bool charExists(char c, vector<stringTuple> stringTuple)
// {
//     for (string::size_type i = 0; i < stringTuple.size(); i++)
//     {
//         if (c == stringTuple[i].foundChar && stringTuple[i].offset == 0 && stringTuple[i].length == 0)
//         {
//             return true;
//         }
//     }
//     return false;
// }
string decompressTuples(vector<stringTuple> tuples, string fileName)
{
    string newStr = "";
    std::ofstream outfile(fileName, std::ofstream::binary);
    for (int i = 0; i < tuples.size(); i++)
    {
        string sub = newStr.substr(newStr.size() - tuples[i].offset, tuples[i].length);
        if (tuples[i].foundChar[0] == 0)
        {
            newStr = newStr + sub;
        }
        else
        {
            newStr = newStr + sub + tuples[i].foundChar;
        }
    }
    cout << "NEW STRING: " << (int)newStr[newStr.size() - 1] << endl;
    outfile << newStr;
    return newStr;
}
string compressTuples(vector<stringTuple> tuples, string fileName)
{
    std::ofstream outfile(fileName, std::ofstream::binary);

    string newStr = "";
    for (int i = 0; i < tuples.size(); i++)
    {
        char offsetAndLength;
        char nextChar = tuples[i].foundChar[0];
        offsetAndLength = (char)((tuples[i].length << 4) | (tuples[i].offset));
        // newStr+=nextChar;
        // newStr+= offsetAndLength;
        outfile << nextChar;
        outfile << offsetAndLength;
    }
    return newStr;
}
vector<stringTuple> decompress(string str)
{
    vector<stringTuple> emptyTuple;
    for (int i = 0; i < str.size(); i += 2)
    {
        string foundChar = (i >= str.size()) ? "" : std::string(1, str[i]);
        char offsetAndLength = str[i + 1];
        int offset = offsetAndLength & 0xF;
        int length = (offsetAndLength & 0xF0) >> 4;
        stringTuple temp = {foundChar, offset, length, !(offset == 0 && length == 0)};
        emptyTuple.push_back(temp);
    }
    return emptyTuple;
}
searchQuery findTuple(string query, string comparison, int queryIndex)

// searchQuery findTuple(string query, vector<stringTuple> comparison, int queryIndex)
{
    std::string buffer;

    int charIndex = query.size() - 1;
    int offset = 0;

    for (int i = queryIndex; i >= 0; i--, offset++)
    {
        if (offset >= 10)
        {
            // cout << "REACHED MAX OFFSET" << endl;
            break;
        }
        char compChar = comparison[i];
        char queryChar = query[charIndex];
        if (comparison[i] == query[charIndex])
        {

            buffer.insert(0, 1, query[charIndex]);
            charIndex--;

            if (buffer.compare(query) == 0)
            {
                if (buffer.size() >= 10)
                {
                    // cout << "REACHED MAX LENGTH" << endl;
                    break;
                }
                searchQuery temp = {
                    offset + 1,
                    buffer.size(), true, buffer};
                return temp;
            }
        }
        else
        {
            buffer.clear();
            charIndex = query.size() - 1;
        }
    }
    return {0, 0, false, ""};
}

vector<stringTuple> convertToTuples(std::string str)
{
    vector<stringTuple> tupleVector;
    int i = 0;
    while (i < str.size())
    {
        string buff;
        buff.push_back(str[i]);
        searchQuery result = findTuple(buff, str, i - 1);
        searchQuery previous = result;

        if (!result.found)
        {
            stringTuple temp = {std::string(1, str[i]), 0, 0, false};
            tupleVector.push_back(temp);
            i++;

            continue;
        }
        else
        {
            int j = 0;
            while (result.found && ((j + i) < str.size()))
            {
                cout << "FOUND";
                j++;
                if (j + i == str.size())
                {
                    previous = result;
                    break;
                }
                buff.push_back(str[j + i]);
                previous = result;
                result = findTuple(buff, str, i + j - buff.size());
            }
            int offset = previous.offset;
            string s = ((j + i) < str.size()) ? std::string(1, str[i + j]) : "";
            stringTuple temp = {s, offset, previous.length, true};
            tupleVector.push_back(temp);
            i += buff.size();
        }
    }
    return tupleVector;
};

int main(int argc, char **argv)
{
    if (argc < 4)
    {
        cout << "Not enough Arguments." << endl;
        return 1;
    }
    //argv[1]: mode; argv[2] infile; argv[3] outfile;

    switch (argv[1][0])
    {
    case 'c':
    {
        ifstream infile(argv[2]);
        string str = "";
        string line;
        if (infile.is_open())
        {
            while (getline(infile, line))
            {
                str += line + '\n';
            }
            infile.close();
        }

        else
            cout << "Unable to open file";

        vector<stringTuple> tuples = convertToTuples(str);
        for (auto it = std::begin(tuples); it != std::end(tuples); ++it)
        {
            cout << '<' << (*it).found << ',' << (*it).offset << ',' << (*it).length << (*it).foundChar << '>' << endl;
            // std::cout << str;
        }
        string newStr = compressTuples(tuples, argv[3]);

        // std::cout << newStr << "\t MATCHES: " << (newStr.compare(str) == 0) << endl;
        break;
    }
    case 'd':
        string str = "";
        ifstream infile(argv[2]);

        char c;
        while (infile.get(c)) // loop getting single characters
            str += c;
        vector<stringTuple> tuples = decompress(str);
        std::ofstream outfile(argv[3], std::ofstream::binary);
        string newStr = decompressTuples(tuples, argv[3]);
        break;
    }
    return 0;
}