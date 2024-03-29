
// Simulate genetic inheritance of blood type

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Each person has two parents and two alleles
typedef struct person
{
    struct person *parents[2];
    char alleles[2];
} person;

const int GENERATIONS = 3;
const int INDENT_LENGTH = 4;

person *create_family(int generations);
void print_family(person *p, int generation);
void free_family(person *p);
char random_allele();

int main(void)
{
    // Seed random number generator
    srand(time(0));

    // Create a new family with three generations
    person *p = create_family(GENERATIONS);

    // Print family tree of blood types
    print_family(p, 0);

    // Free memory
    free_family(p);
}

// Create a new individual with `generations`
person *create_family(int generations)
{
    // TODO: Allocate memory for new person
    person *new_person = malloc(sizeof(person)); // 新しくperson分メモリ領域を割り当てる

    // If there are still generations left to create
    if (generations > 1) // 作成する世代が残っている場合
    {
        // Create two new parents for current person by recursively calling create_family
        person *parent0 = create_family(generations - 1); // 親世代のpersonを作成
        person *parent1 = create_family(generations - 1);

        // TODO: Set parent pointers for current person
        new_person->parents[0] = parent0; // 親世代へのポインタを格納
        new_person->parents[1] = parent1;

        // TODO: Randomly assign current person's alleles based on the alleles of their parents
        new_person->alleles[0] = parent0->alleles[rand() % 2]; // 親世代の対立遺伝子を格納
        new_person->alleles[1] = parent1->alleles[rand() % 2];
    }

    // If there are no generations left to create
    else // 親世代がいない場合
    {
        // TODO: Set parent pointers to NULL
        new_person->parents[0] = NULL; // 親世代のアドレスをNULLにする
        new_person->parents[1] = NULL;

        // TODO: Randomly assign alleles
        new_person->alleles[0] = random_allele(); // 親世代がないのでランダムな数値を入れる
        new_person->alleles[1] = random_allele();
    }

    // TODO: Return newly created person
    return new_person; // 新しく作ったpoersonのデータを返す
    // return NULL; // もともとNULLを返すようになっていたが、これはエラーを出さないため？
}

// Free `p` and all ancestors of `p`.
void free_family(person *p)
{
    // TODO: Handle base case
    if (p == NULL) // 受け取ったpersonがNULLの場合は
    {
        return; // 終了
    }

    // TODO: Free parents recursively
    free_family(p->parents[0]); // 親世代のメモリ領域を解放
    free_family(p->parents[1]);

    // TODO: Free child
    free(p); // 自分のメモリ領域を解放
}

// Print each family member and their alleles.
void print_family(person *p, int generation)
{
    // Handle base case
    if (p == NULL)
    {
        return;
    }

    // Print indentation
    for (int i = 0; i < generation * INDENT_LENGTH; i++)
    {
        printf(" ");
    }

    // Print person
    if (generation == 0)
    {
        printf("Child (Generation %i): blood type %c%c\n", generation, p->alleles[0], p->alleles[1]);
    }
    else if (generation == 1)
    {
        printf("Parent (Generation %i): blood type %c%c\n", generation, p->alleles[0], p->alleles[1]);
    }
    else
    {
        for (int i = 0; i < generation - 2; i++)
        {
            printf("Great-");
        }
        printf("Grandparent (Generation %i): blood type %c%c\n", generation, p->alleles[0], p->alleles[1]);
    }

    // Print parents of current generation
    print_family(p->parents[0], generation + 1);
    print_family(p->parents[1], generation + 1);
}

// Randomly chooses a blood type allele.
char random_allele()
{
    int r = rand() % 3;
    if (r == 0)
    {
        return 'A';
    }
    else if (r == 1)
    {
        return 'B';
    }
    else
    {
        return 'O';
    }
}
