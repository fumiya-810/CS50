#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width]) // 画像を白黒にする関数
{

    // すべてのピクセルをループします
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // 赤、緑、青の平均を取る
            double a = (image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0;
            int ave = round(a);

            // ピクセル値を更新
            image[i][j].rgbtBlue = ave;
            image[i][j].rgbtGreen = ave;
            image[i][j].rgbtRed = ave;
        }
    }

    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width]) // セピア色にする関数
{

    const int MAX_C = 255; // 8bitカラー値の最大値
    const int MIN_C = 0;   // 8bitカラー各色の最小値

    // Loop over all pixels
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // セピア色の値を計算

            // もともとのRGB値をそれぞれ格納する変数
            int originalRed = image[i][j].rgbtRed;
            int originalGreen = image[i][j].rgbtGreen;
            int originalBlue = image[i][j].rgbtBlue;

            double sepiaRed = .393 * originalRed + .769 * originalGreen + .189 * originalBlue;
            double sepiaGreen = .349 * originalRed + .686 * originalGreen + .168 * originalBlue;
            double sepiaBlue = .272 * originalRed + .534 * originalGreen + .131 * originalBlue;

            // セピア色のRGB値を整数にする
            int sepia_red = round(sepiaRed);
            int sepia_green = round(sepiaGreen);
            int sepia_blue = round(sepiaBlue);

            // セピア色のRGB値が0～255に収まるように修正
            if (sepia_red > MAX_C)
            {
                sepia_red = MAX_C;
            }
            else if (sepia_red < MIN_C)
            {
                sepia_red = MIN_C;
            }

            if (sepia_green > MAX_C)
            {
                sepia_green = MAX_C;
            }
            else if (sepia_green < MIN_C)
            {
                sepia_green = MIN_C;
            }

            if (sepia_blue > MAX_C)
            {
                sepia_blue = MAX_C;
            }
            else if (sepia_blue < MIN_C)
            {
                sepia_blue = MIN_C;
            }

            // ピクセルをセピア色で更新する
            image[i][j].rgbtRed = sepia_red;
            image[i][j].rgbtGreen = sepia_green;
            image[i][j].rgbtBlue = sepia_blue;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width]) // 画像を左右反転させる関数
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            // 各ビットのRGB値を入れ替える
            int tmp_red = image[i][j].rgbtRed;
            image[i][j].rgbtRed = image[i][width - (j + 1)].rgbtRed;
            image[i][width - (j + 1)].rgbtRed = tmp_red;

            int tmp_green = image[i][j].rgbtGreen;
            image[i][j].rgbtGreen = image[i][width - (j + 1)].rgbtGreen;
            image[i][width - (j + 1)].rgbtGreen = tmp_green;

            int tmp_blue = image[i][j].rgbtBlue;
            image[i][j].rgbtBlue = image[i][width - (j + 1)].rgbtBlue;
            image[i][width - (j + 1)].rgbtBlue = tmp_blue;
        }
    }

    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width]) // 画像をぼかす関数
{
    // 二次元配列imageのコピーを作る
    RGBTRIPLE copy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }

    // ぼかした色を計算する
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int a = 0; // 平均を求める際にこれで割る
            double sum_red = 0;
            double sum_green = 0;
            double sum_blue = 0;

            // image[i][j]を中心とした9ピクセルの平均を計算する
            for (int k = i - 1; k <= i + 1; k++) // y方向
            {
                for (int l = j - 1; l <= j + 1; l++) // x方向
                {
                    if ((k >= 0) && (k <= height - 1)) // image[i][j]が端でなければ
                    {
                        if ((l >= 0) && (l <= width - 1))
                        {
                            a++;
                            sum_red += copy[k][l].rgbtRed;
                            sum_green += copy[k][l].rgbtGreen;
                            sum_blue += copy[k][l].rgbtBlue;
                        }
                    }
                }
            }

            double ave_red = sum_red / a;
            double ave_green = sum_green / a;
            double ave_blue = sum_blue / a;

            // 変更後の色をimageに格納
            image[i][j].rgbtRed = round(ave_red);
            image[i][j].rgbtGreen = round(ave_green);
            image[i][j].rgbtBlue = round(ave_blue);
        }
    }
    return;
}
