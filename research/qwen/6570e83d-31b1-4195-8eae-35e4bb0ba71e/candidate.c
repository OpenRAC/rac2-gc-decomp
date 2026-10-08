void FUN_00283098(void *, void *, void *);
void FUN_00282cc0(void *, void *, void *);

void FUN_002AFD88(void *p1, void *p2, void *p3, void *p4, long p5)
{
  void *uStack_20;
  void *uStack_1c;
  void *uStack_18;

  uStack_20 = p1;
  uStack_1c = p2;
  uStack_18 = p3;
  FUN_00283098(&uStack_20, &uStack_20, (void *)((char *)p4 + 0xc0));
  FUN_00282cc0((void *)p5, (void *)p5, &uStack_20);
}