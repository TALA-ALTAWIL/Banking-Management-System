#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <cctype>

using namespace std;
const string ClientsFileName = "Clients.txt";
const string UsersFileName = "Users.txt";


// The Starting OF The Project


struct sClientData
{
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    double AccountBalance;
    bool FlagToDelete = false;
};
struct sUserData
{
    string UserName, PassWord;
    int Permissions;
    bool MarkForDelete = false;
};
enum eMangeUserMenuChoices {
    ShowingUserList = 1,
    AddingNewUser = 2,
    DeletingUser = 3,
    UpdatingUser = 4,
    FindingUser = 5,
    MainMenu = 6
};
enum eMaiMenueChoices
{
    ShowingList = 1,
    AddingNewClient = 2,
    DeletingClient = 3,
    UpdatingClient = 4,
    FindingClient = 5,
    Transactions = 6,
    ManageUsers = 7,
    Logout = 8
};
enum eTransactionsChoices {
    Deposit = 1,
    Withdraw = 2,
    TotalBalances = 3,
    MainMenue = 4,
};
enum eMainMenuePermissions {
    eAll = -1,
    pListClients = 1,
    pAddNewClient = 2,
    pDeleteClient = 4,
    pUpdateClient = 8,
    pFindClient = 16,
    pTransactions = 32,
    pManageUsers = 64
};
void PauseFunction();
void AccessDeniedScreen();;
void ShowMainMenueScreen();
void PauseManageUserFunction();
void ChoicesDirection();
void PauseTransactionsFunction();
void Login();
bool CheckAccessPermission(eMainMenuePermissions Permission);
void ShowTransactionsMenuSecreen();
void ShowMangeUsersMenuSecreen();
sUserData CurrentUser;
string ReadData(string s) {
    cout << "\n";
    string sentence;
    cout << "Enter Your " << s << "? ";
    getline(cin >> ws, sentence);
    return sentence;
}
double ReadDouData(string s) {
    cout << "\n";
    double sentence;
    cout << "Enter Your " << s << "? ";
    cin >> sentence;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return sentence;
}
string tabs(int number) {
    string str = "";
    for (int i = 0; i < number; i++)
    {
        str += "\t";
    }
    return str;
}

void HeaderOfScreens(string s) {
    system("cls");
    cout << "\n";
    cout << "*******************************************************************\n";
    cout << "\t" << s << "Screen" << endl;
    cout << "*******************************************************************\n";
}
vector<string> SplitString(string Sentence, string Delim) {
    int pos;
    vector<string> Vstring;
    string sWord;
    while ((pos = Sentence.find(Delim)) != std::string::npos) {
        sWord = Sentence.substr(0, pos);
        if (!sWord.empty())
        {
            Vstring.push_back(sWord);
        }

        Sentence.erase(0, pos + Delim.length());
    }
    if (Sentence != "") {
        Vstring.push_back(Sentence);
    }
    return Vstring;
}
string ConvertRecordToLine(sClientData& ClientData, string Delim) {
    string stClientRecord = "";
    stClientRecord += ClientData.AccountNumber + Delim;
    stClientRecord += ClientData.PinCode + Delim;
    stClientRecord += ClientData.Name + Delim;
    stClientRecord += ClientData.Phone + Delim;
    stClientRecord += to_string(ClientData.AccountBalance);
    return stClientRecord;
}
sClientData ConvertLinetoRecord(string stLine) {
    vector<string> vSplit = SplitString(stLine, "#//#");
    sClientData ClientData;
    ClientData.AccountNumber = vSplit[0];
    ClientData.PinCode = vSplit[1];
    ClientData.Name = vSplit[2];
    ClientData.Phone = vSplit[3];
    ClientData.AccountBalance = stod(vSplit[4]);
    return ClientData;
}
vector<sClientData> LoadCleintsDataFromFile(string FileName) { // vector store struct of client data
    vector<sClientData> vClientData;
    sClientData ClientData;
    fstream MyFile;
    MyFile.open(FileName, ios::in);
    if (MyFile.is_open()) {
        string line;
        while (getline(MyFile, line)) {
            ClientData = ConvertLinetoRecord(line);
            vClientData.push_back(ClientData);
        }
        MyFile.close();
    }

    return vClientData;
}
string ConvertUserRecordToLine(sUserData& UserData, string Delim){
    string stUserRecord = "";
    stUserRecord += UserData.UserName + Delim;
    stUserRecord += UserData.PassWord + Delim;
    stUserRecord += to_string(UserData.Permissions)  + Delim;
    return stUserRecord;
}
sUserData ConvertUserLinetoRecord(string stLine) {
    vector<string> vSplit = SplitString(stLine, "#//#");
    sUserData UserData;
    UserData.UserName = vSplit[0];
    UserData.PassWord = vSplit[1];
    UserData.Permissions = stoi(vSplit[2]);
    return UserData;
}
vector<sUserData> LoadUsersDataFromFile(string FileName) { // vector store struct of client data
    vector<sUserData> vUserData;
    sUserData UserData;
    fstream MyFile;
    MyFile.open(FileName, ios::in);
    if (MyFile.is_open()) {
        string line;
        while (getline(MyFile, line)) {
            UserData = ConvertUserLinetoRecord(line);
            vUserData.push_back(UserData);
        }
        MyFile.close();
    }

    return vUserData;
}

