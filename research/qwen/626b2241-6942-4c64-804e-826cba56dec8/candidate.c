extern void FUN_00283658(void *param_1);
extern void FUN_002ab378(void *param_1, void *param_2);
extern void FUN_00283678(void *param_1, void *param_2);

void FUN_002ab600(void *param_1, void *param_2)
{
  unsigned char auStack_60[64];

  FUN_00283658(auStack_60);
  FUN_002ab378(param_1, auStack_60);
  FUN_00283678(param_2, auStack_60);
}