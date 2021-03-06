#ifndef RESMANAGER_H
#define RESMANAGER_H

// ResManager declarations.
// Outpost2 uses the ResManager to locate and load game resources.
// It also includes functions to check for the existence of the CD (though these have been partially
// overridden by OPU hacks to allow the user to play without a CD).

// Declared in Outpost2 but not exported and we don't know that much about it.
class StreamIO;

class OP2 ResManager
{
public:
	ResManager & operator=(ResManager const &);
	unsigned int ChecksumStream(char const *);
	StreamIO* CreateStream(char const *);
	int FindCDRoot(char *);
	void GetCDDir(char *);
	int GetFilePath(char const *,char *);
	void GetInstalledDir(char *);
	int Init(void);
	int InitCDDir(void);
	void InitInstalledDir(void);
	void* LockStream(char const *,int *);
	StreamIO* OpenStream(char const *);
	void ReleaseStream(StreamIO *);
	void ShutDown(void);
	void UnlockStream(void *);
	void VerifyCD(void);
};

// Global ResManager object that should be used, instead of creating a new ResManager
OP2 ResManager g_ResManager;

#endif