void PrintClientAsRow(sClientData& Client) {
    cout << "| " << setw(15) << left << Client.AccountNumber;
    cout << "| " << setw(10) << left << Client.PinCode;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(12) << left << Client.Phone;
    cout << "| " << setw(12) << left << Client.AccountBalance;
}
void PrintUserAsRow(sUserData& UserData) {
    cout << "| " << setw(15) << left << UserData.UserName;
    cout << "| " << setw(10) << left << UserData.PassWord;
    cout << "| " << setw(40) << left << UserData.Permissions;

}

void PrintClientAsRowTotalBalancesTable(sClientData& Client) {
    cout << "| " << setw(15) << left << Client.AccountNumber;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(12) << left << Client.AccountBalance;
}

void PrintAllClientsData() {

    if (!CheckAccessPermission(eMainMenuePermissions::pListClients))
    {
        AccessDeniedScreen();
        return;
    }
    vector<sClientData> vClientData = LoadCleintsDataFromFile(ClientsFileName);
    system("cls");
    cout << tabs(20) << "Client List (" << vClientData.size() << ") Client(s).\n" << endl;
    cout << "--------------------------------------------------------------------------------------------------\n" << endl;
    cout << "| " << left << setw(15) << "Accout Number";
    cout << "| " << left << setw(10) << "Pin Code";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Phone";
    cout << "| " << left << setw(12) << "Balance";
    cout << "\n\n--------------------------------------------------------------------------------------------------" << endl;
    for (sClientData Client : vClientData) {
        PrintClientAsRow(Client);
        cout << endl;
    }
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
    PauseFunction();
}
void PrintAllUsersData() {
    vector<sUserData> vUserData = LoadUsersDataFromFile(UsersFileName);
    system("cls");
    cout << tabs(20) << "Users List (" << vUserData.size() << ") User(s).\n" << endl;
    cout << "--------------------------------------------------------------------------------------------------\n" << endl;
    cout << "| " << left << setw(15) << "User Name";
    cout << "| " << left << setw(10) << "Password";
    cout << "| " << left << setw(40) << "Permissions";
    cout << "\n\n--------------------------------------------------------------------------------------------------" << endl;
    for (sUserData User : vUserData) {
        PrintUserAsRow(User);
        cout << endl;
    }
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
    PauseManageUserFunction();
}

void AddClientToFile(string FileName, sClientData& ClientData) {
    fstream MyFile;
    MyFile.open(FileName, ios::out | ios::app);
    if (MyFile.is_open())
    {
        MyFile << ConvertRecordToLine(ClientData, "#//#") << endl;
    }
    MyFile.close();
}
void AddUserToFile(string FileName, sUserData& UserData) {
    fstream MyFile;
    MyFile.open(FileName, ios::out | ios::app);
    if (MyFile.is_open())
    {
        MyFile << ConvertUserRecordToLine(UserData, "#//#") << endl;
    }
    MyFile.close();
}

