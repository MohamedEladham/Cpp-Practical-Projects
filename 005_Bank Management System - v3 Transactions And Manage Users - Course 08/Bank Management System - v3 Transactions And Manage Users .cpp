#include <iostream>
#include <windows.h>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
using namespace std;
HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

enum enMainMenuOptions
{
	eShowClientList = 1, eAddNewClient = 2, eUpdateClientInfo = 3,
	eDeleteClient = 4, eFindClient = 5, eTransactions = 6,
	eManageUsers = 7, eLogout = 8
};

enum enTransactionOperationType
{
	eDeposit = 1, eWithdraw = 2,
	eTotalBalances = 3, eMainMenu = 4
};

enum enManageUserMenuOptions
{
	eListUsers = 1, eAddNewUser = 2, eUpdateUser = 3,
	eDeleteUser = 4, eFindUser = 5, eGoToMainMenu = 6
};

enum enUserPermissions
{
	All = -1,
	pShowClient = 1, pAddClient = 2, pDeleteClient = 4,
	pUpdateClient = 8, pFindClient = 16, pTransactions = 32,
	pManageUsers = 64
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

struct stUser
{
	string UserName;
	string Password;
	short  Permissions = 0;
	bool MarkUserDeleted = false;
};

const string ClientsFileName = "Clients_Data.txt";
const string UsersFileName = "Users.txt";
stUser CurrentUser;

void MainMenuScreen();
void TransactionsMenu();
void GoToMainMenuScreen();
void ManageUsersMenu();
void Login();
void PerformMainMenuOption(enMainMenuOptions MainMenuOption);
void PerformTransactionsOperations(enTransactionOperationType TransType);
void PerformManageUsersMenuOption(enManageUserMenuOptions ManageUser);

string Tap(short Num)
{
	string S = "";

	for (short i = 1; i <= Num; i++)
	{
		S = "\t";
		cout << S;
	}

	return S;
}


string Separator(string S)
{
	SetConsoleTextAttribute(h, 14);
	return S;
}


bool CheckAccessPermission(enUserPermissions Permissions)
{
	if (CurrentUser.Permissions == enUserPermissions::All)
		return true;

	if ((Permissions & CurrentUser.Permissions) == Permissions)
		return true;
	else
		return false;
}


void ShowAccessDeniedMessage()
{
	cout << "\n\n\n";

	SetConsoleTextAttribute(h, 3);
	cout << Tap(8) << "__________________________________________________________\n";
	cout << Tap(8) << "           _______________________________________________\n\n\n";

	SetConsoleTextAttribute(h, 4);
	cout << Tap(8) << "         * * * You Can't Access This Page * * *      \n\n";

	SetConsoleTextAttribute(h, 3);
	cout << Tap(8) << "__________________________________________________________\n";
	cout << Tap(8) << "____________________________________________            \n\n";

}


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


vector <stClient> LoadClientsDataFromFileToVector(string FileName)
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


string ConvertRecordToLine(stClient Clinet, string Delim = "#//#")
{
	string Line = "";
	Line += Clinet.AccountNumber + Delim;
	Line += Clinet.PinCode + Delim;
	Line += Clinet.Name + Delim;
	Line += Clinet.Phone + Delim;
	Line += to_string(Clinet.AccountBalance);
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


void SaveUpdateDataToFile(string FileName, vector <stClient>& vClient)
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


void PrintClientRecord(stClient Client)
{
	cout << Tap(8);
	cout << Separator("| ");
	SetConsoleTextAttribute(h, 7);
	cout << left << setw(15) << Client.AccountNumber;
	cout << Separator("| ");
	SetConsoleTextAttribute(h, 7);
	cout << left << setw(11) << Client.PinCode;
	cout << Separator("| ");
	SetConsoleTextAttribute(h, 7);
	cout << left << setw(29) << Client.Name;
	cout << Separator("| ");
	SetConsoleTextAttribute(h, 7);
	cout << left << setw(14) << Client.Phone;
	cout << Separator("| ");
	SetConsoleTextAttribute(h, 7);
	cout << left << setw(9) << Client.AccountBalance;
}


bool FindClientByAccountNumber(string AccountNumber, vector <stClient> vClient, stClient& Client)
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
	SetConsoleTextAttribute(h, 1);
	cout << Tap(8) << "__________________________________________________________\n";

	SetConsoleTextAttribute(h, 7);
	cout << Tap(8) << "                    Client Details                        \n";

	SetConsoleTextAttribute(h, 1);
	cout << Tap(8) << "__________________________________________________________\n";

	SetConsoleTextAttribute(h, 7);
	cout << Tap(8) << "Account Number : " << Client.AccountNumber << endl;
	cout << Tap(8) << "Pin Code       : " << Client.PinCode << endl;
	cout << Tap(8) << "Name           : " << Client.Name << endl;
	cout << Tap(8) << "Phone          : " << Client.Phone << endl;
	cout << Tap(8) << "Account Balance: " << Client.AccountBalance << endl;

	SetConsoleTextAttribute(h, 1);
	cout << Tap(8) << "__________________________________________________________\n";
	SetConsoleTextAttribute(h, 7);
}


void ShowClientListMenu()
{
	if (!CheckAccessPermission(enUserPermissions::pShowClient))
	{
		ShowAccessDeniedMessage();
		return;
	}

	vector <stClient> vClient;
	vClient = LoadClientsDataFromFileToVector(ClientsFileName);

	cout << "\n\n";
	SetConsoleTextAttribute(h, 1);
	cout << Tap(8) << "_________________________________________________________________________________________\n\n";

	SetConsoleTextAttribute(h, 7);
	cout << Tap(8) << "                         Client List [" << vClient.size() << "]\n";

	SetConsoleTextAttribute(h, 1);
	cout << Tap(8) << "_________________________________________________________________________________________\n\n";

	cout << Tap(8);
	cout << Separator("| ");
	SetConsoleTextAttribute(h, 7);
	cout << left << setw(15) << "Account Number";
	cout << Separator("| ");
	SetConsoleTextAttribute(h, 7);
	cout << left << setw(11) << "Pin Code";
	cout << Separator("| ");
	SetConsoleTextAttribute(h, 7);
	cout << left << setw(29) << "Client Name";
	cout << Separator("| ");
	SetConsoleTextAttribute(h, 7);
	cout << left << setw(14) << "Phone";
	cout << Separator("| ");
	SetConsoleTextAttribute(h, 7);
	cout << left << setw(9) << "Balance" << endl;

	SetConsoleTextAttribute(h, 1);
	cout << Tap(8) << "_________________________________________________________________________________________\n\n";

	for (stClient& C : vClient)
	{
		PrintClientRecord(C);
		cout << "\n\n";
	}

	SetConsoleTextAttribute(h, 1);
	cout << Tap(8) << "_________________________________________________________________________________________\n";
}


string ReadAccountNumber(string Message)
{
	string AccountNumer;
	SetConsoleTextAttribute(h, 7);
	cout << Tap(8) << Message;
	getline(cin >> ws, AccountNumer);
	return AccountNumer;
}


bool IsClientAccountNumberExists(string AccountNumber, vector <stClient> vClient)
{
	for (stClient& C : vClient)
		if (C.AccountNumber == AccountNumber)
			return true;

	return false;
}


stClient ReadClientData(string AccountNumer)
{
	cout << "\n";
	stClient Client;
	Client.AccountNumber = AccountNumer;
	cout << Tap(8) << "Enter Pin Code: ";
	getline(cin >> ws, Client.PinCode);
	cout << Tap(8) << "Enter Name: ";
	getline(cin, Client.Name);
	cout << Tap(8) << "Enter Phone: ";
	getline(cin, Client.Phone);
	cout << Tap(8) << "Enter Account Balance: ";
	cin >> Client.AccountBalance;
	while (cin.fail()) {
		cin.clear();
		cin.ignore(1000, '\n');
		cout << "\n";

		SetConsoleTextAttribute(h, 4);
		cout << Tap(8) << "Invalid Balance, ";
		SetConsoleTextAttribute(h, 7);
		cout << "Please re-enter: ";
		cin >> Client.AccountBalance;
	}
	return Client;
}


stClient AddNewClient()
{
	stClient Client;

	vector <stClient> vClient;
	vClient = LoadClientsDataFromFileToVector(ClientsFileName);

	string AccountNumer = ReadAccountNumber("Enter Account Number: ");

	while (IsClientAccountNumberExists(AccountNumer, vClient))
	{
		cout << "\n";
		cout << Tap(8);
		SetConsoleTextAttribute(h, 4);
		cout << "Client With [";
		SetConsoleTextAttribute(h, 7);
		cout << AccountNumer;
		SetConsoleTextAttribute(h, 4);
		cout << "] Already Exists....";
		cout << "\n";
		cout << Tap(8);
		SetConsoleTextAttribute(h, 7);
		cout << "Enter Another Account Number : ";
		getline(cin >> ws, AccountNumer);
	}

	Client = ReadClientData(AccountNumer);

	return Client;
}


void AddClients()
{
	stClient NewClient = AddNewClient();
	AddDataToFile(ClientsFileName, ConvertRecordToLine(NewClient));
}


void PrintAddClientScreen()
{
	cout << "\n\n";
	SetConsoleTextAttribute(h, 3);
	cout << Tap(8) << "__________________________________________________________\n";
	cout << Tap(8) << "               ___________________________________________\n\n\n";

	SetConsoleTextAttribute(h, 7);
	cout << Tap(8) << "             * * * ADD NEW CLIENT SCREEN * * *\n\n";

	SetConsoleTextAttribute(h, 3);
	cout << Tap(8) << "__________________________________________________________\n";
	cout << Tap(8) << "____________________________________________            \n\n";
}


void AddClientMenu()
{
	char Answer;
	do
	{
		system("cls");
		if (!CheckAccessPermission(enUserPermissions::pAddClient))
		{
			ShowAccessDeniedMessage();
			return;
		}

		PrintAddClientScreen();
		AddClients();

		cout << "\n\n\n";
		SetConsoleTextAttribute(h, 2);
		cout << Tap(8) << "Client Added Successfully...";

		cout << "\n\n";
		SetConsoleTextAttribute(h, 7);
		cout << Tap(8) << "> Do You Want To Add More Clients Y/N: ";
		cin >> Answer;
	} while (toupper(Answer) == 'Y');
}


// >>

void PrintUpdateClientScreen()
{
	cout << "\n\n";
	SetConsoleTextAttribute(h, 3);
	cout << Tap(8) << "__________________________________________________________\n";
	cout << Tap(8) << "               ___________________________________________\n\n\n";

	SetConsoleTextAttribute(h, 7);
	cout << Tap(8) << "             * * * UPDATE CLIENT SCREEN * * *\n\n";

	SetConsoleTextAttribute(h, 3);
	cout << Tap(8) << "__________________________________________________________\n";
	cout << Tap(8) << "____________________________________________            \n\n";
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
	if (!CheckAccessPermission(enUserPermissions::pUpdateClient))
	{
		ShowAccessDeniedMessage();
		return;
	}

	PrintUpdateClientScreen();

	char Answer;
	stClient Client;
	vector <stClient> vClient;

	vClient = LoadClientsDataFromFileToVector(ClientsFileName);
	string AccountNumber = ReadAccountNumber("Enter Account Number: ");

	if (FindClientByAccountNumber(AccountNumber, vClient, Client))
	{
		PrintClientCard(Client);
		cout << "\n";
		cout << Tap(8) << ">> Are You Sure You Want Update This Client Y/N: ";
		cin >> Answer;

		if (toupper(Answer) == 'Y')
		{
			UpdateClientData(AccountNumber, vClient);
			SaveUpdateDataToFile(ClientsFileName, vClient);

			cout << "\n\n";
			SetConsoleTextAttribute(h, 2);
			cout << Tap(8) << "Client Updated Successfully...\n";
			SetConsoleTextAttribute(h, 7);
		}
	}
	else
	{
		cout << "\n";
		cout << Tap(8);
		SetConsoleTextAttribute(h, 4);
		cout << "Client With Account Number[";
		SetConsoleTextAttribute(h, 7);
		cout << AccountNumber;
		SetConsoleTextAttribute(h, 4);
		cout << "] Is Not Found!";
		cout << "\n";
		cout << Tap(8);
		SetConsoleTextAttribute(h, 7);
	}
}


// >> 


void PrintDeleteClientScreen()
{
	cout << "\n\n";
	SetConsoleTextAttribute(h, 3);
	cout << Tap(8) << "__________________________________________________________\n";
	cout << Tap(8) << "               ___________________________________________\n\n\n";

	SetConsoleTextAttribute(h, 7);
	cout << Tap(8) << "             * * * DELETE CLIENT SCREEN * * *\n\n";

	SetConsoleTextAttribute(h, 3);
	cout << Tap(8) << "__________________________________________________________\n";
	cout << Tap(8) << "____________________________________________            \n\n";
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


void DeleteClientMenu()
{

	if (!CheckAccessPermission(enUserPermissions::pDeleteClient))
	{
		ShowAccessDeniedMessage();
		return;
	}

	PrintDeleteClientScreen();

	char Answer;
	stClient Client;
	vector <stClient> vClient;
	vClient = LoadClientsDataFromFileToVector(ClientsFileName);

	string AccountNumber;
	AccountNumber = ReadAccountNumber("Enter Account Number: ");

	if (FindClientByAccountNumber(AccountNumber, vClient, Client))
	{
		PrintClientCard(Client);

		cout << "\n";
		cout << Tap(8) << ">> Are You Sure You Want Delete This Client Y/N: ";
		cin >> Answer;

		if (toupper(Answer) == 'Y')
		{
			MarkClientDeleted(AccountNumber, vClient);
			SaveUpdateDataToFile(ClientsFileName, vClient);

			cout << "\n\n";
			SetConsoleTextAttribute(h, 2);
			cout << Tap(8) << "Client Deleted Successfully...\n";
			SetConsoleTextAttribute(h, 7);
		}
	}
	else
	{
		cout << "\n";
		cout << Tap(8);
		SetConsoleTextAttribute(h, 4);
		cout << "Client With Account Number[";
		SetConsoleTextAttribute(h, 7);
		cout << AccountNumber;
		SetConsoleTextAttribute(h, 4);
		cout << "] Is Not Found!";
		cout << "\n";
		cout << Tap(8);
		SetConsoleTextAttribute(h, 7);
	}
}



// >>

void PrintFindClientScreen()
{
	cout << "\n\n";
	SetConsoleTextAttribute(h, 3);
	cout << Tap(8) << "__________________________________________________________\n";
	cout << Tap(8) << "               ___________________________________________\n\n\n";

	SetConsoleTextAttribute(h, 7);
	cout << Tap(8) << "             * * * FIND CLIENT SCREEN * * *\n\n";

	SetConsoleTextAttribute(h, 3);
	cout << Tap(8) << "__________________________________________________________\n";
	cout << Tap(8) << "____________________________________________            \n\n";
}


void FindClientMenu()
{
	if (!CheckAccessPermission(enUserPermissions::pFindClient))
	{
		ShowAccessDeniedMessage();
		return;
	}

	PrintFindClientScreen();

	stClient Client;
	vector <stClient> vClient;
	vClient = LoadClientsDataFromFileToVector(ClientsFileName);
	string AccountNumber = ReadAccountNumber("Enter Account Number: ");

	if (FindClientByAccountNumber(AccountNumber, vClient, Client))
		PrintClientCard(Client);
	else
	{
		cout << "\n";
		cout << Tap(8);
		SetConsoleTextAttribute(h, 4);
		cout << "Client With Account Number[";
		SetConsoleTextAttribute(h, 7);
		cout << AccountNumber;
		SetConsoleTextAttribute(h, 4);
		cout << "] Is Not Found!";
		cout << "\n";
		cout << Tap(8);
		SetConsoleTextAttribute(h, 7);
	}
}


// >>


void PrintDepositScreen()
{
	cout << "\n\n";
	SetConsoleTextAttribute(h, 3);
	cout << Tap(8) << "__________________________________________________________\n";
	cout << Tap(8) << "               ___________________________________________\n\n\n";

	SetConsoleTextAttribute(h, 7);
	cout << Tap(8) << "             * * * DEPOSIT SCREEN * * *\n\n";

	SetConsoleTextAttribute(h, 3);
	cout << Tap(8) << "__________________________________________________________\n";
	cout << Tap(8) << "____________________________________________            \n\n";
}


void DepositAmountByAccountNumber(string AccountNumber, vector <stClient>& vClient)
{
	float DepositAmount;
	cout << "\n";
	cout << Tap(8) << ">> Please Enter Deposit Amount: ";
	cin >> DepositAmount;

	while (cin.fail())
	{
		cin.clear();
		cin.ignore(1000, '\n');
		SetConsoleTextAttribute(h, 4);
		cout << "\n";
		cout << Tap(8) << ">> Invalid Operation, Please Try Again... \n\n";

		SetConsoleTextAttribute(h, 7);
		cout << "\n";
		cout << Tap(8) << ">> Please Enter Deposit Amount: ";
		cin >> DepositAmount;
	}

	char Answer;

	cout << "\n";
	cout << Tap(8) << ">> Are You Sure You Want Perform This Transaction Y/N: ";
	cin >> Answer;

	if (toupper(Answer) == 'Y')
	{
		for (stClient& C : vClient)
		{
			if (C.AccountNumber == AccountNumber)
			{
				C.AccountBalance += DepositAmount;
				SaveUpdateDataToFile(ClientsFileName, vClient);

				cout << "\n";
				SetConsoleTextAttribute(h, 2);
				cout << Tap(8) << "Done Successfully...\n\n";

				SetConsoleTextAttribute(h, 7);
				cout << Tap(8) << ">> New Balance = [";

				SetConsoleTextAttribute(h, 2);
				cout << C.AccountBalance;

				SetConsoleTextAttribute(h, 7);
				cout << "]" << endl;

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
	vClient = LoadClientsDataFromFileToVector(ClientsFileName);

	string AccountNumber = ReadAccountNumber("Enter Account Number: ");

	if (FindClientByAccountNumber(AccountNumber, vClient, Client))
	{
		PrintClientCard(Client);
		DepositAmountByAccountNumber(AccountNumber, vClient);
	}
	else
	{
		cout << "\n";
		cout << Tap(8);
		SetConsoleTextAttribute(h, 4);
		cout << "Client With Account Number[";
		SetConsoleTextAttribute(h, 7);
		cout << AccountNumber;
		SetConsoleTextAttribute(h, 4);
		cout << "] Is Not Found!";
		cout << "\n";
		cout << Tap(8);
		SetConsoleTextAttribute(h, 7);
	}
}


// >>

void PrintWithdrawScreen()
{
	cout << "\n\n";
	SetConsoleTextAttribute(h, 3);
	cout << Tap(8) << "__________________________________________________________\n";
	cout << Tap(8) << "               ___________________________________________\n\n\n";

	SetConsoleTextAttribute(h, 7);
	cout << Tap(8) << "             * * * WITHDRAW SCREEN * * *\n\n";

	SetConsoleTextAttribute(h, 3);
	cout << Tap(8) << "__________________________________________________________\n";
	cout << Tap(8) << "____________________________________________            \n\n";
}


void WithdrawAmountByAccountNumber(string AccountNumber, vector <stClient>& vClient, stClient Client)
{
	float WithdrawAmount;

	cout << "\n";
	cout << Tap(8) << ">> Please Enter Withdraw Amount: ";
	cin >> WithdrawAmount;


	while (cin.fail())
	{
		cin.clear();
		cin.ignore(1000, '\n');

		cout << "\n";
		SetConsoleTextAttribute(h, 4);
		cout << Tap(8) << ">> Invalid Operation, Please Try Again... \n\n";

		cout << "\n";
		SetConsoleTextAttribute(h, 7);
		cout << Tap(8) << ">> Please Enter Withdraw Amount: ";
		cin >> WithdrawAmount;
	}


	while (WithdrawAmount > Client.AccountBalance)
	{
		cout << "\n";
		SetConsoleTextAttribute(h, 4);
		cout << Tap(8) << "Amount Exceeds The Balance, You Can Withdraw Up To! \n";


		cout << "\n";
		SetConsoleTextAttribute(h, 7);
		cout << Tap(8) << ">> Please Enter Withdraw Amount: ";
		cin >> WithdrawAmount;

		while (cin.fail())
		{
			cin.clear();
			cin.ignore(1000, '\n');

			cout << "\n";
			SetConsoleTextAttribute(h, 4);
			cout << Tap(8) << ">> Invalid Operation, Please Try Again... \n\n";

			cout << "\n";
			SetConsoleTextAttribute(h, 7);
			cout << Tap(8) << ">> Please Enter Withdraw Amount: ";
			cin >> WithdrawAmount;
		}
	}

	char Answer;

	cout << "\n";
	cout << Tap(8) << ">> Are You Sure You Want Perform This Transaction Y/N: ";
	cin >> Answer;

	if (toupper(Answer) == 'Y')
	{
		for (stClient& C : vClient)
		{
			if (C.AccountNumber == AccountNumber)
			{
				C.AccountBalance -= WithdrawAmount;
				SaveUpdateDataToFile(ClientsFileName, vClient);

				cout << "\n";
				SetConsoleTextAttribute(h, 2);
				cout << Tap(8) << "Done Successfully...\n\n";

				SetConsoleTextAttribute(h, 7);
				cout << Tap(8) << ">> New Balance = [";

				SetConsoleTextAttribute(h, 2);
				cout << C.AccountBalance;

				SetConsoleTextAttribute(h, 7);
				cout << "]" << endl;
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
	vClient = LoadClientsDataFromFileToVector(ClientsFileName);

	string AccountNumber = ReadAccountNumber("Enter Account Number: ");

	if (FindClientByAccountNumber(AccountNumber, vClient, Client))
	{
		PrintClientCard(Client);
		WithdrawAmountByAccountNumber(AccountNumber, vClient, Client);
	}
	else
	{
		cout << "\n";
		cout << Tap(8);
		SetConsoleTextAttribute(h, 4);
		cout << "Client With Account Number[";
		SetConsoleTextAttribute(h, 7);
		cout << AccountNumber;
		SetConsoleTextAttribute(h, 4);
		cout << "] Is Not Found!";
		cout << "\n";
		cout << Tap(8);
		SetConsoleTextAttribute(h, 7);
	}
}


// >>

void PrintClientBalanceRecord(stClient Client)
{
	cout << Tap(8);
	cout << Separator("| ");
	SetConsoleTextAttribute(h, 7);
	cout << left << setw(16) << Client.AccountNumber;
	cout << Separator("| ");
	SetConsoleTextAttribute(h, 7);
	cout << left << setw(29) << Client.Name;
	cout << Separator("| ");
	SetConsoleTextAttribute(h, 7);
	cout << left << setw(9) << Client.AccountBalance;
}


void TotalBalancesMenu()
{
	double TotalBalances = 0;
	vector <stClient> vClient;
	vClient = LoadClientsDataFromFileToVector(ClientsFileName);

	cout << "\n\n";
	SetConsoleTextAttribute(h, 1);
	cout << Tap(8) << "______________________________________________________________________________\n\n";

	SetConsoleTextAttribute(h, 7);
	cout << Tap(8) << "                       Balances List [" << vClient.size() << "]\n";

	SetConsoleTextAttribute(h, 1);
	cout << Tap(8) << "______________________________________________________________________________\n\n";

	cout << Tap(8);
	cout << Separator("| ");
	SetConsoleTextAttribute(h, 7);
	cout << left << setw(16) << "Account Number";
	cout << Separator("| ");
	SetConsoleTextAttribute(h, 7);
	cout << left << setw(29) << "Client Name";
	cout << Separator("| ");
	SetConsoleTextAttribute(h, 7);
	cout << left << setw(9) << "Balance" << endl;

	SetConsoleTextAttribute(h, 1);
	cout << Tap(8) << "______________________________________________________________________________\n\n";

	for (stClient& C : vClient)
	{
		PrintClientBalanceRecord(C);
		TotalBalances += C.AccountBalance;
		cout << endl;
	}

	cout << "\n";
	SetConsoleTextAttribute(h, 1);
	cout << Tap(8) << "______________________________________________________________________________\n";

	cout << "\n\n";
	SetConsoleTextAttribute(h, 7);
	cout << Tap(10) << "      >> Total Balances = [";
	SetConsoleTextAttribute(h, 2);
	cout << TotalBalances;
	SetConsoleTextAttribute(h, 7);
	cout << "]" << endl;
}


// >>

short ReadTransactionMenuOption()
{
	short Number;
	do {
		cout << Tap(8) << "> Choose What Do You Want To Do [ 1 To 4 ]: ";
		cin >> Number;
		while (cin.fail()) {
			cin.clear();
			cin.ignore(1000, '\n');

			cout << "\n";
			SetConsoleTextAttribute(h, 4);
			cout << Tap(8) << ">> Invalid Choosing, Please Try Again... \n";

			SetConsoleTextAttribute(h, 7);
			cout << Tap(8) << "Choose What Do You Want To Do [ 1 To 4 ]: ";
			cin >> Number;
		}
	} while (Number < 1 || Number > 4);
	return Number;

}


void GoToTransactionsMenu()
{
	cout << "\n\n\n";
	cout << Tap(8) << "Press Any Key To Go Back Transaction Menu...";
	system("pause >0");
	TransactionsMenu();
}


void PerformTransactionsOperations(enTransactionOperationType TransType)
{
	switch (TransType)
	{
	case::eDeposit:
		system("Cls");
		DepositMenuScreen();
		GoToTransactionsMenu();
		break;

	case::eWithdraw:
		system("Cls");
		WithdrawMenuScreen();
		GoToTransactionsMenu();
		break;

	case::eTotalBalances:
		system("Cls");
		TotalBalancesMenu();
		GoToTransactionsMenu();
		break;

	case::eMainMenu:
		system("Cls");
		MainMenuScreen();
		break;
	}
}


void TransactionsMenu()
{
	if (!CheckAccessPermission(enUserPermissions::pTransactions))
	{
		ShowAccessDeniedMessage();
		GoToMainMenuScreen();
		return;
	}

	system("Cls");

	cout << "\n\n";
	SetConsoleTextAttribute(h, 1);
	cout << Tap(8) << "__________________________________________________________\n";
	cout << Tap(8) << "               ___________________________________________\n\n\n";

	SetConsoleTextAttribute(h, 7);
	cout << Tap(8) << "             * * * TRANSACTIONS MENU SCREEN * * *\n\n";

	SetConsoleTextAttribute(h, 1);
	cout << Tap(8) << "__________________________________________________________\n";
	cout << Tap(8) << "____________________________________________            \n\n";

	SetConsoleTextAttribute(h, 7);
	cout << Tap(8) << "[1] Deposit.\n";
	cout << Tap(8) << "[2] Withdraw.\n";
	cout << Tap(8) << "[3] Total Balances.\n";
	cout << Tap(8) << "[4] Main Menu.\n\n";

	SetConsoleTextAttribute(h, 1);
	cout << Tap(8) << "__________________________________________________________\n";

	SetConsoleTextAttribute(h, 7);
	PerformTransactionsOperations((enTransactionOperationType)ReadTransactionMenuOption());
}


// >>


stUser ConvertUserLineToRecord(string S1)
{
	stUser User;
	vector <string> vS1;
	vS1 = SplitString(S1);
	User.UserName = vS1[0];
	User.Password = vS1[1];
	User.Permissions = stoi(vS1[2]);
	return User;
}


vector <stUser> LoadUsersDataFromFileToVector(string FileName)
{
	vector <stUser> vUser;

	fstream MyFile;
	MyFile.open(FileName, ios::in);

	if (MyFile.is_open())
	{
		string Line;
		stUser User;

		while (getline(MyFile, Line))
		{
			User = ConvertUserLineToRecord(Line);
			vUser.push_back(User);
		}

		MyFile.close();
	}

	return vUser;
}


string ReadUserName(string Message)
{
	string Name;
	cout << Tap(8) << Message;
	getline(cin >> ws, Name);
	return Name;
}


bool FindUserByUserNameAndPassword(string UserName, string Password, stUser& User)
{
	vector <stUser> vUser;
	vUser = LoadUsersDataFromFileToVector(UsersFileName);

	for (stUser& U : vUser)
	{
		if (U.UserName == UserName && U.Password == Password)
		{
			User = U;
			return true;
		}
	}

	return false;
}


void PrintUserRecord(stUser User)
{
	cout << Tap(8);
	cout << Separator("| ");
	SetConsoleTextAttribute(h, 7);
	cout << left << setw(15) << User.UserName;
	cout << Separator("| ");
	SetConsoleTextAttribute(h, 7);
	cout << left << setw(15) << User.Password;
	cout << Separator("| ");
	SetConsoleTextAttribute(h, 7);
	cout << left << setw(15) << User.Permissions;
}


void ShowUserListMenu()
{
	vector <stUser> vUser;
	vUser = LoadUsersDataFromFileToVector(UsersFileName);

	cout << "\n\n";
	SetConsoleTextAttribute(h, 1);
	cout << Tap(8) << "_______________________________________________________________\n\n";

	SetConsoleTextAttribute(h, 7);
	cout << Tap(8) << "                         User List [" << vUser.size() << "]\n";

	SetConsoleTextAttribute(h, 1);
	cout << Tap(8) << "_______________________________________________________________\n\n";

	cout << Tap(8);
	cout << Separator("| ");
	SetConsoleTextAttribute(h, 7);
	cout << left << setw(15) << "User Name";
	cout << Separator("| ");
	SetConsoleTextAttribute(h, 7);
	cout << left << setw(15) << "Password";
	cout << Separator("| ");
	SetConsoleTextAttribute(h, 7);
	cout << left << setw(15) << "Permissions" << endl;

	SetConsoleTextAttribute(h, 1);
	cout << Tap(8) << "_______________________________________________________________\n\n";

	for (stUser& U : vUser)
	{
		PrintUserRecord(U);
		cout << endl;
	}

	SetConsoleTextAttribute(h, 1);
	cout << Tap(8) << "_______________________________________________________________\n";
	SetConsoleTextAttribute(h, 7);
}


// >>

void PrintAddUserScreen()
{
	cout << "\n\n";
	SetConsoleTextAttribute(h, 3);
	cout << Tap(8) << "__________________________________________________________\n";
	cout << Tap(8) << "               ___________________________________________\n\n\n";

	SetConsoleTextAttribute(h, 7);
	cout << Tap(8) << "             * * * ADD NEW USER SCREEN * * *\n\n";

	SetConsoleTextAttribute(h, 3);
	cout << Tap(8) << "__________________________________________________________\n";
	cout << Tap(8) << "____________________________________________            \n\n";
	SetConsoleTextAttribute(h, 7);
}


bool IsUserFound(string UserName, vector <stUser> vUser, stUser& User)
{
	for (stUser& U : vUser)
	{
		if (U.UserName == UserName)
		{
			User = U;
			return true;
		}
	}

	return false;
}


short ReadUserPermissions()
{
	short Permissions = 0;
	char Answer = 'n';


	cout << "\n\n";
	cout << Tap(8) << "> Do You Want Give Full Access Y/N: ";
	cin >> Answer;
	if (toupper(Answer) == 'Y')
		return -1;

	cout << "\n";
	cout << Tap(8) << "> Do You Want To Give Access To: ";

	cout << "\n";
	cout << Tap(8) << ">> Show Client List Y/N: ";
	cin >> Answer;

	if (toupper(Answer) == 'Y')
		Permissions += enUserPermissions::pShowClient;


	cout << "\n";
	cout << Tap(8) << ">> Add New Client Y/N: ";
	cin >> Answer;

	if (toupper(Answer) == 'Y')
		Permissions += enUserPermissions::pAddClient;


	cout << "\n";
	cout << Tap(8) << ">> Delete Client Y/N: ";
	cin >> Answer;

	if (toupper(Answer) == 'Y')
		Permissions += enUserPermissions::pDeleteClient;


	cout << "\n";
	cout << Tap(8) << ">> Update Client Y/N: ";
	cin >> Answer;

	if (toupper(Answer) == 'Y')
		Permissions += enUserPermissions::pUpdateClient;


	cout << "\n";
	cout << Tap(8) << ">> Find Client Y/N: ";
	cin >> Answer;

	if (toupper(Answer) == 'Y')
		Permissions += enUserPermissions::pFindClient;

	cout << "\n";
	cout << Tap(8) << ">> Transactions Y/N: ";
	cin >> Answer;

	if (toupper(Answer) == 'Y')
		Permissions += enUserPermissions::pTransactions;


	cout << "\n";
	cout << Tap(8) << ">> Manage Users Y/N: ";
	cin >> Answer;

	if (toupper(Answer) == 'Y')
		Permissions += enUserPermissions::pManageUsers;

	return Permissions;
}


stUser ReadNewUser()
{
	stUser User;
	vector <stUser> vUser;
	vUser = LoadUsersDataFromFileToVector(UsersFileName);
	User.UserName = ReadUserName("Enter UserName: ");

	while (IsUserFound(User.UserName, vUser, User))
	{
		cout << "\n";
		cout << Tap(8);
		SetConsoleTextAttribute(h, 4);
		cout << "User With [";
		SetConsoleTextAttribute(h, 7);
		cout << User.UserName;
		SetConsoleTextAttribute(h, 4);
		cout << "] Already Exists....\n\n";

		SetConsoleTextAttribute(h, 7);
		User.UserName = ReadUserName("Enter Another UserName: ");
	}
	User.Password = ReadUserName("Enter Password: ");
	User.Permissions = ReadUserPermissions();
	return User;
}


string ConvertUserRecordToLine(stUser User, string Delim = "#//#")
{
	string Line = "";
	Line += User.UserName + Delim;
	Line += User.Password + Delim;
	Line += to_string(User.Permissions);
	return Line;
}


void AddUser()
{
	stUser User = ReadNewUser();
	AddDataToFile(UsersFileName, ConvertUserRecordToLine(User));
}


void AddUserMenuScreen()
{
	char AddMore;
	do
	{
		system("Cls");
		PrintAddUserScreen();
		AddUser();

		cout << "\n\n";
		SetConsoleTextAttribute(h, 2);
		cout << Tap(8) << "User Added Successfully... \n\n";

		SetConsoleTextAttribute(h, 7);
		cout << Tap(8) << "Do You Want Add More Users Y/N:";
		cin >> AddMore;

	} while (toupper(AddMore) == 'Y');
}


// >>

void PrintDeleteUserScreen()
{
	cout << "\n\n";
	SetConsoleTextAttribute(h, 3);
	cout << Tap(8) << "__________________________________________________________\n";
	cout << Tap(8) << "               ___________________________________________\n\n\n";

	SetConsoleTextAttribute(h, 7);
	cout << Tap(8) << "             * * * DELETE USER SCREEN * * *\n\n";

	SetConsoleTextAttribute(h, 3);
	cout << Tap(8) << "__________________________________________________________\n";
	cout << Tap(8) << "____________________________________________            \n\n";
	SetConsoleTextAttribute(h, 7);
}


void PrintUserCard(stUser User)
{
	cout << "\n\n";
	SetConsoleTextAttribute(h, 1);
	cout << Tap(8) << "__________________________________________________________\n";

	SetConsoleTextAttribute(h, 7);
	cout << Tap(8) << "                     User Details                         \n";

	SetConsoleTextAttribute(h, 1);
	cout << Tap(8) << "__________________________________________________________\n";

	SetConsoleTextAttribute(h, 7);
	cout << Tap(8) << "User Name  : " << User.UserName << endl;
	cout << Tap(8) << "Password   : " << User.Password << endl;
	cout << Tap(8) << "Permissions : " << User.Permissions << endl;

	SetConsoleTextAttribute(h, 1);
	cout << Tap(8) << "__________________________________________________________\n";
	SetConsoleTextAttribute(h, 7);
}


void MarkUserDeleted(string UserName, vector <stUser>& vUser)
{
	for (stUser& U : vUser)
	{
		if (U.UserName == UserName)
		{
			U.MarkUserDeleted = true;
			return;
		}
	}
}


void SaveUpdatedUsersDataToFile(string FileName, vector <stUser>& vUser)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out);

	if (MyFile.is_open())
	{
		string Line;

		for (stUser& U : vUser)
		{
			if (U.MarkUserDeleted == false)
			{
				Line = ConvertUserRecordToLine(U);

				MyFile << Line << endl;
			}
		}

		MyFile.close();
	}
}


void DeleteUserMenu()
{
	PrintDeleteUserScreen();

	char Answer;
	stUser User;
	vector <stUser> vUser;
	vUser = LoadUsersDataFromFileToVector(UsersFileName);

	string UserName = ReadUserName("Please Enter User Name: ");

	if (IsUserFound(UserName, vUser, User))
	{

		if (User.UserName == "Admin")
		{
			cout << "\n\n";
			cout << Tap(8) << "> You Cannot Delete This User. \n\n";
			return;
		}

		PrintUserCard(User);

		cout << "\n\n";
		cout << Tap(8) << "> Are You Sure You Want Delete This User Y/N: ";
		cin >> Answer;

		if (toupper(Answer) == 'Y')
		{
			MarkUserDeleted(UserName, vUser);
			SaveUpdatedUsersDataToFile(UsersFileName, vUser);

			cout << "\n\n";
			SetConsoleTextAttribute(h, 2);
			cout << Tap(8) << "User Deleted Successfully...\n";

			SetConsoleTextAttribute(h, 7);
		}
	}
	else
	{
		cout << "\n";
		cout << Tap(8);
		SetConsoleTextAttribute(h, 4);
		cout << "User With Username[";
		SetConsoleTextAttribute(h, 7);
		cout << UserName;
		SetConsoleTextAttribute(h, 4);
		cout << "] Is Not Found!";
		cout << "\n";
		cout << Tap(8);
		SetConsoleTextAttribute(h, 7);
	}
}

// >>

void PrintUpdateUserScreen()
{
	cout << "\n\n";
	SetConsoleTextAttribute(h, 3);
	cout << Tap(8) << "__________________________________________________________\n";
	cout << Tap(8) << "               ___________________________________________\n\n\n";

	SetConsoleTextAttribute(h, 7);
	cout << Tap(8) << "             * * * UPDATE USER SCREEN * * *\n\n";

	SetConsoleTextAttribute(h, 3);
	cout << Tap(8) << "__________________________________________________________\n";
	cout << Tap(8) << "____________________________________________            \n\n";
	SetConsoleTextAttribute(h, 7);
}


stUser ReadUpdateUserData(string UserName)
{
	stUser User;

	User.UserName = UserName;
	User.Password = ReadUserName("Enter Password: ");
	User.Permissions = ReadUserPermissions();

	return User;
}


void UpdateUserData(string UserName, vector <stUser>& vUser)
{
	for (stUser& U : vUser)
	{
		if (U.UserName == UserName)
		{
			U = ReadUpdateUserData(U.UserName);
			return;
		}
	}
}


void UpdateUserMenu()
{
	PrintUpdateUserScreen();

	char Answer;
	stUser User;
	vector <stUser> vUser;
	vUser = LoadUsersDataFromFileToVector(UsersFileName);

	string UserName = ReadUserName("Please Enter User Name: ");

	if (IsUserFound(UserName, vUser, User))
	{
		PrintUserCard(User);

		cout << "\n";
		cout << Tap(8) << "> Are You Sure You Want Update This User Y/N: ";
		cin >> Answer;

		if (toupper(Answer) == 'Y')
		{
			cout << "\n";
			UpdateUserData(UserName, vUser);
			SaveUpdatedUsersDataToFile(UsersFileName, vUser);

			cout << "\n\n";
			SetConsoleTextAttribute(h, 2);
			cout << Tap(8) << "User Updated Successfully...\n";

			SetConsoleTextAttribute(h, 7);
		}
	}
	else
	{
		cout << "\n";
		cout << Tap(8);
		SetConsoleTextAttribute(h, 4);
		cout << "User With Username[";
		SetConsoleTextAttribute(h, 7);
		cout << UserName;
		SetConsoleTextAttribute(h, 4);
		cout << "] Is Not Found!";
		cout << "\n";
		cout << Tap(8);
		SetConsoleTextAttribute(h, 7);

	}
}


// >>

void PrintFindUserScreen()
{
	cout << "\n\n";
	SetConsoleTextAttribute(h, 3);
	cout << Tap(8) << "__________________________________________________________\n";
	cout << Tap(8) << "               ___________________________________________\n\n\n";

	SetConsoleTextAttribute(h, 7);
	cout << Tap(8) << "             * * * FIND USER SCREEN * * *\n\n";

	SetConsoleTextAttribute(h, 3);
	cout << Tap(8) << "__________________________________________________________\n";
	cout << Tap(8) << "____________________________________________            \n\n";
	SetConsoleTextAttribute(h, 7);
}


void FindUserMenu()
{
	PrintFindUserScreen();

	string UserName = ReadUserName("Enter User Name: ");

	stUser User;
	vector <stUser> vUser;
	vUser = LoadUsersDataFromFileToVector(UsersFileName);

	if (IsUserFound(UserName, vUser, User))
		PrintUserCard(User);
	else
	{
		cout << "\n";
		cout << Tap(8);
		SetConsoleTextAttribute(h, 4);
		cout << "User With Username[";
		SetConsoleTextAttribute(h, 7);
		cout << UserName;
		SetConsoleTextAttribute(h, 4);
		cout << "] Is Not Found!";
		cout << "\n";
		cout << Tap(8);
		SetConsoleTextAttribute(h, 7);
	}
}

// >>

void GoToManageUsersMenu()
{
	cout << "\n\n\n\n";
	SetConsoleTextAttribute(h, 7);
	cout << Tap(8) << "Press Any Key To Go Back To Manage Users Screen...";
	system("pause > 0");
	ManageUsersMenu();
}


short ReadManageUsersMenuOption()
{
	short Number;
	do {
		cout << Tap(8) << ">> Choose What Do You Want To Do [ 1 To 6 ]: ";
		cin >> Number;
		while (cin.fail())
		{

			cin.clear();
			cin.ignore(1000, '\n');

			cout << "\n";
			SetConsoleTextAttribute(h, 4);
			cout << Tap(8) << ">> Invalid Choosing, Please Try Again... \n";

			SetConsoleTextAttribute(h, 7);
			cout << Tap(8) << "Choose What Do You Want To Do [ 1 To 6 ]: ";
			cin >> Number;

		}

	} while (Number < 1 || Number > 6);
	return Number;
}


void PerformManageUsersMenuOption(enManageUserMenuOptions ManageUser)
{
	switch (ManageUser)
	{
	case enManageUserMenuOptions::eListUsers:
		system("Cls");
		ShowUserListMenu();
		GoToManageUsersMenu();
		break;

	case enManageUserMenuOptions::eAddNewUser:
		system("Cls");
		AddUserMenuScreen();
		GoToManageUsersMenu();
		break;

	case enManageUserMenuOptions::eUpdateUser:
		system("Cls");
		UpdateUserMenu();
		GoToManageUsersMenu();
		break;

	case enManageUserMenuOptions::eDeleteUser:
		system("Cls");
		DeleteUserMenu();
		GoToManageUsersMenu();
		break;

	case enManageUserMenuOptions::eFindUser:
		system("Cls");
		FindUserMenu();
		GoToManageUsersMenu();
		break;

	case enManageUserMenuOptions::eGoToMainMenu:
		system("Cls");
		MainMenuScreen();
		break;
	}
}


void ManageUsersMenu()
{
	if (!CheckAccessPermission(enUserPermissions::pManageUsers))
	{
		ShowAccessDeniedMessage();
		GoToMainMenuScreen();
		return;
	}

	system("Cls");

	cout << "\n\n";
	SetConsoleTextAttribute(h, 1);
	cout << Tap(8) << "__________________________________________________________\n";
	cout << Tap(8) << "               ___________________________________________\n\n\n";

	SetConsoleTextAttribute(h, 7);
	cout << Tap(8) << "             * * * MANAGE USERS MENU SCREEN * * *\n\n";

	SetConsoleTextAttribute(h, 1);
	cout << Tap(8) << "__________________________________________________________\n";
	cout << Tap(8) << "____________________________________________            \n\n";

	SetConsoleTextAttribute(h, 7);
	cout << Tap(8) << "[1] List Users.\n";
	cout << Tap(8) << "[2] Add New User.\n";
	cout << Tap(8) << "[3] Update User.\n";
	cout << Tap(8) << "[4] Delete User.\n";
	cout << Tap(8) << "[5] Find User.\n";
	cout << Tap(8) << "[6] Main Menu.\n\n";

	SetConsoleTextAttribute(h, 1);
	cout << Tap(8) << "__________________________________________________________\n";

	SetConsoleTextAttribute(h, 7);
	PerformManageUsersMenuOption((enManageUserMenuOptions)ReadManageUsersMenuOption());
}

void PrintLoginScreen()
{
	cout << "\n\n";
	SetConsoleTextAttribute(h, 3);
	cout << Tap(8) << "__________________________________________________________\n";
	cout << Tap(8) << "               ___________________________________________\n\n\n";

	SetConsoleTextAttribute(h, 7);
	cout << Tap(8) << "             * * * LOGIN SCREEN * * *\n\n";

	SetConsoleTextAttribute(h, 3);
	cout << Tap(8) << "__________________________________________________________\n";
	cout << Tap(8) << "____________________________________________            \n\n";
}

void PrintProgramEnd()
{
	cout << "\n---------------------------------------------\n";
	cout << "                Program Ends :-)                ";
	cout << "\n---------------------------------------------\n";
}

short ReadMainMenuOption()
{
	short Number;
	do
	{
		cout << "\n";
		SetConsoleTextAttribute(h, 7);
		cout << Tap(8) << "Select Your Option (1 - 8): ";
		cin >> Number;

		while (cin.fail())
		{
			cin.clear();
			cin.ignore(1000, '\n');

			cout << "\n";
			SetConsoleTextAttribute(h, 4);
			cout << Tap(8) << "Invalid Input, ";
			SetConsoleTextAttribute(h, 7);
			cout << "Please re-enter: ";
			cin >> Number;
		}

	} while (Number < 1 || Number > 8);
	return Number;
}

void PerformMainMenuOption(enMainMenuOptions MainMenuOption)
{
	switch (MainMenuOption)
	{
	case enMainMenuOptions::eShowClientList:
		system("cls");
		ShowClientListMenu();
		GoToMainMenuScreen();
		break;

	case enMainMenuOptions::eAddNewClient:
		system("cls");
		AddClientMenu();
		GoToMainMenuScreen();
		break;

	case enMainMenuOptions::eUpdateClientInfo:
		system("cls");
		UpdateClientMenu();
		GoToMainMenuScreen();
		break;

	case enMainMenuOptions::eDeleteClient:
		system("cls");
		DeleteClientMenu();
		GoToMainMenuScreen();
		break;

	case enMainMenuOptions::eFindClient:
		system("cls");
		FindClientMenu();
		GoToMainMenuScreen();
		break;

	case enMainMenuOptions::eTransactions:
		system("cls");
		TransactionsMenu();
		break;

	case enMainMenuOptions::eManageUsers:
		system("cls");
		ManageUsersMenu();
		break;


	case enMainMenuOptions::eLogout:
		system("cls");
		Login();
		break;
	}
}

void MainMenuScreen()
{
	system("cls");

	cout << "\n\n";
	SetConsoleTextAttribute(h, 1);
	cout << Tap(8) << "__________________________________________________________\n";
	cout << Tap(8) << "               ___________________________________________\n\n\n";

	SetConsoleTextAttribute(h, 7);
	cout << Tap(8) << "             * * * MAIN MENU SCREEN * * *\n\n";

	SetConsoleTextAttribute(h, 1);
	cout << Tap(8) << "__________________________________________________________\n";
	cout << Tap(8) << "____________________________________________            \n\n";

	SetConsoleTextAttribute(h, 7);
	cout << Tap(8) << "[1] Show Client List.\n";
	cout << Tap(8) << "[2] Add New Client.\n";
	cout << Tap(8) << "[3] Update Client Info.\n";
	cout << Tap(8) << "[4] Delete Client.\n";
	cout << Tap(8) << "[5] Find Client.\n";
	cout << Tap(8) << "[6] Transactions.\n";
	cout << Tap(8) << "[7] Manage Users.\n";
	cout << Tap(8) << "[8] Logout.\n\n";

	SetConsoleTextAttribute(h, 1);
	cout << Tap(8) << "__________________________________________________________\n";

	SetConsoleTextAttribute(h, 7);
	PerformMainMenuOption((enMainMenuOptions)ReadMainMenuOption());
}

void GoToMainMenuScreen()
{
	cout << "\n\n\n\n";
	SetConsoleTextAttribute(h, 7);
	cout << Tap(8) << "Press Any Key To Go Back To Main Menu...";
	system("pause > 0");
	MainMenuScreen();
}

bool LoadCurrentUserInfo(string UserName, string Password)
{
	if (FindUserByUserNameAndPassword(UserName, Password, CurrentUser))
		return true;
	else
		return false;
}

void Login()
{
	bool LoginValid = false;
	string UserName, Password;
	do
	{
		system("Cls");
		PrintLoginScreen();

		if (LoginValid)
		{
			SetConsoleTextAttribute(h, 4);
			cout << "\n";
			cout << Tap(8) << "Invalid UserName And Password! :-( ";
			SetConsoleTextAttribute(h, 7);
			cout << "Please re-enter.  \n";
		}

		SetConsoleTextAttribute(h, 7);
		cout << "\n";
		UserName = ReadUserName("Enter UserName: ");
		cout << "\n";
		Password = ReadUserName("Enter Password: ");

		LoginValid = !LoadCurrentUserInfo(UserName, Password);

	} while (LoginValid);

	SetConsoleTextAttribute(h, 2);
	cout << "\n";
	cout << Tap(8) << "You Are Logged In Successfully :-) ! \n\n";

	SetConsoleTextAttribute(h, 7);
	cout << Tap(8) << "Press Any Key To Continue....";
	system("Pause > 0");

	MainMenuScreen();
}

int main()
{
	Login();
	return 0;
}
