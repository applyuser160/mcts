#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define NUMBER_OF_SIDE 3
#define NUMBER_OF_SQUARES 9

enum MASS_STATUS {
    NON = 0,
    CIRCLE = 1,
    CROSS = 2
};

// モンテカルロ木のノード
typedef struct Node
{
    // 前のノード
    struct Node *parent;

    // 次のノード数
    int childCount;

    // 次のノード
    struct Node **child;

    // ターン数
    int turn;

    // アドレス
    int address;

    // 石の種類
    int rock;

    // 有効
    bool isEnable;

    // 終了
    bool isEnd;

    // 通過数
    int throughCount;

    // 勝利数 CIRCLE
    int ciWinCount;

    // 勝利数 CROSS
    int cwWinCount;
}Node;

int randBetween(int max, int min)
{
    srand((unsigned int)time(NULL)); // 現在時刻の情報で初期化
    return (rand()%(max - min + 1)) + min;
}

bool isFinished(int **board)
{
    // 横
    for (int i = 0; i < NUMBER_OF_SIDE; i++)
    {
        int ms = board[i][0];
        for (int j = 1; j < NUMBER_OF_SIDE; j++)
        {
            if (ms == NON || ms != board[i][j])
            {
                break;
            }
            else if (j == NUMBER_OF_SIDE - 1)
            {
                return true;
            }
        }
    }

    // 縦
    for (int i = 0; i < NUMBER_OF_SIDE; i++)
    {
        int ms = board[0][i];
        for (int j = 1; j < NUMBER_OF_SIDE; j++)
        {
            if (ms == NON || ms != board[j][i])
            {
                break;
            }
            else if (j == NUMBER_OF_SIDE - 1)
            {
                return true;
            }
        }
    }

    // 斜め 左上から右下
    int ms = board[0][0];
    for (int i = 1; i < NUMBER_OF_SIDE; i++)
    {
        if (ms == NON || ms != board[i][i])
        {
            break;
        }
        else if (i == NUMBER_OF_SIDE - 1)
        {
            return true;
        }
    }

    // 斜め 右上から左下
    ms = board[0][NUMBER_OF_SIDE - 1];
    for (int i = 1; i < NUMBER_OF_SIDE; i++)
    {
        if (ms == NON || ms != board[i][NUMBER_OF_SIDE - 1 - i])
        {
            break;
        }
        else if (i == NUMBER_OF_SIDE - 1)
        {
            return true;
        }
    }

    // 全てのマスが埋まっている
    for (int i = 0; i < NUMBER_OF_SQUARES; i++)
    {
        if (board[i / 3][i % 3] != NON)
        {
            break;
        }
        else if (i == NUMBER_OF_SQUARES - 1)
        {
            return true;
        }
    }

    // 終了条件を満たしていないとき
    return false;
}

// ノードが既に作成済みの場合インデックスを返す
int isCreated(struct Node *child, struct Node *parent)
{
    for (int i = 0; i < (*parent).childCount; i++)
    {
        bool addEq = (*(*parent).child[i]).address == (*child).address;
        bool rockEq = (*(*parent).child[i]).rock == (*child).rock;
        if (addEq && rockEq)
        {
            return i;
        }
    }
    return 0;
}

// ノードを現在のノードの下に作成する、作成済みの場合は、通過数を加える
void deployNode(struct Node *child, struct Node *parent)
{
    int index = isCreated(child, parent);
    if (index == 0)
    {
        // 次のノード数をインクリメント
        (*parent).childCount++;

        // 現在の次のノードを一時的に移す
        struct Node **old = (*parent).child;

        // 新しい領域を確保（２階層のうちの１階層目）
        (*parent).child = (struct Node **)calloc((*parent).childCount, sizeof(struct Node));

        // 確保した領域に、既存のアドレスをセット
        for (int o = 0; o < (*parent).childCount - 1; o++)
        {
            (*parent).child[o] = old[o];
        }

        // 確保した領域の最後に、新規のノードをセット
        (*parent).child[(*parent).childCount - 1] = child;
    }
    else
    {
        (*parent).childCount++;
    }
}

int main(void)
{
    // 盤面
    int **board = (int **)calloc(NUMBER_OF_SIDE, sizeof(int*));
    for (int o = 0; o < NUMBER_OF_SIDE; o++)
    {
        board[o] = (int *)calloc(NUMBER_OF_SIDE, sizeof(int));
    }

    // モンテカルロ木
    struct Node *tree = (struct Node *)calloc(1, sizeof(struct Node));
    (*tree).parent = NULL;
    (*tree).childCount = 0;
    (*tree).child = NULL;
    (*tree).turn = 0;
    (*tree).isEnable = true;
    (*tree).isEnd = false;
    (*tree).throughCount = 0;
    (*tree).ciWinCount = 0;
    (*tree).cwWinCount = 0;

    struct Node *currentNode = tree;

    // ターンナンバー
    int turnNumber = 0;

    // 石
    int rock = CIRCLE;

    while (!isFinished(board))
    {
        // 新たなノードを作成
        turnNumber++;
        struct Node *node = (struct Node *)calloc(1, sizeof(struct Node));
        (*node).parent = currentNode;
        (*node).childCount = 0;
        (*node).child = NULL;
        (*node).turn = turnNumber;
        (*node).address = randBetween(8, 0);
        (*node).rock = rock;
        (*tree).isEnable = true;
        (*tree).isEnd = false;
        (*tree).throughCount = 0;
        (*tree).ciWinCount = 0;
        (*tree).cwWinCount = 0;

        (*currentNode).throughCount;
        (*currentNode).child = realloc((*currentNode).child, (*currentNode).throughCount);
        (*currentNode).child = &node;

        
    }

    return 0;
}