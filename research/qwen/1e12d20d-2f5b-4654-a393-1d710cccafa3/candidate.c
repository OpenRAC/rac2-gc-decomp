char *
FUN_001163E0(char *param_1, char *param_2)
{
    char c2;
    int i;

    c2 = *param_2;
    if (*param_1 == '\0') {
        return (c2 == '\0') ? param_1 : (char *)0;
    }

    while (*param_1 != '\0') {
        i = 0;
        while (param_2[i] != '\0' && param_2[i] == param_1[i]) {
            i++;
        }
        if (param_2[i] == '\0') {
            return param_1;
        }
        param_1++;
    }

    return (char *)0;
}