bool AccountNumberExists(string accountNumber,
    const vector<sClientData>& vClientData,
    sClientData* ClientData = nullptr)
{
    for (const sClientData& Client : vClientData) {
        if (accountNumber == Client.AccountNumber) {
            if (ClientData != nullptr) {
                *ClientData = Client;
            }
            return true;
        }
    }
    return false;
}
bool UserNameExists(string UserName,
    const vector<sUserData>& vUserData,
    sUserData* UserData = nullptr)
{
    for (const sUserData& User : vUserData) {
        if (UserName == User.UserName) {
            if (UserData != nullptr) {
                *UserData = User;
            }
            return true;
        }
    }
    return false;
}


sClientData ReadNewClientData(string accountNumber = "") { // using default parameter
    sClientData ClientData;
    if (accountNumber == "") {
        ClientData.AccountNumber = ReadData("Account Number");
    }
    else {
        ClientData.AccountNumber = accountNumber;
    }
    ClientData.PinCode = ReadData("Pin Code");
    ClientData.Name = ReadData("Name");
    ClientData.Phone = ReadData("Phone");
    ClientData.AccountBalance = ReadDouData("AccountBalance");
    return ClientData;
}

void AccessDeniedScreen() {
    system("cls");
    cout << "\n********************************************************************\n";
    cout << "Acess Denied,\n You dont have Permission to do this,\nPlaese contact the Admin.";
    cout << "\n********************************************************************\n";
    PauseFunction();
}
int ReadUserPermissions(sUserData& UserData) {
    int PerValue = 0;
    char answer = 'n';
    cout << "\nDo You Want to give Full access? y/n?";
    cin >> answer;
    if (answer == 'y' || answer == 'Y')
        return eMainMenuePermissions::eAll;
    cout << "\nDo You want to give access to:\n";
    cout << "Show Client List? y/n?";
    cin >> answer;
    if (answer == 'y' || answer == 'Y')
        PerValue+= eMainMenuePermissions::pListClients;

    cout << "\nAdd New Client? y/n?";
    cin >> answer;
    if (answer == 'y' || answer == 'Y')
        PerValue += eMainMenuePermissions::pAddNewClient;    
    cout << "\nDelete Client? y/n?";
    cin >> answer;
    if (answer == 'y' || answer == 'Y')
        PerValue += eMainMenuePermissions::pDeleteClient;    
    cout << "\nUpdate Client? y/n?";
    cin >> answer;
    if (answer == 'y' || answer == 'Y')
        PerValue += eMainMenuePermissions::pUpdateClient;    
    cout << "\nFind Client? y/n?";
    cin >> answer;
    if (answer == 'y' || answer == 'Y')
        PerValue += eMainMenuePermissions::pFindClient;    
    cout << "\nTransactions Client? y/n?";
    cin >> answer;
    if (answer == 'y' || answer == 'Y')
        PerValue += eMainMenuePermissions::pTransactions;    
    cout << "\nManage Users? y/n?";
    cin >> answer;
    if (answer == 'y' || answer == 'Y')
        PerValue += eMainMenuePermissions::pManageUsers;
    return PerValue;
}

bool CheckAccessPermission(eMainMenuePermissions Permission)
{
    if (CurrentUser.Permissions == eMainMenuePermissions::eAll)
        return true;

    return ((CurrentUser.Permissions & Permission) == Permission);

}
sUserData ReadNewUserData(string UserName = "") { // using default parameter
    sUserData UserData;
    if (UserName == "") {
        UserData.UserName = ReadData("User Name: ");
    }
    else {
        UserData.UserName = UserName;
    }
    UserData.PassWord = ReadData("Password: ");
    UserData.Permissions = ReadUserPermissions(UserData);

    return UserData;
}

