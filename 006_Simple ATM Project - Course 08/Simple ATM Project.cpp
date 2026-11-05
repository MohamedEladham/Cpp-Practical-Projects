#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <limits>
#include <cctype>

using namespace std;

enum enATMMainMenuOptions
{
    eQuickWithdraw = 1, eNormalWithdraw = 2, eDeposit = 3,
    eCheckBalance = 4, eLogout = 5
};

struct stClient
{
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    float AccountBalance;
};

const string ClientsFileName = "Clients_Data.txt";

stClient CurrentClient;

void ShowQuickWithdrawMenuScreen();
void GoToATMMainMenuScreen();
void ShowMainMenuScreen();
void Login();

vector<string> SplitString(string Text, string Delimiter = "#//#")
{
    vector<string> vWords;

    short Position = 0;
    string Word = "";

    while ((Position = Text.find(Delimiter)) != string::npos)
    {
        Word = Text.substr(0, Position);

        if (Word != "")
        {
            vWords.push_back(Word);
        }

        Text = Text.erase(0, Position + Delimiter.length());
    }

    if (Text != "")
    {
        vWords.push_back(Text);
    }

    return vWords;
}

stClient ConvertLineToRecord(string Line)
{
    stClient Client;

    vector<string> vWords;
    vWords = SplitString(Line);

    if (vWords.size() == 5)
    {
        Client.AccountNumber = vWords[0];
        Client.PinCode = vWords[1];
        Client.Name = vWords[2];
        Client.Phone = vWords[3];
        Client.AccountBalance = stof(vWords[4]);
    }

    return Client;
}

vector<stClient> LoadClientDataFromFileToVector(string FileName)
{
    vector<stClient> vClients;

    fstream MyFile;
    MyFile.open(FileName, ios::in);

    if (MyFile.is_open())
    {
        string Line;
        stClient Client;

        while (getline(MyFile, Line))
        {
            Client = ConvertLineToRecord(Line);

            vClients.push_back(Client);
        }

        MyFile.close();
    }

    return vClients;
}

string ReadStringInput(string Message)
{
    string Value;

    cout << Message;
    getline(cin >> ws, Value);

    return Value;
}

bool CheckIfClientExists(string AccountNumber, string PinCode, stClient& Client)
{
    vector<stClient> vClients;
    vClients = LoadClientDataFromFileToVector(ClientsFileName);

    for (stClient& CurrentClient : vClients)
    {
        if (CurrentClient.AccountNumber == AccountNumber &&
            CurrentClient.PinCode == PinCode)
        {
            Client = CurrentClient;
            return true;
        }
    }

    return false;
}

bool LoadClientData(string AccountNumber, string PinCode)
{
    return CheckIfClientExists(AccountNumber, PinCode, CurrentClient);
}

string ConvertRecordToLine(stClient Client, string Delimiter = "#//#")
{
    string Line = "";

    Line += Client.AccountNumber + Delimiter;
    Line += Client.PinCode + Delimiter;
    Line += Client.Name + Delimiter;
    Line += Client.Phone + Delimiter;
    Line += to_string(Client.AccountBalance);

    return Line;
}

bool SaveUpdateDataToFile(string FileName, vector<stClient>& vClients)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out);

    if (MyFile.is_open())
    {
        string Line;

        for (stClient& Client : vClients)
        {
            Line = ConvertRecordToLine(Client);
            MyFile << Line << endl;
        }

        MyFile.close();

        return true;
    }

    return false;
}

bool UpdateClientBalanceByAccountNumber(
    string AccountNumber,
    vector<stClient>& vClients,
    float BalanceChange)
{
    for (stClient& Client : vClients)
    {
        if (Client.AccountNumber == AccountNumber)
        {
            if (Client.AccountBalance + BalanceChange < 0)
                return false;

            Client.AccountBalance += BalanceChange;

            if (SaveUpdateDataToFile(ClientsFileName, vClients))
            {
                CurrentClient.AccountBalance = Client.AccountBalance;

                cout << "\n\nDone Successfully...\n\n";
                cout << "\n> New Balance Is \"" << Client.AccountBalance << endl;

                return true;
            }

            cout << "\n\nFailed To Save Data!\n\n";

            return false;
        }
    }

    return false;
}

