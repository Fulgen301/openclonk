/*
 * OpenClonk, http://www.openclonk.org
 *
 * Copyright (c) 1998-2000  Matthes Bender
 * Copyright (c) 2001, 2005, 2007  Sven Eberhardt
 * Copyright (c) 2009  Günther Brammer
 * Copyright (c) 2001-2009, RedWolf Design GmbH, http://www.clonk.de
 *
 * Portions might be copyrighted by other authors who have contributed
 * to OpenClonk.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 * See isc_license.txt for full license and disclaimer.
 *
 * "Clonk" is a registered trademark of Matthes Bender.
 * See clonk_trademark_license.txt for full license.
 */

/* Mouse input */

#ifndef INC_C4MouseControl
#define INC_C4MouseControl

#include <C4Facet.h>
#include "C4ObjectList.h"

const int32_t C4MC_Button_None        = 0,
              C4MC_Button_LeftDown    = 1,
              C4MC_Button_LeftUp      = 2,
              C4MC_Button_RightDown   = 3,
              C4MC_Button_RightUp     = 4,
              C4MC_Button_LeftDouble  = 5,
              C4MC_Button_RightDouble = 6,
              C4MC_Button_Wheel       = 7,
              C4MC_Button_MiddleDown  = 8,
              C4MC_Button_MiddleUp    = 9;

const int32_t C4MC_DragSensitivity = 5;

const int32_t C4MC_MD_DragSource = 1,
              C4MC_MD_DropTarget = 2,
              C4MC_MD_NoClick = 4;

class C4MouseControl
{
	friend class C4Viewport;
public:
	C4MouseControl();
	~C4MouseControl();
protected:
	bool Active;
	bool fMouseOwned;
	int32_t Player;
	C4Player *pPlayer; // valid during Move()
	C4Viewport *Viewport; // valid during Move()

	int32_t Cursor;

	StdCopyStrBuf Caption;
	int32_t CaptionBottomY;
	int32_t KeepCaption;

	int32_t VpX,VpY; // Pixel coordinates of mouse pos
	float ViewX,ViewY; // Game coordinate scrolling offset of viewport
	float GameX,GameY; // Game coordinates of mouse pos
	float GuiX,GuiY; // GUI coorindates of mouse pos
	C4Facet fctViewport, fctViewportGame, fctViewportGUI;

	float DownX,DownY; // Game coordinates of mouse-down-pos while dragging

	int32_t ScrollSpeed;
	int32_t Drag;

	bool LeftButtonDown,RightButtonDown,LeftDoubleIgnoreUp;
	bool ButtonDownOnSelection;
	bool ControlDown;
	bool ShiftDown;
	bool AltDown;
	bool Scrolling;
	bool InitCentered;
	bool FogOfWar;
	bool Visible;

	C4ObjectList Selection; //obsolete!

	C4Object *DragObject;
	C4ID DragID;
	C4Def* DragImageDef;
	C4Object* DragImageObject;

	// Target object
	C4Object *TargetObject; // valid during Move()
	C4Object *DownTarget;
	int32_t TimeOnTargetObject;
public:
	void Default();
	void Clear();
	bool Init(int32_t iPlayer);
	void Execute();
	const char *GetCaption();
	void HideCursor();
	void ShowCursor();
	void Draw(C4TargetFacet &cgo, const ZoomData &GameZoom);
	void Move(int32_t iButton, int32_t iX, int32_t iY, DWORD dwKeyFlags, bool fCenter = false);
	void DoMoveInput();
	bool IsViewport(C4Viewport *pViewport);
	void ClearPointers(C4Object *pObj);
	void UpdateClip();  // update clipping region for mouse cursor
	void SetOwnedMouse(bool fToVal) { fMouseOwned = fToVal; }
	bool IsMouseOwned() { return fMouseOwned; }
	bool IsActive() { return !!Active; }
	bool GetLastGUIPos(int32_t *x_out, int32_t *y_out) const;
protected:
	void SendPlayerSelectNext();
	void UpdateFogOfWar();
	void RightUpDragNone();
	void ButtonUpDragScript();
	void LeftUpDragNone();
	void DragScript();
	void Wheel(DWORD dwFlags);
	void RightUp();
	void RightDown();
	void LeftDouble();
	void DragNone();
	void LeftUp();
	void LeftDown();
	void UpdateScrolling();
	void UpdateCursorTarget();
	void SendCommand(int32_t iCommand, int32_t iX=0, int32_t iY=0, C4Object *pTarget=NULL, C4Object *pTarget2=NULL, int32_t iData=0, int32_t iAddMode=C4P_Command_Set);
	int32_t UpdateObjectSelection();
	int32_t UpdateCrewSelection();
	int32_t UpdateSingleSelection();
	bool SendControl(int32_t iCom, int32_t iData=0);
	bool IsValidMenu(C4Menu *pMenu);
	bool UpdatePutTarget(bool fVehicle);
	C4Object *GetTargetObject(); // get MouseSelection object at position
	bool IsPassive(); // return whether mouse is only used to look around
	void ScrollView(float iX, float iY, float ViewWdt, float ViewHgt); // in landscape coordinates

public:
	bool IsDragging();
	int32_t GetPlayer() { return Player; }
};

extern C4MouseControl MouseControl;
#endif