void AddingNewClientFun() {
    if (!CheckAccessPermission(eMainMenuePermissions::pAddNewClient))
    {
        AccessDeniedScreen();
        return;
    }
    HeaderOfScreens("Add New Clients ");
    cout << "Adding New Client:\n";
    string accountNumber;
    char AddMore;
    vector<sClientData> vClientData = LoadCleintsDataFromFile(ClientsFileName);
    do {
        cout << "Enter Account Number? ";
        getline(cin >> ws, accountNumber);
        while (AccountNumberExists(accountNumber, vClientData))
        {
            cout << "Client with [" << accountNumber << "] already exists, Enter Another Account Number? ";
            getline(cin >> ws, accountNumber);
        }
        sClientData ClientData = ReadNewClientData(accountNumber);
        vClientData.push_back(ClientData);
        AddClientToFile(ClientsFileName, ClientData);
        cout << "Client AddedSuccessfully, do you want to add more clients? Y/N? ";
        cin >> AddMore;
    } while (toupper(AddMore) == 'Y');
    PauseFunction();
}
void AddingNewUserFun() {
    HeaderOfScreens("Add New Uers ");
    cout << "Adding New USer:\n";
    string userName;
    char AddMore;
    vector<sUserData> vUserData = LoadUsersDataFromFile(UsersFileName);
    do {
        cout << "Enter User Name? ";
        getline(cin >> ws, userName);
        while (UserNameExists(userName, vUserData))
        {
            cout << "User with [" << userName << "] already exists, Enter Another User Name? ";
            getline(cin >> ws, userName);
        }
        sUserData UserData = ReadNewUserData(userName);
        vUserData.push_back(UserData);
        AddUserToFile(UsersFileName, UserData);
        cout << "User Added Successfully, do you want to add more Users? Y/N? ";
        cin >> AddMore;
    } while (toupper(AddMore) == 'Y');
    PauseManageUserFunction();
}
void MakeFlagToDelete(string AccountNumber, vector<sClientData>& vClientData) {
    for (sClientData& Client : vClientData)
    {
        if (Client.AccountNumber == AccountNumber) {
            Client.FlagToDelete = true;
        }
    }
}
void MakeFlagToDeleteUser(string UserName, vector<sUserData>& vUserData) {
    for (sUserData& User : vUserData)
    {
        if (User.UserName == UserName) {
            User.MarkForDelete = true;
        }
    }
}
void SaveClientToFile(string FileName, vector<sClientData>& vClientData) {
    fstream MyFile;
    string line;
    MyFile.open(FileName, ios::out);
    if (MyFile.is_open()) {
        for (sClientData& Client : vClientData)
        {
            if (!Client.FlagToDelete)
            {
                string reAddClient = ConvertRecordToLine(Client, "#//#");
                MyFile << reAddClient << endl;
            }

        }
        MyFile.close();
        vClientData = LoadCleintsDataFromFile(FileName);
    }
}
void SaveUserToFile(string FileName, vector<sUserData>& vUserData) {
    fstream MyFile;
    string line;
    MyFile.open(FileName, ios::out);
    if (MyFile.is_open()) {
        for (sUserData& User : vUserData)
        {
            if (!User.MarkForDelete)
            {
                string reAddUser = ConvertUserRecordToLine(User, "#//#");
                MyFile << reAddUser << endl;
            }

        }
        MyFile.close();
        vUserData = LoadUsersDataFromFile(FileName);
    }
}
void DeleteClientByAccountNumber(string AccountNumber, vector<sClientData>& vClientData) {
    char ToDelete = 'y';
    cout << "\n\n" << "Are You Sure you want to delete this Client? y/n ? ";
    cin >> ToDelete;
    if (toupper(ToDelete) == 'Y')
    {
        MakeFlagToDelete(AccountNumber, vClientData);
        SaveClientToFile(ClientsFileName, vClientData);
        cout << "\nClient Deleted Successfully";
    }

}

