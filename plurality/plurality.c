#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// Candidates have name and vote count
typedef struct
{
    string name;
    int votes;
} candidate;

// Array of candidates
candidate candidates[MAX];

// Number of candidates
int candidate_count;

// Function prototypes
bool vote(string name);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: plurality [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
    }

    int voter_count = get_int("Number of voters: ");

    // Loop over all voters
    for (int i = 0; i < candidate_count; i++)
    {
        string name = get_string("Vote: ");

        // Check for invalid vote
        if (!vote(name))
        {
            printf("Invalid vote.\n");
        }
    }

    // Display winner of election
    print_winner();
}

// Update vote totals given a new vote
bool vote(string name)
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        // 候補者の名前が指定された名前と一致するかどうかを確認
        if (strcmp(candidates[i].name, name) == 0) // 名前が一致したら
        {
            // 候補者の投票を増やし、true を返す
            candidates[i].votes++;
            return true;
        }
    }
    // 一致しない場合は false を返す
    return false;
}

// Print the winner (or winners) of the election
void print_winner(void)
{
    // TODO
    // 最大投票数を求める
    int max_vote = candidates[0].votes; // 最初の票を最大値として格納

    for (int i = 1; i <= MAX; i++)
    {
        if (max_vote < candidates[i].votes) // より大きい得票数が見つかったら
        {
            max_vote = candidates[i].votes; // 最大得票数として保存
        }
    }

    // 最大得票数の候補者を出力する
    for (int i = 0; i < MAX; i++)
    {
        if (candidates[i].votes == max_vote) // もし得票数が最大得票数と一致したら
        {
            printf("%s\n", candidates[i].name);
        }
    }
    return;
}
