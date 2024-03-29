from decimal import Decimal, ROUND_HALF_UP, ROUND_HALF_EVEN
from collections import Counter


def main():
    text = input("Text: ")

    # 文字数を格納する変数
    letters = len(text)

    # スペースとカンマの数を数える
    count_spece = text.count(" ")
    count_comma = text.count(",")
    count_apostrophe = text.count("'")

    # 単語数を格納する変数
    words = len(text.split())

    # 文の数を格納する変数
    sentences1 = text.count('.')
    sentences2 = text.count('!')
    sentences3 = text.count('?')
    sentences = sentences1 + sentences2 + sentences3

    # 文の数から記号や空白の数を引いて純粋な文字のみの数に変更する
    letters = letters - (count_spece + count_comma + count_apostrophe + sentences1 + sentences2 + sentences3)

    l = letters / words * 100
    s = sentences / words * 100
    index = 0.0588 * l - 0.296 * s - 15.8

    # インデックスを整数に直して学年を表示
    grade = int(round(index))
    print_grade(grade)


def print_grade(grade):
    if grade < 1:
        print("Before Grade 1")

    elif grade > 16:
        print("Grade 16+")

    else:
        print(f"Grade {grade}")


main()
