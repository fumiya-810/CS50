#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int end_ellor(void);
bool only_digits(string s);
char rotate(char c, int n);

int main(int argc, string argv[])
{
    // コマンドライン引数の数が正しいか確認
    if (argc != 2)          // コマンドライン引数が２つ（ファイル名を除くと１つ）でない場合は
    {                       // つまり間違った入力をした場合は
        return end_ellor(); // 異常終了
    }

    // argv[1]の文字が数字であることを確認
    if (only_digits(argv[1]) == false) // 引数の文字列が数字でなかったら
    {
        return end_ellor(); // 異常終了
    }

    // argv[1]の値をstring型からint型に変換
    int key = atoi(argv[1]);

    // ユーザーに平文の入力を求めるプロンプトを表示する
    string plain_text = get_string("plaintext: ");
    int len = strlen(plain_text);

    char cryptography[len];

    // 平文の各文字に対し処理を行う
    for (int i = 0; i < len; i++)
    {
        cryptography[i] = rotate(plain_text[i], key);
    }

    // 暗号文の表示
    printf("ciphertext: ");

    for (int i = 0; i < len; i++)
    {
        printf("%c", cryptography[i]);
    }

    printf("\n");
}

int end_ellor(void)
{
    printf("Usage: ./caesar key\n"); // 正しい入力を促す
    return 1;                        // 異常終了
}

// コマンドライン引数で受け取ったものが数字か判別
bool only_digits(string s)
{
    // 文字列sの長さをlen に格納
    for (int i = 0, len = strlen(s); i < len; i++)
    {
        if (isdigit(s[i]) == 0) // 文字列sのi番目が数字ではなかったら
        {
            return false; // 0を返す
        }
    }
    return true;
}

// 平文を暗号化する関数
char rotate(char c, int n)
{
    if (n > 25)
    {
        n = n % 26;
    }

    int a = (int) c;

    if (isalpha(c) != 0) // 受け取った文字がアルファベット文字だったら
    {
        if (islower(c)) // cが小文字なら
        {
            a = a + n; // 鍵の文だけずらした文字を格納

            if (a > 122) // cの文字コードが122(z)より大きかったら
            {
                a = a - 26; // 26文字分巻き戻してアルファベットにする
            }
        }
        else
        {
            a = a + n; // 鍵の分だけずらした文字を格納

            if (a > 90) // cの文字コードが90(Z)より大きかったら
            {
                a = a - 26; // 26文字巻き戻してアルファベットにする
            }
        }
    }

    c = (char) a;
    return c;
}
