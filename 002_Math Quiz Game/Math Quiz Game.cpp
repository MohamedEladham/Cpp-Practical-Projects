#include <iostream>
#include <cstdlib>
using namespace std;

enum enQuestionsLevel { EasyLevel = 1, MedLevel = 2, HardLevel = 3, Mix = 4 };
enum enOperationType { Add = 1, Sub = 2, Mult = 3, Div = 4, MixOp = 5 };

struct stQuestions
{
    short Number1;
    short Number2;
    enQuestionsLevel QuestionLevel;
    enOperationType OperationType;
    short CorrectAnswer;
    short PlayerAnswer;
    bool AnswerResult = false;
};

struct stQuiz
{
    stQuestions QuestionList[100];
    short NumberOfQuestion;
    enQuestionsLevel QuestionLevel;
    enOperationType OpType;
    short NumberOfRightAnswer = 0;
    short NumberOfWrongAnswer = 0;
    bool IsPass = false;
};

int RandomNumber(int From, int To)
{
    int RandNum = rand() % (To - From + 1) + From;
    return RandNum;
}

short ReadNumberOfQuestions()
{
    short NumberOfQuestions;

    do
    {
        cout << "\nHow Many Questions Do You Want To Answer From 1 To 10: ";
        cin >> NumberOfQuestions;

    } while (NumberOfQuestions < 1 || NumberOfQuestions > 10);

    return NumberOfQuestions;
}

enQuestionsLevel ReadQuestionsLevel()
{
    short QuestionLevel;

    do
    {
        cout << "\nEnter Questions Level: [1]Easy, [2]Med, [3]Hard, [4]Mix: ";
        cin >> QuestionLevel;

    } while (QuestionLevel < 1 || QuestionLevel > 4);

    return enQuestionsLevel(QuestionLevel);
}

enOperationType ReadOperationType()
{
    short OpType;

    do
    {
        cout << "\nEnter Operation Type: [1]Add, [2]Sub, [3]Mul, [4]Div, [5]Mix: ";
        cin >> OpType;

    } while (OpType < 1 || OpType > 5);

    return enOperationType(OpType);
}

int SimpleCalculator(short Num1, short Num2, enOperationType OpType)
{
    switch (OpType)
    {
    case enOperationType::Add:
        return Num1 + Num2;

    case enOperationType::Sub:
        return Num1 - Num2;

    case enOperationType::Mult:
        return Num1 * Num2;

    case enOperationType::Div:
        return Num1 / Num2;

    default:
        return 0;
    }
}

stQuestions GenerateOneQuestion(enOperationType OperationType, enQuestionsLevel QuestionsLevel)
{
    stQuestions Question;

    if (QuestionsLevel == enQuestionsLevel::Mix)
    {
        QuestionsLevel = (enQuestionsLevel)RandomNumber(1, 3);
    }

    if (OperationType == enOperationType::MixOp)
    {
        OperationType = (enOperationType)RandomNumber(1, 4);
    }

    Question.QuestionLevel = QuestionsLevel;
    Question.OperationType = OperationType;

    switch (QuestionsLevel)
    {
    case enQuestionsLevel::EasyLevel:
        Question.Number1 = RandomNumber(1, 10);
        Question.Number2 = RandomNumber(1, 10);
        break;

    case enQuestionsLevel::MedLevel:
        Question.Number1 = RandomNumber(10, 50);
        Question.Number2 = RandomNumber(10, 50);
        break;

    case enQuestionsLevel::HardLevel:
        Question.Number1 = RandomNumber(50, 100);
        Question.Number2 = RandomNumber(50, 100);
        break;
    }

    Question.CorrectAnswer = SimpleCalculator(Question.Number1, Question.Number2, Question.OperationType);

    return Question;
}

void GenerateQuizQuestions(stQuiz& Quiz)
{
    for (short Question = 0; Question < Quiz.NumberOfQuestion; Question++)
    {
        Quiz.QuestionList[Question] = GenerateOneQuestion(Quiz.OpType, Quiz.QuestionLevel);
    }
}

string GetOperationTypeText(enOperationType OpType)
{
    switch (OpType)
    {
    case enOperationType::Add:
        return "+";

    case enOperationType::Sub:
        return "-";

    case enOperationType::Mult:
        return "x";

    case enOperationType::Div:
        return "/";

    default:
        return "Mix";
    }
}

