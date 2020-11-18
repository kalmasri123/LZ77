#include <iostream>
#include <vector>
#include <cstddef>
using namespace std;

struct stringTuple
{
    bool foundBefore;
    char foundChar;
    int offset;
    unsigned int length;
    // int found;
};
struct searchQuery
{

    int offset;
    unsigned int length;
    bool found;
    string queryString;
};
searchQuery findTuple(string foundString, vector<stringTuple> comp)
{
    string strBuffer;
    int charIndex = 0;
    int offset = 0;
    for (auto it = std::begin(comp); it != std::end(comp); ++it, offset++)
    {
        if ((*it).foundChar == foundString[charIndex])
        {

            strBuffer.push_back(foundString[charIndex]);
            charIndex++;

            if (strBuffer.compare(foundString) == 0)
            {
                searchQuery temp = {
                    offset,
                    strBuffer.length(), true, strBuffer};
                return temp;
            }
        }
        else
        {
            strBuffer.clear();
            charIndex = 0;
        }
        // std::cout << (*it).foundChar << "\n";
    }

    return {0, 0, false, ""};
}

vector<stringTuple> convertToTuples(std::string str)
{
    vector<stringTuple> tupleVector;
    // for (string::size_type i = 0; i < str.size(); i++)
    string::size_type i = 0;
    while (i < str.size())
    {
        string s;
        s.push_back(str[i]);
        searchQuery result = findTuple(s, tupleVector);
        int j = 0;
        searchQuery previous = result;
        // cout << "INDEX" << i << endl;
        if (!result.found)
        {
            cout << "NOT FOUND" << '\t' << s << endl;
            struct stringTuple temp = {false, str[i], 0, 0};

            tupleVector.push_back(temp);
            i++;

            continue;
        }
        else
        {
            cout << "FOUND RESULT " << str[i] << endl;
        }
        while (result.found && ((j + 1) + i < str.size()))
        {
            // cout << (j + 1) + i << "\t" << str.size() << endl;
            s.push_back(str[++j + i]);
            // cout << "STRING" << s << endl;
            previous = result;
            result = findTuple(s, tupleVector);
        }
        int offset = (j) + i - previous.offset - previous.length;
        struct stringTuple temp = {
            true,
            str[j + i],
            offset,
            previous.length};
        tupleVector.push_back(temp);
        i += previous.length;

        // cout << str[i];
    }
    return tupleVector;
};

int main(void)
{
    std::string str = "Hello x Hello 335335";
    vector<stringTuple> tuples = convertToTuples(str);
    for (auto it = std::begin(tuples); it != std::end(tuples); ++it)
    {
        cout << '<' << (*it).foundBefore << ',' << (*it).offset << ',' << (*it).length << (*it).foundChar << '>' << endl;
        // std::cout << str;
    }
    return 0;
}