#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max voters and candidates
#define MAX_VOTERS 100
#define MAX_CANDIDATES 9

// preferences[i][j] is jth preference for voter i
int preferences[MAX_VOTERS][MAX_CANDIDATES];

// Candidates have name, vote count, eliminated status
typedef struct
{
    string name;
    int votes;
    bool eliminated;
} candidate;

// Array of candidates
candidate candidates[MAX_CANDIDATES];

// Numbers of voters and candidates
int voter_count;
int candidate_count;

// Function prototypes
bool vote(int voter, int rank, string name);
void tabulate(void);
bool print_winner(void);
int find_min(void);
bool is_tie(int min);
void eliminate(int min);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: runoff [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX_CANDIDATES)
    {
        printf("Maximum number of candidates is %i\n", MAX_CANDIDATES);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
        candidates[i].eliminated = false;
    }

    voter_count = get_int("Number of voters: ");
    if (voter_count > MAX_VOTERS)
    {
        printf("Maximum number of voters is %i\n", MAX_VOTERS);
        return 3;
    }

    // Keep querying for votes
    for (int i = 0; i < voter_count; i++)
    {

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            // Record vote, unless it's invalid
            if (!vote(i, j, name))
            {
                printf("Invalid vote.\n");
                return 4;
            }
        }

        printf("\n");
    }

    // Keep holding runoffs until winner exists
    while (true)
    {
        // Calculate votes given remaining candidates
        tabulate();

        // Check if election has been won
        bool won = print_winner();
        if (won)
        {
            break;
        }

        // Eliminate last-place candidates
        int min = find_min();
        bool tie = is_tie(min);

        // If tie, everyone wins
        if (tie)
        {
            for (int i = 0; i < candidate_count; i++)
            {
                if (!candidates[i].eliminated)
                {
                    printf("%s\n", candidates[i].name);
                }
            }
            break;
        }

        // Eliminate anyone with minimum number of votes
        eliminate(min);

        // Reset vote counts back to zero
        for (int i = 0; i < candidate_count; i++)
        {
            candidates[i].votes = 0;
        }
    }
    return 0;
}

// Record preference if vote is valid
bool vote(int voter, int rank, string name) // 投票の情報を2次元配列に格納
{
    // TODO
    for (int i = 0; i < candidate_count; i++)      // 候補者の数だけ繰り返す
    {                                              // 候補者と投票された名前が一致したら
        if (strcmp(candidates[i].name, name) == 0) // 候補者の名前と受け取った名前が一致したら
        {
            preferences[voter][rank] = i; // 候補者の番号をpreferencesに格納
            return true;                  // 保存が成功したとしてtrueを返す
        }
    }
    // 誰とも一致しなかったら失敗としてfalseを返す
    return false;
}

// Tabulate votes for non-eliminated candidates
void tabulate(void) // 票の集計を行う関数
{
    // TODO
    for (int i = 0; i < voter_count; i++) // 投票者の数だけ繰り返す
    {
        for (int j = 0; j < candidate_count; j++) // 候補者の数だけ（投票者の好みの数だけ）繰り返す
        {
            if (candidates[preferences[i][j]].eliminated == false) // その候補者が落選していなかったら
            {
                candidates[preferences[i][j]].votes++; // 票を入れる
                break;
            }
        }
    }

    return;
}

// Print the winner of the election, if there is one
bool print_winner(void) // 得票の多い候補者の表示
{
    // TODO
    int judge_point = voter_count / 2 + 1; // 過半数となる最低限の票数を計算し格納
    int flag = 0;                          // 当選者がいるかどうか判断するフラグ

    for (int i = 0; i < candidate_count; i++) // 候補者の数だけ繰り返す
    {
        if (candidates[i].votes >= judge_point) // i番目の候補者の得票が過半数以上だったら
        {
            printf("%s\n", candidates[i].name); // 候補者の名前を表示
            flag++;                             // 当選者を増やす
        }
    }

    if (flag > 0) // 当選者が1名以上いたら
    {
        return true;
    }

    return false;
}

// Return the minimum number of votes any remaining candidate has
int find_min(void) // 最も票の少ない候補者を調べる関数
{
    // TODO
    int min_vote = candidates[0].votes; // 1人目の候補者の得票を仮の最低票として格納

    for (int i = 1; i < candidate_count; i++) // 候補者の数だけ繰り返す
    {
        if ((candidates[i].votes < min_vote) &&
            (candidates[i].eliminated == false)) // i番目の候補者の得票の方が少ないかつ落選していないなら
        {
            min_vote = candidates[i].votes; // 最低票を更新
        }
    }
    return min_vote; // 最低票を返す
}

// Return true if the election is tied between all candidates, false otherwise
bool is_tie(int min) // 全員の票が等しいかどうか調べる関数
{
    // TODO
    int not_elim = 0;                         // 落選していない候補者の数を数える変数
    int tie = 0;                              // 最低票数と一致する人がいるか判別する変数
    for (int i = 0; i < candidate_count; i++) // 候補者の数だけ繰り返す
    {
        if (candidates[i].eliminated == false) // その投票者が落選していなかったら
        {
            not_elim++; // 落選していない候補者の数を増やす

            if (min == candidates[i].votes) // 最低票数とi番目の候補者の票数が一致したら
            {
                tie++; // 変数を増やす
            }
        }
    }
    if (tie == not_elim) // 残っている候補者全員が最低票だったら
    {
        return true;
    }
    return false;
}

// Eliminate the candidate (or candidates) in last place
void eliminate(int min) // 最低票の候補者を削除する関数
{
    // TODO
    for (int i = 0; i < candidate_count; i++) // 候補者の数だけ繰り返す
    {
        if (candidates[i].votes == min) // i番目の候補者の得票数が最低票の数と一致したら
        {
            candidates[i].eliminated = true; // その候補者を削除
        }
    }
    return;
}
