#include <bits/stdc++.h>

using namespace std;


pair<int,int> getDifficultyLevel()
{
    int flag=0;
    int gridSize;
    int numberOfMines;
    while(flag==0)
    {
        cout << "Select Difficulty Level : " << endl << "1. Easy" << endl << "2. Medium" << endl << "3. Hard" << endl;
        string difficultyLevel ;
        cin>>difficultyLevel;
        //if(difficultyLevel==0) continue;
        if(difficultyLevel=="1")
        {
            flag=1;
            gridSize=9;
            numberOfMines=10;
        }
        else if(difficultyLevel=="2")
        {
            flag=1;
            gridSize=16;
            numberOfMines=40;
        }
        else if(difficultyLevel=="3")
        {
            flag=1;
            gridSize=24;
            numberOfMines=99;
        }
        else
        {
            cout << "Enter either 1,2 or 3" << endl;

        }
    }
    return {gridSize,numberOfMines};
}

void displayBoard(vector<vector<char>> board)
{
    for(int i=0;i<board.size();i++)
    {
        for(int j=0;j<board.size();j++)
        {
            cout << board[i][j] << " ";
        }
        cout << endl;
    }
    cout<<endl;
}

void assignNumberofMines(vector<vector<char>>& board)
{
    vector<int> dx = {-1, 0, 1, 0, -1, -1, 1, 1};
    vector<int> dy = {0, 1, 0, -1, -1, 1, 1, -1};
    int n = board.size();
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<n;j++)
        {
            int totalMines=0;
            if(board[i][j]!='$')
            {
                for(int k=0;k<8;k++)
                {
                    int adjI = i+dx[k];
                    int adjJ = j+dy[k];
                    if(adjI>=0 && adjI<n && adjJ>=0 && adjJ<n && board[adjI][adjJ]=='$')
                    {
                        totalMines++;
                    }
                }
            }
            if(board[i][j]!='$')
            board[i][j]=(char)totalMines + '0';
        }
    }
}

void makeBoard(vector<vector<char>>& systemBoard, int gridSize, int numberOfMines)
{
    set<pair<int,int>> checker;
    for(int i=0;i<numberOfMines;i++)
    {
        int mineX = rand()%gridSize;
        int mineY = rand()%gridSize;
        if(checker.find({mineX,mineY})!=checker.end())
        {
            i--;
            continue;
        }
        else
        {
            checker.insert({mineX,mineY});
            systemBoard[mineX][mineY]='$';
        }
    }
    assignNumberofMines(systemBoard);
    return;
}

int checkAdj(vector<vector<char>>& board, int x, int y, int gridSize)
{
    int dx[] = {-1, 0, 1, 0, -1, 1, -1, 1};
    int dy[] = {0, 1, 0, -1, -1, 1, 1, -1};
    int cnt=0;

    for(int i=0; i<8; i++)
    {
        int xAdj = x + dx[i];
        int yAdj = y + dy[i];

        if(xAdj>=0 && yAdj>=0 && xAdj<gridSize && yAdj<gridSize && board[xAdj][yAdj]=='*')
        {
            cnt++;
        }
    }
    cout<<cnt<<endl;
    return cnt;
}

bool ifCorrectFlags(vector<vector<char>>& userBoard, vector<vector<char>>& systemBoard, int x, int y, int gridSize)
{
    int dx[] = {-1, 0, 1, 0, -1, 1, -1, 1};
    int dy[] = {0, 1, 0, -1, -1, 1, 1, -1};

    for(int i=0; i<8; i++)
    {
        int xAdj=x+dx[i];
        int yAdj = y+dy[i];
        if(xAdj<0 || yAdj<0 || xAdj>=gridSize || yAdj>=gridSize) continue;

        if(userBoard[xAdj][yAdj]=='*' && systemBoard[xAdj][yAdj]=='$') continue;
        else if(userBoard[xAdj][yAdj]=='*' && systemBoard[xAdj][yAdj]!='$') return false;

        if(userBoard[xAdj][yAdj]!='*' && systemBoard[xAdj][yAdj]=='$') return false;
        else if(userBoard[xAdj][yAdj]!='*' && systemBoard[xAdj][yAdj]!='$') continue;
    }
    return true;
}

void bfsToOpen(vector<vector<char>>& userBoard, vector<vector<char>>& systemBoard, int x, int y, int gridSize, int alreadyOpened)
{
    int dx[] = {-1, 0, 1, 0, -1, 1, -1, 1};
    int dy[] = {0, 1, 0, -1, -1, 1, 1, -1};
    queue<pair<int,int>> q;
    if(systemBoard[x][y]=='0')
    {
        q.push({x,y});
        userBoard[x][y]='0';
    }
    else if(alreadyOpened)
    {
        q.push({x,y});
    }
    else
    {
        userBoard[x][y]=systemBoard[x][y];
        return;
    }
    while(!q.empty())
    {
        int i = q.front().first;
        int j = q.front().second;
        q.pop();
        for(int k=0;k<8;k++)
        {
            int iAdj = i+dx[k];
            int jAdj = j+dy[k];
            if(iAdj>=0 && jAdj>=0 && iAdj<gridSize && jAdj<gridSize && systemBoard[iAdj][jAdj]!='$' && userBoard[iAdj][jAdj]=='?')
            {
                userBoard[iAdj][jAdj]=systemBoard[iAdj][jAdj];
                if(userBoard[iAdj][jAdj]=='0') q.push({iAdj,jAdj});
            }
        }
    }
    return;
}

