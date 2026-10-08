/* Function at 0x00133310: Calls SndIopCommandContinuing with fixed arguments */
/* External helper declared as per task.externals mapping */

extern void SndIopCommandContinuing(long, int, int, long, long);

void FUN_00133310(void)
{
  SndIopCommandContinuing(0x34L, 0, 0, 0L, 0L);
  return;
}