void DeleteUserByUserName(string UserName, vector<sUserData>& vUserData) {
    char ToDelete = 'y';
    cout << "\n\n" << "Are You Sure you want to delete this User ? y/n ? ";
    cin >> ToDelete;
    if (toupper(ToDelete) == 'Y')
    {
        MakeFlagToDeleteUser(UserName, vUserData);
        SaveUserToFile(UsersFileName, vUserData);
        cout << "\nUser Deleted Successfully";
    }

}
void PrintClientDetails(sClientData& ClientData) {
    cout << "\n";
    cout << "\n\nThe following are the client details\n";
    cout << "-------------------------------------------------\n";
    cout << "\nAccout Number: " << ClientData.AccountNumber;
    cout << "\nPin Code     : " << ClientData.PinCode;
    cout << "\nName         : " << ClientData.Name;
    cout << "\nPhone        : " << ClientData.Phone;
    cout << "\nAccount Balance: " << ClientData.AccountBalance;
    cout << "\n-------------------------------------------------\n";
}
void PrintUserDetails(sUserData& UserData) {
    cout << "\n";
    cout << "\n\nThe following are the User details\n";
    cout << "-------------------------------------------------\n";
    cout << "\nUser Name: " << UserData.UserName;
    cout << "\nPassword     : " << UserData.PassWord;
    cout << "\nPermissions         : " << UserData.Permissions;
    cout << "\n-------------------------------------------------\n";
}

void DeletingClientFun() {
    if (!CheckAccessPermission(eMainMenuePermissions::pDeleteClient))
    {
        AccessDeniedScreen();
        return;
    }
    HeaderOfScreens("Delete Client ");
    vector<sClientData> vClientData = LoadCleintsDataFromFile(ClientsFileName);
    string accountNumber;
    sClientData ClientData;
    cout << "Please enter Account Number? ";
    cin >> accountNumber;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    while (!AccountNumberExists(accountNumber, vClientData, &ClientData))
    {
        cout << "Client with [" << accountNumber << "] does not exists, Enter Another Account Number? ";
        getline(cin >> ws, accountNumber);
    }
    PrintClientDetails(ClientData);
    DeleteClientByAccountNumber(ClientData.AccountNumber, vClientData);
    PauseFunction();
}
void DeletingUserFun() {
    HeaderOfScreens("Delete User ");
    vector<sUserData> vUserData = LoadUsersDataFromFile(UsersFileName);
    string UserName;
    sUserData UserData;
    cout << "Please enter User Name? ";
    cin >> UserName;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    while (!UserNameExists(UserName, vUserData, &UserData))
    {
        cout << "User with [" << UserName << "] does not exists, Enter Another User Name? ";
        getline(cin >> ws, UserName);
    }
    PrintUserDetails(UserData);
    if (UserData.UserName != "Admin")
        DeleteUserByUserName(UserData.UserName, vUserData);
    else
        cout << "\nAdmin Users Can not be Deleted.\n";
    PauseManageUserFunction();
}

void UpdateClientData(sClientData& ClientData) {

    ClientData.PinCode = ReadData("Pin Code");
    ClientData.Name = ReadData("Name");
    ClientData.Phone = ReadData("Phone");
    ClientData.AccountBalance = ReadDouData("AccountBalance");
}
void UpdateUserData(sUserData& UserData) {

    UserData.UserName = ReadData("User Name");
    UserData.PassWord = ReadData("Password");
    UserData.Permissions = ReadUserPermissions(UserData);
}

void UpdateClientByAccountNumber(string AccountNumber, vector<sClientData>& vClientData) {
    char Update = 'y';
    cout << "\nAre you want Update this client? y/n ? ";
    cin >> Update;
    if (toupper(Update) == 'Y')
    {
        for (sClientData& Client : vClientData)
        {
            if (AccountNumber == Client.AccountNumber) {
                UpdateClientData(Client);
                break;
            }

        }
        SaveClientToFile(ClientsFileName, vClientData);
        cout << "\nThe Client data is Updated";
    }

}
void UpdateUserByUserName(string UserName, vector<sUserData>& vUserData) {
    char Update = 'y';
    cout << "\nAre you want Update this client? y/n ? ";
    cin >> Update;
    if (toupper(Update) == 'Y')
    {
        for (sUserData& User : vUserData)
        {
            if (UserName == User.UserName) {
                UpdateUserData(User);
                break;
            }

        }
        SaveUserToFile(UsersFileName, vUserData);
        cout << "\nThe User data is Updated";
    }

}

