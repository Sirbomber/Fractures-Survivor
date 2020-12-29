#include <windows.h>
#include "HookHelper.h"


OP2Patch* OP2PatchManager::nextPatch = (OP2Patch*)NULL;
OP2Patch* OP2PatchManager::patchListHead = (OP2Patch*)NULL;
int OP2PatchManager::numPatches = 0;

DWORD OP2PatchManager::imageBase = (DWORD)NULL;

void* OP2PatchManager::_memberFunctionToPatch = (void*)NULL;
void* OP2PatchManager::_newMemberFunctionAddress = (void*)NULL;


OP2Patch::OP2Patch(void *address, void *newBytes, void *expectedBytes, unsigned int sizeOfBytes, bool doApply)
{
	DWORD oldProtection;

	this->address = address;
	this->patchSize = sizeOfBytes;
	this->oldBytesBuffer = new unsigned char[this->patchSize];
	this->newBytesBuffer = new unsigned char[this->patchSize];
	memcpy(this->newBytesBuffer, newBytes, this->patchSize);

	this->invalid = false;

	if (!VirtualProtect(this->address, this->patchSize, PAGE_READWRITE, &oldProtection)
		|| (expectedBytes != NULL && memcmp(expectedBytes, this->address, this->patchSize) != 0)
		|| memcpy(this->oldBytesBuffer, this->address, this->patchSize) < (void*)0
		|| !VirtualProtect(this->address, this->patchSize, oldProtection, &oldProtection))
		this->invalid = true;

	this->isApplied = false;
	this->isPermanent = false;
	if (doApply)
		this->Patch();

	this->previous = NULL;
	this->next = NULL;
}


OP2Patch::~OP2Patch()
{
	if (this->isApplied)
		this->Unpatch(false);

	delete [] this->oldBytesBuffer;
	delete [] this->newBytesBuffer;
}


OP2Patch& OP2Patch::operator= (const OP2Patch &source)
{
	memcpy(this, &source, sizeof(OP2Patch));
	return *this;
}


int OP2Patch::Patch(bool repatch)
{
	DWORD oldAttr;

	if (this->invalid)
		return 0;

	if (this->isApplied && !repatch)
		return 1;

	if (!VirtualProtect(address, this->patchSize, PAGE_EXECUTE_READWRITE, &oldAttr))
		return 0;
	memcpy(this->address, this->newBytesBuffer, this->patchSize);
	if (!this->leaveMemoryUnprotected)
		VirtualProtect(address, this->patchSize, oldAttr, &oldAttr);
	else
		this->oldPageProtection = oldAttr;

	this->isApplied = true;

	if (this->isPermanent && !repatch)
		OP2PatchManager::RemovePatchFromList(this);

	return 1;
}


int OP2Patch::Unpatch(bool doDelete, bool force)
{
	int returnedValue = 1;
	DWORD oldAttr;

	if (this->invalid)
		return 0;

	if (!this->isApplied)
		returnedValue = 1;
	else if (this->isPermanent && !force)
		returnedValue = 0;
	else
	{
		if (!this->leaveMemoryUnprotected)
		{
			if (!VirtualProtect(address, this->patchSize, PAGE_EXECUTE_READWRITE, &oldAttr))
				returnedValue = 0;
		}
		memcpy(this->address, this->oldBytesBuffer, this->patchSize);
		if (!this->leaveMemoryUnprotected)
			VirtualProtect(address, this->patchSize, oldAttr, &oldAttr);
		else
			VirtualProtect(address, this->patchSize, oldPageProtection, &oldAttr);

		this->isApplied = false;
	}

	if (doDelete)
		OP2PatchManager::RemovePatchFromList(this);

	return returnedValue;
}


void* OP2Patch::CopyOriginalBytes(void *buffer, unsigned int sizeOfBuffer)
{
	return memcpy(buffer, this->oldBytesBuffer, ((sizeOfBuffer >= this->patchSize) ? this->patchSize : sizeOfBuffer));
}


void OP2Patch::StoreThis(OP2Patch **reference)
{
	*reference = this;
}


bool OP2Patch::Applied()
{
	return this->isApplied;
}


bool OP2Patch::Valid()
{
	return !(this->invalid);
}


void OP2Patch::SetPermanent(bool permanent)
{
	this->isPermanent = permanent;
}


void OP2Patch::SetLeaveMemoryUnprotected(bool leaveUnprotected)
{
	if (!this->isApplied)
		this->leaveMemoryUnprotected = leaveUnprotected;
}




OP2Patch* OP2PatchManager::Patch(void *address, void *newBytesBuffer, unsigned int sizeOfBytesBuffer, bool onlyQueue)
{
	return OP2PatchManager::Patch(address, newBytesBuffer, NULL, sizeOfBytesBuffer, onlyQueue);
}


