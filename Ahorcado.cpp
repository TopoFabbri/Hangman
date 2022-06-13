#include <iostream>
#include <Windows.h>
#include <conio.h>

using namespace std;

#pragma region Global Variables
enum MODES
{
    Exit,
    Play,
    Settings,
    MainMenu,
};

enum CHARN
{
    upLeftC,
    // ╔
    hor,
    // ═
    upT,
    // ╦
    upRightC,
    // ╗
    ver,
    // ║
    leftT,
    // ╠
    cross,
    // ╬
    rightT,
    // ╣
    lowLeftC,
    // ╚
    lowT,
    // ╩
    lowRightC,
    // ╝
};


struct PASSWORD
{
    char text;
    bool show = false;
};

char wall[11] =
{
    (char)201, (char)205, (char)203, (char)187, (char)186, (char)204,
    (char)206, (char)185, (char)200, (char)202, (char)188
};

enum COLORS
{
    BlueOnBlack = 9,
    GreenOnBlack = 2,
    RedOnBlack = 4,
    PurpleOnBlack = 5,
    YellowOnBlack = 6,
    CyanOnBlack = 11,
    OrangeOnBlack = 12,
    WhiteOnBlack = 15,
    BlackOnBlue = 16,
    BlackOnGreen = 32,
    BlackOnRed = 64,
    BlackOnPurple = 80,
    WhiteOnPurple = 87,
    BlackOnYellow = 96,
    BlackOnWhite = 240,
    BlueOnWhite = 241,
    GreenOnWhite = 242,
    CyanOnWhite = 243,
    PurpleOnWhite = 245,
    YellowOnWhite = 246,
    OrangeOnWhite = 252,
};

HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);

MODES Mode = MainMenu;

const int maxChars = 50;
PASSWORD pass[maxChars];
bool caseSens = false;
char enter = '\r';
char backspace = '\b';
int attQty = 7;

void Defaults();
void RunProgram();
void SceneNavigator();
void GameFlow();
void PrintHangman(int attNum);
void PrintPass(int attNum);
bool CheckLetter(char letter[], int passLength);
bool CheckWin(int passLength);
void CheckWord(char word[], int passLength);
void Attempts(int attQty, int passLength, char atts[]);
void AskPass(int& passLength);
void ResetAtts(char atts[]);
void ResetPass(PASSWORD pass[]);
void PrintAtts(int attNum, char atts[]);
void Menu();
void SettingsMenu();
#pragma endregion;

int main()
{
    RunProgram();
}

void RunProgram()
{
    Defaults();
    SceneNavigator();
}

void SceneNavigator()
{
    do
    {
        switch (Mode)
        {
        case Exit:
            break;

        case Play:
            GameFlow();
            break;

        case Settings:
            SettingsMenu();
            break;

        case MainMenu:
            Menu();
            break;

        default:
            break;
        }
    }
    while (Mode != Exit);
}

void Defaults()
{
    ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);
    SetConsoleTitle((L"Hangman"));
    system("color 07");
}

void GameFlow()
{
    char atts[maxChars];
    int passLength = 0;

    int minChar = 97;
    int maxChar = 122;

    ResetPass(pass);
    AskPass(passLength);

    ResetAtts(atts);
    Attempts(attQty, passLength, atts);
}

void AskPass(int& passLength)
{
    system("cls");
    int i = 0;
    // Ask for password
    do
    {
        SetConsoleCursorPosition(hCon, {0, 0});
        cout << "Password:" << endl;
        for (int j = 0; j < i; j++)
        {
            cout << "*";
        }

        char tmp = _getch();
        if (tmp == backspace)
        {
            i--;
            pass[i] = {'\0', false};
        }
        if (tmp == enter)
        {
            passLength = i;
            break;
        }
        if (!caseSens && isupper(tmp))
        {
            tmp = tolower(tmp);
        }
        pass[i].text = tmp;
        i++;
    }
    while (pass[i].text != enter && i < maxChars && passLength < 1);
}

