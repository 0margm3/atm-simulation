#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <io.h>
#include <time.h>
#include <windows.h>
#define Number ((NumberToReturn_Char_Input >= 48) && (NumberToReturn_Char_Input <= 57))
#define notNumber !number
#define whitespace 32
#define backspace 8
#define enter 13
#define Ext ".txt"
#define MAX 50
#define MXCHAR 50
#define lu 186

int Global_int_ClientNo;
int Global_Int_Current;
int Global_Int_USBFilepath;
int Global_Int_log;

time_t rawtime;
struct tm * timeinfo;
//Client Info
struct ClientCred{
    int Balance;
    char UserID[7];
    char PIN[7];
    char LastName[MXCHAR];
    char FirstName[MXCHAR];
    char MiddleName[MXCHAR];
} ClientCred[MAX];
//Client Log
struct ClientLog{
    int prevBal;
    int newBal;
    int causeBal;
    int type;
    int UserID;
    char timeNdate[16];
}ClientLog[MAX];

extern void disableMaximizeButton(void);
void GetCred(char GetCred_STR_lastname[], char GetCred_STR_firstname[], char GetCred_STR_middlename[], char GetCred_STR_id[], char GetCred_STR_pin[], int GetCred_INT_bal);
void LoadCred();
void SaveCred();
void InitializeVar();
void DetectUSB();
void EjectUSB();
const char* GetUSBPath(char* GetUSBPath_STRptr_path, int GetUSBPath_INT_CharacterIteration);
int CheckID(char ID[]);
const char NumberToReturn();
const char* NumberArrToReturn(char NumberArrToReturn_STR_UserInput[]);
void PinValidation();
void AdminMenu();
void Delay(float Delay_Float_Time);
void Transition(int Transition_INT_TypeOfTransaction);
void HideCursor();
void gotoxy(int positionX, int positionY);
HANDLE wHND;    // Handle to write to the console.
HANDLE rHND;    // Handle to read from the console.

void frontpage();//ATM page

void menu();
    void Transaction(int Transaction_INT_TypeOfTransaction);
        void TranscationConfirmation(int TranscationConfirmation_INT_TypeOfTransaction, int TranscationConfirmation_INT_sum, int TranscationConfirmation_INT_RecipientID);
        const char* GetTime_Date(char GetTime_Date_STRptr_TimeNDateInfo[]);
        int recipient();
    void Balance();
    void TransactionHistory();
        void HistoryPrint(int HistoryPrint_INT_TypeOfTransactionHistory);
        void LoadLog(int LoadLog_INT_TypeOfHistory, int LoadLog_INT_UserID);
        void SaveLog(int SaveLog_INT_TypeOfHistory, int SaveLog_INT_UserID);
        void GetLog(char GetLog_STR_TimeNDateINFO[], int GetLog_INT_PreBalance, int GetLog_INT_CauseBalance, int GetLog_INT_NewBalance, int GetLog_INT_UserID, int GetLog_INT_TypeOfTransactionHistory);
        void Initializelog();
        const char* logpath(char logpath_STR_PathDirectory[], int logpath_INT_TypeOfTransactionHistory, int logpath_INT_UserID);
    void ChangePIN();
        void CheckChangePINInput(char CheckChangePINInput_STR_CurrentPIN[], char CheckChangePINInput_STR_NewPIN[], char CheckChangePINInput_STR_PINConfirmation[]);

