#include <stdio.h>
#include <string.h>
#include <ctype.h>

int isVowel(char ch)
{
    ch = tolower(ch);
    return (ch == 'a' || ch == 'e' || ch == 'i' || ch == 'o' || ch == 'u');
}

void reverseString(char *str)
{
    int length = strlen(str);
    for (int i = 0; i < length / 2; i++)
    {
        char temp = str[i];
        str[i] = str[length - i - 1];
        str[length - i - 1] = temp;
    }
}

int countVowels(char *str)
{
    int count = 0;
    for (int i = 0; str[i] != '\0'; i++)
    {
        if (isVowel(str[i]))
        {
            count++;
        }
    }
    return count;
}

int main()
{
    char ch = 'A';
    printf("Is %c a vowel? %s\n", ch, (isVowel(ch) ? "Yes" : "No"));

    char str[] = "Hello, World!";
    printf("Original string: %s\n", str);
    reverseString(str);
    printf("Reversed string: %s\n", str);
    printf("Number of vowels: %d\n", countVowels(str));

    return 0;
}
