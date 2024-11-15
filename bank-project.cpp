#include <iostream>
#include <fstream> 
#include <string>
#include <vector>
#include <cctype>
#include <iomanip>
using namespace std;

const string ClientsFileName = "ClientsFile.txt";

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
	cout << "\t\t[6] Exit.\n";
	cout << "=========================================================\n";

}

struct stClientData {

	string AccountNumber = "";
	string PinCode = "";
	string Name = "";
	string Phone = "";
	double AccountBalance = 0.0;
	bool MarkForDelete = false;

};

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

	cout << "\n\nPress any key to go back to Main Menue...";
	system("pause>0");

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

	cout << "\n\nPress any key to go back to Main Menue...";
	system("pause>0");
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

	cout << "\n\nPress any key to go back to Main Menue...";
	system("pause>0");
}

void	FindClientScreen(vector<stClientData> vClients) {

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

	cout << "\n\nPress any key to go back to Main Menue...";
	system("pause>0");
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

		string AccountNumber = ReadAccountNumber("\nEnter Account 0Number? ");

		while (FindClientByAccountNumber(vClients, Client, AccountNumber)) {
			cout << "\nClient with [" << AccountNumber << "] already exists, Enter another Account Number? ";
			cin >> AccountNumber;
		}

		AddClient(FileName, true, AccountNumber);

		cout << "\nClient Added Successfully, do you wat to add more clients? Y/N? ";
		cin >> Answer;

	} while (toupper(Answer) == 'Y');

	cout << "\n\nPress any key to go back to Main Menue...";
	system("pause>0");
}

void ExitScreen() {

	cout << "\n----------------------------------------\n";
	cout << "\tProgram End :-)";
	cout << "\n----------------------------------------\n";

	cout << "\n\nPress any key to go back to Main Menue...";
	system("pause>0");

}

void BankMainScreen() {

	vector<stClientData> vClients = LoadClientsDataFromFile(ClientsFileName);

	PrintMainMenueScreen();
	short Choose = ReadNumberInRange(1, 6, "Choose what do you want to do? [1 to 6]? ");

	switch (Choose)
	{
	case 1:
		system("cls");
		PrintAllClientsScreen(vClients);
		BankMainScreen();
		break;
	case 2:
		system("cls");
		AddNewClientsScreen(vClients, ClientsFileName);
		BankMainScreen();
		break;
	case 3:
		system("cls");
		DeleteClientScreen(vClients, ClientsFileName);
		BankMainScreen();
		break;
	case 4:
		system("cls");
		UpdateClientInfoScreen(vClients, ClientsFileName);
		BankMainScreen();
		break;
	case 5:
		system("cls");
		FindClientScreen(vClients);
		BankMainScreen();
		break;
	case 6:
		system("cls");
		ExitScreen();
		break;
	}

}


int main() {

	BankMainScreen();


}


