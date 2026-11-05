#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <limits>
using namespace std;

const string ClientsFileName = "Clients_Data.txt";

enum enOperationType
{
	eShowClientList = 1, eAddNewClient = 2, eUpdateClientInfo = 3,
	eFindClient = 4, eDeleteClient = 5, eTransactions = 6,
	eExit = 7
};

enum enTransactionType
{
	eDeposit = 1, eWithdraw = 2, eTotalBalances = 3, eMainMenu = 4
};

struct stClient
{
	string AccountNumber;
	string PinCode;
	string Name;
	string Phone;
	float AccountBalance;
	bool MarkClientDelete = false;
};

void MainMenuScreen();
void TransactionsMenu();

vector <string> SplitString(string S1, string Delim = "#//#")
{
	vector <string> vS1;

	short Pos = 0;
	string Word = "";

	while ((Pos = S1.find(Delim)) != S1.npos)
	{
		Word = S1.substr(0, Pos);

		if (Word != "")
		{
			vS1.push_back(Word);
		}

		S1 = S1.erase(0, Pos + Delim.length());
	}

	if (S1 != "")
	{
		vS1.push_back(S1);
	}

	return vS1;
}

stClient ConvertLineToRecord(string S1)
{
	stClient Client;
	vector <string> vS1;
	vS1 = SplitString(S1);

	Client.AccountNumber = vS1[0];
	Client.PinCode = vS1[1];
	Client.Name = vS1[2];
	Client.Phone = vS1[3];
	Client.AccountBalance = stof(vS1[4]);

	return Client;
}

vector <stClient> LoadDataFromFileToVector(string FileName)
{
	vector <stClient> vClient;

	fstream MyFile;
	MyFile.open(FileName, ios::in);

	if (MyFile.is_open())
	{
		string Line;
		stClient Client;

		while (getline(MyFile, Line))
		{
			Client = ConvertLineToRecord(Line);
			vClient.push_back(Client);
		}

		MyFile.close();
	}

	return vClient;
}

void PrintClientRecord(stClient Client)
{
	cout << "| " << left << setw(15) << Client.AccountNumber;
	cout << "| " << left << setw(11) << Client.PinCode;
	cout << "| " << left << setw(29) << Client.Name;
	cout << "| " << left << setw(14) << Client.Phone;
	cout << "| " << left << setw(9) << Client.AccountBalance;
}

void ShowClientMenu()
{
	vector <stClient> vClient;
	vClient = LoadDataFromFileToVector(ClientsFileName);

	cout << "\n\n";
	cout << "\t\t\t\t Client List [" << vClient.size() << "] Client(s). \n";
	cout << "_________________________________________________________________________________________\n\n";
	cout << "| " << left << setw(15) << "Account Number" << "| " << left << setw(11) << "Pin Code";
	cout << "| " << left << setw(29) << "Client Name" << "| " << left << setw(14) << "Phone";
	cout << "| " << left << setw(9) << "Balance" << endl;
	cout << "_________________________________________________________________________________________\n\n";

	for (stClient& C : vClient)
	{
		PrintClientRecord(C);
		cout << endl;
	}

	cout << "\n_________________________________________________________________________________________\n";
}

void PrintAddClientScreen()
{
	cout << "\n---------------------------------------------\n";
	cout << "              Add New Client Screen             ";
	cout << "\n---------------------------------------------\n";
	cout << "Adding New Client: \n\n";
}

string ReadAccountNumber()
{
	string AccountNumber;

	cout << "\nEnter Account Number: ";
	getline(cin >> ws, AccountNumber);

	return AccountNumber;
}

bool IsClientFound(string AccountNumber, vector <stClient> vClient)
{
	for (stClient& C : vClient)
		if (C.AccountNumber == AccountNumber)
			return true;

	return false;
}

