#ifndef HOOKHELPER_H
#define HOOKHELPER_H

#include <windows.h>
#include <limits.h>

#define EXPECTED_OP2_ADDR 0x00400000


class OP2Patch;
class OP2PatchManager;

// Do not instantiate this class yourself. Use the OP2PatchManager class to do so.
class OP2Patch
{
	// Member functions
public:
	OP2Patch(void *address, void *newBytes, void *expectedBytes, unsigned int sizeOfBytes, bool doApply);
	OP2Patch& operator= (const OP2Patch &source);
	~OP2Patch();

	int Patch(bool repatch = false);
	int Unpatch(bool doDelete, bool force = false);

	void* CopyOriginalBytes(void *buffer, unsigned int sizeOfBuffer);
	template <typename ReturnType>
		ReturnType GetOriginalBytes();

	void StoreThis(OP2Patch **reference);
	bool Applied();
	bool Valid();

	void SetPermanent(bool permanent);
	void SetLeaveMemoryUnprotected(bool leaveUnprotected);

	inline OP2Patch* GetNext() {return this->next;}
	inline OP2Patch* GetPrevious() {return this->previous;}

	// Member variables
private:
	OP2Patch *next;
	OP2Patch *previous;

	void *address;
	unsigned int patchSize;
	unsigned char *oldBytesBuffer;
	DWORD oldPageProtection;
	unsigned char *newBytesBuffer;

	bool isApplied;
	bool isPermanent;				// If the patch is set to permanent before being applied, the object will be deleted upon application;
									// if set to be permanent after application, the object will be preserved
	bool leaveMemoryUnprotected;
	bool invalid;

	// Friend classes
	friend class OP2PatchManager;
};


class OP2PatchManager
{
	// Static functions
public:
	static OP2Patch* Patch(void *address, void *newBytesBuffer, void *expectedBytesBuffer, unsigned int sizeOfBytesBuffer, bool onlyQueue);
	static OP2Patch* Patch(void *address, void *newBytesBuffer, unsigned int sizeOfBytesBuffer, bool onlyQueue);
	template <typename PatchType>
		static OP2Patch* Patch(void *address, PatchType newBytes, PatchType expectedBytes, bool onlyQueue);
	template <typename PatchType>
		static OP2Patch* Patch(void *address, PatchType newBytes, bool onlyQueue);

	static OP2Patch* PatchFunction(void *functionAddress, void *newFunctionAddress, bool onlyQueue);
	static OP2Patch* PatchFunctionCall(void *functionCallAddress, void *newFunctionAddress, bool onlyQueue);
	template <class Class>
		static OP2Patch* PatchFunctionVirtual(void *functionAddress, void *newFunctionAddress, bool onlyQueue);
	template <class Class>
		static OP2Patch* PatchFunctionVirtual(Class &classObj, void *functionAddress, void *newFunctionAddress, bool onlyQueue);
	template <class Class>
		static OP2Patch* PatchFunctionVirtual(int vtblEntryIndex, void *newFunctionAddress, bool onlyQueue);
	template <class Class>
		static OP2Patch* PatchFunctionVirtual(Class &classObj, int vtblEntryIndex, void *newFunctionAddress, bool onlyQueue);
	static OP2Patch* PatchFunctionVirtual(void *vtblAddress, void *functionAddress, void *newFunctionAddress, bool onlyQueue);
	static OP2Patch* PatchFunctionVirtual(void *vtblAddress, int vtblEntryIndex, void *newFunctionAddress, bool onlyQueue);

	static OP2Patch* PatchGlobalReferences(void *oldGlobalAddress, void *newGlobalAddress, bool onlyQueue);

	static int DoPatchAll();
	static int DoUnpatchAll(bool doDelete, bool force = false);

	static OP2Patch* GetNextPatchAddress();

private:
	static void AppendPatchToList(OP2Patch *patch);
	static void RemovePatchFromList(OP2Patch *patch, bool forceUnpatch = false);

	static void AllocateNextPatch();

	static void InitModuleBase();

	// Static variables
private:
	static OP2Patch *patchListHead;				// Latest applied patch
	static int numPatches;

	static OP2Patch *nextPatch;					// Next allocated block of memory to store a new patch in

	static DWORD imageBase;

public:
	static void *_memberFunctionToPatch;
	static void *_newMemberFunctionAddress;

	// Dummy function
public:
	static inline void null() { return; };

	// Friend classes
	friend class OP2Patch;

	// Friend functions
	friend DWORD OP2Addr(DWORD address);
	friend void* OP2Addr(void *address);
};


// Templates and inlined functions must be declared and defined in the same file
template <typename ReturnType>
ReturnType OP2Patch::GetOriginalBytes()
{
	ReturnType returnedValue;
	OP2Patch::CopyOriginalBytes(&returnedValue, sizeof(ReturnType));
	return returnedValue;
}


template <typename PatchType>
OP2Patch* OP2PatchManager::Patch(void *address, PatchType newBytes, bool onlyQueue)
{
	return OP2PatchManager::Patch(address, &newBytes, NULL, sizeof(PatchType), onlyQueue); 
}