void UpdatingUserFun() {
    HeaderOfScreens("Update User ");
    vector<sUserData> vUserData = LoadUsersDataFromFile(UsersFileName);
    string UserName;
    sUserData UserData;
    cout << "Please enter User Name? ";
    cin >> UserName;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    while (!UserNameExists(UserName, vUserData, &UserData))
    {
        cout << "User with [" << UserName << "] does not exists, Enter Another User Name? ";
        getline(cin >> ws, UserName);
    }
    PrintUserDetails(UserData);
    UpdateUserByUserName(UserData.UserName, vUserData);
    PauseManageUserFunction();
}
void UpdatingClientFun() {
    if (!CheckAccessPermission(eMainMenuePermissions::pUpdateClient))
    {
        AccessDeniedScreen();
        return;
    }
    HeaderOfScreens("Update Client ");
    vector<sClientData> vClientData = LoadCleintsDataFromFile(ClientsFileName);
    string accountNumber;
    sClientData ClientData;
    cout << "Please enter Account Number? ";
    cin >> accountNumber;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    while (!AccountNumberExists(accountNumber, vClientData, &ClientData))
    {
        cout << "Client with [" << accountNumber << "] does not exists, Enter Another Account Number? ";
        getline(cin >> ws, accountNumber);
    }
    PrintClientDetails(ClientData);
    UpdateClientByAccountNumber(ClientData.AccountNumber, vClientData);
    PauseFunction();
}

void FindingClientFun() {
    if (!CheckAccessPermission(eMainMenuePermissions::pFindClient))
    {
        AccessDeniedScreen();
        return;
    }
    HeaderOfScreens("Find Client ");
    vector<sClientData> vClientData = LoadCleintsDataFromFile(ClientsFileName);
    string accountNumber;
    sClientData ClientData;
    cout << "Please enter Account Number? ";
    getline(cin >> ws, accountNumber);
    while (!AccountNumberExists(accountNumber, vClientData, &ClientData))
    {
        cout << "Client with [" << accountNumber << "] does not exists, Enter Another Account Number? ";
        getline(cin >> ws, accountNumber);
    }
    PrintClientDetails(ClientData);
    PauseFunction();
}
void FindingUserFun() {
    HeaderOfScreens("Find User ");
    vector<sUserData> vUserData = LoadUsersDataFromFile(UsersFileName);
    string UserName;
    sUserData UserData;
    cout << "Please enter User Name? ";
    getline(cin >> ws, UserName);
    while (!UserNameExists(UserName, vUserData, &UserData))
    {
        cout << "User with [" << UserName << "] does not exists, Enter Another User Name? ";
        getline(cin >> ws, UserName);
    }
    PrintUserDetails(UserData);
    PauseManageUserFunction();
}
void DepositBalanceToClientByAccountNumber(string AccountNumber, double Amount, vector <sClientData>& vClients) {
    char Answer;
    cout << "Are you Sure you want to preforme this transaction? Y/N? ";
    cin >> Answer;
    if (tolower(Answer) == 'y')
    {
        for (sClientData& Client : vClients) {
            if (Client.AccountNumber == AccountNumber)
            {
                Client.AccountBalance += Amount;
                cout << "\nNew Balance = " << Client.AccountBalance << endl;
                break;
            }
        }
    }
    SaveClientToFile(ClientsFileName, vClients);
    PauseTransactionsFunction();
}
void DepositFun() {
    HeaderOfScreens("Deposit ");

    vector<sClientData> vClientData = LoadCleintsDataFromFile(ClientsFileName);
    string accountNumber;
    double AddedValue;
    sClientData ClientData;
    cout << "\n\nPlease enter Account Number? ";
    cin >> accountNumber;
    while (!AccountNumberExists(accountNumber, vClientData, &ClientData))
    {
        cout << "Client with [" << accountNumber << "] does not exists,\nEnter Another Account Number? ";
        getline(cin >> ws, accountNumber);
    }
    PrintClientDetails(ClientData);
    cout << "Enter the deposite amount: ";
    cin >> AddedValue;
    DepositBalanceToClientByAccountNumber(accountNumber, AddedValue, vClientData);
}
void WithdrawFun() {
    HeaderOfScreens("Withdraw ");

    vector<sClientData> vClientData = LoadCleintsDataFromFile(ClientsFileName);
    string accountNumber;
    double WithdrawAmount;
    sClientData ClientData;
    cout << "\n\nPlease enter Account Number? ";
    cin >> accountNumber;
    while (!AccountNumberExists(accountNumber, vClientData, &ClientData))
    {
        cout << "Client with [" << accountNumber << "] does not exists,\nEnter Another Account Number? ";
        getline(cin >> ws, accountNumber);
    }
    PrintClientDetails(ClientData);
    cout << "Enter the Withdraw amount: ";
    cin >> WithdrawAmount;
    while (WithdrawAmount > ClientData.AccountBalance) {
        cout << "\nAmount Exceeds the balance, you can withdraw up to : " << ClientData.AccountBalance << endl;
        cout << "Please enter another amount? ";
        cin >> WithdrawAmount;
    }

    DepositBalanceToClientByAccountNumber(accountNumber, (-1 * WithdrawAmount), vClientData);
}
void TotalBalancesFun() {
    double TotalBalances = 0;
    vector<sClientData> vClientData = LoadCleintsDataFromFile(ClientsFileName);
    system("cls");
    cout << tabs(20) << "Client List (" << vClientData.size() << ") Client(s).\n" << endl;
    cout << "--------------------------------------------------------------------------------------------------\n" << endl;
    cout << "| " << left << setw(15) << "Accout Number";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Balance";
    cout << "\n\n--------------------------------------------------------------------------------------------------" << endl;
    for (sClientData Client : vClientData) {
        TotalBalances += Client.AccountBalance;
        PrintClientAsRowTotalBalancesTable(Client);
        cout << endl;
    }
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n";
    cout << tabs(24) << "Total Balances = " << TotalBalances << endl;
    PauseTransactionsFunction();
}

