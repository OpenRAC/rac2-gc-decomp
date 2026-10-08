void FUN_00132C48(int param_1, int param_2, void *param_3);

void FUN_00133490(int param_1)
{
    int local_array[4];

    local_array[0] = param_1;
    FUN_00132C48(0x36, 4, local_array);
    return;
}