#include <iostream>
#include <cstdlib>
using namespace std;

enum enGameChoice { Stone = 1, Paper = 2, Scissors = 3 };
enum enWinner { Player1 = 1, Computer = 2, Draw = 3 };

struct stRoundInfo
{
    short RoundNumber = 0;
    enGameChoice Player1Choice;
    enGameChoice ComputerChoice;
    enWinner Winner;
    string WinnerName = "";
};

struct stGameResults
{
    short GameRounds = 0;
    short Player1WinTimes = 0;
    short ComputerWinTimes = 0;
    short DrawTimes = 0;
    enWinner GameWinner;
    string WinnerName = "";
};

int RandomNumber(int From, int To)
{
    int RandomNum = rand() % (To - From + 1) + From;
    return RandomNum;
}

short HowManyRounds()
{
    short NumberOfRounds = 1;

    do
    {
        cout << "\nHow Many Rounds (From 1 To 10): ";
        cin >> NumberOfRounds;

    } while (NumberOfRounds < 1 || NumberOfRounds > 10);

    return NumberOfRounds;
}

enGameChoice ReadPlayer1Choice()
{
    short Player1Choice = 1;

    do
    {
        cout << "\nYour Choice: [1]:Stone, [2]:Paper, [3]:Scissors: ";
        cin >> Player1Choice;

    } while (Player1Choice < 1 || Player1Choice > 3);

    return enGameChoice(Player1Choice);
}

enGameChoice GetComputerChoice()
{
    return enGameChoice(RandomNumber(1, 3));
}

string ChoiceName(enGameChoice GameChoice)
{
    string ArrGameChoice[3] = { "Stone", "Paper", "Scissors" };
    return ArrGameChoice[GameChoice - 1];
}

string WinnerName(enWinner Winner)
{
    string ArrWinnerName[3] = { "Player1", "Computer", "No Winner" };
    return ArrWinnerName[Winner - 1];
}

enWinner WhoWonTheRound(stRoundInfo RoundInfo)
{
    if (RoundInfo.Player1Choice == RoundInfo.ComputerChoice)
    {
        return enWinner::Draw;
    }

    switch (RoundInfo.Player1Choice)
    {
    case enGameChoice::Paper:
        if (RoundInfo.ComputerChoice == enGameChoice::Scissors)
            return enWinner::Computer;
        break;

    case enGameChoice::Scissors:
        if (RoundInfo.ComputerChoice == enGameChoice::Stone)
            return enWinner::Computer;
        break;

    case enGameChoice::Stone:
        if (RoundInfo.ComputerChoice == enGameChoice::Paper)
            return enWinner::Computer;
        break;
    }

    return enWinner::Player1;
}

enWinner WhoWonTheGame(short Player1WinTimes, short ComputerWinTimes)
{
    if (Player1WinTimes > ComputerWinTimes)
        return enWinner::Player1;
    else if (ComputerWinTimes > Player1WinTimes)
        return enWinner::Computer;
    else
        return enWinner::Draw;
}

void SetWinnerScreenColor(enWinner Winner)
{
    switch (Winner)
    {
    case enWinner::Player1:
        system("Color 2F");
        break;

    case enWinner::Computer:
        system("Color 4F");
        cout << "\a";
        break;

    case enWinner::Draw:
        system("Color 6F");
        break;
    }
}

void PrintRoundResult(stRoundInfo RoundInfo)
{
    cout << "\n__________Round [" << RoundInfo.RoundNumber << "] ______________\n\n";
    cout << "Player1  Choice: " << ChoiceName(RoundInfo.Player1Choice) << endl;
    cout << "Computer Choice: " << ChoiceName(RoundInfo.ComputerChoice) << endl;
    cout << "Round Winner   : " << RoundInfo.WinnerName << endl;
    cout << "__________________________________\n\n";

    SetWinnerScreenColor(RoundInfo.Winner);
}

string Tabs(short NumberOfTabs)
{
    string T = "";

    for (short i = 1; i < NumberOfTabs; i++)
    {
        T += "\t";
    }

    return T;
}

stGameResults FillGameResults(short GameRounds, short Player1WinTimes, short ComputerWinTimes, short DrawTimes)
{
    stGameResults GameResults;

    GameResults.GameRounds = GameRounds;
    GameResults.Player1WinTimes = Player1WinTimes;
    GameResults.ComputerWinTimes = ComputerWinTimes;
    GameResults.DrawTimes = DrawTimes;
    GameResults.GameWinner = WhoWonTheGame(Player1WinTimes, ComputerWinTimes);
    GameResults.WinnerName = WinnerName(GameResults.GameWinner);

    return GameResults;
}

void ShowGameOverScreen()
{
    cout << Tabs(2) << "________________________________________________________\n\n";

    cout << Tabs(2) << "                 +++ G a m e O v e r +++                  \n";

    cout << Tabs(2) << "________________________________________________________\n\n";
}

void ShowGameResultsScreen(stGameResults GameResults)
{
    cout << Tabs(2) << "______________________[Game Results ]___________________\n\n";
    cout << Tabs(2) << "Game Rounds         : " << GameResults.GameRounds << endl;
    cout << Tabs(2) << "Player1 Win Times   : " << GameResults.Player1WinTimes << endl;
    cout << Tabs(2) << "Computer Win Times  : " << GameResults.ComputerWinTimes << endl;
    cout << Tabs(2) << "Draw Times          : " << GameResults.DrawTimes << endl;
    cout << Tabs(2) << "Final Winner        : " << GameResults.WinnerName << endl;
    cout << Tabs(2) << "________________________________________________________\n";

    SetWinnerScreenColor(GameResults.GameWinner);
}

stGameResults PlayRounds(short HowManyRounds)
{
    stRoundInfo RoundInfo;
    short Player1WinTimes = 0, ComputerWinTimes = 0, DrawTimes = 0;

    for (short GameRound = 1; GameRound <= HowManyRounds; GameRound++)
    {
        cout << "\n\nRound [" << GameRound << "] Begins: \n";

        RoundInfo.RoundNumber = GameRound;
        RoundInfo.Player1Choice = ReadPlayer1Choice();
        RoundInfo.ComputerChoice = GetComputerChoice();
        RoundInfo.Winner = WhoWonTheRound(RoundInfo);
        RoundInfo.WinnerName = WinnerName(RoundInfo.Winner);

        if (RoundInfo.Winner == enWinner::Player1)
            Player1WinTimes++;
        else if (RoundInfo.Winner == enWinner::Computer)
            ComputerWinTimes++;
        else
            DrawTimes++;

        PrintRoundResult(RoundInfo);
    }

    return FillGameResults(HowManyRounds, Player1WinTimes, ComputerWinTimes, DrawTimes);
}

void ResetScreen()
{
    system("cls");
    system("Color 0F");
}

void StartGame()
{
    char PlayAgain = 'Y';

    do
    {
        ResetScreen();

        stGameResults GameResults = PlayRounds(HowManyRounds());

        ShowGameOverScreen();
        ShowGameResultsScreen(GameResults);

        cout << Tabs(2) << "Do You Want To Play Again: (Y/N) ";
        cin >> PlayAgain;

    } while (PlayAgain == 'y' || PlayAgain == 'Y');
}

int main()
{
    srand((unsigned)time(0));

    StartGame();

    return 0;
}