void ChoicesDirection() {
    int Choice;
    cout << "Choose what do You want to do? [1 to 6]?";
    cin >> Choice;
    switch (Choice)
    {
    case eMaiMenueChoices::ShowingList:
        PrintAllClientsData();
        break;
    case eMaiMenueChoices::AddingNewClient:
        AddingNewClientFun();
        break;
    case eMaiMenueChoices::DeletingClient:
        DeletingClientFun();
        break;
    case eMaiMenueChoices::UpdatingClient:
        UpdatingClientFun();
        break;
    case eMaiMenueChoices::FindingClient:
        FindingClientFun();
        break;
    case eMaiMenueChoices::Transactions:
        ShowTransactionsMenuSecreen();
        break;
    case eMaiMenueChoices::ManageUsers:
        ShowMangeUsersMenuSecreen();
        break;

    case eMaiMenueChoices::Logout:
        Login();
        break;

    default:
        cout << "\nWrong Choice, Choose Again: \n";
        ChoicesDirection();
        break;
    }
}
void ChoicesTransactionsDirection() {

    int Choice;
    cout << "Choose what do You want to do? [1 to 4]?";
    cin >> Choice;
    switch (Choice)
    {
    case eTransactionsChoices::Deposit:
        DepositFun();
        break;
    case eTransactionsChoices::Withdraw:
        WithdrawFun();
        break;
    case eTransactionsChoices::TotalBalances:
        TotalBalancesFun();
        break;
    case eTransactionsChoices::MainMenue:
        ShowMainMenueScreen();
        break;

    default:
        cout << "\nWrong Choice, Choose Again: \n";
        ChoicesTransactionsDirection();
        break;
    }
}
void ChoicesMangeUsersDirection() {

    int Choice;
    cout << "Choose what do You want to do? [1 to 6]?";
    cin >> Choice;
    switch (Choice)
    {
    case eMangeUserMenuChoices:: ShowingUserList:
        PrintAllUsersData();
        break;
    case eMangeUserMenuChoices::AddingNewUser:
        AddingNewUserFun();
        break;
    case eMangeUserMenuChoices::DeletingUser:
        DeletingUserFun();
        break;
    case eMangeUserMenuChoices::UpdatingUser:
        UpdatingUserFun();
        break;
    case eMangeUserMenuChoices::FindingUser:
        FindingUserFun();
        break;
    case eMangeUserMenuChoices::MainMenu:
        ShowMainMenueScreen();
        break;

    default:
        cout << "\nWrong Choice, Choose Again: \n";
        ChoicesMangeUsersDirection();
        break;
    }
}