void Attempts(int attQty, int passLength, char atts[])
{
    // Attempts to guess password
    for (int attNum = attQty; attNum > 0; attNum--)
    {
        bool letterFound = false;
        char letter[maxChars];

        for (int i = 0; i < maxChars; ++i)
        {
            letter[i] = '\0';
        }

        system("cls");

        do
        {
            SetConsoleCursorPosition(hCon, {0, 0});
            letterFound = false;

            PrintHangman(attNum);
            PrintPass(passLength);
            PrintAtts(attQty - attNum, atts);

            cin >> letter;

            if (letter[1] == '\0')
            {
                if (CheckLetter(letter, passLength))
                {
                    letterFound = true;
                }
                else
                {
                    cout << "Wrong letter!" << endl;
                }
            }
            else
            {
                CheckWord(letter, passLength);
                if (!CheckWin(passLength))
                {
                    cout << "You lost!" << endl;
                    system("pause");
                }
            }

            if (CheckWin(passLength))
            {
                system("cls");
                PrintHangman(attNum);
                PrintPass(passLength);
                cout << "You won!" << endl;
                system("pause");
                break;
            }
            else
            {
                atts[attQty - attNum] = letter[0];
            }
        }
        while (letterFound && letter[1] == '\0');

        if (letter[1] != '\0')
        {
            break;
        }

        if (CheckWin(passLength))
        {
            break;
        }
    }
    if (!CheckWin(passLength))
    {
        cout << "You lost!" << endl;
        system("pause");
    }
}

void PrintHangman(int attNum)
{
    switch (attNum)
    {
    case 7:
        cout << "                              " << wall[upLeftC] << wall[hor] << wall[hor] << wall[hor] << wall
            [hor] <<
            wall[upRightC] <<
            endl
            << "                              " << wall[ver] << "    " << wall[ver] << endl
            << "                              " << wall[lowT] << "    " << wall[ver] << endl
            << "                                   " << wall[ver] << endl
            << "                                   " << wall[ver] << endl
            << "                                   " << wall[ver] << endl;
        break;

    case 6:
        cout << "                              " << wall[upLeftC] << wall[hor] << wall[hor] << wall[hor] << wall
            [hor] <<
            wall[upRightC] <<
            endl
            << "                              " << wall[ver] << "    " << wall[ver] << endl
            << "                              " << wall[lowT] << "    " << wall[ver] << endl
            << "                              O    " << wall[ver] << endl
            << "                                   " << wall[ver] << endl
            << "                                   " << wall[ver] << endl;
        break;

    case 5:
        cout << "                              " << wall[upLeftC] << wall[hor] << wall[hor] << wall[hor] << wall
            [hor] <<
            wall[upRightC] <<
            endl
            << "                              " << wall[ver] << "    " << wall[ver] << endl
            << "                              " << wall[lowT] << "    " << wall[ver] << endl
            << "                              O    " << wall[ver] << endl
            << "                              |    " << wall[ver] << endl
            << "                                   " << wall[ver] << endl;
        break;

    case 4:
        cout << "                              " << wall[upLeftC] << wall[hor] << wall[hor] << wall[hor] << wall
            [hor] <<
            wall[upRightC] <<
            endl
            << "                              " << wall[ver] << "    " << wall[ver] << endl
            << "                              " << wall[lowT] << "    " << wall[ver] << endl
            << "                              O/   " << wall[ver] << endl
            << "                              |    " << wall[ver] << endl
            << "                                   " << wall[ver] << endl;
        break;

    case 3:
        cout << "                              " << wall[upLeftC] << wall[hor] << wall[hor] << wall[hor] << wall
            [hor] <<
            wall[upRightC] <<
            endl
            << "                              " << wall[ver] << "    " << wall[ver] << endl
            << "                              " << wall[lowT] << "    " << wall[ver] << endl
            << "                             \\O/   " << wall[ver] << endl
            << "                              |    " << wall[ver] << endl
            << "                                   " << wall[ver] << endl;
        break;

    case 2:
        cout << "                              " << wall[upLeftC] << wall[hor] << wall[hor] << wall[hor] << wall
            [hor] <<
            wall[upRightC] <<
            endl
            << "                              " << wall[ver] << "    " << wall[ver] << endl
            << "                              " << wall[lowT] << "    " << wall[ver] << endl
            << "                             \\O/   " << wall[ver] << endl
            << "                              |    " << wall[ver] << endl
            << "                             /     " << wall[ver] << endl;
        break;

    case 1:
        cout << "                              " << wall[upLeftC] << wall[hor] << wall[hor] << wall[hor] << wall
            [hor] <<
            wall[upRightC] <<
            endl
            << "                              " << wall[ver] << "    " << wall[ver] << endl
            << "                              " << wall[lowT] << "    " << wall[ver] << endl
            << "                             \\O/   " << wall[ver] << endl
            << "                              |    " << wall[ver] << endl
            << "                             / \\   " << wall[ver] << endl;
        break;

    case 0:
        cout << "                              " << wall[upLeftC] << wall[hor] << wall[hor] << wall[hor] << wall
            [hor] <<
            wall[upRightC] <<
            endl
            << "                              " << wall[ver] << "    " << wall[ver] << endl
            << "                              " << wall[lowT] << "    " << wall[ver] << endl
            << "                              O    " << wall[ver] << endl
            << "                             /|\\   " << wall[ver] << endl
            << "                              " << wall[ver] << "    " << wall[ver] << endl;
        break;
    }
}

