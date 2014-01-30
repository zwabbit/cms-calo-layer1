#include "platform.h"

#include "xparameters.h"
#include "xintc.h"
#include "xspi.h"
#include "xil_exception.h"

#define WRSR_INSTR     0x01   //write status reg
#define PP_INSTR       0x02   //page program
#define RD_INSTR       0x03   //read data bytes
#define WRDI_INSTR     0x04   //write disable
#define RDSR_INSTR     0x05   //read status reg
#define WREN_INSTR     0x06   //write enable
#define FAST_RD_INSTR  0x0B   //fast read
#define RDID_INSTR     0x9F   //read id
#define BE_INSTR       0xC7   //bulk erase
#define SE_INSTR       0xD8   //sector erase

#define FLASH_PAGE_SIZE   256
#define FLASH_INSTR_BYTES 4
#define FLASH_STATUS_RD_BYTES 2
#define FLASH_STATUS_READY 0x01

//Doublecheck this value
#define STM_SPI_SELECT 0x01

XSpi SpiDevice;

static u8 WriteBuffer[FLASH_PAGE_SIZE + FLASH_INSTR_BYTES];
static u8 ReadBuffer[FLASH_PAGE_SIZE + FLASH_INSTR_BYTES];

volatile static int SpiBusy;

int ErrorCount;

int SpiFlashWaitOperation()
{
  while(SpiBusy);
  
  if(ErrorCount != 0)
  {
    ErrorCount = 0;
    return XST_FAILURE;
  }
  
  return XST_SUCCESS;
}

int SpiFlashWriteEnable()
{
  int Status;
  
  WriteBuffer[0] = WREN_INSTR;
  
  SpiBusy = TRUE;
  Status = XSpi_Transfer(&SpiDevice, WriteBuffer, NULL, 1);
  if(Status != XST_SUCCESS) return XST_FAILURE;
  
  return SpiFlashWaitOperation();
}

int SpiFlashWrite(u32 Address, u8 *Data, u32 ByteCount)
{
  int Status;
  u32 Index;
  
  WriteBuffer[0] = PP_INSTR;
  WriteBuffer[1] = (u8)(Address >> 16);
  WriteBuffer[2] = (u8)(Address >> 8);
  WriteBuffer[3] = (u8)(Address);
  
  for(Index = 4; Index < ByteCount + FLASH_INSTR_BYTES; ++Index)
    WriteBuffer[Index] = Data[Index - FLASH_INSTR_BYTES];
  
  SpiBusy = TRUE;
  Status = XSpi_Transfer(&SpiDevice, WriteBuffer, NULL, ByteCount + FLASH_INSTR_BYTES);
  if(Status != XST_SUCCESS) return XST_FAILURE;
  
  return SpiFlashWaitOperation();
}

int SpiFlashRead(u32 Address, u32 ByteCount)
{
  int Status;
  
  WriteBuffer[0] = RD_INSTR;
  WriteBuffer[1] = (u8)(Address >> 16);
  WriteBuffer[2] = (u8)(Address >> 8);
  WriteBuffer[3] = (u8)(Address);
  
  SpiBusy = TRUE;
  Status = XSpi_Transfer(&SpiDevice, WriteBuffer, ReadBuffer, ByteCount + FLASH_INSTR_BYTES);
  if(Status != XST_SUCCESS) return XST_FAILURE;
  
  return SpiFlashWaitOperation();
}

int SpiFlashEraseDevice()
{
  int Status;
  
  WriteBuffer[0] = BE_INSTR;
  
  SpiBusy = TRUE;
  Status = XSpi_Transfer(&SpiDevice, WriteBuffer, NULL, 0x01);
  if(Status != XST_SUCCESS) return XST_FAILURE;
  
  return SpiFlashWaitOperation();
}

int SpiFlashGetStatus()
{
  int Status;
  
  WriteBuffer[0] = RDSR_INSTR;
  
  SpiBusy = TRUE;
  Status = XSpi_Transfer(&SpiDevice, WriteBuffer, ReadBuffer, FLASH_STATUS_RD_BYTES);
  if(Status != XST_SUCCESS) return XST_FAILURE;
  
  return SpiFlashWaitOperation();
}

int SpiFlashBusyWait()
{
  int Status;
  u8 StatusReg;
  
  while(1)
  {
    Status = SpiFlashGetStatus();
    if(Status != XST_SUCCESS) return XST_FAILURE;
    
    StatusReg = ReadBuffer[1];
    if((StatusReg & FLASH_STATUS_READY) == 0) break;
  }
  
  return XST_SUCCESS;
}

int SpiWriteImage(u8 *Image, u32 ByteCount)
{
  int Status;
  
  Status = SpiFlashWriteEnable();
  if(Status != XST_SUCCESS) return Status;
  
  Status = SpiFlashBusyWait();
  if(Status != XST_SUCCESS) return Status;
  
  Status = SpiFlashEraseDevice();
  if(Status != XST_SUCCESS) return Status;
  
  Status = SpiFlashBusyWait();
  if(Status != XST_SUCCESS) return Status;
  
  Status = SpiFlashWriteEnable();
  if(Status != XST_SUCCESS) return Status;

  return XST_SUCCESS;
}
/*
static int SpiFlashInterruptSetup(XIntc *InterruptController)
{
  int Status;
  
  Status = XIntc_Connect(
    InterruptController,
    XPAR_INTC_0_SPI_0_VEC_ID,
    (XInterruptHandler)XInterruptHandler,
    (void*)(&SpiDevice));
  if(Status != XST_SUCCESS) return Status;
  
}
*/
void SpiHandler(void *CallBackRef, u32 StatusEvent, unsigned int ByteCount)
{
  SpiBusy = FALSE;
  
  if (StatusEvent != XST_SPI_TRANSFER_DONE) {
		ErrorCount++;
	}
}

int SpiFlashInitializePreInterrupt()
{
  XSpi_Config *ConfigPtr = NULL;
  int Status;
  
  ConfigPtr = XSpi_LookupConfig(XPAR_SPI_0_DEVICE_ID);
  if(ConfigPtr == NULL) return XST_DEVICE_NOT_FOUND;
  
  Status = XSpi_CfgInitialize(&SpiDevice, ConfigPtr, ConfigPtr->BaseAddress);
  if(Status != XST_SUCCESS) return Status;
  
  return XST_SUCCESS;
}

int SpiFlashInitializePostInterrupt()
{
  int Status;
  
  XSpi_SetStatusHandler(&SpiDevice, &SpiDevice, (XSpi_StatusHandler)SpiHandler);
  
  Status = XSpi_SetOptions(&SpiDevice, XSP_MASTER_OPTION | XSP_MANUAL_SSELECT_OPTION);
	if(Status != XST_SUCCESS) return XST_FAILURE;
  
  Status = XSpi_SetSlaveSelect(&SpiDevice, STM_SPI_SELECT);
  if(Status != XST_SUCCESS) return XST_FAILURE;
  
  XSpi_Start(&SpiDevice);
  
  return XST_SUCCESS;
}
