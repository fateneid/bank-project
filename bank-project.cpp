#include <iostream>
#include <fstream> 
#include <string>
#include <vector>
#include <cctype>
#include <iomanip>
using namespace std;

const string ClientsFileName = "ClientsFile.txt";

void Bank();
void TransactionsMenueScreen();

short ReadNumberInRange(short From, short To, string Message) {
	short Choose;
	do {
		cout << Message;
		cin >> Choose;
	} while (Choose < From || Choose > To);
	return Choose;
}

string ReadAccountNumber(string Message) {
	string AccountNumber;
	cout << Message;
	cin >> AccountNumber;
	return AccountNumber;
}

struct stClientData {

	string AccountNumber = "";
	string PinCode = "";
	string Name = "";
	string Phone = "";
	double AccountBalance = 0.0;
	bool MarkForDelete = false;

};

enum enMenue { BankMainMenueScreen = 1, TransactionsMainMenueScreen = 2 };

void PrintMainMenueScreen() {

	system("cls");
	cout << "=========================================================\n";
	cout << "\t\t\tMain Menue Screen\n";
	cout << "=========================================================\n";
	cout << "\t\t[1] Show Client List.\n";
	cout << "\t\t[2] Add New Client.\n";
	cout << "\t\t[3] Delete Client.\n";
	cout << "\t\t[4] Update Client Info.\n";
	cout << "\t\t[5] Find Client.\n";
	cout << "\t\t[6] Transactions.\n";
	cout << "\t\t[7] Exit.\n";
	cout << "=========================================================\n";

}

vector<string> SplitString(string MyString, string Split = "#//#") {

	vector<string> vStringWords;
	string sWord;
	short pos = 0;

	while ((pos = MyString.find(Split)) != std::string::npos) {

		sWord = MyString.substr(0, pos);
		if (sWord != "") {
			vStringWords.push_back(sWord);
		}

		MyString.erase(0, pos + Split.length());

	}

	if (MyString != "") {
		vStringWords.push_back(MyString);
	}

	return vStringWords;

}

stClientData ConvertDataLineToRecord(string DataLine, string Seperator = "#//#") {

	vector<string> vStringWords = SplitString(DataLine, Seperator);
	stClientData stClient;

	stClient.AccountNumber = vStringWords[0];
	stClient.PinCode = vStringWords[1];
	stClient.Name = vStringWords[2];
	stClient.Phone = vStringWords[3];
	stClient.AccountBalance = stod(vStringWords[4]);

	return stClient;

}

vector<stClientData> LoadClientsDataFromFile(string FileName) {

	vector<stClientData> vClients;
	stClientData stClient;

	fstream MyFile;

	MyFile.open(FileName, ios::in);

	if (MyFile.is_open()) {

		string Line;

		while (getline(MyFile, Line)) {

			stClient = ConvertDataLineToRecord(Line);
			vClients.push_back(stClient);

		}

		MyFile.close();
	}

	return vClients;

}

void ShowClient(stClientData stClient) {

	cout << "| " << setw(15) << left << stClient.AccountNumber;
	cout << "| " << setw(10) << left << stClient.PinCode;
	cout << "| " << setw(40) << left << stClient.Name;
	cout << "| " << setw(12) << left << stClient.Phone;
	cout << "| " << setw(12) << left << stClient.AccountBalance;

}

void PrintAllClientsScreen(vector<stClientData> vClients) {

	cout << "\n\t\t\t\tClient List (" << vClients.size() << ") Client (s).";
	cout << "\n__________________________________________________";
	cout << "___________________________________________________\n\n";

	cout << "| " << setw(15) << left << "Account Number";
	cout << "| " << setw(10) << left << "Pin Code";
	cout << "| " << setw(40) << left << "Client Name";
	cout << "| " << setw(12) << left << "Phone";
	cout << "| " << setw(12) << left << "Balance";

	cout << "\n__________________________________________________";
	cout << "___________________________________________________\n\n";

	for (stClientData stClient : vClients) {

		ShowClient(stClient);
		cout << endl;

	}
	cout << "\n__________________________________________________";
	cout << "___________________________________________________\n";


}

stClientData AddNewClient() {

	stClientData stClient;

	cout << "Enter Account Number? ";
	getline(cin >> ws, stClient.AccountNumber);
	cout << "Enter PinCode? ";
	getline(cin, stClient.PinCode);
	cout << "Enter Name? ";
	getline(cin, stClient.Name);
	cout << "Enter Phone? ";
	getline(cin, stClient.Phone);
	cout << "Enter AccountBalance? ";
	cin >> stClient.AccountBalance;

	return stClient;

}

