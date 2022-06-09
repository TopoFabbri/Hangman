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
    (char)201, (char)205, (char)203, (char)187, (char)186, (char)204, (char)206, (char)185, (char)200, (char)202,
    (char)188
};

MODES Mode = MainMenu;

const int maxChars = 50;
PASSWORD pass[maxChars];
bool caseSens = false;
char enter = '\r';
char backspace = '\b';

void Defaults();
void RunProgram();
void Menu();
void GameFlow();
void PrintHangman(int attNum);
void PrintPass(int attNum);
bool CheckLetter(char letter[], int passLength);
void CheckWord(char word[], int passLength);
bool CheckWin(int passLength);
void Attempts(int attQty, int passLength, char atts[]);
void AskPass(int& passLength);
void ResetLetters();
void ResetPass();
void ResetAttempts();
void PrintAtts(int attNum, char atts[]);

#pragma endregion;

int main()
{
    RunProgram();
    return 0;
}

void RunProgram()
{
    Defaults();
    Menu();
    return;
}

void Menu()
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
            break;

        case MainMenu:
            Mode = Play;
            break;

        default:
            break;
        }
    }
    while (Mode != Exit);

    return;
}

void Defaults()
{
    ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);
    SetConsoleTitle((L"Hangman"));
    system("color 07");
    return;
}

void GameFlow()
{
    char atts[maxChars];
    int attQty = 7;
    int passLength = 0;

    int minChar = 97;
    int maxChar = 122;

    AskPass(passLength);

    for (int i = 0; i < maxChars; i++)
    {
        atts[i] = ' ';
        pass[i].show = false;
    }

    Attempts(attQty, passLength, atts);
    return;
}

void AskPass(int& passLength)
{
    // Ask for password
    for (int i = 0; i < maxChars; i++)
    {
        system("cls");
        cout << "Password:" << endl;
        for (int j = 0; j < i; j++)
        {
            cout << "*";
        }

        char tmp = _getch();
        if (!caseSens && tmp <= 65 && tmp >= 97)
        {
            tmp += 32;
        }
        pass[i].text = tmp;

        if (pass[i].text == enter)
        {
            passLength = i;
            break;
        }
    }
}

void Attempts(int attQty, int passLength, char atts[])
{
    int attNum = 0;

    // Attempts to guess password
    for (int attNum = attQty; attNum >= 0; attNum--)
    {
        char letter[maxChars];
        bool letterFound = false;

        do
        {
            system("cls");

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

        if (CheckWin(passLength))
        {
            break;
        }
    }
    return;
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
