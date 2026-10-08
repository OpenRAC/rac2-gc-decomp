typedef short int16;
typedef int int32;

void FUN_002d5b58(int16,int32,int);

void FUN_002d5e70(int param_1,int param_2)
{
  FUN_002d5b58(*(int16 *)(param_1 + 2),*(int32 *)(param_1 + 4),param_2);
  return;
}