void ShowTransactionsMenuSecreen() {
    if (!CheckAccessPermission(eMainMenuePermissions::pTransactions))
    {
        AccessDeniedScreen();
        return;
    }
    system("cls");
    HeaderOfScreens("Transactios ");
    cout << "\t[1] Deposit.\n";
    cout << "\t[2] Withdraw.\n";
    cout << "\t[3] Total Balances.\n";
    cout << "\t[4] Main Menue.\n";
    cout << "*************************************************************************\n";
    ChoicesTransactionsDirection();
}
void ShowMangeUsersMenuSecreen() {
    if (!CheckAccessPermission(eMainMenuePermissions::pManageUsers))
    {
        AccessDeniedScreen();
        return;
    }
    system("cls");
    HeaderOfScreens("Mange Users Menu ");
    cout << "\t[1] List Users.\n";
    cout << "\t[2] Add New User.\n";
    cout << "\t[3] Delete User.\n";
    cout << "\t[4] Update User.\n";
    cout << "\t[5] Find User.\n";
    cout << "\t[6] Main Menue.\n";
    cout << "*************************************************************************\n";
    ChoicesMangeUsersDirection();
}
void ShowMainMenueScreen() {

    system("cls");
    cout << "*************************************************************************\n";
    cout << tabs(2) << "Main Menue Screen\n";
    cout << "*************************************************************************\n";
    cout << "\t[1] Show Client list.\n";
    cout << "\t[2] Add New Client.\n";
    cout << "\t[3] Delete Client.\n";
    cout << "\t[4] Update Client.\n";
    cout << "\t[5] Find Client.\n";
    cout << "\t[6] Transactios.\n";
    cout << "\t[7] Mange Users.\n";
    cout << "\t[8] Logout.\n";
    cout << "*************************************************************************\n";
    ChoicesDirection();
}

void PauseFunction() {
    cout << "\n\nPress any key to go back to Main Menue, ";
    system("pause");
    ShowMainMenueScreen();
}
void PauseTransactionsFunction() {
    cout << "\n\nPress any key to go back to Transactions Menue,";
    system("pause");
    ShowTransactionsMenuSecreen();
}
void PauseManageUserFunction() {
    cout << "\n\nPress any key to go back to Manage User Menue,";
    system("pause");
    ShowMangeUsersMenuSecreen();
}

bool FindUserByNameAndPassword(string UserName, string PassWord, sUserData& UserData) {
    vector<sUserData> vUsers = LoadUsersDataFromFile(UsersFileName);
    for (sUserData User : vUsers) {
        if (User.UserName == UserName && User.PassWord == PassWord)
        {
            UserData = User;
            return true;
        }
    }
    return false;
}
bool LoadUserInfo(string UserName, string PassWord) {

    return FindUserByNameAndPassword(UserName, PassWord, CurrentUser);
    
}
void Login() {

    bool LoginFaild = false;
    string UserName, Password;
    do
    {
        system("cls");
        HeaderOfScreens("Login ");
        if (LoginFaild)
        {
            cout << "Invalid Username/Password!\n";
        }
        cout << "\nEnter UserName? ";
        getline(cin >> ws, UserName);
        cout << "\nEnter PassWord? ";
        getline(cin >> ws, Password);
        LoginFaild = !LoadUserInfo(UserName, Password);
    } while (LoginFaild);
    ShowMainMenueScreen();
}

int main()
{
    Login();
    system("pause");
    return 0;
}