// All the OP2PatchManager::Patch* functions (including templated ones) chain down to this function
OP2Patch* OP2PatchManager::Patch(void *address, void *newBytesBuffer, void *expectedBytesBuffer, unsigned int sizeOfBytesBuffer, bool onlyQueue)
{
	address = OP2Addr(address);

	if (address == (void*)NULL)
		return (OP2Patch*)NULL;

	OP2Patch *patch = new OP2Patch(address, newBytesBuffer, expectedBytesBuffer, sizeOfBytesBuffer, !onlyQueue);
	OP2PatchManager::AppendPatchToList(patch);
	return patch;
}


OP2Patch* OP2PatchManager::PatchFunction(void *functionAddress, void *newFunctionAddress, bool onlyQueue)
{
#pragma pack(push,1)
	struct JMP_M32
	{
		unsigned char opcode; // 0xE9
		void *address;
	};
#pragma pack(pop)

	if (functionAddress == NULL || newFunctionAddress == NULL)
		return (OP2Patch*)NULL;

	JMP_M32 code;
	code.opcode = 0xE9;
	code.address = (void*)((char*)newFunctionAddress - ((char*)functionAddress + 5));

	return OP2PatchManager::Patch(functionAddress, &code, sizeof(JMP_M32), onlyQueue);
}


OP2Patch* OP2PatchManager::PatchFunctionCall(void *functionCallAddress, void *newFunctionAddress, bool onlyQueue)
{
#pragma pack(push,1)
	struct JMP_M32
	{
		unsigned char opcode; // 0xE8
		void *address;
	};
#pragma pack(pop)

	if (functionCallAddress == NULL || newFunctionAddress == NULL)
		return (OP2Patch*)NULL;

	JMP_M32 code;
	code.opcode = 0xE8;
	code.address = (void*)((char*)newFunctionAddress - ((char*)functionCallAddress + 5));

	return OP2PatchManager::Patch(functionCallAddress, &code, sizeof(JMP_M32), onlyQueue);
}


OP2Patch* OP2PatchManager::PatchFunctionVirtual(void *vtblAddress, void *functionAddress, void *newFunctionAddress, bool onlyQueue)
{
	if (vtblAddress == NULL || functionAddress == NULL || newFunctionAddress == NULL)
		return (OP2Patch*)NULL;

	void **vtbl = (void**)vtblAddress;

	int i = 0;
	// Iterate through the vtbl until we find the function entry we want to replace
	while (vtbl[i] != functionAddress)
	{
		if (i + 1 > INT_MAX - 1)
			return (OP2Patch*)NULL;
		i ++;
	}

	return OP2PatchManager::PatchFunctionVirtual(vtblAddress, i, newFunctionAddress, onlyQueue);
}


OP2Patch* OP2PatchManager::PatchFunctionVirtual(void *vtblAddress, int vtblEntryIndex, void *newFunctionAddress, bool onlyQueue)
{
	if (vtblAddress == NULL || newFunctionAddress == NULL)
		return (OP2Patch*)NULL;

	void **vtbl = (void**)vtblAddress;

	return OP2PatchManager::Patch(&vtbl[vtblEntryIndex], &newFunctionAddress, sizeof(vtbl[vtblEntryIndex]), onlyQueue);
}


