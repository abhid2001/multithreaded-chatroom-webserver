#include <bits/stdc++.h>
#include <string.h>

using namespace std;

const string ALPHABET = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 ";

int index(char c)
{
    for (int i = 0; i < ALPHABET.length(); i++)
    {
        if (c == ALPHABET[i])
            return i;
    }
    return -1;
}

string encrypt(string msg, string key)
{
    int i = 0, j = 0, kl = key.length(), ml = msg.length();
    string encMsg(ml, '0');

    for (i = 0; i < ml; i++)
    {
        if (isalnum(msg[i]) || msg[i] == ' ')
        {
            encMsg[i] = ALPHABET[(index(msg[i]) + index(key[j])) % ALPHABET.length()];
        }
        else
        {
            encMsg[i] = msg[i];
        }

        if (j + 1 == kl)
        {
            j = 0;
        }
        else
        {
            j++;
        }
    }

    encMsg[i] = '\0';
    return encMsg;
}

string decrypt(string msg, string key)
{
    int i = 0, j = 0, kl = key.length(), ml = msg.length();
    string decMsg(ml, '0');

    for (i = 0; i < ml; i++)
    {
        if (isalnum(msg[i]) || msg[i] == ' ')
        {
            decMsg[i] = ALPHABET[(index(msg[i]) - index(key[j]) + ALPHABET.length()) % ALPHABET.length()];
        }
        else
        {
            decMsg[i] = msg[i];
        }

        if (j + 1 == kl)
        {
            j = 0;
        }
        else
        {
            j++;
        }
    }

    decMsg[i] = '\0';
    return decMsg;
}
