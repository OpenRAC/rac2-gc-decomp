unsigned int FUN_0029db28(int *param_1, long param_2, int *param_3, long param_4) {
  short *psVar6;
  unsigned int *puVar5;
  unsigned int uVar7;
  unsigned int bVar3;
  short sVar1;
  unsigned short uVar2;
  int iVar4;

  psVar6 = (short *)0x001b1a58; /* DAT_001b1a58 */
  puVar5 = (unsigned int *)((unsigned int)param_2);
  uVar7 = 0;
  bVar3 = 1;
  *param_1 = 0;
  if (param_2 != 0) {
    *puVar5 = 0;
  }
  if (param_3 != (int *)0x0) {
    *param_3 = -1;
  }
  if (psVar6 == (short *)0x0) {
    return 0;
  }
  if (*psVar6 == 0) {
LAB_0029dc2c:
    if ((param_2 != 0) && (bVar3)) {
      *puVar5 = *puVar5 | 0x80000000;
    }
    return uVar7;
  }
  sVar1 = psVar6[0x12];
  do {
    uVar2 = psVar6[8];
    if ((sVar1 != 2) && ((uVar2 & 2) == 0)) {
      bVar3 = 0;
    }
    if (((uVar2 & 2) == 0) && (sVar1 != 0)) {
      if ((uVar2 & 1) == 0) {
        sVar1 = *psVar6;
      }
      else {
        if (sVar1 == 2) goto LAB_0029dc20;
        sVar1 = *psVar6;
      }
      *param_1 = (int)sVar1;
      if (param_4 != 0) {
        if (psVar6[0x12] == 2) {
          iVar4 = 0x112d;
        }
        else {
          iVar4 = (int)psVar6[psVar6[0x13] + 10];
        }
        *param_1 = iVar4;
      }
      if ((param_2 != 0) && (psVar6[0x12] == 2)) {
        *puVar5 = *puVar5 | 1 << (uVar7 & 0x1f);
      }
      param_1 = param_1 + 1;
      if (param_3 != (int *)0x0) {
        *param_3 = (int)psVar6[9] + (int)psVar6[0x13];
        param_3 = param_3 + 1;
      }
      uVar7 = uVar7 + 1;
    }
LAB_0029dc20:
    if (psVar6[0x14] == 0) goto LAB_0029dc2c;
    sVar1 = psVar6[0x26];
    psVar6 = psVar6 + 0x14;
  } while( true );
}