void PrintBalanceIsInsufficient()
{
    system("Cls");

    cout << "\n\n";
    cout << "---------------------------------------------------------\n\n";
    cout << "           *** Your Balance Is Insufficient  ***         \n\n";
    cout << "---------------------------------------------------------\n\n";
}

void ShowClientBalanceOnTheScreen()
{
    cout << "\nYour Balance Is \"" << CurrentClient.AccountBalance << "\" \n\n";
}

void ShowCheckBalanceMenuScreen()
{
    cout << "==================================================\n";
    cout << "                 Check Balance Screen             \n";
    cout << "==================================================\n";

    ShowClientBalanceOnTheScreen();
}

void GoToQuickWithdrawMenuScreen()
{
    cout << "\n\n\nPress Any Key To Go Quick Withdraw Menu Screen...";
    system("Pause >0");

    ShowQuickWithdrawMenuScreen();
}

short GetQuickWithdrawAmount(short OptionNumber)
{
    short QuickWithdrawAmounts[] = { 20, 50, 100, 200, 400, 600, 800, 1000 };

    return QuickWithdrawAmounts[OptionNumber - 1];
}

short ReadNumberFrom1To9()
{
    short Number;

    do
    {
        cout << "Choose What To Do From [1 To 9]: ";
        cin >> Number;

        while (cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            cout << "Invalid Operation, Please Try Again! \n";
            cout << "Choose What To Do From [1 To 9]: ";

            cin >> Number;
        }

    } while (Number < 1 || Number > 9);

    return Number;
}

void PerformQuickWithdrawOptions(short OptionNumber)
{
    if (OptionNumber == 9)
        return;

    short WithdrawAmount = GetQuickWithdrawAmount(OptionNumber);

    if (WithdrawAmount > CurrentClient.AccountBalance)
    {
        PrintBalanceIsInsufficient();
        GoToQuickWithdrawMenuScreen();

        return;
    }

    char Answer;

    cout << "\nAre You Sure You Want Perform This Transaction Y/N: ";
    cin >> Answer;

    if (toupper(static_cast<unsigned char>(Answer)) == 'Y')
    {
        vector<stClient> vClients;
        vClients = LoadClientDataFromFileToVector(ClientsFileName);

        UpdateClientBalanceByAccountNumber(
            CurrentClient.AccountNumber,
            vClients,
            WithdrawAmount * -1
        );
    }
}

void ShowQuickWithdrawMenuScreen()
{
    system("Cls");

    cout << "============================================\n";
    cout << "               Quick Withdraw               \n";
    cout << "============================================\n";
    cout << "\t [1] 20   \t[2] 50 \n";
    cout << "\t [3] 100  \t[4] 200 \n";
    cout << "\t [5] 400  \t[6] 600 \n";
    cout << "\t [7] 800  \t[8] 1000 \n";
    cout << "\t [9] Exit \n";
    cout << "============================================\n";

    ShowClientBalanceOnTheScreen();

    PerformQuickWithdrawOptions(ReadNumberFrom1To9());
}

void PrintNormalWithdrawScreen()
{
    cout << "===============================================\n";
    cout << "              Normal Withdraw Screen           \n";
    cout << "===============================================\n";
}

void PerformNormalWithdraw()
{
    ShowClientBalanceOnTheScreen();

    int WithdrawAmount;

    do
    {
        cout << "\nEnter An Amount Multiple Of 5's: ";
        cin >> WithdrawAmount;

        while (cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            cout << "\nInvalid Operation, Please Try Again! \n";
            cout << "Enter An Amount Multiple Of 5's: ";

            cin >> WithdrawAmount;
        }

    } while (WithdrawAmount <= 0 || WithdrawAmount % 5 != 0);

    if (WithdrawAmount > CurrentClient.AccountBalance)
    {
        PrintBalanceIsInsufficient();
        return;
    }

    char Answer;

    cout << "\n\nAre You Sure You Want Perform This Transaction Y/N: ";
    cin >> Answer;

    if (toupper(static_cast<unsigned char>(Answer)) == 'Y')
    {
        vector<stClient> vClients;
        vClients = LoadClientDataFromFileToVector(ClientsFileName);

        UpdateClientBalanceByAccountNumber(
            CurrentClient.AccountNumber,
            vClients,
            WithdrawAmount * -1
        );
    }
}

