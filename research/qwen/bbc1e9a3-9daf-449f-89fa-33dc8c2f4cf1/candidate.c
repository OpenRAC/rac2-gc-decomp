extern unsigned int DAT_001b160c;
extern unsigned int DAT_001ba040[64];
extern unsigned int DAT_001ba140[64];

void SpawnChunkFailed(unsigned int param_1,unsigned int param_2)

{
  if (DAT_001b160c < 0x40) {
    DAT_001ba040[DAT_001b160c] = param_1;
    DAT_001ba140[DAT_001b160c] = param_2;
    DAT_001b160c = DAT_001b160c + 1;
  }
  return;
}