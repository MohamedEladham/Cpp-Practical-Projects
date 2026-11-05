#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include <limits>
#include <cctype>
using namespace std;

const string ClientsFileName = "Clients_Data.txt";

enum enOperationType { eShowClient = 1, eAddClient = 2, eUpdateClient = 3, eDeleteClient = 4, eFindClient = 5, eExit = 6 };

struct stClient
{
	string AccountNumber;
	string PinCode;
	string Name;
	string Phone;
	double AccountBalance;
	bool MarkDelete = false;
};

void MainMenuScreen();

vector <string> SplitString(string String, string Delim = "#//#")
{
	vector <string> vString;

	short Pos = 0;
	string Word = "";

	while ((Pos = String.find(Delim)) != String.npos)
	{
		Word = String.substr(0, Pos);

		if (Word != "")
		{
			vString.push_back(Word);
		}

		String = String.erase(0, Pos + Delim.length());
	}

	if (String != "")
	{
		vString.push_back(String);
	}

	return vString;
}

stClient ConvertLineToRecord(string String)
{
	stClient Client;
	vector <string> vString;
	vString = SplitString(String);

	Client.AccountNumber = vString[0];
	Client.PinCode = vString[1];
	Client.Name = vString[2];
	Client.Phone = vString[3];
	Client.AccountBalance = stod(vString[4]);

	return Client;
}

vector <stClient> LoadDataFromFileToVector(string FileName)
{
	vector <stClient> vClients;

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

void PrintClientData(stClient Client)
{
	cout << "| " << left << setw(16) << Client.AccountNumber;
	cout << "| " << left << setw(10) << Client.PinCode;
	cout << "| " << left << setw(28) << Client.Name;
	cout << "| " << left << setw(15) << Client.Phone;
	cout << "| " << left << setw(10) << Client.AccountBalance;
}

void PrintAllClientsOnTheScreen()
{
	vector <stClient> vClients;
	vClients = LoadDataFromFileToVector(ClientsFileName);
	cout << "\n";
	cout << "\t\t\t   Client List [" << vClients.size() << "] Client(s). \n";
	cout << "_______________________________________________________________________________________\n\n";
	cout << "| " << left << setw(16) << "Account Number" << "| " << left << setw(10) << "Pin Code";
	cout << "| " << left << setw(28) << "Client Name" << "| " << left << setw(15) << "Phone";
	cout << "| " << left << setw(10) << "Balance" << endl;
	cout << "_______________________________________________________________________________________\n\n";

	for (stClient& Client : vClients)
	{
		PrintClientData(Client);
		cout << endl;
	}

	cout << "\n\n_______________________________________________________________________________________\n\n";
}

void AddingClientsMenu()
{
	cout << "\n";
	cout << "-----------------------------------------------\n";
	cout << "              Add New Client Screen            \n";
	cout << "-----------------------------------------------\n";
	cout << "Adding New Client: \n\n";
}

string ReadAccountNumber()
{
	string AccountNumber;
	cout << "\nEnter Account Number: ";
	getline(cin >> ws, AccountNumber);
	return AccountNumber;
}

bool CheckIfClientExists(string AccountNumber, vector <stClient>& vClients)
{
	for (stClient& Client : vClients)
	{
		if (Client.AccountNumber == AccountNumber)
			return true;
	}

	return false;
}

stClient ReadNewClientData()
{
	stClient Client;

	vector <stClient> vClients;
	vClients = LoadDataFromFileToVector(ClientsFileName);
	string AccountNumber = ReadAccountNumber();

	while (CheckIfClientExists(AccountNumber, vClients))
	{
		cout << "\nClient With [" << AccountNumber << "] Already Exists, Enter Another Account Number: ";
		getline(cin, AccountNumber);
	}
	cout << "\n";

	Client.AccountNumber = AccountNumber;

	cout << "Enter Pin Code: ";
	getline(cin >> ws, Client.PinCode);
	cout << "Enter Name    : ";
	getline(cin, Client.Name);
	cout << "Enter Phone   : ";
	getline(cin, Client.Phone);
	cout << "Enter Account Balance: ";
	cin >> Client.AccountBalance;
	while (cin.fail())
	{
		cin.clear();
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		cout << "\nInvalid Balance, Please Try Again: \n";
		cout << "Enter Account Balance: ";
		cin >> Client.AccountBalance;
	}

	return Client;
}

string ConvertClientRecordToLine(stClient Client, string Delim = "#//#")
{
	string Line = "";

	Line += Client.AccountNumber + Delim;
	Line += Client.PinCode + Delim;
	Line += Client.Name + Delim;
	Line += Client.Phone + Delim;
	Line += to_string(Client.AccountBalance);

	return Line;
}

void AddDataToFile(string FileName, string String)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out | ios::app);

	if (MyFile.is_open())
	{
		MyFile << String << endl;

		MyFile.close();
	}
}

