// LSC DLL Header

#if defined(LSCDLL_EXPORTS)
#define DLLFUNC extern "C" __declspec(dllexport)
#elif defined(__cplusplus)
#define DLLFUNC extern "C" __declspec(dllimport)
#else
#define DLLFUNC __declspec(dllimport)
#endif

// Set parameters
	DLLFUNC void SetParameter( int nPedestal, int nWidth, int nHeight, int nGr, int nR, 
	   int nB, int nGb, double dbAlpha, double dbSeed, BOOL bLensModeling, BOOL bScooby );

// Run LSC
DLLFUNC BOOL Run( BYTE *pRawBuffer, BYTE *pTrgBuffer, BOOL bSimulatedResults, CString strWorkDir, BOOL b2ndSet, BOOL *is_SINGULAR, BOOL *bSEED_overflow );

// Get Buffer Sizes
DLLFUNC int GetOTPSetfileSize();
DLLFUNC int GetSRAMSetfileSize();

// Output to Files
DLLFUNC BOOL SaveOTPSetfile( char *szFile );
DLLFUNC BOOL SaveSRAMSetfile( char *szFile );

// Output to Memory Buffers
DLLFUNC BOOL GetOTPSetfile( char *szBuf );
DLLFUNC BOOL GetSRAMSetfile( char *szBuf );

DLLFUNC void SetUserLensRI( double RI_1, double RI_2, double RI_3, double RI_4,
								double RI_5, double RI_6, double RI_7, double RI_8,
								double RI_9, double RI_10, double RI_11);
