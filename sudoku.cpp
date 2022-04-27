#include <random>
#include <tuple>
#include <time.h>
#include <iostream>
#include <conio.h>
#include <cstring>
#include <stack>
using namespace std;

#define RB "\x1B[31m"
#define RE "\033[0m"

#define GB "\x1B[32m"
#define GE "\033[0m"

#define YB "\x1B[33m"
#define YE "\033[0m"

#define BB "\x1B[34m"
#define BE "\033[0m"

class Random
{
private:
    float randomizer = 0.01;
    bool devActive = true;

public:
    tuple<int, int, int> triple(int a, int b)
    {
        const int x1 = this->unique(a, b);
        const int x2 = this->unique(a, b);
        const int x3 = this->unique(a, b);
        return tuple<int, int, int>(x1, x2, x3);
    }

    int unique(int a, int b)
    {
        srand(time(NULL) * this->randomizer);
        this->randomizer += 0.001;
        return rand() % b + a;
    }
};

class SudokuTable
{
private:
    int pos[9][9];
    Random RandomGenerator;
    bool devActive;
    int solution[9][9];

public:
    stack<pair<int, int>> input;

    SudokuTable()
    {
        this->resetTable();
        this->devActive = false;
        this->insertRandomValues(this->RandomGenerator.unique(1, 10));
        this->solve();
        this->devActive = true;
        this->saveSolution();
        this->extractValues(this->RandomGenerator.unique(30, 50));
    }

    bool isFull()
    {
        int i, j;
        return !findEmpty(i, j);
    }

    void showElement(int iPos, int jPos, char *color)
    {
        if (strcmp(color, "red") == 0)
            cout << RB << this->pos[iPos][jPos] << RE;
        else if (strcmp(color, "green") == 0)
            cout << GB << this->pos[iPos][jPos] << GE;
        else if (strcmp(color, "blue") == 0)
            cout << BB << this->pos[iPos][jPos] << BE;
        else
            cout << this->pos[iPos][jPos];
    }

    void showElement(int iPos, int jPos)
    {
        if (this->pos[iPos][jPos] == 0)
            this->showElement(iPos, jPos, (char *)"green");
        else
            this->showElement(iPos, jPos, (char *)"blue");
    }

    void saveSolution()
    {
        for (int i = 0; i < 9; i++)
            for (int j = 0; j < 9; j++)
                this->solution[i][j] = this->pos[i][j];
    }

    void retrieveSolution()
    {
        for (int i = 0; i < 9; i++)
            for (int j = 0; j < 9; j++)
                this->pos[i][j] = this->solution[i][j];
    }

    void insertRandomValues(int valNum)
    {
        while (valNum)
        {
            int iPos, jPos, value;
            tie(iPos, jPos, value) = this->RandomGenerator.triple(0, 8);
            value += 1; // Value must be between 1 and 9
            if (this->insertValue(iPos, jPos, value))
                valNum--;
        }
    }

    void resetValue(int iPos, int jPos)
    {
        this->pos[iPos][jPos] = 0;
    }

    bool isEmpty(int iPos, int jPos)
    {
        return this->pos[iPos][jPos] == 0;
    }

    void extractValues(int num)
    {
        int iPos, jPos;
        for (int i = 0; i < num; i++)
        {
            iPos = this->RandomGenerator.unique(0, 8);
            jPos = this->RandomGenerator.unique(0, 8);
            if (!this->isEmpty(iPos, jPos))
                this->resetValue(iPos, jPos);
        }
    }

    bool solve()
    {
        int row, col;

        if (!this->findEmpty(row, col))
            return true;
        for (int num = 1; num <= 9; num++)
        {

            if (this->verify(row, col, num))
            {

                this->pos[row][col] = num;

                if (this->solve())
                    return true;

                this->pos[row][col] = 0;
            }
        }

        return false;
    }

    bool findEmpty(int &iPos, int &jPos)
    {
        for (iPos = 0; iPos < 9; iPos++)
            for (jPos = 0; jPos < 9; jPos++)
                if (this->pos[iPos][jPos] == 0)
                    return true;
        return false;
    }

    bool verify(int iPos, int jPos, int value)
    {
        return this->verifyColumnAndLine(iPos, jPos, value) && this->verifySquare(iPos, jPos, value) && this->isEmpty(iPos, jPos);
    }

