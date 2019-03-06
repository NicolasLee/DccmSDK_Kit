// LSC DLL Header

#if defined(LSCDLL_EXPORTS)
#define DLLFUNC extern "C" __declspec(dllexport)
#elif defined(__cplusplus)
#define DLLFUNC extern "C" __declspec(dllimport)
#else
#define DLLFUNC __declspec(dllimport)
#endif

// Set parameters
DLLFUNC void SetParameter_3L8( int nPedestal, int nWidth, int nHeight,BOOL nFileFormat, int nGr, int nR, 
   int nB, int nGb, double dbAlpha, double dbSeed, BOOL bLensModeling, BOOL bScooby );


///
// Run LSC
//DLLFUNC BOOL Run_5E2( BYTE *pRawBuffer, BYTE *pTrgBuffer, BOOL bSimulatedResults, CString strWorkDir );
DLLFUNC BOOL Run_3L8( BYTE *pRawBuffer, BYTE *pTrgBuffer, BOOL bSimulatedResults, BOOL b1SetUse, CString strWorkDir );

// Get Buffer Sizes
DLLFUNC int GetOTPSetfileSize_3L8();
DLLFUNC int GetSRAMSetfileSize_3L8();

// Output to Files
DLLFUNC BOOL SaveOTPSetfile_3L8( char *szFile );
DLLFUNC BOOL SaveSRAMSetfile_3L8( char *szFile );

// Output to Memory Buffers
DLLFUNC BOOL GetOTPSetfile_3L8( char *szBuf );
DLLFUNC BOOL GetSRAMSetfile_3L8( char *szBuf );

DLLFUNC void SetUserLensRI_3L8( double RI_1, double RI_2, double RI_3, double RI_4,
								double RI_5, double RI_6, double RI_7, double RI_8,
								double RI_9, double RI_10, double RI_11);
