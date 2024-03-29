import csv
import sys


def main():

    # TODO: Check for command-line usage
    if len(sys.argv) != 3:
        print("Missing command-line argument")
        sys.exit(1)

    args = sys.argv

    # TODO: Read database file into a variable
    # csvファイルからデータをメモリに読み込む
    # csvファイルからすべての行を読み取る
    rows = []
    with open(args[1]) as file:
        reader = csv.DictReader(file)
        headers = reader.fieldnames
        for row in reader:
            rows.append(row)

    # TODO: Read DNA sequence file into a variable
    # DNA配列ファイルを変数に読み込み
    with open(args[2], "r") as file:
        dna_sequence = file.read()

    # TODO: Find longest match of each STR in DNA sequence
    # DNA 配列内の各 STR の最長一致を検索
    max_strs = {}
    for i in headers:
        max_strs[i] = longest_match(dna_sequence, i)

    # TODO: Check database for matching profiles
    # データベースで一致するプロファイルを確認する
    name = None

    for i in rows:
        match_person = True
        for j in max_strs.keys():
            if j == "name":
                continue
            if int(i[j]) != max_strs[j]:  # 一致しなかったら
                match_person = False
                break  # ループを抜ける

        if match_person == True:  # もし一致した人が見つかったら名前を表示して終了
            name = i["name"]  # 名前を格納して表示
            print(f"{name}")
            return

    # 一致する人が見つからない場合はループを抜けて以下が実行される
    print("No match")
    return


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()
