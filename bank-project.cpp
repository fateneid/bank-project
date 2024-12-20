#include <iostream>
#include <fstream> 
#include <string>
#include <vector>
#include <cctype>
#include <iomanip>
using namespace std;

const string ClientsFileName = "ClientsFile.txt";
const string UsersFileName = "Users.txt";

struct stUserData {
	string Username = "";
	string Password = "";
	short Permission = 0;
	bool MarkForDelete = false;
};

stUserData CurrentUser;

void Bank();
void TransactionsMenueScreen();
void Login();
void ManageUsersMenueScreen();
void AccessDeniedScreen();

short ReadNumberInRange(short From, short To, string Message) {
	short Choose;
	do {
		cout << Message;
		cin >> Choose;
	} while (Choose < From || Choose > To);
	return Choose;
}

string ReadString(string Message) {
	string String;
	cout << Message;
	cin >> String;
	return String;
}

bool AskYesNo(string Message) {
	char Answer;
	cout << Message;
	cin >> Answer;
	return toupper(Answer) == 'Y' ? true : false;
}

struct stClientData {

	string AccountNumber = "";
	string PinCode = "";
	string Name = "";
	string Phone = "";
	double AccountBalance = 0.0;
	bool MarkForDelete = false;

};

enum enMenue { BankMainMenueScreen = 1, TransactionsMainMenueScreen = 2, ManageUsersMainMenueScreen = 3 };

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
	cout << "\t\t[7] Manage Users.\n";
	cout << "\t\t[8] Logout.\n";
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
	vClients = LoadClientsDataFromFile(FileName);

}