string ConvertRecordToLine(stClientData stClient, string Seperator = "#//#") {

	string DataLine = "";

	DataLine += stClient.AccountNumber + Seperator;
	DataLine += stClient.PinCode + Seperator;
	DataLine += stClient.Name + Seperator;
	DataLine += stClient.Phone + Seperator;
	DataLine += to_string(stClient.AccountBalance);

	return DataLine;

}

void AddDataLineToFile(string FileName, string DataLine) {

	fstream MyFile;

	MyFile.open(FileName, ios::out | ios::app);

	if (MyFile.is_open()) {

		MyFile << DataLine << endl;

		MyFile.close();

	}

}

void PrintClientCard(stClientData stClient) {

	cout << "\nThe following are the client details: " << endl;

	cout << "-----------------------------------------------\n";
	cout << "Account Number: " << stClient.AccountNumber << endl;
	cout << "Pin Code      : " << stClient.PinCode << endl;
	cout << "Name          : " << stClient.Name << endl;
	cout << "Phone         : " << stClient.Phone << endl;
	cout << "Account Balance: " << stClient.AccountBalance << endl;

	cout << "-----------------------------------------------\n\n";

}

bool FindClientByAccountNumber(vector<stClientData> vClients, stClientData& Client, string AccountNumber) {

	for (stClientData stClient : vClients) {

		if (stClient.AccountNumber == AccountNumber) {
			Client = stClient;
			return true;
		}

	}

	return false;

}

void MarkForDelete(vector<stClientData>& vClients, string AccountNumber) {

	for (stClientData& stClient : vClients) {

		if (stClient.AccountNumber == AccountNumber) {
			stClient.MarkForDelete = true;
			break;
		}

	}

}

void SaveDataFromVectorToFile(vector<stClientData> vClients, string FileName) {

	fstream MyFile;

	MyFile.open(FileName, ios::out);

	if (MyFile.is_open()) {

		string DataLine;

		for (stClientData stClient : vClients) {

			if (stClient.MarkForDelete == false) {

				DataLine = ConvertRecordToLine(stClient);
				MyFile << DataLine << endl;

			}
		}

		MyFile.close();

	}


}

void DeleteClient(vector<stClientData>& vClients, string AccountNumber, string FileName) {

	MarkForDelete(vClients, AccountNumber);
	SaveDataFromVectorToFile(vClients, FileName);
	vClients = LoadClientsDataFromFile(ClientsFileName);

}

void DeleteClientScreen(vector<stClientData> vClients, string FileName) {

	cout << "\n----------------------------------------\n";
	cout << "\tDelete Client Screen";
	cout << "\n----------------------------------------\n";

	stClientData Client;

	string AccountNumber = ReadAccountNumber("\nPlease enter AccountNumber? ");

	char Answer;

	if (FindClientByAccountNumber(vClients, Client, AccountNumber)) {

		PrintClientCard(Client);

		cout << "\n\nAre you sure you want to delete this client? y/n ? ";
		cin >> Answer;

		if (toupper(Answer) == 'Y') {

			DeleteClient(vClients, AccountNumber, FileName);
			cout << "\nClient deleted Successfully.\n";

		}

	}
	else {

		cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!\n";

	}

}

stClientData UpdateClientByAccountNumber(string AccountNumber) {

	stClientData stClient;

	stClient.AccountNumber = AccountNumber;
	cout << "Enter PinCode? ";
	getline(cin >> ws, stClient.PinCode);
	cout << "Enter Name? ";
	getline(cin, stClient.Name);
	cout << "Enter Phone? ";
	getline(cin, stClient.Phone);
	cout << "Enter AccountBalance? ";
	cin >> stClient.AccountBalance;

	return stClient;

}

void UpdateClientInfo(vector<stClientData>& vClients, string AccountNumber, string FileName) {

	for (stClientData& stClient : vClients) {

		if (stClient.AccountNumber == AccountNumber) {

			cout << "\n\n";
			stClient = UpdateClientByAccountNumber(AccountNumber);
			break;

		}

	}

	SaveDataFromVectorToFile(vClients, FileName);

}

void UpdateClientInfoScreen(vector<stClientData> vClients, string FileName) {

	cout << "\n----------------------------------------\n";
	cout << "\tUpdate Client Info Screen";
	cout << "\n----------------------------------------\n";

	stClientData Client;
	char Answer;

	string AccountNumber = ReadAccountNumber("\nPlease enter AccountNumber? ");

	if (FindClientByAccountNumber(vClients, Client, AccountNumber)) {

		PrintClientCard(Client);

		cout << "\n\nAre you sure you want to update this client? y/n ? ";
		cin >> Answer;

		if (toupper(Answer) == 'Y') {

			UpdateClientInfo(vClients, AccountNumber, FileName);
			cout << "\nClient updated Successfully.\n";

		}

	}
	else {

		cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!\n";

	}

}

