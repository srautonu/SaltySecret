//
// This module is used to evaluate user experience of salty secret
// 
// Author - Wasifur Rahman
// Contributor - Saifur Rahman
//
// Input file: inputData.txt
// Format:
//      <Total length of password>
//      <Secret_text>
//      <Number_of_salt_segments>
//      <Start_position_of_salt_1 size_of_salt_1>
//      ...
//      <Start_position_of_salt_n size_of_salt_n>
//

#include<bits/stdc++.h>

using namespace std;
typedef pair<int,int> ii;

class User
{
public:
    int id;
    int totPassLen;
    int numRandParts;
    int randLen = 0;
    string password;
    vector<ii>randParts;
};

vector<User> users;

ifstream fin("inputData.txt");

void showUser(int index)
{
    User curUser = users[index];

    cout << "id: " << curUser.id << endl;
    cout << "pass: " << curUser.password << endl;
    cout << "totLen: " << curUser.totPassLen << endl;
    cout << "randParts:\n";
    for(int i = 0; i < curUser.numRandParts; i++)
    {
        cout << curUser.randParts[i].first << " " << curUser.randParts[i].second << endl;
    }
    cout << endl << endl;
}

void addNewUser(int index)
{
    User newUser;
    int fromInd, randPartLen;

    fin >> newUser.totPassLen;
    fin >> newUser.password;
    newUser.id = index;
    newUser.randLen = 0;
    fin >> newUser.numRandParts;

    for(int i = 0; i < newUser.numRandParts; i++)
    {
        fin >> fromInd >> randPartLen;
        newUser.randParts.push_back(ii(fromInd, randPartLen));
        newUser.randLen += randPartLen;
    }

    users.push_back(newUser);

    showUser(index);
}

bool matchPass(int index,string userPass)
{
    if(index >= users.size())
    {
        cout << "This user does not exist\n";
        return false;
    }

    cout << "user entered: " << userPass << endl;

    User curUser = users[index];
    if(userPass.size() != curUser.totPassLen)
    {
        cout << "password length mismatch\n";
        return false;
    }
    int N = curUser.totPassLen;
    vector<bool> randomNons(N);

    for(int i=0; i<curUser.numRandParts; i++)
    {
        int fromInd = curUser.randParts[i].first, randLen = curUser.randParts[i].second;
        for(int i=0; i<randLen; i++)randomNons[fromInd + i]=true;
    }

    string matchedPass;
    for(int i = 0; i<N; i++)
    {
        if(!randomNons[i])
            matchedPass += userPass[i];
    }
    if(matchedPass == curUser.password)
    {
        cout << "password matched\n";
    }
    else
    {
        cout << "password mismatched\n";
        return false;
    }
    cout << matchedPass << endl;
    return true;
}

int numMismatch(string recoredPass,string enteredPass)
{
    if(recoredPass.size() != enteredPass.size())return -1;
    int mismatchCount = 0;
    for(int i = 0; i < enteredPass.size(); i++)
    {
        if(recoredPass[i] != enteredPass[i])
            mismatchCount++;
    }

    return mismatchCount;
}

bool fullMatch(string recordedPass, string enteredPass, int userId)
{
    int misMatchCount = numMismatch(recordedPass,enteredPass);
    int randLen = users[userId].randLen;
    cout << misMatchCount << " mismatches in " << randLen << " length\n";
    return misMatchCount == randLen;
}

bool partialMatch(string recordedPass,string enteredPass,int userId)
{
    vector<ii> intervals = users[userId].randParts;

    for(int i = 0; i < intervals.size(); i++)
    {
        int beginIndex = intervals[i].first, length = intervals[i].second;
        bool misMatch = false;
        for(int j = 0; j < length; j++)
        {
            if(enteredPass[beginIndex + j] != recordedPass[beginIndex+j])
            {
                misMatch = true;
                break;
            }
        }
        if(!misMatch)
            return false;
    }

    return true;
}

bool quarterMatch(string recordedPass, string enteredPass, int userId)
{
    int misMatchCount = numMismatch(recordedPass,enteredPass);
    int randLen = users[userId].randLen;
    cout << misMatchCount << " mismatches in " << randLen << " length\n";
    return misMatchCount >= ceil(0.25 * randLen);
}

bool halfMatch(string recordedPass, string enteredPass, int userId)
{
    int misMatchCount = numMismatch(recordedPass,enteredPass);
    int randLen = users[userId].randLen;
    cout<<misMatchCount << " mismatches in " << randLen << " length\n";
    return misMatchCount >= ceil(0.5 * randLen);
}

bool threeQuarterMatch(string recordedPass, string enteredPass, int userId)
{
    int misMatchCount = numMismatch(recordedPass,enteredPass);
    int randLen = users[userId].randLen;
    cout << misMatchCount << " mismatches in " << randLen << " length\n";
    return misMatchCount >= ceil(0.75 * randLen);
}

int main()
{
    char *strOptions[] = {
        "Salt mismatch must be 100% of total salt length",
        "Salt mismatch must be at least 1 character in every segment",
        "Salt mismatch must be 25% or more of total salt length",
        "Salt mismatch must be 50% or more of total salt length"
    };

    int id, numUser,numQuery;;
    int option;
    string storedPass;
    
    fin >> numUser;
    for(int i = 0; i < numUser; i++)
    {
        addNewUser(i);
    }
    
    cout << "Available models:" << endl;
    for (int i = 1; i <=4 ; i++)
        cout << "\t" << i << " - " << strOptions[i-1] << endl;
    cout << "Enter your choice: ";
    cin >> option;
    cout << "You have chosen " << option << " - " << strOptions[option-1] << endl;
    cout << "enter id: ";
    cin >> id;

    cout<<"enter reference password:\n";
    cin>>storedPass;
    cout<<"And The game begins Now!!!!\n";
    int numSuccess = 0;
    char successTracker[10 + 1] = {0};
    for(int i=0; i<10; i++)
    {
        string pass;

        cout<<"\nEnter pass: ";
        cin>>pass;
        if(matchPass(id,pass))
        {
            if (
                (1 == option && fullMatch(storedPass,pass,id))    ||
                (2 == option && partialMatch(storedPass,pass,id)) ||
                (3 == option && quarterMatch(storedPass,pass,id)) ||
                (4 == option && halfMatch(storedPass,pass,id))    ||
                (5 == option && threeQuarterMatch(storedPass,pass,id))
               )
            {
                numSuccess++;
                successTracker[i] = 'S';
                cout << "Authentication succeeded\n";
                storedPass = pass;
            }
            else
            {
                cout << "Authentication FAILED!!!\n";
                successTracker[i] = 'F';
            }
        }
        else
        {
            cout<<"secret password mismatch\n";
            successTracker[i] = 'F';
        }
    }
    cout<<"Succeeded : "<<numSuccess<<" out of 10 times\n";
    printf("SuccessTracker: %s\n", successTracker);

    return 0;
}