    void resetTable()
    {
        for (int i = 0; i < 9; i++)
            for (int j = 0; j < 9; j++)
                this->pos[i][j] = 0;
    }

    bool insertValue(int iPos, int jPos, int value)
    {
        if (iPos == -1)
            return false;
        if (iPos == -2)
        {
            this->retrieveSolution();
            return false;
        }
        if (iPos == -3)
            return false;
        if (!this->verifyColumnAndLine(iPos, jPos, value))
            return false;
        if (!this->verifySquare(iPos, jPos, value))
            return false;

        this->pos[iPos][jPos] = value;
        return true;
    }

    bool verifySquare(int iPos, int jPos, int value)
    {
        // Verify Square
        int iVerify = (iPos / 3) * 3;
        int jVerify = (jPos / 3) * 3;
        for (int i = iVerify; i <= iVerify + 2; i++)
            for (int j = jVerify; j <= jVerify + 2; j++)
                if (this->pos[i][j] == value && i != iPos && j != jPos)
                {
                    if (this->devActive)
                    {
                        cout << RB << "Patratul este invalid!" << RE << endl;
                        getch();
                    }
                    return false;
                }
        return true;
    }

    bool verifyColumnAndLine(int iPos, int jPos, int value)
    {
        // Verify Column and Line
        for (int z = 0; z < 9; z++)
            if (value == this->pos[iPos][z] && z != jPos || value == this->pos[z][jPos] && z != iPos)
            {
                if (this->devActive)
                {
                    if (this->pos[iPos][z] == value)
                        cout << RB << "Linia este invalida!" << RE << endl;
                    if (this->pos[z][jPos] == value)
                        cout << RB << "Coloana este invalida!" << RE << endl;
                    getch();
                }
                return false;
            }
        return true;
    }

    void show()
    {
        int row = 1;
        cout << RB << "\t\t\t\t\t\t1 2 3   4 5 6   7 8 9" << RE;
        cout << "\n\n\n";
        for (int i = 0; i < 9; i++)
        {
            cout << "\t\t\t\t\t" << YB << row++ << YE << "\t";
            for (int j = 0; j < 9; j++)
                if ((j + 1) % 3 == 0)
                {
                    this->showElement(i, j);
                    cout << "   ";
                }
                else
                {
                    this->showElement(i, j);
                    cout << " ";
                }
            if ((i + 1) % 3 == 0)
                cout << "\n";
            cout << endl;
        }
    }
};

class Controller
{
public:
    Controller()
    {
    }

    void awaitInput()
    {
        cout << "Apasati orice buton pentru a continua...";
        getch();
    }

    void clearScreen()
    {
        system("cls");
    }

    tuple<int, int, int> insertThree()
    {
        int iPos, jPos, value;
        cout << YB << "Linia: " << YE;
        cin >> iPos;
        if (iPos == -1)
            return tuple<int, int, int>(-1, 0, 0);
        if (iPos == -2)
            return tuple<int, int, int>(-2, 0, 0);
        if (iPos == -3)
            return tuple<int, int, int>(-3, 0, 0);
        cout << RB << "Coloana: " << RE;
        cin >> jPos;
        cout << BB << "Valoarea: " << BE;
        cin >> value;
        iPos--;
        jPos--;
        return tuple<int, int, int>(iPos, jPos, value);
    }
};

int main()
{
    SudokuTable Table;
    Controller Handler;
    int iPos, jPos, value;
    Handler.awaitInput();
    do
    {
        Handler.clearScreen();
        Table.show();
        tie(iPos, jPos, value) = Handler.insertThree();
        if (Table.insertValue(iPos, jPos, value))
            Table.input.push(pair<int, int>(iPos, jPos));
        if (iPos == -2)
        {
            Handler.clearScreen();
            Table.show();
            Handler.awaitInput();
            Handler.clearScreen();
            break;
        }
        if (iPos == -3)
        {
            if (Table.input.empty())
                continue;
            pair<int, int> position = Table.input.top();
            Table.input.pop();
            Table.resetValue(position.first, position.second);
        }
        if (Table.isFull())
        {
            cout << "The game is complete!\n";
            Handler.awaitInput();
            break;
        }
    } while (iPos != -1);
    return 0;
}