template <typename PatchType>
OP2Patch* OP2PatchManager::Patch(void *address, PatchType newBytes, PatchType expectedBytes, bool onlyQueue)
{
	return OP2PatchManager::Patch(address, &newBytes, &expectedBytes, sizeof(PatchType), onlyQueue); 
}


template <class Class>
OP2Patch* OP2PatchManager::PatchFunctionVirtual(void *functionAddress, void *newFunctionAddress, bool onlyQueue)
{
	Class classObj; // Dummy for getting the vtbl pointer, doesn't work if the class has no default constructor
	return OP2PatchManager::PatchFunctionVirtual<Class>(classObj, functionAddress, newFunctionAddress, onlyQueue);
}


template <class Class>
OP2Patch* OP2PatchManager::PatchFunctionVirtual(Class &classObj, void *functionAddress, void *newFunctionAddress, bool onlyQueue)
{
	void **vtbl = (void**&)classObj;
	return OP2PatchManager::PatchFunctionVirtual((void*)vtbl, functionAddress, newFunctionAddress, onlyQueue);
}


template <class Class>
OP2Patch* OP2PatchManager::PatchFunctionVirtual(int vtblEntryIndex, void *newFunctionAddress, bool onlyQueue)
{
	Class classObj; // Dummy for getting the vtbl pointer, doesn't work if the class has no default constructor
	return OP2PatchManager::PatchFunctionVirtual<Class>(classObj, vtblEntryIndex, newFunctionAddress, onlyQueue);
}


template <class Class>
OP2Patch* OP2PatchManager::PatchFunctionVirtual(Class &classObj, int vtblEntryIndex, void *newFunctionAddress, bool onlyQueue)
{
	void **vtbl = (void**&)classObj;
	return OP2PatchManager::PatchFunctionVirtual(&vtbl[vtblEntryIndex], vtblEntryIndex, newFunctionAddress, onlyQueue);
}


// Ultra-nasty workaround for the compiler not wanting to treat Class::Function as a pointer to the function (cannot use &(void*&)Class:Function
//  on virtual functions to get the true address of the function; you will get the address of a specially-generated thunk). There doesn't appear
//  to be any way to pass Class::Function as a function argument either, thus the macro.
// Use them as ex. OP2PatchManager::PatchMemberFunction(...);
// If you need a reference to the OP2Patch object, then use OP2Patch::StoreThis ex. OP2Patch *reference; OP2PatchManager::PatchMemberFunction(...).StoreThis(&reference);
// Another caveat is that, if you use these inside conditional statements, you are required to enclose the conditional body within brackets
//  (even though it only looks like one line of code in the written syntax).
#define _GetFunctionPtr(function, pointer)														\
	do {																						\
		__asm mov eax, function																	\
		__asm mov pointer, eax																	\
	} while(0)
#define _StoreFunctionPtrs(function, newFunction)												\
	do {																						\
		_GetFunctionPtr(function, OP2PatchManager::_memberFunctionToPatch);						\
		_GetFunctionPtr(newFunction, OP2PatchManager::_newMemberFunctionAddress);				\
	} while(0)

// OP2PatchManager::PatchMemberFunction
#define PatchMemberFunction(Class, functionToPatch, newFunction, onlyQueue)						\
	null();																						\
	_StoreFunctionPtrs(Class::functionToPatch, newFunction);									\
	OP2PatchManager::PatchFunction(																\
		OP2PatchManager::_memberFunctionToPatch,												\
		OP2PatchManager::_newMemberFunctionAddress,												\
		onlyQueue																				\
	)

// OP2PatchManager::PatchVirtualFunction
#define PatchVirtualFunction(Class, functionToPatch, newFunction, onlyQueue)					\
	null();																						\
	_StoreFunctionPtrs(Class::functionToPatch, newFunction);									\
	OP2PatchManager::PatchFunctionVirtual<Class>(												\
		OP2PatchManager::_memberFunctionToPatch,												\
		OP2PatchManager::_newMemberFunctionAddress,												\
		onlyQueue																				\
	)

// OP2PatchManager::PatchObjectVirtualFunction
#define PatchObjectVirtualFunction(obj, Class, functionToPatch, newFunction, onlyQueue)			\
	null();																						\
	_StoreFunctionPtrs(Class::functionToPatch, newFunction);									\
	OP2PatchManager::PatchFunctionVirtual<Class>(												\
		obj,																					\
		OP2PatchManager::_memberFunctionToPatch,												\
		OP2PatchManager::_newMemberFunctionAddress,												\
		onlyQueue																				\
	)

// OP2PatchManager::PatchReferencedVirtualFunction
// To use this you need to make a struct filled with function pointers that represents the vftable you want to hook
#define PatchReferencedVirtualFunction(referencedFunctionToPatch, newFunction, onlyQueue)		\
	null();																						\
	_GetFunctionPtr(newFunction, OP2PatchManager::_newMemberFunctionAddress);					\
	OP2PatchManager::Patch(																		\
		&referencedFunctionToPatch,																\
		&OP2PatchManager::_newMemberFunctionAddress,											\
		sizeof(void*),																			\
		onlyQueue																				\
	)

#endif