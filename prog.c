#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Candidate
{
    int id;
    char name[50];
    int votes;
};

struct Voter
{
    int studentID;
};
void addCandidate();
void vote();
void viewResults();
int hasVoted(int sid);
void saveVoter(int sid);

int main()
{
    int choice;

    while (1)
    {
        printf("\n====== ONLINE VOTING SYSTEM ======\n");
        printf("1. Add Candidate (Admin)\n");
        printf("2. Vote (Student)\n");
        printf("3. View Results & Winner\n");
        printf("4. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            addCandidate();
            break;
        case 2:
            vote();
            break;
        case 3:
            viewResults();
            break;
        case 4:
            exit(0);
        default:
            printf("Invalid choice!\n");
        }
    }
}

void addCandidate()
{
    FILE *fp;
    struct Candidate c;

    fp = fopen("candidates.dat", "ab");

    printf("Enter Candidate ID: ");
    scanf("%d", &c.id);
    printf("Enter Candidate Name: ");
    scanf(" %[^\n]", c.name);
    c.votes = 0;

    fwrite(&c, sizeof(c), 1, fp);
    fclose(fp);

    printf("Candidate added successfully!\n");
}

int hasVoted(int sid)
{
    FILE *fp;
    struct Voter v;

    fp = fopen("voters.dat", "rb");
    if (fp == NULL)
        return 0;

    while (fread(&v, sizeof(v), 1, fp))
    {
        if (v.studentID == sid)
        {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

void saveVoter(int sid)
{
    FILE *fp;
    struct Voter v;

    fp = fopen("voters.dat", "ab");
    v.studentID = sid;
    fwrite(&v, sizeof(v), 1, fp);
    fclose(fp);
}

void vote()
{
    FILE *fp;
    struct Candidate c;
    int id, sid, found = 0;

    printf("Enter Student ID: ");
    scanf("%d", &sid);

    if (hasVoted(sid))
    {
        printf("You have already voted!\n");
        return;
    }

    fp = fopen("candidates.dat", "rb+");
    if (fp == NULL)
    {
        printf("No candidates available!\n");
        return;
    }

    printf("\nAvailable Candidates:\n");
    while (fread(&c, sizeof(c), 1, fp))
    {
        printf("ID: %d | Name: %s\n", c.id, c.name);
    }

    printf("Enter Candidate ID to vote: ");
    scanf("%d", &id);

    rewind(fp);

    while (fread(&c, sizeof(c), 1, fp))
    {
        if (c.id == id)
        {
            c.votes++;
            fseek(fp, -sizeof(c), SEEK_CUR);
            fwrite(&c, sizeof(c), 1, fp);
            found = 1;
            break;
        }
    }

    fclose(fp);

    if (found)
    {
        saveVoter(sid);
        printf("Vote cast successfully!\n");
    }
    else
    {
        printf("Invalid Candidate ID!\n");
    }
}

void viewResults()
{
    FILE *fp;
    struct Candidate c, winner;
    int maxVotes = -1;

    fp = fopen("candidates.dat", "rb");
    if (fp == NULL)
    {
        printf("No data available!\n");
        return;
    }

    printf("\n====== VOTING RESULTS ======\n");
    while (fread(&c, sizeof(c), 1, fp))
    {
        printf("Candidate: %s | Votes: %d\n", c.name, c.votes);

        if (c.votes > maxVotes)
        {
            maxVotes = c.votes;
            winner = c;
        }
    }

    fclose(fp);

    if (maxVotes >= 0)
    {
        printf("\n WINNER \n");
        printf("Name: %s\nVotes: %d\n", winner.name, winner.votes);
    }
}