bool CheckLetter(char letter[], int passLength)
{
    bool isInPass = false;

    for (int i = 0; i < passLength; i++)
    {
        if (letter[0] == pass[i].text)
        {
            pass[i].show = true;
            isInPass = true;
        }
    }
    return isInPass;
}

void CheckWord(char word[], int passLength)
{
    for (int i = 0; i < passLength; i++)
    {
        if (word[i] == pass[i].text)
        {
            pass[i].show = true;
        }
    }

    int count = 0;
    for (int i = 0; i < passLength; i++)
    {
        if (pass[i].show)
        {
            count++;
        }
    }
}

bool CheckWin(int passLength)
{
    for (int i = 0; i < passLength; i++)
    {
        if (!pass[i].show)
        {
            return false;
        }
    }
    return true;
}

void PrintPass(int passLength)
{
    for (int pos = 0; pos < ((17 - passLength) * 2); pos++)
    {
        cout << " ";
    }
    for (int pos = 0; pos < passLength; pos++)
    {
        if (pass[pos].show)
        {
            cout << pass[pos].text;
        }
        else
        {
            cout << "_";
        }
        cout << " ";
    }
    cout << wall[hor] << wall[lowT] << wall[hor] << endl;
}

void PrintAtts(int attNum, char atts[])
{
    cout << endl;
    for (int i = 0; i < attNum; i++)
    {
        cout << (i > 0 ? " - " : " ") << atts[i];
    }
}

void ResetAtts(char atts[])
{
    for (int i = 0; i < maxChars; ++i)
    {
        atts[i] = '\0';
    }
    return;
}

void ResetPass(PASSWORD pass[])
{
    for (int i = 0; i < maxChars; ++i)
    {
        pass[i].show = false;
        pass[i].text = '\0';
    }
    return;
}

void Menu()
{
    char ans;

    system("cls");
    SetConsoleTextAttribute(hCon, BlackOnWhite);
    cout << "                                                                         " << endl
        << "                                 M E N U                                 " << endl
        << "                                                                         " << endl << endl;
    SetConsoleTextAttribute(hCon, WhiteOnBlack);

    cout << "0: Exit application" << endl
        << "1: Play" << endl
        << "2: Options" << endl;

    do
    {
        ans = _getch();
        ans -= 48;
    }
    while (ans > 2 || ans < 0);

    Mode = (MODES)ans;
}

void SettingsMenu()
{
    enum OPTIONS
    {
        Back,
        EnterGame,
        Attempts,
    };
    OPTIONS Ops;
    do
    {
        SetConsoleCursorPosition(hCon, { 0, 0 });
        SetConsoleTextAttribute(hCon, BlackOnWhite);
        cout << "                                                                         " << endl
            << "                             S E T T I N G S                             " << endl
            << "                                                                         " << endl << endl;
        SetConsoleTextAttribute(hCon, WhiteOnBlack);

        cout << "0: Back to menu" << endl
            << "1: Play" << endl
            << "2: Attempts      Default: 7  Current: " << attQty << endl;

        char ans = _getch();
        ans -= 48;
        Ops = (OPTIONS)ans;

        switch (Ops)
        {
        case EnterGame:
            Mode = Play;
            return;
            break;

        case Attempts:
            cout << "Enter number of attempts(1 - 7): ";
            do
            {
                cin >> attQty;
            }
            while (attQty < 1 || attQty > 7);
            system("cls");
            break;

        case Back:
            break;

        default:
            break;
        }
    }while (Ops != Back);
}