stClient ReadClientData(string AccountNumber)
{
	stClient Client;

	Client.AccountNumber = AccountNumber;

	cout << "\nEnter Pin Code: ";
	getline(cin >> ws, Client.PinCode);

	cout << "Enter Name: ";
	getline(cin, Client.Name);

	cout << "Enter Phone: ";
	getline(cin, Client.Phone);

	cout << "Enter Account Balance: ";
	cin >> Client.AccountBalance;

	while (cin.fail())
	{
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		cout << "\nInvalid Account Balance, Please Try Again: \n";
		cout << "Enter Account Balance: ";
		cin >> Client.AccountBalance;
	}

	return Client;
}

stClient AddNewClient()
{
	stClient Client;

	vector <stClient> vClient;
	vClient = LoadDataFromFileToVector(ClientsFileName);

	string AccountNumber = ReadAccountNumber();

	while (IsClientFound(AccountNumber, vClient))
	{
		cout << "\nClient With [" << AccountNumber << "] Already Exists, Enter Another Account Number: ";
		getline(cin >> ws, AccountNumber);
	}

	Client = ReadClientData(AccountNumber);

	return Client;
}

string ConvertRecordToLine(stClient Client, string Delim = "#//#")
{
	string Line = "";

	Line += Client.AccountNumber + Delim;
	Line += Client.PinCode + Delim;
	Line += Client.Name + Delim;
	Line += Client.Phone + Delim;
	Line += to_string(Client.AccountBalance);

	return Line;
}

void AddDataToFile(string FileName, string S1)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out | ios::app);

	if (MyFile.is_open())
	{
		MyFile << S1 << endl;

		MyFile.close();
	}
}

void AddClients()
{
	stClient NewClient = AddNewClient();

	AddDataToFile(ClientsFileName, ConvertRecordToLine(NewClient));
}

void AddClientMenu()
{
	char Answer;

	do
	{
		system("cls");

		PrintAddClientScreen();
		AddClients();

		cout << "\nClient Added Successfully...\n\n";
		cout << "Do You Want To Add More Clients Y/N: ";
		cin >> Answer;

	} while (toupper(Answer) == 'Y');
}

void PrintDeleteClientScreen()
{
	cout << "\n---------------------------------------------\n";
	cout << "              Delete Client Screen             ";
	cout << "\n---------------------------------------------\n";
}

bool IsClientFound(string AccountNumber, vector <stClient> vClient, stClient& Client)
{
	for (stClient& C : vClient)
	{
		if (C.AccountNumber == AccountNumber)
		{
			Client = C;
			return true;
		}
	}

	return false;
}

void PrintClientCard(stClient Client)
{
	cout << "\nThe Following Are The Client Details: \n";
	cout << "----------------------------------------\n";
	cout << "Account Number : " << Client.AccountNumber << endl;
	cout << "Pin Code       : " << Client.PinCode << endl;
	cout << "Name           : " << Client.Name << endl;
	cout << "Phone          : " << Client.Phone << endl;
	cout << "Account Balance: " << Client.AccountBalance << endl;
	cout << "----------------------------------------\n";
}

void MarkClientDeleted(string AccountNumber, vector <stClient>& vClient)
{
	for (stClient& C : vClient)
	{
		if (C.AccountNumber == AccountNumber)
		{
			C.MarkClientDelete = true;
			break;
		}
	}
}

void SaveUpdatedDataToFile(string FileName, vector <stClient>& vClient)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out);

	if (MyFile.is_open())
	{
		string Line;

		for (stClient& C : vClient)
		{
			if (C.MarkClientDelete == false)
			{
				Line = ConvertRecordToLine(C);
				MyFile << Line << endl;
			}
		}

		MyFile.close();
	}
}

