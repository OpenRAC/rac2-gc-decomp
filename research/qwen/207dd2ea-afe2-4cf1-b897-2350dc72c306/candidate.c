void FUN_00288B38(int);
void FUN_00288B18(void*);
void FUN_002889b8(int);
void FUN_00115484(void*, int, int);

void FUN_00288BB8(void *param)
{
  FUN_00288B38((int)param);
  *(unsigned int *)param = 0;
  FUN_00288B18(param);
}