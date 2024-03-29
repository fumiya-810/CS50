#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>

int score_check(string str);

int main(void)
{
    // ユーザーに単語の入力を求める
    string word1 = get_string("Player 1 : ");
    string word2 = get_string("Player 2 : ");

    // 各単語のスコアを計算する
    int score_p1 = score_check(word1);
    int score_p2 = score_check(word2);

    // 勝者を表示する
    if (score_p1 > score_p2)
    {
        printf("player 1 wins!\n");
    }
    else if (score_p1 < score_p2)
    {
        printf("player 2 wins!\n");
    }
    else
    {
        printf("Tie!\n");
    }
}

int score_check(string str)
{
    // 文字ごとの点数
    int score[26] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

    string example = "abcdefghijklmnopqrstuvwxyz";

    // 得点を格納するための変数
    int point = 0;

    // 引数の文字列の長さを格納
    int n = strlen(str);
    int num_str;
    for (int i = 0; i < n; i++)
    {
        num_str = str[i];
        for (int j = 0; j < 26; j++)
        {
            // 文字列strのi番目が文字化記号か識別 文字なら実行
            if ((num_str >= 'a' && num_str <= 'z') || (num_str >= 'A' && num_str <= 'Z'))
            {
                // 引数として受け取った文字列のi文字目と
                // 比較用文字列exampleのj番目を大文字と小文字の区別なく比較
                if (str[i] == example[j] || str[i] == example[j] - 32)
                {
                    point = point + score[j]; // もし一致したらポイントを加算
                    break;
                }
            }
        }
    }

    return point;
}