void DeleteClientMenu()
{
	PrintDeleteClientScreen();

	char Answer;
	stClient Client;
	vector <stClient> vClient;

	vClient = LoadDataFromFileToVector(ClientsFileName);

	string AccountNumber;
	AccountNumber = ReadAccountNumber();

	if (IsClientFound(AccountNumber, vClient, Client))
	{
		PrintClientCard(Client);

		cout << "\n\nAre You Sure You Want Delete This Client Y/N: ";
		cin >> Answer;

		if (toupper(Answer) == 'Y')
		{
			MarkClientDeleted(AccountNumber, vClient);
			SaveUpdatedDataToFile(ClientsFileName, vClient);

			cout << "\nClient Deleted Successfully...\n";
		}
	}
	else
	{
		cout << "\nClient With Account Number [" << AccountNumber << "] Is Not Found! \n";
	}
}

void PrintUpdateClientScreen()
{
	cout << "\n---------------------------------------------\n";
	cout << "              Update Client Info Screen          ";
	cout << "\n---------------------------------------------\n";
}

void UpdateClientData(string AccountNumber, vector <stClient>& vClient)
{
	for (stClient& C : vClient)
	{
		if (C.AccountNumber == AccountNumber)
		{
			C = ReadClientData(AccountNumber);
			break;
		}
	}
}

void UpdateClientMenu()
{
	PrintUpdateClientScreen();

	char Answer;
	stClient Client;
	vector <stClient> vClient;

	vClient = LoadDataFromFileToVector(ClientsFileName);

	string AccountNumber = ReadAccountNumber();

	if (IsClientFound(AccountNumber, vClient, Client))
	{
		PrintClientCard(Client);

		cout << "\nAre You Sure You Want Update This Client Y/N: ";
		cin >> Answer;

		if (toupper(Answer) == 'Y')
		{
			UpdateClientData(AccountNumber, vClient);
			SaveUpdatedDataToFile(ClientsFileName, vClient);

			cout << "\nClient Updated Successfully...\n";
		}
	}
	else
	{
		cout << "\nClient With Account Number [" << AccountNumber << "] Is Not Found! \n";
	}
}

void PrintFindClientScreen()
{
	cout << "\n---------------------------------------------\n";
	cout << "                Find Client Screen               ";
	cout << "\n---------------------------------------------\n";
}

void FindClientMenu()
{
	PrintFindClientScreen();

	stClient Client;
	vector <stClient> vClient;

	vClient = LoadDataFromFileToVector(ClientsFileName);

	string AccountNumber = ReadAccountNumber();

	if (IsClientFound(AccountNumber, vClient, Client))
		PrintClientCard(Client);
	else
		cout << "\nClient With Account Number [" << AccountNumber << "] Is Not Found! \n";
}

void PrintDepositScreen()
{
	cout << "\n---------------------------------------\n";
	cout << "                Deposit Screen             ";
	cout << "\n---------------------------------------\n";
}

void DepositAmountByAccountNumber(string AccountNumber, vector <stClient>& vClient)
{
	float DepositAmount;

	cout << "\nPlease Enter Deposit Amount: ";
	cin >> DepositAmount;

	while (cin.fail() || DepositAmount <= 0)
	{
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		cout << "\nInvalid Deposit Amount, Please Try Again: ";
		cout << "\nPlease Enter Deposit Amount: ";
		cin >> DepositAmount;
	}

	char Answer;

	cout << "\nAre You Sure You Want Perform This Transaction Y/N: ";
	cin >> Answer;

	if (toupper(Answer) == 'Y')
	{
		for (stClient& C : vClient)
		{
			if (C.AccountNumber == AccountNumber)
			{
				C.AccountBalance += DepositAmount;

				SaveUpdatedDataToFile(ClientsFileName, vClient);

				cout << "\nDone Successfully...\n\n";
				cout << "New Balance = " << C.AccountBalance << endl;

				return;
			}
		}
	}
}

void DepositMenuScreen()
{
	PrintDepositScreen();

	stClient Client;
	vector <stClient> vClient;

	vClient = LoadDataFromFileToVector(ClientsFileName);

	string AccountNumber = ReadAccountNumber();

	if (IsClientFound(AccountNumber, vClient, Client))
	{
		PrintClientCard(Client);
		DepositAmountByAccountNumber(AccountNumber, vClient);
	}
	else
	{
		cout << "\nClient With [" << AccountNumber << "] Does Not Exist. \n";
	}
}