int main()
{
    wHND = GetStdHandle(STD_OUTPUT_HANDLE);
    rHND = GetStdHandle(STD_INPUT_HANDLE);
    // Set up the required window size:
    SMALL_RECT WindowSize = {0, 0, 800, 400};
    // Buffer size
    COORD BufferSize = {10, 10};

    SetConsoleWindowInfo(wHND, 1, &WindowSize);
    //Prevent User resizing windows terminal
    HWND consoleWindow = GetConsoleWindow();
    SetWindowLong(consoleWindow, GWL_STYLE, GetWindowLong(consoleWindow, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);

    disableMaximizeButton();
    SetConsoleTitle("ATM Simulation");
    HideCursor();
    InitializeVar();
    LoadCred();
    frontpage();
    DetectUSB();
}
//Disable maximize/minimize button
extern void disableMaximizeButton(void)
{
    HWND disableMaximizeButton_HWND_hwnd = GetConsoleWindow();
       SetWindowLong(disableMaximizeButton_HWND_hwnd, GWL_STYLE, GetWindowLong(disableMaximizeButton_HWND_hwnd, GWL_STYLE) & ~WS_MAXIMIZEBOX );
}

//Initialize the User Credentials Array and creation of user log folder
void InitializeVar()
{
    Global_int_ClientNo = -1;
    mkdir("log");
}
void GetCred(char GetCred_STR_lastname[], char GetCred_STR_firstname[], char GetCred_STR_middlename[], char GetCred_STR_id[], char GetCred_STR_pin[], int GetCred_INT_bal)//Copying the data from database textfile to struct array
{
    Global_int_ClientNo++;

    ClientCred[Global_int_ClientNo].Balance = GetCred_INT_bal;
    strcpy(ClientCred[Global_int_ClientNo].LastName, GetCred_STR_lastname);
    strcpy(ClientCred[Global_int_ClientNo].FirstName, GetCred_STR_firstname);
    strcpy(ClientCred[Global_int_ClientNo].MiddleName, GetCred_STR_middlename);
    strncpy(ClientCred[Global_int_ClientNo].UserID, GetCred_STR_id, 6);
    strncpy(ClientCred[Global_int_ClientNo].PIN, GetCred_STR_pin, 6);
}
//Load Database
void LoadCred()
{
    FILE* LoadCred_FILEptr_Database;
    int LoadCred_Int_bal, LoadCred_Int_no;
    char LoadCred_Str_TempoL[MXCHAR], LoadCred_Str_TempoF[MXCHAR], LoadCred_Str_TempoM[MXCHAR], LoadCred_Str_TempoPIN[MXCHAR], LoadCred_Str_TempoID[MXCHAR];
    char LoadCred_Str_line[500];

    LoadCred_FILEptr_Database = fopen("database.txt", "r");

    while((fgets(LoadCred_Str_line, sizeof(LoadCred_Str_line), LoadCred_FILEptr_Database)) != NULL)
    {
        sscanf(LoadCred_Str_line, "%d %s %s %s %d %s %s", &LoadCred_Int_no, LoadCred_Str_TempoL, LoadCred_Str_TempoF, LoadCred_Str_TempoM, &LoadCred_Int_bal, LoadCred_Str_TempoID, LoadCred_Str_TempoPIN);
        GetCred(LoadCred_Str_TempoL, LoadCred_Str_TempoF, LoadCred_Str_TempoM, LoadCred_Str_TempoID, LoadCred_Str_TempoPIN, LoadCred_Int_bal);
    }
}
//Save to Database text file
void SaveCred()
{
    FILE* SaveCred_FILEptr_Database;

    SaveCred_FILEptr_Database = fopen("database.txt", "w");

    for(int x = 0; x <= Global_int_ClientNo; x++)
        fprintf(SaveCred_FILEptr_Database,"%d %s %s %s %d %s %s\n", x, ClientCred[x].LastName, ClientCred[x].FirstName, ClientCred[x].MiddleName, ClientCred[x].Balance, ClientCred[x].UserID, ClientCred[x].PIN);

    fclose(SaveCred_FILEptr_Database);
}
//Flash Drive Detection
void DetectUSB()
{
    FILE* DetectUSB_FILEptr_Card;
    char DetectUSB_Str_filepath[100];
    char DetectUSB_Str_ID[7];
    int DetectUSB_Int_Letter = 0;

    system("cls");
    gotoxy(45,10); printf("Please Insert your Flash Drive.");
    do
    {
        if(DetectUSB_Int_Letter == 25)
            DetectUSB_Int_Letter = 0;

        GetUSBPath(DetectUSB_Str_filepath, DetectUSB_Int_Letter);
        DetectUSB_FILEptr_Card = fopen(DetectUSB_Str_filepath, "r");
        fscanf(DetectUSB_FILEptr_Card, "%s", DetectUSB_Str_ID);
        Global_Int_USBFilepath = DetectUSB_Int_Letter;
        DetectUSB_Int_Letter++;

    }
    while(DetectUSB_FILEptr_Card == NULL);

    fclose(DetectUSB_FILEptr_Card);

    Delay(2);

    if((CheckID(DetectUSB_Str_ID)) == 1)
    {
        Delay(1);
        system("cls");
        gotoxy(45,10); printf("User Confirmed.\n");
        Delay(1.5);
        PinValidation();
    }
    else
    {
        Delay(1);
        system("cls");
        gotoxy(45,10); printf("Invalid ID\n");
        Delay(0.5);
        EjectUSB();
        Delay(1);
        DetectUSB();
    }
}
//Get the FlashDrive path
const char* GetUSBPath(char* GetUSBPath_STRptr_path, int GetUSBPath_INT_CharacterIteration)
{

    char GetUSBPath_Str_temp[100] = "";
    GetUSBPath_Str_temp[0] = GetUSBPath_INT_CharacterIteration + 'A';
    strcat((strcat(GetUSBPath_Str_temp, ":\\")), "UserID.txt");
    strcpy(GetUSBPath_STRptr_path, GetUSBPath_Str_temp);

    return 0;
}
//Eject Flash Drive
void EjectUSB()
{
    FILE* EjectUSB_FILEptr_ClientCard;
    char EjectUSB_FILEptr_filepath[100];

    do
    {
        system("cls");
        gotoxy(45,10); printf("Please Remove The Flash Drive");
        GetUSBPath(EjectUSB_FILEptr_filepath, Global_Int_USBFilepath);
        EjectUSB_FILEptr_ClientCard = fopen(EjectUSB_FILEptr_filepath, "r");

    }
    while(EjectUSB_FILEptr_ClientCard != NULL);

    fclose(EjectUSB_FILEptr_ClientCard);
}
//ID Check
int CheckID(char ID[])
{

    for(int x = 0; x <= Global_int_ClientNo; x++)
    {
        if((strcmp(ID, ClientCred[x].UserID)) == 0)
        {
            Global_Int_Current = x;
            return 1;
        }
    }
    return 0;
}
//Input Checker
const char NumberToReturn()
{
    char NumberToReturn_Char_Input;

    NumberToReturn_Char_Input = getch();
    if(NumberToReturn_Char_Input == backspace)
        return 0;

    else if(NumberToReturn_Char_Input == enter)
        return 1;

    else if(!(Number))
        return -1;

    return NumberToReturn_Char_Input;
}
//User PIN Input
const char* NumberArrToReturn(char NumberArrToReturn_STR_UserInput[]){
    char NumberArrToReturn_Char_temp;

    for(int x = 0; x < 6; x++)
    {

        NumberArrToReturn_Char_temp = NumberToReturn();//Checks the input

        if(NumberArrToReturn_Char_temp == 1)
        {//Exit the function
            NumberArrToReturn_STR_UserInput[x] = '\0';
            break;
        }
        if(NumberArrToReturn_Char_temp == -1 && x >= 0)
        {//Ignores symbols
            if( x >= 0){
                printf("%c", whitespace);
                printf("%c", backspace);
                x--;
            }
        }
        else if(NumberArrToReturn_Char_temp == 0)
        {//Delete char
            if(x == 0)
                x--;

            if(x >= 0)
            {
                x--;
                NumberArrToReturn_STR_UserInput[x] = '\0';
                printf("%c", backspace);
                printf("%c", whitespace);
                printf("%c", backspace);
                x--;
            }
        }
        else//copy the NumberArrToReturn_Char_temp char to char arry
        {
            NumberArrToReturn_STR_UserInput[x] = NumberArrToReturn_Char_temp;
            printf("*");
        }
    }
    NumberArrToReturn_STR_UserInput[6] = '\0';
    return 0;
}
//Pin Validation
void PinValidation()
{
    char PinValidation_str_PIN[7];

    EjectUSB();

    system("cls");
    //Design Border
    gotoxy(43, 9);  printf("============================");
    gotoxy(43, 10); printf("%c", lu);
    gotoxy(70, 10); printf("%c", lu);
    gotoxy(43, 11); printf("%c", lu);
    gotoxy(70, 11); printf("%c", lu);
    gotoxy(43, 12);  printf("============================");


    //function
    gotoxy(45, 10); printf("please enter your PIN \n");
    gotoxy(45, 11); printf("PIN: ");
    gotoxy(50, 11); NumberArrToReturn(PinValidation_str_PIN);


    if((strcmp(PinValidation_str_PIN, ClientCred[Global_Int_Current].PIN)) == 0)
        menu();
    else
        PinValidation();

}
//User Menu
void menu()
{
    int menu_int_opt, menu_int_type;


    system("cls");
    // Menu
    gotoxy(45, 10); printf("WELCOME TO BANGKO DE OMARU");
    gotoxy(45, 11); printf("[1] WITHDRAW");
    gotoxy(45, 12); printf("[2] DEPOSIT");
    gotoxy(45, 13); printf("[3] MONEY TRANSFER");
    gotoxy(45, 14); printf("[4] BALANCE INQUIRY");
    gotoxy(45, 15); printf("[5] TRANSACTION HISTORY");
    gotoxy(45, 16); printf("[6] CHANGE PIN");
    gotoxy(45, 17); printf("[7] SIGN OUT");

    //Design Border
    gotoxy(43, 8);  printf("================================");
    gotoxy(42, 9);  printf("%c", lu);
    gotoxy(75, 9);  printf("%c", lu);
    gotoxy(42, 10); printf("%c", lu);
    gotoxy(75, 10);  printf("%c", lu);
    gotoxy(42, 11); printf("%c", lu);
    gotoxy(75, 11); printf("%c", lu);
    gotoxy(42, 12); printf("%c", lu);
    gotoxy(75, 12);  printf("%c", lu);
    gotoxy(42, 13); printf("%c", lu);
    gotoxy(75, 13);  printf("%c", lu);
    gotoxy(42, 14); printf("%c", lu);
    gotoxy(75, 14);  printf("%c", lu);
    gotoxy(42, 15); printf("%c", lu);
    gotoxy(75, 15);  printf("%c", lu);
    gotoxy(42, 16); printf("%c", lu);
    gotoxy(75, 16);  printf("%c", lu);
    gotoxy(42, 17); printf("%c", lu);
    gotoxy(75, 17);  printf("%c", lu);
    gotoxy(42, 18); printf("%c", lu);
    gotoxy(75, 18);  printf("%c", lu);
    gotoxy(43, 19); printf("================================");

    switch(menu_int_opt = getch() - '0')
    {
        case 1: menu_int_type = 0; Transaction(menu_int_type); break;
        case 2: menu_int_type = 1; Transaction(menu_int_type); break;
        case 3: menu_int_type = 2; Transaction(menu_int_type); break;
        case 4: Balance(); break;
        case 5: TransactionHistory(); break;
        case 6: ChangePIN(); break;
        case 7: SaveCred(); DetectUSB(); break;
        default: menu(); break;
    }
}
void Transaction(int Transaction_INT_TypeOfTransaction)
{
    int Transaction_int_CashOpt[6] = {50, 100, 200, 500, 1000, 10000};
    int Transaction_int_opt = -1, Transaction_int_sum = 0;
    int Transaction_int_id;

    if(Transaction_INT_TypeOfTransaction == 2)
        Transaction_int_id = recipient();
    do
    {
        system("cls");
        if(Transaction_INT_TypeOfTransaction == 0){
            gotoxy(45, 9); printf("WITHDRAW");
        }
        else if(Transaction_INT_TypeOfTransaction == 1){
            gotoxy(45, 9); printf("DEPOSIT");
        }
        else if(Transaction_INT_TypeOfTransaction == 2){
            gotoxy(45, 9); printf("MONEY TRANSER");
        }

        gotoxy(45, 19); printf("CASH: %d", Transaction_int_sum);

        for(int x = 0; x < 6; x++){
            gotoxy(45, x + 10); printf("[%d] %d", x + 1, Transaction_int_CashOpt[x]);
        }

        gotoxy(45, 16); printf("[7] Menu");
        gotoxy(45, 17); printf("[8] Clear");
        gotoxy(45, 18); printf("[9] Confirm");

        //Design Layout
        gotoxy(43, 7);  printf("======================");
        gotoxy(42, 8);  printf("%c", lu);
        gotoxy(65, 8);  printf("%c", lu);
        gotoxy(42, 9);  printf("%c", lu);
        gotoxy(65, 9);  printf("%c", lu);
        gotoxy(42, 10); printf("%c", lu);
        gotoxy(65, 10); printf("%c", lu);
        gotoxy(42, 11); printf("%c", lu);
        gotoxy(65, 11); printf("%c", lu);
        gotoxy(42, 12); printf("%c", lu);
        gotoxy(65, 12); printf("%c", lu);
        gotoxy(42, 13); printf("%c", lu);
        gotoxy(65, 13); printf("%c", lu);
        gotoxy(42, 14); printf("%c", lu);
        gotoxy(65, 14); printf("%c", lu);
        gotoxy(42, 15); printf("%c", lu);
        gotoxy(65, 15); printf("%c", lu);
        gotoxy(42, 16); printf("%c", lu);
        gotoxy(65, 16); printf("%c", lu);
        gotoxy(42, 17); printf("%c", lu);
        gotoxy(65, 17); printf("%c", lu);
        gotoxy(42, 18); printf("%c", lu);
        gotoxy(65, 18); printf("%c", lu);
        gotoxy(42, 19); printf("%c", lu);
        gotoxy(65, 19); printf("%c", lu);
        gotoxy(42, 20); printf("%c", lu);
        gotoxy(65, 20); printf("%c", lu);
        gotoxy(43, 21);  printf("======================");

        Transaction_int_opt = getch() - '0';

        if(Transaction_int_opt == 7)
            break;

        else if(Transaction_int_opt == 8)
            Transaction_int_sum = 0;

        else if(Transaction_int_opt >= 1 && Transaction_int_opt <= 6)
            Transaction_int_sum = Transaction_int_sum + Transaction_int_CashOpt[Transaction_int_opt - 1];

    }
    while(Transaction_int_opt != 9);
    if(Transaction_int_opt == 7)
        menu();

    else if(Transaction_int_sum == 0)
        Transaction(Transaction_INT_TypeOfTransaction);

    else if(Transaction_int_opt == 9)
        TranscationConfirmation(Transaction_INT_TypeOfTransaction, Transaction_int_sum, Transaction_int_id);
}
void TranscationConfirmation(int TranscationConfirmation_INT_TypeOfTransaction, int TranscationConfirmation_INT_sum, int TranscationConfirmation_INT_RecipientID)
{
    int TranscationConfirmation_Int_opt;
    char TranscationConfirmation_Str_time[25];

    system("cls");

    Initializelog();
    LoadLog(TranscationConfirmation_INT_TypeOfTransaction, -1);

                gotoxy(45, 9); printf("CONFIRM: \n");
    switch(TranscationConfirmation_INT_TypeOfTransaction)
    {
        case 0:
            gotoxy(45, 10);
            printf("WITHDRAW %d?\n", TranscationConfirmation_INT_sum);
            break;
        case 1:
            gotoxy(45, 10);
            printf("DEPOSIT %d?\n", TranscationConfirmation_INT_sum);
            break;
        case 2:
             gotoxy(45, 10);
             printf("TRANSFER %d TO %s?\n", TranscationConfirmation_INT_sum, ClientCred[TranscationConfirmation_INT_RecipientID].LastName);
            break;
    }
                gotoxy(45, 11); printf("[1] YES [2] RETURN [3] MENU");

    //Design Layout
        gotoxy(43, 7);   printf("===============================");
        gotoxy(42, 8);   printf("%c", lu);
        gotoxy(74, 8);   printf("%c", lu);
        gotoxy(42, 9);   printf("%c", lu);
        gotoxy(74, 9);   printf("%c", lu);
        gotoxy(42, 10);  printf("%c", lu);
        gotoxy(74, 10);  printf("%c", lu);
        gotoxy(42, 11);  printf("%c", lu);
        gotoxy(74, 11);  printf("%c", lu);
        gotoxy(42, 12);  printf("%c", lu);
        gotoxy(74, 12);  printf("%c", lu);
        gotoxy(43, 13);  printf("===============================");

    TranscationConfirmation_Int_opt = getch() - '0';
    switch(TranscationConfirmation_Int_opt)
    {
        case 1:
            Transition(TranscationConfirmation_INT_TypeOfTransaction);
            switch(TranscationConfirmation_INT_TypeOfTransaction)
            {
                case 0:
                    if(ClientCred[Global_Int_Current].Balance < TranscationConfirmation_INT_sum)
                    {
                        gotoxy(45, 20); printf("You do not have enough balance...");
                        Delay(3);
                    }
                    else//Withdraw of the money and saving the user log
                    {
                        GetTime_Date(TranscationConfirmation_Str_time);
                        ClientCred[Global_Int_Current].Balance = ClientCred[Global_Int_Current].Balance - TranscationConfirmation_INT_sum;
                        GetLog(TranscationConfirmation_Str_time, ClientCred[Global_Int_Current].Balance + TranscationConfirmation_INT_sum, TranscationConfirmation_INT_sum, ClientCred[Global_Int_Current].Balance, -1, -1);
                        SaveLog(TranscationConfirmation_INT_TypeOfTransaction, -1);
                    }
                    SaveCred();
                    menu();
                    break;
                case 1:
                    if(TranscationConfirmation_INT_sum > 100000)//Transaction will be cancelled if sum exceed the maximum amount of money atm can hold
                    {
                        gotoxy(45, 20); printf("Please go to our teller for bigger larger transaction...");
                        Delay(3);
                    }
                    else//Deposit of the money and saving the user log
                    {
                        GetTime_Date(TranscationConfirmation_Str_time);
                        ClientCred[Global_Int_Current].Balance = ClientCred[Global_Int_Current].Balance + TranscationConfirmation_INT_sum;
                        GetLog(TranscationConfirmation_Str_time, ClientCred[Global_Int_Current].Balance - TranscationConfirmation_INT_sum, TranscationConfirmation_INT_sum, ClientCred[Global_Int_Current].Balance, -1, -1);
                        SaveLog(TranscationConfirmation_INT_TypeOfTransaction, -1);
                     }
                    SaveCred();
                    menu();
                    break;
                case 2:
                    if(TranscationConfirmation_INT_sum > 100000)//Transaction will be cancelled if sum exceed the maximum amount of money atm can hold
                    {
                        gotoxy(45, 20); printf("Please go to our teller for bigger larger transaction...");
                        Delay(3);
                    }
                    else if(TranscationConfirmation_INT_sum > ClientCred[Global_Int_Current].Balance){
                        gotoxy(45, 20); printf("You do not have enough balance...");
                        Delay(3);
                    }
                    else//Transfer the money to the recipient and saving the user log
                    {
                        GetTime_Date(TranscationConfirmation_Str_time);
                        ClientCred[Global_Int_Current].Balance = ClientCred[Global_Int_Current].Balance - TranscationConfirmation_INT_sum;
                        GetLog(TranscationConfirmation_Str_time, ClientCred[Global_Int_Current].Balance + TranscationConfirmation_INT_sum, TranscationConfirmation_INT_sum, ClientCred[Global_Int_Current].Balance, TranscationConfirmation_INT_RecipientID, 0);
                        SaveLog(TranscationConfirmation_INT_TypeOfTransaction, -1);
                        Initializelog();
                        LoadLog(TranscationConfirmation_INT_TypeOfTransaction, TranscationConfirmation_INT_RecipientID);
                        ClientCred[TranscationConfirmation_INT_RecipientID].Balance = ClientCred[TranscationConfirmation_INT_RecipientID].Balance + TranscationConfirmation_INT_sum;
                        GetLog(TranscationConfirmation_Str_time, ClientCred[TranscationConfirmation_INT_RecipientID].Balance - TranscationConfirmation_INT_sum, TranscationConfirmation_INT_sum, ClientCred[TranscationConfirmation_INT_RecipientID].Balance, Global_Int_Current, 1);
                        SaveLog(TranscationConfirmation_INT_TypeOfTransaction, TranscationConfirmation_INT_RecipientID);
                    }
                    SaveCred();
                    menu();
                    break;
            }
            break;
        case 2: Transaction(TranscationConfirmation_INT_TypeOfTransaction);
            break;
        case 3: menu();
            break;
        default: TranscationConfirmation(TranscationConfirmation_INT_TypeOfTransaction, TranscationConfirmation_INT_sum, TranscationConfirmation_INT_RecipientID);
            break;
    }
}
int recipient()//Get the ID of the recipient for the transfer of money
{
    char recipient_Str_recipientID[7];
    system("cls");

    gotoxy(45, 9); printf("recipient ID: ");
    gotoxy(59, 9); NumberArrToReturn(recipient_Str_recipientID);

    for(int x = 0; x <= Global_int_ClientNo; x++)
    {
        if((strcmp(recipient_Str_recipientID, ClientCred[x].UserID)) == 0)
            return x;
    }
    system("cls");
    gotoxy(45, 9);printf("Invalid ID");
    Delay(1.5);
    menu();
    return 0;
}
//CHANGE PIN
void ChangePIN()
{
    char ChangePIN_str_CurrentPIN[7], ChangePIN_str_NewPIN[7], ChangePIN_str_ConfirmPIN[7];

    system("cls");
    //Design Layout
    gotoxy(43, 9);  printf("======================================");
    gotoxy(43, 10); printf("%c", lu);
    gotoxy(80, 10); printf("%c", lu);
    gotoxy(43, 11); printf("%c", lu);
    gotoxy(80, 11); printf("%c", lu);
    gotoxy(43, 12); printf("%c", lu);
    gotoxy(80, 12); printf("%c", lu);
    gotoxy(43, 12); printf("%c", lu);
    gotoxy(80, 12); printf("%c", lu);
    gotoxy(43, 13); printf("%c", lu);
    gotoxy(80, 13); printf("%c", lu);
    gotoxy(43, 14); printf("%c", lu);
    gotoxy(80, 14); printf("%c", lu);
    gotoxy(43, 15); printf("=====================================");

    //Change Pin
    gotoxy(45, 10);
    printf("Current PIN: ");
    NumberArrToReturn(ChangePIN_str_CurrentPIN);
    gotoxy(45, 11);
    printf("New PIN: ");
    NumberArrToReturn(ChangePIN_str_NewPIN);
    gotoxy(45, 12);
    printf("Confirm PIN: ");
    NumberArrToReturn(ChangePIN_str_ConfirmPIN);

    CheckChangePINInput(ChangePIN_str_CurrentPIN, ChangePIN_str_NewPIN, ChangePIN_str_ConfirmPIN);
}
void CheckChangePINInput(char CheckChangePINInput_STR_CurrentPIN[], char CheckChangePINInput_STR_NewPIN[], char CheckChangePINInput_STR_PINConfirmation[])//Conditions of changing pin
{

        system("cls");
    if((strcmp(CheckChangePINInput_STR_CurrentPIN, ClientCred[Global_Int_Current].PIN)) != 0)//If the Current pin and input are not equal
    {
        gotoxy(45, 13);
    printf("Current PIN is Invalid...");
    }
    else if((strcmp(CheckChangePINInput_STR_CurrentPIN, CheckChangePINInput_STR_NewPIN)) == 0)//if the Current and new pin are identical
    {
        gotoxy(45, 13);
        printf("Current PIN and New PIN are Identical...");
    }
    else if((strcmp(CheckChangePINInput_STR_NewPIN, CheckChangePINInput_STR_PINConfirmation)) != 0)//if the new and confirmation of pin are not identical
    {
        gotoxy(45, 13);
        printf("New PIN and Confirmation PIN are Not Identical...");
    }
    else//PIN will be change if the conditions above are not satisfied
        strcpy(ClientCred[Global_Int_Current].PIN, CheckChangePINInput_STR_NewPIN);

    getch();
    menu();
}
void Balance()
{//Printing of Current Balance
    system("cls");
    //Design Layout
    gotoxy(42, 9);  printf("======================================");
    gotoxy(42, 10); printf("%c",lu);
    gotoxy(80, 10); printf("%c",lu);
    gotoxy(42, 11); printf("%c",lu);
    gotoxy(80, 11); printf("%c",lu);
    gotoxy(42, 12); printf("======================================");

    //Balance Check
    gotoxy(45, 10);
    printf("[Press any key to go back]");
    gotoxy(45, 11);
    printf("Balance: %d php", ClientCred[Global_Int_Current].Balance);
    getch();
    menu();
}
void TransactionHistory()
{//Menu of Transaction
    int TransactionHistory_INT_opt, TransactionHistory_INT_TypeOfHistory;

    system("cls");
    //Design Layout
    gotoxy(43, 9);  printf("======================================");
    gotoxy(43, 10); printf("%c", lu);
    gotoxy(80, 10); printf("%c", lu);
    gotoxy(43, 11); printf("%c", lu);
    gotoxy(80, 11); printf("%c", lu);
    gotoxy(43, 12); printf("%c", lu);
    gotoxy(80, 12); printf("%c", lu);
    gotoxy(43, 13); printf("%c", lu);
    gotoxy(80, 13); printf("%c", lu);
    gotoxy(43, 14); printf("%c", lu);
    gotoxy(80, 14); printf("%c", lu);
    gotoxy(43, 15); printf("======================================");


    //Transaction History
    gotoxy(45, 10);
    printf("TRANSACTION HISTORY");
    gotoxy(45, 11);
    printf("[1] WITHDRAWN");
    gotoxy(45, 12);
    printf("[2] DEPOSITED");
    gotoxy(45, 13);
    printf("[3] MONEY TRANSFERRED");
    gotoxy(45, 14);
    printf("[4] EXIT");

    switch(TransactionHistory_INT_opt = getch() - '0')
    {
        case 1: TransactionHistory_INT_TypeOfHistory = 0; HistoryPrint(TransactionHistory_INT_TypeOfHistory); break;
        case 2: TransactionHistory_INT_TypeOfHistory = 1; HistoryPrint(TransactionHistory_INT_TypeOfHistory); break;
        case 3: TransactionHistory_INT_TypeOfHistory = 2; HistoryPrint(TransactionHistory_INT_TypeOfHistory); break;
        case 4: menu(); break;
        default: TransactionHistory(); break;
    }
}
void HistoryPrint(int HistoryPrint_INT_TypeOfTransactionHistory)//Printing of Transaction
{
    int HistoryPrint_int_gotoxyIncrement = 0;
    Initializelog();
    LoadLog(HistoryPrint_INT_TypeOfTransactionHistory, -1);

    system("cls");
    gotoxy(45, 10);
    printf("Press any key to go back...");
    if(Global_Int_log < 0)
    {
        gotoxy(45, 11);
        printf("Empty...");
    }
    else{
        for(int x = Global_Int_log; x >= 0; x--)//Loop for printing the transaction
        {
            gotoxy(46, 11 + HistoryPrint_int_gotoxyIncrement); printf("%s", ClientLog[x].timeNdate);

            if(ClientLog[x].type == 0)//Prints If the transfer was choosen
            {
                gotoxy(46, 12 + HistoryPrint_int_gotoxyIncrement);
                printf("RECIPIENT ID....: %s", ClientCred[ClientLog[x].UserID].UserID);
                gotoxy(46, 13 + HistoryPrint_int_gotoxyIncrement);
                printf("RECIPIENT.......: %s", ClientCred[ClientLog[x].UserID].LastName);
                HistoryPrint_int_gotoxyIncrement += 2;
            }
            else if(ClientLog[x].type == 1)//Prints If the transfer was choosen
            {
                gotoxy(46, 12 + HistoryPrint_int_gotoxyIncrement);
                printf("SENDER ID.......: %s", ClientCred[ClientLog[x].UserID].UserID);
                gotoxy(46, 13 + HistoryPrint_int_gotoxyIncrement);
                printf("SENDER..........: %s ", ClientCred[ClientLog[x].UserID].LastName);
                HistoryPrint_int_gotoxyIncrement += 2;
            }

            gotoxy(46, 12 + HistoryPrint_int_gotoxyIncrement);
            printf(" BALANCE........: %d", ClientLog[x].prevBal);

            if(HistoryPrint_INT_TypeOfTransactionHistory == 0)//If the WITHDRAWN was choosen
            {
                gotoxy(46, 13  + HistoryPrint_int_gotoxyIncrement);
                printf(" WITHDRAWN......: -%d", ClientLog[x].causeBal);
            }
            else if(HistoryPrint_INT_TypeOfTransactionHistory == 1)//If the DEPOSITED was choosen
            {
                gotoxy(46, 13  + HistoryPrint_int_gotoxyIncrement);
                printf(" DEPOSITED......: +%d", ClientLog[x].causeBal);
            }
            else if(HistoryPrint_INT_TypeOfTransactionHistory == 2)//If the transfer was choosen
            {
                if(ClientLog[x].type == 0)
                {
                    gotoxy(46, 13  + HistoryPrint_int_gotoxyIncrement);
                    printf(" TRANSFERRED....: -%d", ClientLog[x].causeBal);
                }
                else if(ClientLog[x].type == 1)
                {
                    gotoxy(46, 13 + HistoryPrint_int_gotoxyIncrement);
                    printf(" TRANSFERRED....: +%d", ClientLog[x].causeBal);
                }
            }
            gotoxy(46, 14  + HistoryPrint_int_gotoxyIncrement);
            printf(" TOTAL BALANCE..: %d", ClientLog[x].newBal);
            HistoryPrint_int_gotoxyIncrement += 4;
        }
    }
    getch();
    TransactionHistory();
}
const char* logpath(char logpath_STR_PathDirectory[], int logpath_INT_TypeOfTransactionHistory, int logpath_INT_UserID)
{
    char logpath_str_log[] = "log";
    char logpath_str_file_1[] = "withdrawn", logpath_str_file_2[] = "deposited", logpath_str_file_3[] = "transferred";

    if(logpath_INT_UserID == -1)
        strcpy(logpath_STR_PathDirectory, (strcat((strcat(logpath_str_log, "\\")), ClientCred[Global_Int_Current].UserID)));
    else
        strcpy(logpath_STR_PathDirectory, (strcat((strcat(logpath_str_log, "\\")), ClientCred[logpath_INT_UserID].UserID)));

    mkdir(logpath_STR_PathDirectory);

    if(logpath_INT_TypeOfTransactionHistory == 0)
        strcat((strcat((strcat((strcat(logpath_STR_PathDirectory, "\\")), logpath_str_file_1)), Ext)), "\0");

    else if(logpath_INT_TypeOfTransactionHistory == 1)
        strcat((strcat((strcat((strcat(logpath_STR_PathDirectory,"\\")), logpath_str_file_2)), Ext)), "\0");

    else
        strcat((strcat((strcat((strcat(logpath_STR_PathDirectory,"\\")), logpath_str_file_3)), Ext)), "\0");

    return 0;
}
void LoadLog(int LoadLog_INT_TypeOfHistory, int LoadLog_INT_UserID)
{
    FILE* userlog;
    int LoadLog_INT_PreBalance, LoadLog_INT_CauseBalance, LoadLog_INT_NewBalance, LoadLog_INT_TransType, LoadLog_INT_ID;
    char LoadLog_STR_timeNdate[20], LoadLog_STR_pathdirectory[25];
    char LoadLog_STR_textlinedata[1000];

    logpath(LoadLog_STR_pathdirectory, LoadLog_INT_TypeOfHistory, LoadLog_INT_UserID);

    userlog = fopen(LoadLog_STR_pathdirectory, "r");
    if(userlog == NULL)
        userlog = fopen(LoadLog_STR_pathdirectory, "w");

    while((fgets(LoadLog_STR_textlinedata, sizeof(LoadLog_STR_textlinedata), userlog)) != NULL)
    {
        sscanf(LoadLog_STR_textlinedata, "%s %d %d %d %d %d", LoadLog_STR_timeNdate, &LoadLog_INT_PreBalance, &LoadLog_INT_CauseBalance, &LoadLog_INT_NewBalance, &LoadLog_INT_ID, &LoadLog_INT_TransType);
        GetLog(LoadLog_STR_timeNdate, LoadLog_INT_PreBalance, LoadLog_INT_CauseBalance, LoadLog_INT_NewBalance, LoadLog_INT_ID, LoadLog_INT_TransType);
    }

    fclose(userlog);
}
void SaveLog(int SaveLog_INT_TypeOfHistory, int SaveLog_INT_UserID)
{
    FILE* userlog;
    char SaveLog_STR_PathDirectory[25];

    logpath(SaveLog_STR_PathDirectory, SaveLog_INT_TypeOfHistory, SaveLog_INT_UserID);

    userlog = fopen(SaveLog_STR_PathDirectory, "w");

    for(int x = 0; x <= Global_Int_log; x++)
        fprintf(userlog, "%s %d %d %d %d %d\n", ClientLog[x].timeNdate, ClientLog[x].prevBal, ClientLog[x].causeBal, ClientLog[x].newBal, ClientLog[x].UserID, ClientLog[x].type);

    fclose(userlog);
}
void GetLog(char GetLog_STR_TimeNDateINFO[], int GetLog_INT_PreBalance, int GetLog_INT_CauseBalance, int GetLog_INT_NewBalance, int GetLog_INT_UserID, int GetLog_INT_TypeOfTransactionHistory)
{
    Global_Int_log++;

    strcpy(ClientLog[Global_Int_log].timeNdate, GetLog_STR_TimeNDateINFO);
    ClientLog[Global_Int_log].prevBal = GetLog_INT_PreBalance;
    ClientLog[Global_Int_log].causeBal = GetLog_INT_CauseBalance;
    ClientLog[Global_Int_log].newBal = GetLog_INT_NewBalance;
    ClientLog[Global_Int_log].UserID = GetLog_INT_UserID;
    ClientLog[Global_Int_log].type = GetLog_INT_TypeOfTransactionHistory;
}
const char* GetTime_Date(char GetTime_Date_STRptr_TimeNDateInfo[])
{
char GetTime_Date_temp[25];

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    int n = snprintf(GetTime_Date_temp,sizeof(GetTime_Date_temp),"[%d:%d_%d/%d]", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_mday, timeinfo->tm_mon);

    if(n >= 0 && n < sizeof(GetTime_Date_temp))
        strcpy(GetTime_Date_STRptr_TimeNDateInfo, GetTime_Date_temp);

    else
        GetTime_Date(GetTime_Date_STRptr_TimeNDateInfo);

    return 0;
}
void Initializelog()
{
    Global_Int_log = -1;
}
void Delay(float Delay_Float_Time)
{
    int Delay_Int_milli_seconds = 1000 * Delay_Float_Time;

    clock_t start_time = clock();

    while (clock() < start_time + Delay_Int_milli_seconds) ;
}
void gotoxy(int positionX, int positionY)
{
    COORD coordinate;

    coordinate.X = positionX;
    coordinate.Y = positionY;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coordinate);
}
void Transition(int Transition_INT_TypeOfTransaction)
{
    system("cls");

    if(Transition_INT_TypeOfTransaction == 0)
    {
        gotoxy(45, 9);
        printf("WITHDRAWING...");
    }
    else if(Transition_INT_TypeOfTransaction == 1)
    {
        gotoxy(45, 9);
        printf("DEPOSITING...");
    }
    else if(Transition_INT_TypeOfTransaction == 2)
    {
        gotoxy(45, 9);
        printf("TRANSEFFERING...");
    }
    Delay(2);
}
void frontpage()
{
    gotoxy(25, 4);  printf("=======================================================================");
    gotoxy(25, 5);  printf("=======================================================================");
    gotoxy(25, 6);  printf("=======================================================================");
    gotoxy(25, 7);  printf("=========#####       ###############    #########           ########===");
    gotoxy(25, 8);  printf("========### ###      ###############    ##########         #########===");
    gotoxy(25, 9);  printf("=======###   ###          ####          ###########       ##########===");
    gotoxy(25, 10); printf("======###     ###         ####          ######  ###########   ######===");
    gotoxy(25, 11); printf("=====#############        ####          ######   #########    ######===");
    gotoxy(25, 12); printf("====###         ###       ####          ######                ######===");
    gotoxy(25, 13); printf("===###           ###      ####          ######                ######===");
    gotoxy(25, 14); printf("=======================================================================");
    gotoxy(25, 15); printf("=======================================================================");
    gotoxy(25, 16); printf("=======================================================================");
    gotoxy(20, 25); printf("Created By:");
    gotoxy(20, 26); printf("Omar Gerardo");
    gotoxy(20, 27); printf("Cristian Bien Armedilla");
    Delay(3);
}
void HideCursor()
{
    HANDLE ConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(ConsoleHandle, &info);

}












