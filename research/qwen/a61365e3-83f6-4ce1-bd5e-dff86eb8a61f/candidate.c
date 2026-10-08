typedef unsigned int uint32_t;

extern volatile uint32_t D_001C51D0;
extern void FUN_00295c70(void);

void FUN_00295c50(uint32_t param_1) {
    D_001C51D0 = param_1;
    FUN_00295c70();
}