bool unleashTheUnknown(vector<vector<char>>& systemBoard, vector<vector<char>>& userBoard, int x, int y, int gridSize, int numberOfMines)
{
    if(systemBoard[x][y]=='$')
    {

        return true;
    }

    if(userBoard[x][y]=='?' && systemBoard[x][y]!='0')
    {
        userBoard[x][y] = systemBoard[x][y];
        return false;
    }

    if(userBoard[x][y]!='?')
    {
        int Count=checkAdj(userBoard, x, y, gridSize);
        char CountChar = Count +'0';
        if(CountChar !=userBoard[x][y])
        {cout<<Count<<" "<<userBoard[x][y]<<endl; return false;}
        else
        {
            if(!ifCorrectFlags(userBoard, systemBoard, x, y, gridSize))
            {
                //cout<<"ddd"<<endl;
                return true;
            }
            else
            {
                //cout<<"ddd"<<endl;
                bfsToOpen(userBoard, systemBoard, x, y, gridSize, 1);
                displayBoard(userBoard);
                return false;
            }
        }
    }
    else
    {
        if(systemBoard[x][y]=='$')
        {

            return true;
        }
        else
        {
            cout<<"ddd"<<endl;
            bfsToOpen(userBoard, systemBoard, x, y, gridSize, 0);
            displayBoard(userBoard);
            return false;
        }
    }
}

void makeMove(vector<vector<char>>& systemBoard, vector<vector<char>>& userBoard, int gridSize, int numberOfMines, int& flag)
{
    int x, y;
    int option;
    cout<<"1. Mark a flag"<<endl<< "2. Open a cell"<<endl;
    cin>>option;
    cout<<"Enter x-coordinate & y-coordinate of the cell"<<endl;
    cin>>x>>y;
    x--;
    y--;
    if(option==1)
    {
        if(x<0 || x>=gridSize || y<0 || y>=gridSize)
        {
            cout<<"Invalid Co-Ordinates"<<endl;
            return;
        }

        if(userBoard[x][y]!='?' && userBoard[x][y]!='*')
        {
            cout<<"Already Opened"<<endl;
            return;
        }
        else if(userBoard[x][y]=='*')
        {
            cout<<"Already marked as flag. Do you want to un-flag it? Enter 1 for Yes and 0 for No: ";
            int removeFlag;
            cin>>removeFlag;
            if(removeFlag)
            {
                userBoard[x][y]='?';
                displayBoard(userBoard);
            }
            else return;

        }
        else
        {
            userBoard[x][y]='*';
            displayBoard(userBoard);
        }
    }
    else if(option==2)
    {
        if(x<0 || x>=gridSize || y<0 || y>=gridSize)
        {
            cout<<"Invalid Co-Ordinates ddddd"<<endl;
            return;
        }

        if(userBoard[x][y]=='*')
        {
            cout<<"Already Marked as flag"<<endl;
            return;
        }
        else
        {
            bool isMine = unleashTheUnknown(systemBoard, userBoard, x, y, gridSize, numberOfMines);
            if(isMine)
            {
                displayBoard(systemBoard);
                flag=1;
                return;
            }
            else
            {
                displayBoard(userBoard);
                return;
            }
        }
    }
    else
    {
        cout<<"Invalid Choice"<<endl;
        return;
    }
}

bool CheckIfEqual(vector<vector<char>>& systemBoard, vector<vector<char>>& userBoard)
{
    int n=systemBoard.size();
    for(int i=0; i<n; i++)
    {
        for(int j=0; j<n; j++)
        {
            if(userBoard[i][j]=='*' && systemBoard[i][j]=='$') continue;
            else if(userBoard[i][j]==systemBoard[i][j]) continue;
            else return false;
        }
    }
    return true;
}

int main()
{
    int gridSize,numberOfMines;
    pair<int,int> config = getDifficultyLevel();
    gridSize = config.first;
    numberOfMines = config.second;
    //cout << gridSize << " " << numberOfMines << endl;
    vector<vector<char>> systemBoard(gridSize, vector<char>(gridSize));
    makeBoard(systemBoard, gridSize, numberOfMines);
    displayBoard(systemBoard);
    vector<vector<char>> userBoard(gridSize, vector<char>(gridSize, '?'));
    displayBoard(userBoard);

    while(1)
    {
        if(CheckIfEqual(systemBoard, userBoard))
        {
            cout<<"Congratulations! You Won the game"<<endl;
            break;
        }
        int flag=0;
        makeMove(systemBoard, userBoard, gridSize, numberOfMines, flag);
        if(flag==1)
        {
            cout<<"Play Again :)"<<endl;
            return 0;
        }
    }
    return 0;
}
