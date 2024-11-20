#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN 100

// Function to check if the mapping is a valid substitution (one-to-one)
int is_valid_substitution(char mapping[256])
{
    char seen[256] = {0};
    for (int i = 0; i < 256; i++)
    {
        if (mapping[i] && seen[mapping[i]])
        {
            return 0;
        }
        seen[mapping[i]] = 1;
    }
    return 1;
}

// Function to apply permutation to the message
void apply_permutation(char *msg, int *perm, int n, char *result)
{
    for (int i = 0; i < n; i++)
    {
        result[i] = msg[perm[i]];
    }
    result[n] = '\0';
}

// Function to find the inverse of a permutation
void inverse_permutation(int *perm, int n, int *inv_perm)
{
    for (int i = 0; i < n; i++)
    {
        inv_perm[perm[i]] = i;
    }
}

// Function to check if the encrypted message could be the result of encrypting the original message
char *could_be_encrypted(char *encrypted_msg, char *original_msg)
{
    static char result[4];
    int n = strlen(encrypted_msg);
    int perm[n];
    for (int i = 0; i < n; i++)
    {
        perm[i] = i;
    }

    // Check for all possible permutations
    do
    {
        // Apply inverse permutation to the encrypted message
        int inv_perm[n];
        inverse_permutation(perm, n, inv_perm);
        char temp_msg[MAX_LEN + 1];
        apply_permutation(encrypted_msg, inv_perm, n, temp_msg);

        // Derive the substitution mapping
        char mapping[256] = {0};
        for (int i = 0; i < n; i++)
        {
            mapping[(int)original_msg[i]] = temp_msg[i];
        }

        // Check if the mapping is a valid substitution
        if (is_valid_substitution(mapping))
        {
            // Apply the substitution to the original message
            char substituted_msg[MAX_LEN + 1];
            for (int i = 0; i < n; i++)
            {
                substituted_msg[i] = mapping[(int)original_msg[i]];
            }
            substituted_msg[n] = '\0';

            // If the result matches the temp message, return "YES"
            if (strcmp(substituted_msg, temp_msg) == 0)
            {
                return "YES";
            }
        }
    } while (next_permutation(perm, perm + n));

    // If no match found, return "NO"
    return "NO";
}

// Function to get the next lexicographical permutation of the array
int next_permutation(int *begin, int *end)
{
    if (begin == end)
    {
        return 0;
    }
    int *i = end - 1;
    while (i > begin && *(i - 1) >= *i)
    {
        i--;
    }
    if (i == begin)
    {
        return 0;
    }
    int *j = end - 1;
    while (*j <= *(i - 1))
    {
        j--;
    }
    int temp = *(i - 1);
    *(i - 1) = *j;
    *j = temp;
    j = end - 1;
    while (i < j)
    {
        temp = *i;
        *i = *j;
        *j = temp;
        i++;
        j--;
    }
    return 1;
}

int main()
{
    char encrypted_msg[MAX_LEN + 1], original_msg[MAX_LEN + 1];
    scanf("%s", encrypted_msg);
    scanf("%s", original_msg);

    char *result = could_be_encrypted(encrypted_msg, original_msg);
    printf("%s\n", result);

    return 0;
}
