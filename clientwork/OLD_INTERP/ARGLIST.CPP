//
// ARGLIST.CPP
// 
// Command line argument kernel call.
//

#include "sol.hpp"

#include	"array.hpp"
#include	"kernel.hpp"
#include	"math.hpp"
#include	"msg.hpp"
#include "object.hpp"
#include "pmachine.hpp"
#include "selector.hpp"
#include "textid.hpp"
#include "sciwin.hpp"

int gArgc = 0;
char **gArgv = NULL;

void KArgList ( kArgs args )
{
	enum {
		ArgGetCount,
		ArgGetString,
		ArgGetProfileStr,
		ArgSetProfileStr,
		ArgGetWindowsInfo,
	};

	switch ( arg(1) ) {
		case ArgGetCount:
			pm.acc = (Acc)gArgc;
			break;

		case ArgGetString: {
			if ( gArgc <= arg(2) ) {
				pm.acc = 0;
				break;
			}

			TextID text;
			text.Get ( strlen ( gArgv[arg(2)] ) + 1 );
			strcpy ( *text, gArgv[arg(2)] );

			pm.acc = text;
		}

		break;

		case ArgGetProfileStr: {
			TextID newID, strID;
			newID.Get ( 5000 );

			strID = (TextID)arg(5);
			pm.acc = GetPrivateProfileString ( *(TextID)arg(2), *(TextID)arg(3), "", *newID, 5000, *(TextID)arg(4) );

			if ( pm.acc ) {
				newID.Snug();
				strID = *newID;
				newID.Free();
			}
		}

		break;

		case ArgSetProfileStr:
			pm.acc = WritePrivateProfileString ( *(TextID)arg(2), *(TextID)arg(3), *(TextID)arg(4), *(TextID)arg(5) );
			break;

#if 0
		case ArgGetWindowsInfo: {
			char *dataFile = "t[vxjs5lj^";
			char *registeredOwner = "SdhhtsfqfcPvods";
			char *ipAddress = "JOBceqfrt";
			char *productID = "QqpevduHe";
							 			
			char filename [1024] = "";
			GetWindowsDirectory ( filename, 1024 );

			if ( filename[strlen ( filename)] != '\\' )
				strcat ( filename, "\\" );

			MessageBox (GetFocus(), dataFile, "Sierra", MB_OK);
			MessageBox (GetFocus(), filename, "Sierra", MB_OK);

			// decrypt variables
			for ( int i=0; i<strlen ( dataFile ); i++ ) 
				dataFile[i] = toupper ( dataFile[i] ) - (i + 1);

			for ( i=0; i<strlen ( dataFile ); i++ ) 
				dataFile[i] = toupper ( dataFile[i] ) - (i + 1);

			strcat ( filename, dataFile );

			FILE *file = fopen ( filename, "rb" );

			if ( file ) {
				fseek ( file, 0, SEEK_END );
				int size = ftell ( file );
				fseek ( file, 0, SEEK_SET );

				char *ptr = (char *)malloc ( size );
				fread ( ptr, size, 1, file );

				fclose ( file );

				char id[10240] = "";
				char *idPtr = id;

				// scan for all ProductId variables
				for ( i=0; i<size; i++ ) {
					if ( !strncmp ( &ptr[i], registeredOwner, 15 ) ) {
						while ( isprint ( ptr[i] ) ) {
							*idPtr++ = ptr[i];
							i++;
						}
					} 

					else if ( !strncmp ( &ptr[i], ipAddress, 9 ) ) {
						while ( isprint ( ptr[i] ) ) {
							*idPtr++ = ptr[i];
							i++;
						}
					} 

					else if ( !strncmp ( &ptr[i], productID, 9 ) ) {
						if ( isdigit ( ptr[i+9] ) ) {
							while ( isprint ( ptr[i] ) ) {
								*idPtr++ = ptr[i];
								i++;
							}
						}
					}
				}

				*idPtr = 0;

				free ( ptr );

				MessageBox (GetFocus(), id, "Sierra", MB_OK);
			}

		}

		break;
#endif
	}
}
