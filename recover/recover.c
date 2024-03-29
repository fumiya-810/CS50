#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    // 単一のコマンドライン引数を受け入れる
    if (argc != 2) // コマンドライン引数が二つでない場合は
    {
        // 正しい使い方を促して終了
        printf("Usage: ./recover FILE\n");
        return 1;
    }

    // メモリカードを開く
    FILE *card = fopen(argv[1], "r");

    int num_file = 0; // ファイルの数を保存する変数

    // メモリカードから読み取るデータがまだ残っている間に
    uint8_t buffer[512]; // 1つあたり1バイト(1bit)の変数を配列で512個用意

    FILE *img = NULL;

    char filename[10];

    // データから JPEG を作成
    while (fread(buffer, 1, 512, card) == 512) // メモリーカードに読み取るデータが残っているならループ
    {
        if ((buffer[0] == 0xff) && (buffer[1] == 0xd8) && (buffer[2] == 0xff) &&
            ((buffer[3] & 0xf0) == 0xe0)) // 読み取ったファイルがjpegファイルのヘッダー形式と一致したら
        {
            // 書き込み先のファイルを閉じる
            if (img != NULL)
            {
                fclose(img);
            }

            // 書き込み先のファイル名の文字列を作る
            sprintf(filename, "%03i.jpg", num_file);

            // 書き込み先のファイルを開く
            img = fopen(filename, "wb");

            if (img == NULL)
            {
                printf("ファイルを開けません\n");
                return 1;
            }

            num_file++; // ファイルの数を増やす
        }

        // 書き込み先に書き込む
        if (img != NULL)
        {
            fwrite(buffer, 1, 512, img);
        }
    }
    if (img != NULL)
    {
        fclose(img);
    }
    fclose(card);
}