void FindClientScreen(vector<stClientData> vClients) {

	cout << "\n----------------------------------------\n";
	cout << "\tFind Client Screen";
	cout << "\n----------------------------------------\n";

	stClientData Client;

	string AccountNumber = ReadAccountNumber("\nPlease enter AccountNumber? ");

	if (FindClientByAccountNumber(vClients, Client, AccountNumber)) {

		PrintClientCard(Client);

	}
	else {

		cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!\n";

	}

}

void AddClient(string FileName, bool WithAccountNumber = false, string AccountNumber = "") {

	stClientData stClient;

	if (WithAccountNumber) {
		stClient = UpdateClientByAccountNumber(AccountNumber);
	}
	else {
		stClient = AddNewClient();
	}

	string DataLine = ConvertRecordToLine(stClient);
	AddDataLineToFile(FileName, DataLine);

}

void AddNewClientsScreen(vector<stClientData> vClients, string FileName) {

	char Answer;
	stClientData Client;
	
	cout << "\n----------------------------------------\n";
	cout << "\tAdd New Clients Screen";
	cout << "\n----------------------------------------\n";
	cout << "Adding New Client:\n\n";

	do {

		string AccountNumber = ReadAccountNumber("\nEnter Account Number? ");

		while (FindClientByAccountNumber(vClients, Client, AccountNumber)) {
			cout << "\nClient with [" << AccountNumber << "] already exists, Enter another Account Number? ";
			cin >> AccountNumber;
		}

		AddClient(FileName, true, AccountNumber);

		cout << "\nClient Added Successfully, do you wat to add more clients? Y/N? ";
		cin >> Answer;

	} while (toupper(Answer) == 'Y');


}

void ExitScreen() {

	cout << "\n----------------------------------------\n";
	cout << "\tProgram End :-)";
	cout << "\n----------------------------------------\n";

	system("pause>0");

}

void GoBackToMainMenue(enMenue Menue) {

	switch (Menue)
	{
	case enMenue::BankMainMenueScreen:
		cout << "\n\nPress any key to go back to Main Menue...";
		system("pause>0");
		Bank();
		break;
	case enMenue::TransactionsMainMenueScreen:
		cout << "\n\nPress any key to go back to Transactions Menue...";
		system("pause>0");
		TransactionsMenueScreen();
	    break;
	}
}

void PrintTransactionsMenue() {

	system("cls");
	cout << "=========================================================\n";
	cout << "\t\t\tTransactions Menue Screen\n";
	cout << "=========================================================\n";
	cout << "\t\t[1] Deposit.\n";
	cout << "\t\t[2] Withdraw.\n";
	cout << "\t\t[3] Total Balances.\n";
	cout << "\t\t[4] Main Menue.\n";
	cout << "=========================================================\n";

}

double ReadTransactionAmount(string Message) {
	double TransactionAmount = 0.0;
	cout << Message;
	cin >> TransactionAmount;
	return TransactionAmount;
}

void Transaction(vector<stClientData>& vClients, double TransactionAmount, string AccountNumber, string FileName, bool TransactionType = true) {

	if (!TransactionType) {
		TransactionAmount = TransactionAmount * -1;  // if TransactionType false: Withdraw, true: Deposit
	}

	for (stClientData& stClient : vClients) {

		if (stClient.AccountNumber == AccountNumber) {

			stClient.AccountBalance += TransactionAmount;
			break;

		}
	}

	SaveDataFromVectorToFile(vClients, FileName);

}

void DepositScreen(vector<stClientData> vClients, string FileName) {

	cout << "\n----------------------------------------\n";
	cout << "\tDeposit Screen";
	cout << "\n----------------------------------------\n";

	stClientData Client;
	char Answer;

	string AccountNumber = ReadAccountNumber("\nPlease enter AccountNumber? ");

	while (!FindClientByAccountNumber(vClients, Client, AccountNumber)) {

		cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!\n";
		AccountNumber = ReadAccountNumber("\nPlease enter AccountNumber? ");

	}

	PrintClientCard(Client);

	double DepositAmount = ReadTransactionAmount("\n\nPlease enter deposit amount? ");

	cout << "\n\nAre you sure you want to perform this transaction? y/n ? ";
	cin >> Answer;

	if (toupper(Answer) == 'Y') {

		Transaction(vClients, DepositAmount, AccountNumber, FileName);
		cout << "\nDone Successfully. New balance is: " << Client.AccountBalance + DepositAmount << endl;

	}


}