void ShowNormalWithdrawMenuScreen()
{
    PrintNormalWithdrawScreen();
    PerformNormalWithdraw();
}

void PrintDepositMenuScreen()
{
    cout << "===============================================\n";
    cout << "                Deposit Screen                 \n";
    cout << "===============================================\n";
}

float ReadDepositAmount()
{
    float DepositAmount;

    do
    {
        cout << "\nEnter A Positive Deposit Amount: ";
        cin >> DepositAmount;

        while (cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            cout << "\nInvalid Operation, Please Try Again! \n";
            cout << "Enter A Positive Deposit Amount: ";

            cin >> DepositAmount;
        }

    } while (DepositAmount <= 0);

    return DepositAmount;
}

void PerformDepositOption()
{
    ShowClientBalanceOnTheScreen();

    float DepositAmount = ReadDepositAmount();

    char Answer;

    cout << "\nAre You Sure You Want Perform This Transaction Y/N: ";
    cin >> Answer;

    if (toupper(static_cast<unsigned char>(Answer)) == 'Y')
    {
        vector<stClient> vClients;
        vClients = LoadClientDataFromFileToVector(ClientsFileName);

        UpdateClientBalanceByAccountNumber(
            CurrentClient.AccountNumber, vClients, DepositAmount
        );
    }
}

void ShowDepositMenuScreen()
{
    PrintDepositMenuScreen();
    PerformDepositOption();
}

short ReadNumberFrom1To5()
{
    short Number;

    do
    {
        cout << "Choose What Do You Want To Do From [1 To 5]: ";
        cin >> Number;

        while (cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            cout << "Invalid Operation, Please Try Again! \n";
            cout << "Choose What Do You Want To Do From [1 To 5]: ";

            cin >> Number;
        }

    } while (Number < 1 || Number > 5);

    return Number;
}

void GoToATMMainMenuScreen()
{
    cout << "\n\n\nPress Any Key To Go Main Menu Screen...";
    system("Pause >0");

    ShowMainMenuScreen();
}

void PerformATMMainMenuOptions(enATMMainMenuOptions ATMOption)
{
    switch (ATMOption)
    {
    case enATMMainMenuOptions::eQuickWithdraw:

        system("Cls");
        ShowQuickWithdrawMenuScreen();
        GoToATMMainMenuScreen();

        break;

    case enATMMainMenuOptions::eNormalWithdraw:

        system("Cls");
        ShowNormalWithdrawMenuScreen();
        GoToATMMainMenuScreen();

        break;

    case enATMMainMenuOptions::eDeposit:

        system("Cls");
        ShowDepositMenuScreen();
        GoToATMMainMenuScreen();

        break;

    case enATMMainMenuOptions::eCheckBalance:

        system("Cls");
        ShowCheckBalanceMenuScreen();
        GoToATMMainMenuScreen();

        break;

    case enATMMainMenuOptions::eLogout:

        system("Cls");
        Login();

        break;
    }
}

void ShowMainMenuScreen()
{
    system("Cls");

    cout << "\n";
    cout << "=========================================================\n";
    cout << "              ATM Main Menu Screen                       \n";
    cout << "=========================================================\n";
    cout << "\t[1] Quick Withdraw. \n";
    cout << "\t[2] Normal Withdraw. \n";
    cout << "\t[3] Deposit. \n";
    cout << "\t[4] Check Balance. \n";
    cout << "\t[5] Logout. \n";
    cout << "=========================================================\n";

    PerformATMMainMenuOptions((enATMMainMenuOptions)ReadNumberFrom1To5()
    );
}

void Login()
{
    bool LoginFailed = false;

    string AccountNumber, PinCode;

    do
    {
        system("Cls");

        cout << "\n\n";
        cout << "---------------------------------------------------------\n";
        cout << "                     Login Screen                        \n";
        cout << "---------------------------------------------------------\n";

        if (LoginFailed)
            cout << "Invalid Account Number/PinCode! \n";

        AccountNumber = ReadStringInput("Enter Account Number: ");
        PinCode = ReadStringInput("Enter Pin Code: ");

        LoginFailed = !LoadClientData(AccountNumber, PinCode);

    } while (LoginFailed);

    ShowMainMenuScreen();
}

int main()
{
    Login();

    return 0;
}