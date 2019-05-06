#include "StdAfx.h"
#include "DiskID32.hpp"
#include "SingleInstance.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Define global buffers.
BYTE IdOutCmd [sizeof (SENDCMDOUTPARAMS) + IDENTIFY_BUFFER_SIZE - 1];

char HardDriveSerialNumber [1024] = {	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
										0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
										0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
										0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
										0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
										0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
										0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
										0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
										0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
										0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
										0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
										0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
										0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
										0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
										0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
										0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
										0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
										0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
										0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
										0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
										0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
										0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
										0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
										0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
										0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
										0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
										0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
										0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
										0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
										0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
										0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
										0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

HANDLE hDriver;
bool IsWinIoInitialized = false;

int ReadPhysicalDriveInNT (void) {
   int done = FALSE;
   int drive = 0;

   for (drive = 0; drive < MAX_IDE_DRIVES; drive++) {
      HANDLE hPhysicalDriveIOCTL = 0;

         //  Try to get a handle to PhysicalDrive IOCTL, report failure
         //  and exit if can't.
      char driveName [256];

      sprintf (driveName, "\\\\.\\PhysicalDrive%d", drive);

         //  Windows NT, Windows 2000, must have admin rights
      hPhysicalDriveIOCTL = CreateFile (driveName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);

      if (hPhysicalDriveIOCTL != INVALID_HANDLE_VALUE) {
         GETVERSIONOUTPARAMS VersionParams;
         DWORD               cbBytesReturned = 0;

            // Get the version, etc of PhysicalDrive IOCTL
         memset ((void*) &VersionParams, 0, sizeof(VersionParams));

         DeviceIoControl (hPhysicalDriveIOCTL, DFP_GET_VERSION, NULL, 0, &VersionParams, sizeof(VersionParams),	&cbBytesReturned, NULL );

            // If there is a IDE device at number "i" issue commands
            // to the device
         if (VersionParams.bIDEDeviceMap > 0) {
            BYTE             bIDCmd = 0;   // IDE or ATAPI IDENTIFY cmd
            SENDCMDINPARAMS  scip;
            //SENDCMDOUTPARAMS OutCmd;

			// Now, get the ID sector for all IDE devices in the system.
               // If the device is ATAPI use the IDE_ATAPI_IDENTIFY command,
               // otherwise use the IDE_ATA_IDENTIFY command
            bIDCmd = (VersionParams.bIDEDeviceMap >> drive & 0x10) ? IDE_ATAPI_IDENTIFY : IDE_ATA_IDENTIFY;

            memset (&scip, 0, sizeof(scip));
            memset (IdOutCmd, 0, sizeof(IdOutCmd));

            if ( DoIDENTIFY (hPhysicalDriveIOCTL, &scip, (PSENDCMDOUTPARAMS)&IdOutCmd, (BYTE) bIDCmd, (BYTE) drive, &cbBytesReturned) ) {
               SaveInfo ( (PIDSECTOR) ((PSENDCMDOUTPARAMS) IdOutCmd) -> bBuffer);

               done = TRUE;
            }
	    }

         CloseHandle (hPhysicalDriveIOCTL);
      }
   }

   return done;
}

BOOL DoIDENTIFY (HANDLE hPhysicalDriveIOCTL, PSENDCMDINPARAMS pSCIP, PSENDCMDOUTPARAMS pSCOP, BYTE bIDCmd, BYTE bDriveNum, PDWORD lpcbBytesReturned) {
	// Set up data structures for IDENTIFY command.
	pSCIP -> cBufferSize = IDENTIFY_BUFFER_SIZE;
	pSCIP -> irDriveRegs.bFeaturesReg = 0;
	pSCIP -> irDriveRegs.bSectorCountReg = 1;
	pSCIP -> irDriveRegs.bSectorNumberReg = 1;
	pSCIP -> irDriveRegs.bCylLowReg = 0;
	pSCIP -> irDriveRegs.bCylHighReg = 0;

	// Compute the drive number.
	pSCIP -> irDriveRegs.bDriveHeadReg = 0xA0 | ((bDriveNum & 1) << 4);

	// The command can either be IDE identify or ATAPI identify.
	pSCIP -> irDriveRegs.bCommandReg = bIDCmd;
	pSCIP -> bDriveNumber = bDriveNum;
	pSCIP -> cBufferSize = IDENTIFY_BUFFER_SIZE;

	return ( DeviceIoControl (hPhysicalDriveIOCTL, DFP_RECEIVE_DRIVE_DATA, (LPVOID) pSCIP, sizeof(SENDCMDINPARAMS) - 1, (LPVOID) pSCOP, sizeof(SENDCMDOUTPARAMS) + IDENTIFY_BUFFER_SIZE - 1, lpcbBytesReturned, NULL) );
}

bool InitializeWinIo() {
	char szExePath[MAX_PATH];
	PSTR pszSlash;

	if ( checkVersion( MY_VER_PLATFORM_WIN32_NT ) ) {
		if (!GetModuleFileName(GetModuleHandle(NULL), szExePath, sizeof(szExePath)))
			return false;

		pszSlash = strrchr(szExePath, '\\');

		if (pszSlash)
			pszSlash[1] = 0;
		else
			return false;

		strcat(szExePath, "winio.sys");
	}

	IsWinIoInitialized = true;

	return true;
}

void ShutdownWinIo() {
}

// These are our ring 0 functions responsible for tinkering with the hardware ports.
// They have a similar privilege to a Windows VxD and are therefore free to access
// protected system resources (such as the page tables) and even place calls to
// exported VxD services.

__declspec(naked) void Ring0GetPortVal() {
	_asm {
		Cmp CL, 1
		Je ByteVal

		Cmp CL, 2
		Je WordVal

		Cmp CL, 4
		Je DWordVal

ByteVal:
		In AL, DX
		Mov [EBX], AL
		Retf

WordVal:
		In AX, DX
		Mov [EBX], AX
		Retf

DWordVal:
		In EAX, DX
		Mov [EBX], EAX
		Retf
	}
}


__declspec(naked) void Ring0SetPortVal() {
	_asm {
		Cmp CL, 1
		Je ByteVal

		Cmp CL, 2
		Je WordVal

		Cmp CL, 4
		Je DWordVal

ByteVal:
		Mov AL, [EBX]
		Out DX, AL
		Retf

WordVal:
		Mov AX, [EBX]
		Out DX, AX
		Retf

DWordVal:
		Mov EAX, [EBX]
		Out DX, EAX
		Retf
	}
}


// This function makes it possible to call ring 0 code from a ring 3
// application.
bool CallRing0(PVOID pvRing0FuncAddr, WORD wPortAddr, PWORD pdwPortVal, BYTE bSize) {
	struct GDT_DESCRIPTOR *pGDTDescriptor;
	struct GDTR gdtr;
	WORD CallgateAddr[3];
	WORD wGDTIndex = 1;

	_asm Sgdt [gdtr]

	// Skip the null descriptor
	pGDTDescriptor = (struct GDT_DESCRIPTOR *)(gdtr.dwGDTBase + 8);

	// Search for a free GDT descriptor
	for (wGDTIndex = 1; wGDTIndex < (gdtr.wGDTLimit / 8); wGDTIndex++) {
		if (pGDTDescriptor->Type == 0     &&
			pGDTDescriptor->System == 0   &&
			pGDTDescriptor->DPL == 0      &&
			pGDTDescriptor->Present == 0) {
			// Found one !
			// Now we need to transform this descriptor into a callgate.
			// Note that we're using selector 0x28 since it corresponds
			// to a ring 0 segment which spans the entire linear address
			// space of the processor (0-4GB).
			struct CALLGATE_DESCRIPTOR *pCallgate;

			pCallgate =	(struct CALLGATE_DESCRIPTOR *) pGDTDescriptor;
			pCallgate->Offset_0_15 = LOWORD(pvRing0FuncAddr);
			pCallgate->Selector = 0x28;
			pCallgate->ParamCount =	0;
			pCallgate->Unused = 0;
			pCallgate->Type = 0xc;
			pCallgate->System = 0;
			pCallgate->DPL = 3;
			pCallgate->Present = 1;
			pCallgate->Offset_16_31 = HIWORD(pvRing0FuncAddr);

			// Prepare the far call parameters
			CallgateAddr[0] = 0x0;
			CallgateAddr[1] = 0x0;
			CallgateAddr[2] = (wGDTIndex << 3) | 3;

			// Please fasten your seat belts!
			// We're about to make a hyperspace jump into RING 0.
			_asm Mov DX, [wPortAddr]
			_asm Mov EBX, [pdwPortVal]
			_asm Mov CL, [bSize]
			_asm Call FWORD PTR [CallgateAddr]

			// We have made it !
			// Now free the GDT descriptor
			memset(pGDTDescriptor, 0, 8);

			// Our journey was successful. Seeya.
			return true;
		}

		// Advance to the next GDT descriptor
		pGDTDescriptor++; 
	}

	// Whoops, the GDT is full
	return false;
}

bool GetPortVal(WORD wPortAddr, PWORD pdwPortVal, BYTE bSize) {
	bool Result;
	DWORD dwBytesReturned;
	struct tagPort32Struct Port32Struct;

	if ( checkVersion( MY_VER_PLATFORM_WIN32_NT ) ) {
		if (!IsWinIoInitialized)
			return false;

		Port32Struct.wPortAddr = wPortAddr;
		Port32Struct.bSize = bSize;

		if (!DeviceIoControl(hDriver, IOCTL_WINIO_READPORT, &Port32Struct, sizeof(struct tagPort32Struct), &Port32Struct, sizeof(struct tagPort32Struct), &dwBytesReturned, NULL))
			return false;
		else
			*pdwPortVal = Port32Struct.dwPortVal;
	} else {
		Result = CallRing0((PVOID)Ring0GetPortVal, wPortAddr, pdwPortVal, bSize);

		if (Result == false)
			return false;
	}

	return true;
}

bool SetPortVal(WORD wPortAddr, WORD dwPortVal, BYTE bSize) {
	DWORD dwBytesReturned;
	struct tagPort32Struct Port32Struct;

	if ( checkVersion( MY_VER_PLATFORM_WIN32_NT ) ) {
		if (!IsWinIoInitialized)
			return false;

		Port32Struct.wPortAddr = wPortAddr;
		Port32Struct.dwPortVal = dwPortVal;
		Port32Struct.bSize = bSize;

		if (!DeviceIoControl(hDriver, IOCTL_WINIO_WRITEPORT, &Port32Struct, sizeof(struct tagPort32Struct), NULL, 0, &dwBytesReturned, NULL)) 
			return false;
	} else {
		return CallRing0((PVOID)Ring0SetPortVal, wPortAddr, &dwPortVal, bSize);
	}

	return true;
}

int ReadDrivePortsInWin9X (void) {
	int done = FALSE;
	int drive = 0;

	InitializeWinIo ();   

	//  Get IDE Drive info from the hardware ports
	//  loop thru all possible drives
	for (drive = 0; drive < 8; drive++) {
		unsigned short diskdata [256];
		WORD  baseAddress = 0;   //  Base address of drive controller 
		WORD portValue = 0;
		int waitLoop = 0;
		int index = 0;

		switch (drive / 2) {
			case 0: baseAddress = 0x1f0; break;
			case 1: baseAddress = 0x170; break;
			case 2: baseAddress = 0x1e8; break;
			case 3: baseAddress = 0x168; break;
		}

		//  Wait for controller not busy 
		waitLoop = 100000;

		while (--waitLoop > 0) {
			GetPortVal ((WORD) (baseAddress + 7), &portValue, (BYTE) 1);
			//  drive is ready
			if ((portValue & 0x40) == 0x40)
				break;
			//  previous drive command ended in error
			if ((portValue & 0x01) == 0x01)
				break;
		}

		if (waitLoop < 1)
			continue;

		//  Set Master or Slave drive
		if ((drive % 2) == 0)
			SetPortVal ((WORD) (baseAddress + 6), 0xA0, 1);
		else
			SetPortVal ((WORD) (baseAddress + 6), 0xB0, 1);

		//  Get drive info data
		SetPortVal ((WORD) (baseAddress + 7), 0xEC, 1);

		// Wait for data ready 
		waitLoop = 100000;
		while (--waitLoop > 0) {
			GetPortVal ((WORD) (baseAddress + 7), &portValue, 1);
			//  see if the drive is ready and has it's info ready for us
			if ((portValue & 0x48) == 0x48)
				break;
			//  see if there is a drive error
			if ((portValue & 0x01) == 0x01)
				break;
		}

		//  check for time out or other error                                                    
		if (waitLoop < 1 || portValue & 0x01)
			continue;

		//  read drive id information
		for (index = 0; index < 256; index++) {
			diskdata [index] = 0;   //  init the space
			GetPortVal (baseAddress, &(diskdata [index]), 2);
		}

		SaveInfo ( (PIDSECTOR) diskdata );
		done = TRUE;
	}

	ShutdownWinIo ();

	return done;
}

int ReadIdeDriveAsScsiDriveInNT (void) {
	int done = FALSE;
	int controller = 0;

	for (controller = 0; controller < 2; controller++) {
		HANDLE hScsiDriveIOCTL = 0;
		char   driveName [256];

		//  Try to get a handle to PhysicalDrive IOCTL, report failure
		//  and exit if can't.
		sprintf (driveName, "\\\\.\\Scsi%d:", controller);

		//  Windows NT, Windows 2000, any rights should do
		hScsiDriveIOCTL = CreateFile (driveName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);

		if (hScsiDriveIOCTL != INVALID_HANDLE_VALUE) {
			int drive = 0;

			for (drive = 0; drive < 2; drive++) {
				char buffer [sizeof (SRB_IO_CONTROL) + SENDIDLENGTH];
				SRB_IO_CONTROL *p = (SRB_IO_CONTROL *) buffer;
				SENDCMDINPARAMS *pin = (SENDCMDINPARAMS *) (buffer + sizeof (SRB_IO_CONTROL));
				DWORD dummy;

				memset (buffer, 0, sizeof (buffer));
				p -> HeaderLength = sizeof (SRB_IO_CONTROL);
				p -> Timeout = 10000;
				p -> Length = SENDIDLENGTH;
				p -> ControlCode = IOCTL_SCSI_MINIPORT_IDENTIFY;
				strncpy ((char *) p -> Signature, "SCSIDISK", 8);

				pin -> irDriveRegs.bCommandReg = IDE_ATA_IDENTIFY;
				pin -> bDriveNumber = drive;

				if (DeviceIoControl (hScsiDriveIOCTL, IOCTL_SCSI_MINIPORT, buffer, sizeof (SRB_IO_CONTROL) + sizeof (SENDCMDINPARAMS) - 1, buffer, sizeof (SRB_IO_CONTROL) + SENDIDLENGTH, &dummy, NULL)) {
					SENDCMDOUTPARAMS *pOut = (SENDCMDOUTPARAMS *) (buffer + sizeof (SRB_IO_CONTROL));
					IDSECTOR *pId = (IDSECTOR *) (pOut -> bBuffer);

					if (pId -> sModelNumber [0]) {
						SaveInfo ( pId );

						done = TRUE;
					}
				}
			}

			CloseHandle (hScsiDriveIOCTL);
		}
	}

	return done;
}

void SaveInfo ( IDSECTOR* pID ) {
	int nLoc = HardDriveSerialNumber[ 0 ];

	for (int nCheck = 0;nCheck < 20;nCheck++) {
		if ( ! pID->sSerialNumber[ nCheck ] || pID->sSerialNumber[ nCheck ] == ' ' )
			pID->sSerialNumber[ nCheck ] = '_';

		if ( ! pID->sModelNumber[ nCheck ] || pID->sModelNumber[ nCheck ] == ' ' )
			pID->sModelNumber[ nCheck ] = '_';
	}

	for (;nCheck < 40;nCheck++) {
		if ( ! pID->sModelNumber[ nCheck ] || pID->sModelNumber[ nCheck ] == ' ' )
			pID->sModelNumber[ nCheck ] = '_';
	}

	memcpy( &HardDriveSerialNumber[ nLoc * 60 + 1 ], pID->sSerialNumber, 20 );
	memcpy( &HardDriveSerialNumber[ nLoc * 60 + 21 ], pID->sModelNumber, 40 );

	HardDriveSerialNumber[ nLoc * 60 + 61 ] = 0;
	HardDriveSerialNumber[ 0 ]++;
}

char* getHardDriveComputerID () {
	if ( HardDriveSerialNumber[ 0 ] == 0 ) {
		//  this works under WinNT4 or Win2K if you have admin rights
		if ( !ReadPhysicalDriveInNT () ) {
			//  this should work in WinNT or Win2K if previous did not work
			//  this is kind of a backdoor via the SCSI mini port driver into
			//     the IDE drives
			if ( !ReadIdeDriveAsScsiDriveInNT () ) {
				//  this works under Win9X and calls WINIO.DLL
				ReadDrivePortsInWin9X ();
			}
		}
	}

	return HardDriveSerialNumber;
}
