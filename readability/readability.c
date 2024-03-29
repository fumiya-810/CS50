#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);
void print_grade(int grade); // グレード（学年）の表示

int main(void)
{
    // ユーザーにテキストの入力を求める
    string text = get_string("Text:");

    // テキスト内の文字、単語、文の数を数える
    int letters = count_letters(text);     // 文字数
    int words = count_words(text);         // 単語数
    int sentences = count_sentences(text); // 文の数

    // Coleman-Liau インデックスの計算
    // lは100語あたりの平均文字数、つまり、文字数を単語数で割った値に100を掛けた値
    float l = letters / (float) words * 100;

    // sはテキスト内の100語あたりの平均文数、つまり、文の数を単語数で割った値に100を掛けたもの
    float s = sentences / (float) words * 100;

    float index = 0.0588 * l - 0.296 * s - 15.8;

    int grade = round(index); // indexの値を最も近い整数に丸める(四捨五入)

    // 学年レベルを表示する
    print_grade(grade);
}

// 文字数を数える
int count_letters(string text)
{
    int letters = 0;

    for (int i = 0, len = strlen(text); i < len; i++)
    {
        // textのi番目が英数字なら
        if (isalpha(text[i]) != 0)
        {
            letters++;
        }
    }

    // textの文字数を返す
    return letters;
}

// 単語の数を数える
int count_words(string text)
{

    int words = 0;
    // 単語はスペースで区切るのでスペースの数＝単語の数としてカウント
    for (int i = 0, len = strlen(text); i < len; i++)
    {
        if (isspace(text[i]))
        {
            words++;
        }
    }

    // 一番最後の単語の後ろにスペースがないため1つ加算
    words++;

    // textの文字数を返す
    return words;
}

// 文の数を数える
int count_sentences(string text)
{
    int centences = 0;

    for (int i = 0, len = strlen(text); i < len; i++)
    {
        char c = text[i];
        // textのi番目が句読点のときは実行
        if (ispunct(text[i]))
        { // textのi番目が. ! ? のいずれかのときは実行
            if ((text[i] == '.') || (text[i] == '!') || (text[i] == '?'))
            {
                centences++;
            }
        }
    }
    // textの文の数を返す
    return centences;
}

// グレードの表示をする
void print_grade(int grade)
{
    if (grade < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (grade > 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", grade);
    }
}