void PrintWithdrawScreen()
{
	cout << "\n---------------------------------------\n";
	cout << "                Withdraw Screen             ";
	cout << "\n---------------------------------------\n";
}

void WithdrawAmountByAccountNumber(string AccountNumber, vector <stClient>& vClient, stClient Client)
{
	float WithdrawAmount;

	cout << "\nPlease Enter Withdraw Amount: ";
	cin >> WithdrawAmount;

	while (cin.fail() || WithdrawAmount <= 0)
	{
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		cout << "\nInvalid Withdraw Amount, Please Try Again: ";
		cout << "\nPlease Enter Withdraw Amount: ";
		cin >> WithdrawAmount;
	}

	while (WithdrawAmount > Client.AccountBalance)
	{
		cout << "\nAmount Exceeds The Balance, You Can Withdraw Up To! \n";
		cout << "\nPlease Enter Another Amount: ";

		cin >> WithdrawAmount;

		while (cin.fail() || WithdrawAmount <= 0)
		{
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');

			cout << "\nInvalid Withdraw Amount, Please Try Again: ";
			cout << "\nPlease Enter Withdraw Amount: ";
			cin >> WithdrawAmount;
		}
	}

	char Answer;

	cout << "\nAre You Sure You Want Perform This Transaction Y/N: ";
	cin >> Answer;

	if (toupper(Answer) == 'Y')
	{
		for (stClient& C : vClient)
		{
			if (C.AccountNumber == AccountNumber)
			{
				C.AccountBalance -= WithdrawAmount;

				SaveUpdatedDataToFile(ClientsFileName, vClient);

				cout << "\nDone Successfully...\n\n";
				cout << "New Balance = " << C.AccountBalance << endl;

				return;
			}
		}
	}
}

void WithdrawMenuScreen()
{
	PrintWithdrawScreen();

	stClient Client;
	vector <stClient> vClient;

	vClient = LoadDataFromFileToVector(ClientsFileName);

	string AccountNumber = ReadAccountNumber();

	if (IsClientFound(AccountNumber, vClient, Client))
	{
		PrintClientCard(Client);
		WithdrawAmountByAccountNumber(AccountNumber, vClient, Client);
	}
	else
	{
		cout << "\nClient With [" << AccountNumber << "] Does Not Exist. \n";
	}
}

void PrintClientsBalances(stClient Client)
{
	cout << "| " << left << setw(16) << Client.AccountNumber;
	cout << "| " << left << setw(29) << Client.Name;
	cout << "| " << left << setw(9) << Client.AccountBalance;
}

void TotalBalancesMenuScreen()
{
	double TotalBalances = 0;

	vector <stClient> vClient;
	vClient = LoadDataFromFileToVector(ClientsFileName);

	cout << "\n\n";
	cout << "\t\t\tBalances List [" << vClient.size() << "] Client(s). \n";
	cout << "______________________________________________________________________________\n\n";
	cout << "| " << left << setw(16) << "Account Number" << "| " << left << setw(29) << "Client Name";
	cout << "| " << left << setw(9) << "Balance" << endl;
	cout << "______________________________________________________________________________\n\n";

	for (stClient& C : vClient)
	{
		PrintClientsBalances(C);

		TotalBalances += C.AccountBalance;

		cout << endl;
	}

	cout << "\n______________________________________________________________________________\n\n";
	cout << "\t\t\t Total Balances = " << TotalBalances << endl;
}

void GoToTransactionMenu()
{
	cout << "\n\nPress Any Key To Go Back Transaction Menu...";
	system("pause >0");

	TransactionsMenu();
}