void WithdrawScreen(vector<stClientData> vClients, string FileName) {

	cout << "\n----------------------------------------\n";
	cout << "\tWithdraw Screen";
	cout << "\n----------------------------------------\n";

	stClientData Client;
	char Answer;
	double WithdrawAmount;

	string AccountNumber = ReadAccountNumber("\nPlease enter AccountNumber? ");

	while (!FindClientByAccountNumber(vClients, Client, AccountNumber)) {

		AccountNumber = ReadAccountNumber("\nPlease enter AccountNumber? ");
		cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!\n";

	}

	PrintClientCard(Client);

	do {

		WithdrawAmount = ReadTransactionAmount("\n\nPlease enter withdraw amount? ");

		if (WithdrawAmount > Client.AccountBalance) {

			cout << "\n\nAmount Exceeds the balance, you can withdraw up to: " << Client.AccountBalance << endl;

		}

	} while (WithdrawAmount > Client.AccountBalance);

	cout << "\n\nAre you sure you want to perform this transaction? y/n ? ";
	cin >> Answer;

	if (toupper(Answer) == 'Y') {

		Transaction(vClients, WithdrawAmount, AccountNumber, FileName, false);

		cout << "\nDone Successfully. New balance is: " << Client.AccountBalance - WithdrawAmount << endl;

	}

}

void TotalBalancesScreen(vector<stClientData> vClients) {

	cout << "\n\t\t\t\tBalances List (" << vClients.size() << ") Client (s).";
	cout << "\n__________________________________________________";
	cout << "___________________________________________________\n\n";

	cout << "| " << setw(20) << left << "Account Number";
	cout << "| " << setw(50) << left << "Client Name";
	cout << "| " << setw(20) << left << "Balance";

	cout << "\n__________________________________________________";
	cout << "___________________________________________________\n\n";

	long double TotalBalances = 0.0;

	for (stClientData stClient : vClients) {

		cout << "| " << setw(20) << left << stClient.AccountNumber;
		cout << "| " << setw(50) << left << stClient.Name;
		cout << "| " << setw(20) << left << stClient.AccountBalance;

		cout << endl;

		TotalBalances += stClient.AccountBalance;

	}
	cout << "\n__________________________________________________";
	cout << "___________________________________________________\n";

	cout << fixed << setprecision(2);
	cout << "\n\t\t\t\t\tTotal Balances = " << TotalBalances << endl;

}

void TransactionsMenueScreen() {

	vector<stClientData> vClients = LoadClientsDataFromFile(ClientsFileName);

	PrintTransactionsMenue();
	short Choose = ReadNumberInRange(1, 4, "Choose what do you want to do? [1 to 4]? ");

	switch (Choose)
	{
	case 1:
		system("cls");
		DepositScreen(vClients, ClientsFileName);
		GoBackToMainMenue(enMenue::TransactionsMainMenueScreen);
		break;
	case 2:
		system("cls");
		WithdrawScreen(vClients, ClientsFileName);
		GoBackToMainMenue(enMenue::TransactionsMainMenueScreen);
		break;
	case 3:
		system("cls");
		TotalBalancesScreen(vClients);
		GoBackToMainMenue(enMenue::TransactionsMainMenueScreen);
		break;
	case 4:
		system("cls");
		Bank();
		break;
	}

}

void Bank() {

	vector<stClientData> vClients = LoadClientsDataFromFile(ClientsFileName);

	PrintMainMenueScreen();
	short Choose = ReadNumberInRange(1, 7, "Choose what do you want to do? [1 to 7]? ");

	switch (Choose)
	{
	case 1:
		system("cls");
		PrintAllClientsScreen(vClients);
		GoBackToMainMenue(enMenue::BankMainMenueScreen);
		break;
	case 2:
		system("cls");
		AddNewClientsScreen(vClients, ClientsFileName);
		GoBackToMainMenue(enMenue::BankMainMenueScreen);
		break;
	case 3:
		system("cls");
		DeleteClientScreen(vClients, ClientsFileName);
		GoBackToMainMenue(enMenue::BankMainMenueScreen);
		break;
	case 4:
		system("cls");
		UpdateClientInfoScreen(vClients, ClientsFileName);
		GoBackToMainMenue(enMenue::BankMainMenueScreen);
		break;
	case 5:
		system("cls");
		FindClientScreen(vClients);
		GoBackToMainMenue(enMenue::BankMainMenueScreen);
		break;
	case 6:
		system("cls");
		TransactionsMenueScreen();
		break;
	case 7:
		system("cls");
		ExitScreen();
		break;
	}

}

int main() {

	Bank();


}


