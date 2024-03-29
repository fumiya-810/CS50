// Implements a dictionary's functionality
#include "dictionary.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 26;

// Hash table
node *table[N];

// 単語数をカウントする変数
int word_count;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    int table_word = hash(word); // 受け取った単語をハッシュする

    node *cursor = table[table_word]; // 辞書内の単語を示すカーソル

    while (cursor != NULL) // 次の単語があったら
    {
        if (strcasecmp(cursor->word, word) == 0) // 調べる文字列が辞書の中にあったら
        {
            return true; // trueを返して終了
        }
        cursor = cursor->next; // カーソルを次の単語に移す
    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    return toupper(word[0]) - 'A'; // 1文字目に応じた数値を0~25で返す
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary) // 辞書に登録されている言葉をメモリにロードする関数
{
    // TODO
    // dictionaryファイルを開く
    FILE *source = fopen(dictionary, "r"); // 読み取り専用で開く

    char buffer[LENGTH]; // 読み込んだ文字列を入れるバッファー

    if (source == NULL) // ファイルを開けなかったら
    {
        // メッセージを表示して終了
        printf("ファイルを開けませんでした\n");
        return false;
    }

    // ファイル内の各単語を読み取る
    while (fscanf(source, "%s", buffer) != EOF)
    {

        node *new_word = malloc(sizeof(node)); // 新しいノードを保存するための領域を作る

        if (new_word == NULL) // 領域を割り当てられなかったら
        {
            return false; // falseを返して終了
        }

        strcpy(new_word->word, buffer); // バッファーの文字列を先ほど作成したメモリ領域にコピー

        // 各単語をハッシュ テーブルに追加する
        int t = hash(new_word->word); // hash関数の返した番号の場所に単語を格納する

        new_word->next = table[t]; // 新しいノードをハッシュテーブルにつなぐ
        table[t] = new_word;       // テーブルの先頭がnew_wordになるようにする
        word_count++;              // 単語数を増やす
    }

    // 辞書ファイルを閉じる
    fclose(source);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    if (word_count > 0) // 単語が1つ以上あったら
    {
        return word_count;
    }
    return 0;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    for (int i = 0; i < N; i++)
    {
        node *cursor = table[i]; // 削除するワードを選択する変数
        node *tmp = cursor;      // 単語を削除しても次の単語を見失わないようにするための変数

        while (cursor != NULL) // 次の単語があったら
        {
            cursor = cursor->next; // 次の単語へ行く
            free(tmp);             // 先ほど見ていた単語の領域を解放
            tmp = cursor;          // 次の単語をtmpにもいれる
        }
    }

    return true;
}
