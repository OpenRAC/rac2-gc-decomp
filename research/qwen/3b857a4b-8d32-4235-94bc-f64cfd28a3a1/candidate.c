char * FUN_001184d0(char *param_1);
char * FUN_001184d0(char *param_1)
{
  char cVar1;
  char *pcVar2;
  char *pcVar3;
  int iVar4;
  int iVar5;

  iVar4 = 0;
  if (*param_1 != '\0') {
    for (iVar4 = 1; param_1[iVar4] != '\0'; iVar4 = iVar4 + 1) {
    }
  }
  iVar4 = iVar4 + -1;
  iVar5 = 0;
  if (0 < iVar4) {
    pcVar3 = param_1 + iVar4;
    pcVar2 = param_1;
    do {
      iVar5 = iVar5 + 1;
      cVar1 = *pcVar2;
      iVar4 = iVar4 + -1;
      *pcVar2 = *pcVar3;
      *pcVar3 = cVar1;
      pcVar2 = pcVar2 + 1;
      pcVar3 = pcVar3 + -1;
    } while (iVar5 < iVar4);
  }
  return param_1;
}