#include "xparameters.h"

int SpiFlashWritePage(u32 BramBaseAddress, u32 FlashBaseAddress, u32 ByteCount);
int SpiFlashInitializePreInterrupt();
int SpiFlashInitializePostInterrupt();
void SpiHandler(void *CallBackRef, u32 StatusEvent, unsigned int ByteCount);
int SpiFlashEraseDevice();
