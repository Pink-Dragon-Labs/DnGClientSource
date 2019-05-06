#include <stdlib.h>
#include <stdio.h>
#include <windows.h>

#include <winioctl.h>

#ifdef WINIO_DLL
#define PORT32API _declspec(dllexport)
#else
#define PORT32API _declspec(dllimport)
#endif

// Define the various device type values.  Note that values used by Microsoft
// Corporation are in the range 0-32767, and 32768-65535 are reserved for use
// by customers.
#define FILE_DEVICE_WINIO 0x00008010

// Macro definition for defining IOCTL and FSCTL function control codes.
// Note that function codes 0-2047 are reserved for Microsoft Corporation,
// and 2048-4095 are reserved for customers.
#define WINIO_IOCTL_INDEX 0x810

// Define our own private IOCTL
#define IOCTL_WINIO_MAPPHYSTOLIN  CTL_CODE(FILE_DEVICE_WINIO, WINIO_IOCTL_INDEX, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_WINIO_UNMAPPHYSADDR CTL_CODE(FILE_DEVICE_WINIO, WINIO_IOCTL_INDEX + 1, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_WINIO_WRITEPORT     CTL_CODE(FILE_DEVICE_WINIO, WINIO_IOCTL_INDEX + 2, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_WINIO_READPORT      CTL_CODE(FILE_DEVICE_WINIO, WINIO_IOCTL_INDEX + 3, METHOD_BUFFERED, FILE_ANY_ACCESS)

struct tagPhys32Struct {
  HANDLE PhysicalMemoryHandle;
  ULONG dwPhysMemSizeInBytes;
  PVOID pvPhysAddress;
  PVOID pvPhysMemLin;
};

extern struct tagPhys32Struct Phys32Struct;

struct tagPort32Struct {
  USHORT wPortAddr;
  ULONG dwPortVal;
  UCHAR bSize;
};

extern struct tagPort32Struct Port32Struct;

//  Required to ensure correct PhysicalDrive IOCTL structure setup
#pragma pack(1)

struct GDT_DESCRIPTOR
{
  WORD Limit_0_15;
  WORD Base_0_15;
  BYTE Base_16_23;
  BYTE Type         : 4;
  BYTE System       : 1;
  BYTE DPL          : 2;
  BYTE Present      : 1;
  BYTE Limit_16_19  : 4;
  BYTE Available    : 1;
  BYTE Reserved     : 1;
  BYTE D_B          : 1;
  BYTE Granularity  : 1;
  BYTE Base_24_31;
};

struct CALLGATE_DESCRIPTOR
{
  WORD Offset_0_15;
  WORD Selector;
  WORD ParamCount   : 5;
  WORD Unused       : 3;
  WORD Type         : 4;
  WORD System       : 1;
  WORD DPL          : 2;
  WORD Present      : 1;
  WORD Offset_16_31;
};

struct GDTR
{
  WORD wGDTLimit;
  DWORD dwGDTBase;
};

   //  Max number of drives assuming primary/secondary, master/slave topology
#define  MAX_IDE_DRIVES  4
#define  IDENTIFY_BUFFER_SIZE  512

   //  IOCTL commands
#define  DFP_GET_VERSION				0x00074080
#define  DFP_SEND_DRIVE_COMMAND			0x0007c084
#define  DFP_RECEIVE_DRIVE_DATA			0x0007c088

#define  FILE_DEVICE_SCSI				0x0000001b
#define  IOCTL_SCSI_MINIPORT_IDENTIFY	((FILE_DEVICE_SCSI << 16) + 0x0501)
#define  IOCTL_SCSI_MINIPORT			0x0004D008

   //  GETVERSIONOUTPARAMS contains the data returned from the 
   //  Get Driver Version function.
typedef struct _GETVERSIONOUTPARAMS {
   BYTE bVersion;      // Binary driver version.
   BYTE bRevision;     // Binary driver revision.
   BYTE bReserved;     // Not used.
   BYTE bIDEDeviceMap; // Bit map of IDE devices.
   DWORD fCapabilities; // Bit mask of driver capabilities.
   DWORD dwReserved[4]; // For future use.
} GETVERSIONOUTPARAMS, *PGETVERSIONOUTPARAMS, *LPGETVERSIONOUTPARAMS;

   //  Bits returned in the fCapabilities member of GETVERSIONOUTPARAMS 
#define  CAP_IDE_ID_FUNCTION             1  // ATA ID command supported
#define  CAP_IDE_ATAPI_ID                2  // ATAPI ID command supported
#define  CAP_IDE_EXECUTE_SMART_FUNCTION  4  // SMART commannds supported

   //  IDE registers
