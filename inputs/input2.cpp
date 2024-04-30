#include <iostream>
#include <string>

using namespace std;

bool isVowel(char ch)
{
    ch = tolower(ch);
    return (ch == 'a' || ch == 'e' || ch == 'i' || ch == 'o' || ch == 'u');
}

string reverseString(string str)
{
    int n = str.length();
    for (int i = 0; i < n / 2; i++)
    {
        char temp = str[i];
        str[i] = str[n - i - 1];
        str[n - i - 1] = temp;
    }
    return str;
}

int countVowels(string str)
{
    int count = 0;
    for (char ch : str)
    {
        if (isVowel(ch))
        {
            count++;
        }
    }
    return count;
}

int main()
{
    char ch = 'A';
    cout << "Is " << ch << " a vowel? " << (isVowel(ch) ? "Yes" : "No") << endl;

    string str = "Hello, World!";
    cout << "Original string: " << str << endl;
    cout << "Reversed string: " << reverseString(str) << endl;
    cout << "Number of vowels: " << countVowels(str) << endl;

    return 0;
}
