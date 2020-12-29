#ifndef OP2GRAPHICS_H
#define OP2GRAPHICS_H

#pragma pack(push,1)

struct OP2Rect
{
	int left;
	int top;
	int right;
	int bottom;
};


struct OP2FrameComponentInfo
{
	short pixelXOffset;
	short pixelYOffset;
	short imageInfoIndex;
};


struct OP2FrameOptionalInfo
{
	char offsetX;
	char offsetY;
	char unknown[2]; // **
};


struct OP2FrameInfo
{
	struct
	{
		short left;
		short top;
		short right;
		short bottom;
	} rect;
	short numFrameComponents;
	OP2FrameComponentInfo *frameComponentInfo;
};


struct OP2Graphics
{
	char *unk_palData; // **
	int unk_palSize; // **
	struct ImageInfo
	{
		int scanlineByteWidth;
		int dataOffset;
		int height;
		int width;
		short typeFlags;
		short paletteIndex;
	} imageInfo[5608];
	int numImages;
	struct AnimationInfo
	{
		int unknown; // **
		OP2Rect selectionBox;
		int pixelXDisplacement;
		int pixelYDisplacement;
		int unknown2; // **
		int numFrames;
		OP2FrameInfo **frameInfo;
		short frameOptionalInfoStartIndex;
	} animationInfo[2176];
	int numAnimations;
	void *bmpManager;
	OP2FrameInfo **frameInfo;
	OP2FrameComponentInfo **frameComponentInfo;
	OP2FrameOptionalInfo **frameOptionalInfo;
};

#pragma pack(pop)

#endif