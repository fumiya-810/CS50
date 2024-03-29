#include <cs50.h>
#include <stdio.h>

int calculate_quarters(int cents);
int caluclate_10cents(int cents);
int caluclate_5cents(int cents);
int caluclate_1cents(int cents);

int main(void)
{
    int cents;
    // いくら支払うか求める。入力値が0より小さい場合は再度入力を求める
    do
    {
        cents = get_int("Change owed: ");
    }
    while (cents < 0);

    // 25セント硬貨が何枚使えるか計算
    int quarters = calculate_quarters(cents);

    // 使用できる25セント硬貨の分だけ値を入力した金額を減らす
    cents = cents - (quarters * 25);

    // 10セント硬貨が何枚使えるかを計算
    int ten_cents = caluclate_10cents(cents);

    // 使用できる10セント硬貨の分だけ入力された金額から減らす
    cents = cents - (ten_cents * 10);

    // 5セント硬貨を何枚使えるかを計算
    int five_cents = caluclate_5cents(cents);

    // 使用できる5セント硬貨の分だけ入力された金額から減らす
    cents = cents - (five_cents * 5);

    // 1セント硬貨が何枚使えるかを考える。
    int one_cents = caluclate_1cents(cents);

    // 1セント硬貨の分だけ入力された金額から減らす
    cents = cents - one_cents;

    // 硬貨の合計枚数を計算して表示
    int sum = quarters + ten_cents + five_cents + one_cents;
    printf("%i\n", sum);
}

// 25セント硬貨が何枚使えるかを判定する関数
int calculate_quarters(int cents)
{
    int quarters = 0;
    while (cents >= 25)
    {
        quarters++;
        cents = cents - 25;
    }
    return quarters;
}

// 10セント硬貨を何枚使えるか判定する関数
int caluclate_10cents(int cents)
{
    int ten_cents = 0;
    while (cents >= 10)
    {
        ten_cents++;
        cents = cents - 10;
    }
    return ten_cents;
}

// 5セント硬貨を何枚使えるかを計算する
int caluclate_5cents(int cents)
{
    int five_cents = 0;
    while (cents >= 5)
    {
        five_cents++;
        cents = cents - 5;
    }
    return five_cents;
}

int caluclate_1cents(int cents)
{
    int one_cents = 0;
    while (cents >= 1)
    {
        one_cents++;
        cents = cents - 1;
    }
    return one_cents;
}
