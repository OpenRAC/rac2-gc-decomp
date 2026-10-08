void FUN_00336390(void *out, void *table, int key);

extern int FUN_00336d50(void *table, int key);

void FUN_00336390(void *out, void *table, int key) {
  int r = FUN_00336d50(table, key);
  *(int *)(out + 64) = r;
}