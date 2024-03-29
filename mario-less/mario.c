#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int row;

    // 行数を入力、1～８以外の数値以外は再度入力を求める
    do
    {
        row = get_int("height: ");
    }
    while (row < 1 || row > 8);

    // ピラミッドを作る
    for (int i = 0; i < row; i++)
    {
        // ブロックを右詰めにするための空白を記述
        for (int j = 0; j < row - i - 1; j++)
        {
            printf(" ");
        }

        // ブロックの記述
        for (int k = 0; k < i + 1; k++)
        {
            printf("#");
        }

        printf("\n");
    }
}