void AddNewClient()
{
	stClient Client;
	Client = ReadNewClientData();
	AddDataToFile(ClientsFileName, ConvertClientRecordToLine(Client));
}

void AddClients()
{
	char AddMore;

	do
	{
		system("cls");
		AddingClientsMenu();
		AddNewClient();

		cout << "\n  Client Added Successfully..";
		cout << "\n\n Do You Want Add More Clients Y/N: ";
		cin >> AddMore;

	} while (toupper(AddMore) == 'Y');
}

void PrintDeleteClientsMenu()
{
	cout << "\n";
	cout << "-----------------------------------------------\n";
	cout << "             Delete Client Screen            \n";
	cout << "-----------------------------------------------\n";
}

bool CheckIfClientExists(string AccountNumber, vector <stClient>& vClients, stClient& Client)
{
	for (stClient& CurrentClient : vClients)
	{
		if (CurrentClient.AccountNumber == AccountNumber)
		{
			Client = CurrentClient;
			return true;
		}
	}

	return false;
}

void PrintClientCard(stClient Client)
{
	cout << "\nThe Following Are The Client Details: \n";
	cout << "--------------------------------------- \n";
	cout << "Account Number : " << Client.AccountNumber << endl;
	cout << "Pin Code       : " << Client.PinCode << endl;
	cout << "Name           : " << Client.Name << endl;
	cout << "Phone          : " << Client.Phone << endl;
	cout << "Account Balance: " << Client.AccountBalance << endl;
	cout << "--------------------------------------- \n";
}

void DeleteOneClientFromFile(string FileName, string AccountNumber, vector <stClient>& vClients)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out);

	if (MyFile.is_open())
	{
		string Line;

		for (stClient& Client : vClients)
		{
			if (Client.AccountNumber != AccountNumber)
			{
				Line = ConvertClientRecordToLine(Client);
				MyFile << Line << endl;
			}
		}

		MyFile.close();
	}
}

void ClientsDeleteMenu()
{
	PrintDeleteClientsMenu();
	char Answer;
	stClient Client;
	vector <stClient> vClients;
	vClients = LoadDataFromFileToVector(ClientsFileName);
	string AccountNumber = ReadAccountNumber();

	if (CheckIfClientExists(AccountNumber, vClients, Client))
	{
		PrintClientCard(Client);

		cout << "\nAre You Sure Do You Want Delete This Client Y/N: ";
		cin >> Answer;

		if (toupper(Answer) == 'Y')
		{
			DeleteOneClientFromFile(ClientsFileName, AccountNumber, vClients);

			cout << "\n\n   Client Deleted Successfully...";
		}

	}
	else
	{
		cout << "\nClient With Account Number [" << AccountNumber << "] Is Not Found. \n\n";
	}
}

void PrintUpdateClientMenu()
{
	cout << "\n";
	cout << "-----------------------------------------------\n";
	cout << "             Update Client Screen            \n";
	cout << "-----------------------------------------------\n";
}

stClient ReadUpdatedClientData(string AccountNumber)
{
	stClient Client;

	Client.AccountNumber = AccountNumber;
	cout << "\n";
	cout << "Enter Pin Code: ";
	getline(cin >> ws, Client.PinCode);
	cout << "Enter Name    : ";
	getline(cin, Client.Name);
	cout << "Enter Phone   : ";
	getline(cin, Client.Phone);
	cout << "Enter Account Balance: ";
	cin >> Client.AccountBalance;
	while (cin.fail())
	{
		cin.clear();
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		cout << "\nInvalid Balance, Please Try Again: \n";
		cout << "Enter Account Balance: ";
		cin >> Client.AccountBalance;
	}

	return Client;
}