void DeleteClientScreen(vector<stClientData> vClients, string FileName) {

	cout << "\n----------------------------------------\n";
	cout << "\tDelete Client Screen";
	cout << "\n----------------------------------------\n";

	stClientData Client;

	string AccountNumber = ReadString("\nPlease enter AccountNumber? ");

	if (FindClientByAccountNumber(vClients, Client, AccountNumber)) {

		PrintClientCard(Client);

		if (AskYesNo("\n\nAre you sure you want to delete this client? y/n ? ")) {

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

	string AccountNumber = ReadString("\nPlease enter AccountNumber? ");

	if (FindClientByAccountNumber(vClients, Client, AccountNumber)) {

		PrintClientCard(Client);


		if (AskYesNo("\n\nAre you sure you want to update this client? y/n ? ")) {

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

	string AccountNumber = ReadString("\nPlease enter AccountNumber? ");

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

	stClientData Client;
	
	cout << "\n----------------------------------------\n";
	cout << "\tAdd New Clients Screen";
	cout << "\n----------------------------------------\n";
	cout << "Adding New Client:\n\n";

	do {

		string AccountNumber = ReadString("\nEnter Account Number? ");

		while (FindClientByAccountNumber(vClients, Client, AccountNumber)) {
			cout << "\nClient with [" << AccountNumber << "] already exists, Enter another Account Number? ";
			cin >> AccountNumber;
		}

		AddClient(FileName, true, AccountNumber);

	} while (AskYesNo("\nClient Added Successfully, do you want to add more clients? Y/N? "));


}

void GoBackToMainMenue(enMenue Menue) {

	cout << "\n\nPress any key to go back to Transactions Menue...";
	system("pause>0");

	switch (Menue)
	{
	case enMenue::BankMainMenueScreen:
		Bank();
		break;
	case enMenue::TransactionsMainMenueScreen:
		TransactionsMenueScreen();
	    break;
	case enMenue::ManageUsersMainMenueScreen:
		ManageUsersMenueScreen();
		break;
	}
}

void Bank() {

	vector<stClientData> vClients = LoadClientsDataFromFile(ClientsFileName);

	PrintMainMenueScreen();
	short Choose = ReadNumberInRange(1, 8, "Choose what do you want to do? [1 to 8]? ");

	switch (Choose)
	{
	case 1:
		system("cls");
		if (CurrentUser.Permission & (1 << 0)) {
			PrintAllClientsScreen(vClients);
		}
		else {
			AccessDeniedScreen();
		}
		GoBackToMainMenue(enMenue::BankMainMenueScreen);
		break;
	case 2:
		system("cls");
		if (CurrentUser.Permission & (1 << 1)) {
			AddNewClientsScreen(vClients, ClientsFileName);
		}
		else {
			AccessDeniedScreen();
		}
		GoBackToMainMenue(enMenue::BankMainMenueScreen);
		break;
	case 3:
		system("cls");
		if (CurrentUser.Permission & (1 << 2)) {
			DeleteClientScreen(vClients, ClientsFileName);
		}
		else {
			AccessDeniedScreen();
		}
		GoBackToMainMenue(enMenue::BankMainMenueScreen);
		break;
	case 4:
		system("cls");
		if (CurrentUser.Permission & (1 << 3)) {
			UpdateClientInfoScreen(vClients, ClientsFileName);
		}
		else {
			AccessDeniedScreen();
		}
		GoBackToMainMenue(enMenue::BankMainMenueScreen);
		break;
	case 5:
		system("cls");
		if (CurrentUser.Permission & (1 << 4)) {
			FindClientScreen(vClients);
		}
		else {
			AccessDeniedScreen();
		}
		GoBackToMainMenue(enMenue::BankMainMenueScreen);
		break;
	case 6:
		system("cls");
		if (CurrentUser.Permission & (1 << 5)) {
			TransactionsMenueScreen();
		}
		else {
			AccessDeniedScreen();
			GoBackToMainMenue(enMenue::BankMainMenueScreen);
		}
		break;
	case 7:
		system("cls");
		if (CurrentUser.Permission & (1 << 6)) {
			ManageUsersMenueScreen();
		}
		else {
			AccessDeniedScreen();
			GoBackToMainMenue(enMenue::BankMainMenueScreen);
		}
		break;
	case 8:
		system("cls");
		Login();
		break;
	}

}

/*    :::: Extension 1 ::::    */

// Transactions

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

void Transaction(vector<stClientData>& vClients, double TransactionAmount, string AccountNumber, 
	string FileName, bool TransactionType = true) {

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

	string AccountNumber = ReadString("\nPlease enter AccountNumber? ");

	while (!FindClientByAccountNumber(vClients, Client, AccountNumber)) {

		cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!\n";
		AccountNumber = ReadString("\nPlease enter AccountNumber? ");

	}

	PrintClientCard(Client);

	double DepositAmount = ReadTransactionAmount("\n\nPlease enter deposit amount? ");

	if (AskYesNo("\n\nAre you sure you want to perform this transaction? y/n ? ")) {

		Transaction(vClients, DepositAmount, AccountNumber, FileName);
		cout << "\nDone Successfully. New balance is: " << Client.AccountBalance + DepositAmount << endl;

	}


}

void WithdrawScreen(vector<stClientData> vClients, string FileName) {

	cout << "\n----------------------------------------\n";
	cout << "\tWithdraw Screen";
	cout << "\n----------------------------------------\n";

	stClientData Client;
	double WithdrawAmount;

	string AccountNumber = ReadString("\nPlease enter AccountNumber? ");

	while (!FindClientByAccountNumber(vClients, Client, AccountNumber)) {

		AccountNumber = ReadString("\nPlease enter AccountNumber? ");
		cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!\n";

	}

	PrintClientCard(Client);

	do {

		WithdrawAmount = ReadTransactionAmount("\n\nPlease enter withdraw amount? ");

		if (WithdrawAmount > Client.AccountBalance) {

			cout << "\n\nAmount Exceeds the balance, you can withdraw up to: " << Client.AccountBalance << endl;

		}

	} while (WithdrawAmount > Client.AccountBalance);

	if (AskYesNo("\n\nAre you sure you want to perform this transaction? y/n ? ")) {

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

/*    :::: Extension 2 ::::    */

// Manage Users

void PrintManageUsersMenue() {

	system("cls");
	cout << "=========================================================\n";
	cout << "\t\t\tManage Users Menue Screen\n";
	cout << "=========================================================\n";
	cout << "\t\t[1] List Users.\n";
	cout << "\t\t[2] Add New User.\n";
	cout << "\t\t[3] Delete User.\n";
	cout << "\t\t[4] Update User.\n";
	cout << "\t\t[5] Find User.\n";
	cout << "\t\t[6] Main Menue.\n";
	cout << "=========================================================\n";

}

stUserData UserDataLineToRecord(string DataLine, string Seperator = "#//#") {

	vector<string> vStringWords = SplitString(DataLine, Seperator);
	stUserData stUser;

	stUser.Username = vStringWords[0];
	stUser.Password = vStringWords[1];
	stUser.Permission = stoi(vStringWords[2]);

	return stUser;

}

vector<stUserData> LoadUsersDataFromFile(string FileName) {

	vector<stUserData> vUsers;
	stUserData stUser;

	fstream MyFile;

	MyFile.open(FileName, ios::in);

	if (MyFile.is_open()) {

		string Line;

		while (getline(MyFile, Line)) {

			stUser = UserDataLineToRecord(Line);
			vUsers.push_back(stUser);

		}

		MyFile.close();
	}

	return vUsers;

}

bool FindUserByUsername(vector<stUserData> vUsers, stUserData& User, string Username) {

	for (stUserData stUser : vUsers) {

		if (stUser.Username == Username) {
			User = stUser;
			return true;
		}

	}

	return false;

}

void ListUsersScreen(vector<stUserData> vUsers) {

	cout << "\n\t\t\t\tUsers List (" << vUsers.size() << ") User(s).";
	cout << "\n__________________________________________________";
	cout << "___________________________________________________\n\n";

	cout << "| " << setw(15) << left << "User Name";
	cout << "| " << setw(10) << left << "Password";
	cout << "| " << setw(40) << left << "Permissions";

	cout << "\n__________________________________________________";
	cout << "___________________________________________________\n\n";

	for (stUserData stUser : vUsers) {

		cout << "| " << setw(15) << left << stUser.Username;
		cout << "| " << setw(10) << left << stUser.Password;
		cout << "| " << setw(40) << left << stUser.Permission;

		cout << endl;

	}
	cout << "\n__________________________________________________";
	cout << "___________________________________________________\n";


}

void PrintUserCard(stUserData stUser) {

	cout << "\nThe following are the user details: " << endl;

	cout << "-----------------------------------------------\n";
	cout << "Username    : " << stUser.Username << endl;
	cout << "Password    : " << stUser.Password << endl;
	cout << "Permissions : " << stUser.Permission << endl;

	cout << "-----------------------------------------------\n\n";

}

void FindUserScreen(vector<stUserData> vUsers) {

	cout << "\n----------------------------------------\n";
	cout << "\tFind User Screen";
	cout << "\n----------------------------------------\n";

	stUserData User;

	string Username = ReadString("\nPlease enter Username? ");

	if (FindUserByUsername(vUsers, User, Username)) {

		PrintUserCard(User);

	}
	else {

		cout << "\nUser with Username (" << Username << ") is Not Found!\n";

	}

}

stUserData AddNewUser() {

	stUserData stUser;

	cout << "Enter Username? ";
	getline(cin >> ws, stUser.Username);
	cout << "Enter Password? ";
	getline(cin, stUser.Password);

	if (AskYesNo("\nDo you want to give full access? y/n? ")) {
		stUser.Permission = -1;
	}
	else {
		cout << "\nDo you want to give access to :\n";

		string arrPermissions[7] = { "Show Client List", "Add New Client", "Delete Client", 
			"Update Client", "Find Client", "Transactions", "Manage Users" };

		for (short i = 0; i < 7; i++) {
			if (AskYesNo('\n' + arrPermissions[i] + "? y/n? ")) {
				stUser.Permission |= (1 << i);
			}

		}
	}

	return stUser;

}

stUserData UpdateUserByUsername(string Username) {

	stUserData stUser;

	stUser.Username = Username;
	cout << "Enter Password? ";
	getline(cin >> ws, stUser.Password); 

	if (AskYesNo("\nDo you want to give full access? y/n? ")) {
		stUser.Permission = -1;
	}
	else {
		cout << "\nDo you want to give access to :\n";

		string arrPermissions[7] = { "Show Client List", "Add New Client", "Delete Client",
			"Update Client", "Find Client", "Transactions", "Manage Users" };

		for (short i = 0; i < 7; i++) {
			if (AskYesNo('\n' + arrPermissions[i] + "? y/n? ")) {
				stUser.Permission |= (1 << i);
			}

		}
	}

	return stUser;

}

string ConvertRecordToLine(stUserData stUser, string Seperator = "#//#") {

	string DataLine = "";

	DataLine += stUser.Username + Seperator;
	DataLine += stUser.Password + Seperator;
	DataLine += to_string(stUser.Permission);

	return DataLine;

}

void AddUser(string FileName, bool WithUsername = false, string Username = "") {

	stUserData stUser;

	if (WithUsername) {
		stUser = UpdateUserByUsername(Username);
	}
	else {
		stUser = AddNewUser();
	}

	string DataLine = ConvertRecordToLine(stUser);
	AddDataLineToFile(FileName, DataLine);

}

void AddNewUserScreen(vector<stUserData> vUsers, string FileName) {

	stUserData User;

	cout << "\n----------------------------------------\n";
	cout << "\tAdd New User Screen";
	cout << "\n----------------------------------------\n";
	cout << "Adding New User:\n";

	do {

		string Username = ReadString("\nEnter Username? ");

		while (FindUserByUsername(vUsers, User, Username)) {
			cout << "\nUser with [" << Username << "] already exists, Enter another Username? ";
			cin >> Username;
		}

		AddUser(FileName, true, Username);

	} while (AskYesNo("\nUser Added Successfully, do you want to add more users? Y/N? "));


}

void MarkForDelete(vector<stUserData>& vUsers, string Username) {

	for (stUserData& stUser : vUsers) {

		if (stUser.Username == Username) {
			stUser.MarkForDelete = true;
			break;
		}

	}

}

void SaveDataFromVectorToFile(vector<stUserData> vUsers, string FileName) {

	fstream MyFile;

	MyFile.open(FileName, ios::out);

	if (MyFile.is_open()) {

		string DataLine;

		for (stUserData stUser : vUsers) {

			if (stUser.MarkForDelete == false) {

				DataLine = ConvertRecordToLine(stUser);
				MyFile << DataLine << endl;

			}
		}

		MyFile.close();

	}


}

void DeleteUser(vector<stUserData>& vUsers, string Username, string FileName) {

	MarkForDelete(vUsers, Username);
	SaveDataFromVectorToFile(vUsers, FileName);
	vUsers = LoadUsersDataFromFile(FileName);

}

void DeleteUserScreen(vector<stUserData>& vUsers, string FileName) {

	cout << "\n----------------------------------------\n";
	cout << "\tDelete Users Screen";
	cout << "\n----------------------------------------\n";

	stUserData User;

	string Username = ReadString("\nPlease enter Username? ");

	if (FindUserByUsername(vUsers, User, Username)) {

		PrintUserCard(User);

		if (AskYesNo("\n\nAre you sure you want to delete this user? y/n ? ")) {

			DeleteUser(vUsers, Username, FileName);
			cout << "\nUser deleted Successfully.\n";

		}

	}
	else {

		cout << "\nUser with Username (" << Username << ") is Not Found!\n";

	}

}

void UpdateUser(vector<stUserData>& vUsers, string Username, string FileName) {

	for (stUserData& stUser : vUsers) {

		if (stUser.Username == Username) {

			cout << "\n\n";
			stUser = UpdateUserByUsername(Username);
			break;

		}

	}

	SaveDataFromVectorToFile(vUsers, FileName);

}

void UpdateUserScreen(vector<stUserData>& vUsers, string FileName) {

	cout << "\n----------------------------------------\n";
	cout << "\tUpdate Users Screen";
	cout << "\n----------------------------------------\n";

	stUserData User;

	string Username = ReadString("\nPlease enter Username? ");

	if (FindUserByUsername(vUsers, User, Username)) {

		PrintUserCard(User);

		if (AskYesNo("\n\nAre you sure you want to update this user? y/n ? ")) {

			UpdateUser(vUsers, Username, FileName);
			cout << "\nUser updated Successfully.\n";

		}

	}
	else {

		cout << "\nUser with Username (" << Username << ") is Not Found!\n";

	}

}

void ManageUsersMenueScreen() {

	vector<stUserData> vUsers = LoadUsersDataFromFile(UsersFileName);

	PrintManageUsersMenue();
	short Choose = ReadNumberInRange(1, 6, "Choose what do you want to do? [1 to 6]? ");

	switch (Choose)
	{
	case 1:
		system("cls");
		ListUsersScreen(vUsers);
		GoBackToMainMenue(enMenue::ManageUsersMainMenueScreen);
		break;
	case 2:
		system("cls");
		AddNewUserScreen(vUsers, UsersFileName);
		GoBackToMainMenue(enMenue::ManageUsersMainMenueScreen);
		break;
	case 3:
		system("cls");
		DeleteUserScreen(vUsers, UsersFileName);
		GoBackToMainMenue(enMenue::ManageUsersMainMenueScreen);
		break;
	case 4:
		system("cls");
		UpdateUserScreen(vUsers, UsersFileName);
		GoBackToMainMenue(enMenue::ManageUsersMainMenueScreen);
		break;
	case 5:
		system("cls");
		FindUserScreen(vUsers);
		GoBackToMainMenue(enMenue::ManageUsersMainMenueScreen);
		break;
	case 6:
		system("cls");
		Bank();
		break;
	}

}

// Login 

void AccessDeniedScreen() {

	cout << "\n----------------------------------------\n";
	cout << "Access Denied,\n";
	cout << "You Dont Have Permission To Do This,\n";
	cout << "Please Contact Your Admin,";
	cout << "\n----------------------------------------\n";

}

void PrintLogin() {

	system("cls");
	cout << "\n----------------------------------------\n";
	cout << "\tLogin Screen";
	cout << "\n----------------------------------------\n";

}

void Login() {

	PrintLogin();
	vector<stUserData> vUsers = LoadUsersDataFromFile(UsersFileName);

	while (true) {
		string Username = ReadString("Enter Username? ");
		string Password = ReadString("Enter Password? ");
		bool UsernameFound = FindUserByUsername(vUsers, CurrentUser, Username);

		if (UsernameFound && Password == CurrentUser.Password) {

			Bank();
			break;

		}
		else {

			PrintLogin();
			cout << "Invalid Username/Password!\n";

		}
	}
	
}

int main() {

	Login();

	

}


