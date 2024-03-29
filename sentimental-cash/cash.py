def main():
    while True:
        try:
            # ドルで入力を受け付ける
            money = float(input("Change: "))

            if money > 0:
                break
        except ValueError:
            pass

    # ドルをセントに変換する
    money = money * 100

    quarter = quarters(money)
    money = money - (quarter * 25)

    ten_sents = ten_sent(money)
    money = money - (ten_sents * 10)

    five_sents = five_sent(money)
    money = money - (five_sents * 5)

    one_sents = one_sent(money)
    money = money - (one_sents * 1)

    coins = quarter + ten_sents + five_sents + one_sents
    print(f"{coins}")


# 25セント硬貨を使えるか判断する関数
def quarters(sent):
    coin = 0
    if sent >= 25:
        coin = sent/25
    return int(coin)


# 10セント硬貨を使えるか判断する関数
def ten_sent(sent):
    coin = 0
    if sent >= 10:
        coin = sent / 10
    return int(coin)


# 5セント硬貨を使えるか判断する関数
def five_sent(sent):
    coin = 0
    if sent >= 5:
        coin = sent / 5
    return int(coin)


# 1セント硬貨を使えるか判断する関数
def one_sent(sent):
    coin = 0
    if sent >= 1:
        coin = sent / 1
    return int(coin)


# main関数の実行
main()