void PrintQuestion(stQuiz& Quiz, short QuestionNumber)
{
    cout << "\n\n";
    cout << "Question [" << QuestionNumber + 1 << "/" << Quiz.NumberOfQuestion << "] \n\n";
    cout << Quiz.QuestionList[QuestionNumber].Number1 << endl;
    cout << Quiz.QuestionList[QuestionNumber].Number2 << " ";
    cout << GetOperationTypeText(Quiz.QuestionList[QuestionNumber].OperationType) << "\n";
    cout << "_______________\n";
}

short ReadPlayerAnswer()
{
    short Answer;
    cin >> Answer;
    return Answer;
}

string GetAnswerResultText(bool IsCorrect)
{
    if (IsCorrect)
        return "Right Answer :-)";
    else
        return "Wrong Answer :-(";
}

void SetScreenColor(bool IsPass)
{
    if (IsPass)
    {
        system("Color 2F");
    }
    else
    {
        cout << "\a";
        system("Color 4F");
    }
}

void CheckQuestionAnswer(stQuiz& Quiz, short QuestionNumber)
{
    if (Quiz.QuestionList[QuestionNumber].PlayerAnswer != Quiz.QuestionList[QuestionNumber].CorrectAnswer)
    {
        Quiz.QuestionList[QuestionNumber].AnswerResult = false;
        Quiz.NumberOfWrongAnswer++;

        cout << GetAnswerResultText(false) << endl;
        cout << "The Right Answer Is: ";
        cout << Quiz.QuestionList[QuestionNumber].CorrectAnswer << endl;
    }
    else
    {
        Quiz.QuestionList[QuestionNumber].AnswerResult = true;
        Quiz.NumberOfRightAnswer++;

        cout << GetAnswerResultText(true) << endl;
    }

    SetScreenColor(Quiz.QuestionList[QuestionNumber].AnswerResult);
}

void AskQuestionsAndCheckAnswers(stQuiz& Quiz)
{
    for (short QuestionNumber = 0; QuestionNumber < Quiz.NumberOfQuestion; QuestionNumber++)
    {
        PrintQuestion(Quiz, QuestionNumber);
        Quiz.QuestionList[QuestionNumber].PlayerAnswer = ReadPlayerAnswer();
        CheckQuestionAnswer(Quiz, QuestionNumber);
    }

    Quiz.IsPass = (Quiz.NumberOfRightAnswer >= Quiz.NumberOfWrongAnswer);
}

string GetFinalResultText(bool IsPass)
{
    if (IsPass)
        return "PASS :-)";
    else
        return "Fail :-(";
}

string GetQuestionLevelText(enQuestionsLevel QuestionsLevel)
{
    string ArrQuestionLevel[4] = { "Easy", "Med", "Hard", "Mix" };
    return ArrQuestionLevel[QuestionsLevel - 1];
}

void PrintQuizResults(stQuiz Quiz)
{
    cout << "\n\n";
    cout << "_______________________________________\n\n";
    cout << " Final Results Is " << GetFinalResultText(Quiz.IsPass) << endl;
    cout << "_______________________________________\n\n";
    cout << "Number Of Questions: " << Quiz.NumberOfQuestion << endl;
    cout << "Questions Level    : " << GetQuestionLevelText(Quiz.QuestionLevel) << endl;
    cout << "Op Type            : " << GetOperationTypeText(Quiz.OpType) << endl;
    cout << "Number Of Right Answer: " << Quiz.NumberOfRightAnswer << endl;
    cout << "Number Of Wrong Answer: " << Quiz.NumberOfWrongAnswer << endl;
    cout << "_______________________________________\n";

    SetScreenColor(Quiz.IsPass);
}

void PlayMathGame()
{
    stQuiz Quiz;

    Quiz.NumberOfQuestion = ReadNumberOfQuestions();
    Quiz.QuestionLevel = ReadQuestionsLevel();
    Quiz.OpType = ReadOperationType();

    GenerateQuizQuestions(Quiz);
    AskQuestionsAndCheckAnswers(Quiz);
    PrintQuizResults(Quiz);
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
        PlayMathGame();

        cout << "\n> Do You Want Play Again Y/N: ";
        cin >> PlayAgain;

    } while (PlayAgain == 'y' || PlayAgain == 'Y');
}

int main()
{
    srand((unsigned)time(0));
    StartGame();
    return 0;
}