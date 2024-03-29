// Modifies the volume of an audio file

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Number of bytes in .wav header
const int HEADER_SIZE = 44;

int main(int argc, char *argv[])
{
    // Check command-line arguments
    if (argc != 4)
    {
        printf("Usage: ./volume input.wav output.wav factor\n");
        return 1;
    }

    // Open files and determine scaling factor
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    FILE *output = fopen(argv[2], "w");
    if (output == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    float factor = atof(argv[3]);

    // TODO: Copy header from input file to output file
    uint8_t header[HEADER_SIZE];            //
    fread(header, HEADER_SIZE, 1, input);   // 元のファイルのヘッダーを読み込む
    fwrite(header, HEADER_SIZE, 1, output); // 読み込んだヘッダーをコピー先のファイルに書き込む

    // TODO: Read samples from input file and write updated data to output file
    int16_t buffer; // データを一時的に保存するバッファ

    // Read single sample from input into buffer while there are samples left to read
    while (fread(&buffer, sizeof(int16_t), 1, input) != 0) // 読み込んでいるファイルが終わってなかったら
    {
        // Update volume of sample
        buffer *= factor; // 音量を変更

        // Write updated sample to new file
        fwrite(&buffer, sizeof(int16_t), 1, output); // 変更したデータを書き込む
    }

    // Close files
    fclose(input);
    fclose(output);
}