typedef struct _IDEREGS {
   BYTE bFeaturesReg;       // Used for specifying SMART "commands".
   BYTE bSectorCountReg;    // IDE sector count register
   BYTE bSectorNumberReg;   // IDE sector number register
   BYTE bCylLowReg;         // IDE low order cylinder value
   BYTE bCylHighReg;        // IDE high order cylinder value
   BYTE bDriveHeadReg;      // IDE drive/head register
   BYTE bCommandReg;        // Actual IDE command.
   BYTE bReserved;          // reserved for future use.  Must be zero.
} IDEREGS, *PIDEREGS, *LPIDEREGS;

   //  SENDCMDINPARAMS contains the input parameters for the 
   //  Send Command to Drive function.
typedef struct _SENDCMDINPARAMS {
   DWORD     cBufferSize;   //  Buffer size in bytes
   IDEREGS   irDriveRegs;   //  Structure with drive register values.
   BYTE bDriveNumber;       //  Physical drive number to send 
                            //  command to (0,1,2,3).
   BYTE bReserved[3];       //  Reserved for future expansion.
   DWORD     dwReserved[4]; //  For future use.
   BYTE      bBuffer[1];    //  Input buffer.
} SENDCMDINPARAMS, *PSENDCMDINPARAMS, *LPSENDCMDINPARAMS;

   //  Valid values for the bCommandReg member of IDEREGS.
#define  IDE_ATAPI_IDENTIFY  0xA1  //  Returns ID sector for ATAPI.
#define  IDE_ATA_IDENTIFY    0xEC  //  Returns ID sector for ATA.

   // Status returned from driver
typedef struct _DRIVERSTATUS {
   BYTE  bDriverError;  //  Error code from driver, or 0 if no error.
   BYTE  bIDEStatus;    //  Contents of IDE Error register.
                        //  Only valid when bDriverError is SMART_IDE_ERROR.
   BYTE  bReserved[2];  //  Reserved for future expansion.
   DWORD  dwReserved[2];  //  Reserved for future expansion.
} DRIVERSTATUS, *PDRIVERSTATUS, *LPDRIVERSTATUS;

   // Structure returned by PhysicalDrive IOCTL for several commands
typedef struct _SENDCMDOUTPARAMS {
   DWORD         cBufferSize;   //  Size of bBuffer in bytes
   DRIVERSTATUS  DriverStatus;  //  Driver status structure.
   BYTE          bBuffer[1];    //  Buffer of arbitrary length in which to store the data read from the                                                       // drive.
} SENDCMDOUTPARAMS, *PSENDCMDOUTPARAMS, *LPSENDCMDOUTPARAMS;

// The following struct defines the interesting part of the IDENTIFY
// buffer:
typedef struct _IDSECTOR {
   USHORT  wGenConfig;
   USHORT  wNumCyls;
   USHORT  wReserved;
   USHORT  wNumHeads;
   USHORT  wBytesPerTrack;
   USHORT  wBytesPerSector;
   USHORT  wSectorsPerTrack;
   USHORT  wVendorUnique[3];
   CHAR    sSerialNumber[20];
   USHORT  wBufferType;
   USHORT  wBufferSize;
   USHORT  wECCSize;
   CHAR    sFirmwareRev[8];
   CHAR    sModelNumber[40];
   USHORT  wMoreVendorUnique;
   USHORT  wDoubleWordIO;
   USHORT  wCapabilities;
   USHORT  wReserved1;
   USHORT  wPIOTiming;
   USHORT  wDMATiming;
   USHORT  wBS;
   USHORT  wNumCurrentCyls;
   USHORT  wNumCurrentHeads;
   USHORT  wNumCurrentSectorsPerTrack;
   ULONG   ulCurrentSectorCapacity;
   USHORT  wMultSectorStuff;
   ULONG   ulTotalAddressableSectors;
   USHORT  wSingleWordDMA;
   USHORT  wMultiWordDMA;
   BYTE    bReserved[128];
} IDSECTOR, *PIDSECTOR;

typedef struct _SRB_IO_CONTROL {
   ULONG HeaderLength;
   UCHAR Signature[8];
   ULONG Timeout;
   ULONG ControlCode;
   ULONG ReturnCode;
   ULONG Length;
} SRB_IO_CONTROL, *PSRB_IO_CONTROL;

char *ConvertToString (unsigned short diskdata [256], int firstIndex, int lastIndex);
void SaveInfo ( PIDSECTOR pID );
BOOL DoIDENTIFY (HANDLE, PSENDCMDINPARAMS, PSENDCMDOUTPARAMS, BYTE, BYTE, PDWORD);

#define  SENDIDLENGTH  sizeof (SENDCMDOUTPARAMS) + IDENTIFY_BUFFER_SIZE

char* getHardDriveComputerID ();

#pragma pack()