void SaveUpdateDataToFile(string FileName, vector <stClient>& vClients)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out);

	if (MyFile.is_open())
	{
		string Line;

		for (stClient& Client : vClients)
		{
			Line = ConvertClientRecordToLine(Client);
			MyFile << Line << endl;
		}

		MyFile.close();
	}
}

void UpdateClientsMenu()
{
	PrintUpdateClientMenu();
	char Answer;
	stClient Client;
	vector <stClient> vClients;
	vClients = LoadDataFromFileToVector(ClientsFileName);
	string AccountNumber = ReadAccountNumber();

	if (CheckIfClientExists(AccountNumber, vClients, Client))
	{
		PrintClientCard(Client);

		cout << "\nAre You Sure Do You Want Update This Client Y/N: ";
		cin >> Answer;

		if (toupper(Answer) == 'Y')
		{
			for (stClient& CurrentClient : vClients)
			{
				if (CurrentClient.AccountNumber == AccountNumber)
				{
					CurrentClient = ReadUpdatedClientData(CurrentClient.AccountNumber);
					break;
				}
			}

			SaveUpdateDataToFile(ClientsFileName, vClients);

			cout << "\n\n   Client Updated Successfully...\n";
		}

	}
	else
	{
		cout << "\nClient With Account Number [" << AccountNumber << "] Is Not Found. \n\n";
	}
}

void PrintFindClientMenu()
{
	cout << "\n";
	cout << "-----------------------------------------------\n";
	cout << "             Find Client Screen            \n";
	cout << "-----------------------------------------------\n";
}

void FindClientMenu()
{
	PrintFindClientMenu();

	stClient Client;
	vector <stClient> vClients;
	vClients = LoadDataFromFileToVector(ClientsFileName);
	string AccountNumber = ReadAccountNumber();

	if (CheckIfClientExists(AccountNumber, vClients, Client))
		PrintClientCard(Client);
	else
		cout << "\nClient With Account Number [" << AccountNumber << "] Is Not Found. \n\n";
}

void ProgramEndMenu()
{
	cout << "\n";
	cout << "-----------------------------------------------\n";
	cout << "          ***   Program Ends   ***             \n";
	cout << "-----------------------------------------------\n";
}

void GoToMainMenuScreen()
{
	cout << "\n\nPress Any Key To Go Back To Main Menu...";
	system("pause>0");
	MainMenuScreen();
}

short ReadNumber()
{
	short Number;
	do
	{
		cout << "Choose What Do You Want To Do [1 To 6]: ";
		cin >> Number;
		while (cin.fail())
		{
			cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

			cout << "\nInvalid Number, Please Try Again: \n";
			cout << "Choose What Do You Want To Do [1 To 6]: ";
			cin >> Number;
		}

	} while (Number < 1 || Number > 6);

	return Number;
}

void PerformOperationOrdered(enOperationType OperationType)
{
	switch (OperationType)
	{
	case enOperationType::eShowClient:
		system("cls");
		PrintAllClientsOnTheScreen();
		GoToMainMenuScreen();
		break;

	case enOperationType::eAddClient:
		system("cls");
		AddClients();
		GoToMainMenuScreen();
		break;

	case enOperationType::eUpdateClient:
		system("cls");
		UpdateClientsMenu();
		GoToMainMenuScreen();
		break;

	case enOperationType::eDeleteClient:
		system("cls");
		ClientsDeleteMenu();
		GoToMainMenuScreen();
		break;

	case enOperationType::eFindClient:
		system("cls");
		FindClientMenu();
		GoToMainMenuScreen();
		break;

	case enOperationType::eExit:
		system("cls");
		ProgramEndMenu();
		break;
	}
}

void MainMenuScreen()
{
	system("cls");
	cout << "\n";
	cout << "=======================================\n";
	cout << "           Main Menu Screen           \n";
	cout << "=======================================\n";
	cout << "\t[1] Show Client List. \n";
	cout << "\t[2] Add New Client. \n";
	cout << "\t[3] Update Client Info. \n";
	cout << "\t[4] Delete Client. \n";
	cout << "\t[5] Find Client. \n";
	cout << "\t[6] Exit. \n";
	cout << "=======================================\n";
	PerformOperationOrdered((enOperationType)ReadNumber());
}

int main()
{
	MainMenuScreen();
	return 0;
}