// Patches all crossreferences to a global by scanning the PE base relocation table and patching each address found
OP2Patch* OP2PatchManager::PatchGlobalReferences(void *oldGlobalAddress, void *newGlobalAddress, bool onlyQueue)
{
	#pragma pack(push,1)
	struct TypeOffset
  {
		unsigned short offset	:12; // offset, relative to the VirtualAddress value of the parent IMAGE_BASE_RELOCATION block
		unsigned short type		:4;	// IMAGE_REL_BASED_xxx - usually 3, 0 is sometimes used as a terminator/padder
	};
	#pragma pack(pop)

	OP2Patch *patch, *first = NULL;

	if (!oldGlobalAddress || !newGlobalAddress)
	{
		return NULL;
	}

	// Obtain PE file header info and use it to locate the base relocation table and get its size
	static IMAGE_DOS_HEADER *dosHeader = (IMAGE_DOS_HEADER*)OP2Addr(EXPECTED_OP2_ADDR);	// Call to OP2Addr ensures imageBase is initialized
	if (!dosHeader)
	{
		return NULL;
	}
	static IMAGE_OPTIONAL_HEADER *optHeader = (IMAGE_OPTIONAL_HEADER*)(imageBase + dosHeader->e_lfanew + 0x18);
	static IMAGE_DATA_DIRECTORY *relocDataDir = &optHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC];
	if (!relocDataDir->VirtualAddress || !relocDataDir->Size) // No base relocation table
	{
		return NULL;
	}

	static IMAGE_BASE_RELOCATION *baseRelocTable
		= (IMAGE_BASE_RELOCATION*)(imageBase + relocDataDir->VirtualAddress); // 0x00489000

	IMAGE_BASE_RELOCATION *relocBlock = baseRelocTable; // Relocation table starts with the first block's header
	// Iterate through each relocation table block (for OP2 and most PE apps, each block represents 4096 bytes, i.e. 0x401000-0x402000)
	while ((DWORD)relocBlock < (DWORD)baseRelocTable + relocDataDir->Size - 1 && relocBlock->SizeOfBlock)
	{
		TypeOffset *relocArray = (TypeOffset*)((DWORD)relocBlock + sizeof(IMAGE_BASE_RELOCATION));
		// Iterate through each relocation in this page to find references to the global and replace them
		for (DWORD i = 0; i < (relocBlock->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION)) / sizeof(relocArray[0]); ++i)
		{
			if (relocArray[i].type != IMAGE_REL_BASED_HIGHLOW)
			{
				continue;
			}

			DWORD *addr = (DWORD*)(imageBase + relocBlock->VirtualAddress + relocArray[i].offset);
			if (*addr == (DWORD)oldGlobalAddress) // Global reference found, patch it with a pointer to our global
      {
				if ((patch = Patch(addr,
						&newGlobalAddress,
						&oldGlobalAddress,
						sizeof(newGlobalAddress),
						onlyQueue)))
				{
					if (!first)
					{
						first = patch;
					}
				}
				else // Failed to apply patch
				{
					while (first) // Clean up any previously applied reference patches
					{
						OP2Patch *delPatch = first;
						first = first->previous;
						delPatch->Unpatch(true);
					}
					return NULL;
				}
			}
		}

		// Set pointer to next relocation table block
		relocBlock = (IMAGE_BASE_RELOCATION*)((DWORD)relocBlock + relocBlock->SizeOfBlock);
	}

	return first; // You can do while (first) { [...] first = first->previous; } to get all the reference patches
								//	(if done before any new patches are applied)
								// If no patches applied/global not found in relocation table, this returned value will be NULL
}


int OP2PatchManager::DoPatchAll()
{
	if (OP2PatchManager::numPatches <= 0)
		return 0;

	int returnedValue = 1, numErrors = 0;
	OP2Patch *curPatch = OP2PatchManager::patchListHead;

	while (curPatch != NULL)
	{
		if (!curPatch->Applied())
		{
			if (curPatch->Patch() == 0)
			{
				returnedValue = 0;
				numErrors ++;
			}
		}

		curPatch = curPatch->next;
	}

	return returnedValue;
}


int OP2PatchManager::DoUnpatchAll(bool doDelete, bool force)
{
	if (OP2PatchManager::numPatches <= 0)
		return 0;

	int returnedValue = 1, numErrors = 0;
	OP2Patch *curPatch = OP2PatchManager::patchListHead;
	OP2Patch *nextPatch = NULL;

	while (curPatch != NULL)
	{
		nextPatch = curPatch->next;

		if (curPatch->Applied())
		{
			if (curPatch->Unpatch(doDelete, force) == 0)
			{
				returnedValue = 0;
				numErrors ++;
			}
		}

		curPatch = nextPatch;
	}

	return returnedValue;
}


OP2Patch* OP2PatchManager::GetNextPatchAddress()
{
	if (OP2PatchManager::nextPatch == (OP2Patch*)NULL)
		OP2PatchManager::AllocateNextPatch();
	return OP2PatchManager::nextPatch;
}


void OP2PatchManager::AppendPatchToList(OP2Patch *patch)
{
	if (OP2PatchManager::patchListHead != NULL)
		OP2PatchManager::patchListHead->previous = patch;

	patch->next = OP2PatchManager::patchListHead;
	OP2PatchManager::patchListHead = patch;

	OP2PatchManager::numPatches ++;
}


void OP2PatchManager::RemovePatchFromList(OP2Patch *patch, bool forceUnpatch)
{
	if (OP2PatchManager::patchListHead == patch)
		OP2PatchManager::patchListHead = OP2PatchManager::patchListHead->next;

	if (patch->previous != NULL)
		patch->previous->next = patch->next;
	if (patch->next != NULL)
		patch->next->previous = patch->previous;

	if (forceUnpatch && patch->Applied())
		patch->Unpatch(false, true);

	OP2PatchManager::numPatches --;

	delete patch;
}


void OP2PatchManager::AllocateNextPatch()
{
	OP2PatchManager::nextPatch = (OP2Patch*)NULL; //##**IMPLEMENT
}


void OP2PatchManager::InitModuleBase()
{
	OP2PatchManager::imageBase = (DWORD)GetModuleHandle("Outpost2.exe");
}


DWORD OP2Addr(DWORD address)
{
	if (OP2PatchManager::imageBase == NULL)
		OP2PatchManager::InitModuleBase();
	if (OP2PatchManager::imageBase == NULL) // Outpost2 not found
		return NULL;

	return (DWORD)(address - EXPECTED_OP2_ADDR + OP2PatchManager::imageBase);
}


void* OP2Addr(void *address)
{
	return (void*)OP2Addr((DWORD)address);
}