void PerformTransactionsOperations(enTransactionType TransactionType)
{
	switch (TransactionType)
	{
	case enTransactionType::eDeposit:
		system("Cls");
		DepositMenuScreen();
		GoToTransactionMenu();
		break;

	case enTransactionType::eWithdraw:
		system("Cls");
		WithdrawMenuScreen();
		GoToTransactionMenu();
		break;

	case enTransactionType::eTotalBalances:
		system("Cls");
		TotalBalancesMenuScreen();
		GoToTransactionMenu();
		break;

	case enTransactionType::eMainMenu:
		system("Cls");
		MainMenuScreen();
		break;
	}
}

short ReadNumberFrom1To4()
{
	short Number;

	do
	{
		cout << "Choose What Do You Want To Do [ 1 To 4 ]: ";
		cin >> Number;

		while (cin.fail())
		{
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');

			cout << "\nInvalid Choosing, Please Try Again: \n";
			cout << "Choose What Do You Want To Do [ 1 To 4 ]: ";
			cin >> Number;
		}

	} while (Number < 1 || Number > 4);

	return Number;
}

void TransactionsMenu()
{
	system("Cls");

	cout << "\n===================================================\n";
	cout << "               Transactions Menu Screen                ";
	cout << "\n===================================================\n";
	cout << "\t [1] Deposit. \n";
	cout << "\t [2] Withdraw. \n";
	cout << "\t [3] Total Balances. \n";
	cout << "\t [4] Main Menu. \n";
	cout << "===================================================\n";

	PerformTransactionsOperations((enTransactionType)ReadNumberFrom1To4());
}

void PrintProgramEnd()
{
	cout << "\n---------------------------------------------\n";
	cout << "                Program Ends :-)                ";
	cout << "\n---------------------------------------------\n";
}

short ReadNumberFrom1To7()
{
	short Number;

	do
	{
		cout << "Choose What Do You Want To Do [ 1 To 7 ]: ";
		cin >> Number;

		while (cin.fail())
		{
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');

			cout << "\nInvalid Choosing, Please Try Again: \n";
			cout << "Choose What Do You Want To Do [ 1 To 7 ]: ";
			cin >> Number;
		}

	} while (Number < 1 || Number > 7);

	return Number;
}

void GoToMainMenuScreen()
{
	cout << "\n\nPress Any Key To Go Back To Main Menu...";
	system("pause >0");

	MainMenuScreen();
}

void PerformOperation(enOperationType OperationType)
{
	switch (OperationType)
	{
	case enOperationType::eShowClientList:
		system("cls");
		ShowClientMenu();
		GoToMainMenuScreen();
		break;

	case enOperationType::eAddNewClient:
		system("cls");
		AddClientMenu();
		GoToMainMenuScreen();
		break;

	case enOperationType::eUpdateClientInfo:
		system("cls");
		UpdateClientMenu();
		GoToMainMenuScreen();
		break;

	case enOperationType::eFindClient:
		system("cls");
		FindClientMenu();
		GoToMainMenuScreen();
		break;

	case enOperationType::eDeleteClient:
		system("cls");
		DeleteClientMenu();
		GoToMainMenuScreen();
		break;

	case enOperationType::eTransactions:
		system("cls");
		TransactionsMenu();
		break;

	case enOperationType::eExit:
		system("cls");
		PrintProgramEnd();
		break;
	}
}

void MainMenuScreen()
{
	system("cls");

	cout << "\n=======================================================\n";
	cout << "                 Main Menu Screen                         ";
	cout << "\n=======================================================\n";
	cout << "\t [1] Show Client List. \n";
	cout << "\t [2] Add New Client. \n";
	cout << "\t [3] Update Client Info. \n";
	cout << "\t [4] Find Client. \n";
	cout << "\t [5] Delete Client. \n";
	cout << "\t [6] Transactions. \n";
	cout << "\t [7] Exit. \n";
	cout << "=======================================================\n";

	PerformOperation((enOperationType)ReadNumberFrom1To7());
}

int main()
{
	MainMenuScreen();

	return 0;
}