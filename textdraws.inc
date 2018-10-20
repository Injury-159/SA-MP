#if defined _inc_textdraws_
	#endinput
#endif
#define _inc_textdraws_


#include <a_samp>


#if !defined __MEMORY_INCLUDED__
	#tryinclude <memory>
	#if !defined __MEMORY_INCLUDED__
		#error Memory plugin by BigETI not found.
	#endif
#endif

#if !defined _inc_y_iterate
	#tryinclude <YSI_Data\y_iterate>
	#if !defined _inc_y_iterate
 		#error YSI package not found.
	#endif
#endif

forward PlayerText:TDS_Process_PTD(playerid, PlayerText:textid, bool:update_pos);
forward Text:TDS_Process_TD(Text:textid, bool:update_pos);


#if !defined MAX_TEXT_DRAW_LENGTH
	#define MAX_TEXT_DRAW_LENGTH  		 	(1024)
#endif


#define INVALID_PLAYER_TEXT_DRAW 			PlayerText:INVALID_TEXT_DRAW
#define INVALID_GLOBAL_TEXT_DRAW 			Text:INVALID_TEXT_DRAW


static 
	Iterator:Player_TDs[MAX_PLAYERS]<MAX_PLAYER_TEXT_DRAWS>,
	Iterator:Global_TDs<MAX_TEXT_DRAWS>,

	Pointer:Pointer_PTDs[MAX_PLAYERS][MAX_PLAYER_TEXT_DRAWS],
	Pointer:Pointer_GTDs[MAX_TEXT_DRAWS]
;

enum
{
	TD_INDEX_TEXT,
	TD_INDEX_POS_X,
	TD_INDEX_POS_Y,
	TD_INDEX_LX,
	TD_INDEX_LY,
	TD_INDEX_TX,
	TD_INDEX_TY,
	TD_INDEX_COLOR,
	TD_INDEX_FONT,
	TD_INDEX_BOX_COLOR,
	TD_INDEX_BACK_COLOR,
	TD_INDEX_BOX,
	TD_INDEX_SELECTED,
	TD_INDEX_PROPORTIONAL,
	TD_INDEX_ALIGNMENT,
	TD_INDEX_SHADOW,
	TD_INDEX_OUTLINE,
	TD_INDEX_MODEL_ID,
	TD_INDEX_MODEL_RX,
	TD_INDEX_MODEL_RY,
	TD_INDEX_MODEL_RZ,
	TD_INDEX_MODEL_ZOOM,
	TD_INDEX_MODEL_VEH_1,
	TD_INDEX_MODEL_VEH_2
};


/* ————————————————————————————————————————————————————————————————————————————————————————————————

# FUNCTIONS PLAYER TEXT DRAWS

————————————————————————————————————————————————————————————————————————————————————————————————— */

stock TDS_Valid_PTD(playerid, PlayerText:index)
	return Iter_Contains(Player_TDs[playerid], _:index);


stock PlayerText:TDS_Create_PTD(playerid, Float:x, Float:y, string[])
{
	new idx;
	if((idx = Iter_Alloc(Player_TDs[playerid])) > -1)
	{
		new Pointer:pointer = MEM_new(24);
		MEM_set_val(pointer, TD_INDEX_TEXT,         _:MEM_new_arr(string, strlen(string) + 1));
		MEM_set_val(pointer, TD_INDEX_POS_X,        _:x);
		MEM_set_val(pointer, TD_INDEX_POS_Y,        _:y);
		MEM_set_val(pointer, TD_INDEX_LX,           _:0.48);
		MEM_set_val(pointer, TD_INDEX_LY,           _:1.12);
		MEM_set_val(pointer, TD_INDEX_TX,		    _:1280.0);
		MEM_set_val(pointer, TD_INDEX_TY,		    _:1280.0);
		MEM_set_val(pointer, TD_INDEX_COLOR,        0xE1E1E1FF);
		MEM_set_val(pointer, TD_INDEX_FONT,         1);
		MEM_set_val(pointer, TD_INDEX_BOX_COLOR,    0x80808080);
		MEM_set_val(pointer, TD_INDEX_BACK_COLOR,   0x000000FF);
		MEM_set_val(pointer, TD_INDEX_BOX,			0);
		MEM_set_val(pointer, TD_INDEX_SELECTED,     0);
		MEM_set_val(pointer, TD_INDEX_PROPORTIONAL, 1);
		MEM_set_val(pointer, TD_INDEX_ALIGNMENT,    0);
		MEM_set_val(pointer, TD_INDEX_SHADOW,       2);
		MEM_set_val(pointer, TD_INDEX_OUTLINE,      0);
		MEM_set_val(pointer, TD_INDEX_MODEL_ID,     0);
		MEM_set_val(pointer, TD_INDEX_MODEL_RX,     _:0.0);
		MEM_set_val(pointer, TD_INDEX_MODEL_RY,     _:0.0);
		MEM_set_val(pointer, TD_INDEX_MODEL_RZ,     _:0.0);
		MEM_set_val(pointer, TD_INDEX_MODEL_ZOOM,   _:1.0);
		MEM_set_val(pointer, TD_INDEX_MODEL_VEH_1,	65535);
		MEM_set_val(pointer, TD_INDEX_MODEL_VEH_2,  65535);

		Pointer_PTDs[playerid][idx] = pointer;

		TDS_Process_PTD(playerid, PlayerText:idx, true);
		return PlayerText:idx;
	}
	return INVALID_PLAYER_TEXT_DRAW;
}

stock TDS_Destroy_PTD(playerid, PlayerText:index)
{
	if(!TDS_Valid_PTD(playerid, index))
		return 0;

	PlayerTextDrawDestroy(playerid, index);

	MEM_delete(Pointer:MEM_get_val(Pointer_PTDs[playerid][_:index], TD_INDEX_TEXT));	
	MEM_delete(Pointer_PTDs[playerid][index]);

	Pointer_PTDs[playerid][_:index] = MEM_NULLPTR;
	Iter_Remove(Player_TDs[playerid], _:index);
	return 1;
}

stock TDS_SetString_PTD(playerid, PlayerText:index, string[])
{
	if(!TDS_Valid_PTD(playerid, index))
		return 0;

	new 
		Pointer:p = Pointer_PTDs[playerid][_:index];

		
	MEM_delete(Pointer:MEM_get_val(p, TD_INDEX_TEXT));
	MEM_set_val(p, TD_INDEX_TEXT, _:MEM_new_arr(string, strlen(string) + 1));

	PlayerTextDrawSetString(playerid, index, string);	
	return 1;		
}

stock TDS_GetString_PTD(playerid, PlayerText:index, string[])
{
	if(!TDS_Valid_PTD(playerid, index))
		return 0;

	new Pointer:tmp = Pointer:MEM_get_val(Pointer_PTDs[playerid][_:index], TD_INDEX_TEXT);
	MEM_get_arr(tmp, 0, string, MEM_get_size(tmp));
	return 1;
}

stock TDS_SetPos_PTD(playerid, PlayerText:index, Float:newx, Float:newy)
{
	if(!TDS_Valid_PTD(playerid, index))
		return 0;

	MEM_set_val(Pointer_PTDs[playerid][_:index], TD_INDEX_POS_X, _:newx);
	MEM_set_val(Pointer_PTDs[playerid][_:index], TD_INDEX_POS_Y, _:newy);

	return (TDS_Process_PTD(playerid, index, true) != INVALID_PLAYER_TEXT_DRAW);
}

stock TDS_GetPos_PTD(playerid, PlayerText:index, &Float:x, &Float:y)
{
	if(!TDS_Valid_PTD(playerid, index))
		return 0;

	x = Float:MEM_get_val(Pointer_PTDs[playeyrid][_:index], TD_INDEX_POS_X);
	y = Float:MEM_get_val(Pointer_PTDs[playeyrid][_:index], TD_INDEX_POS_Y);

	return 1;
}

stock TDS_SetLetterSize_PTD(playerid, PlayerText:index, Float:lx, Float:ly)
{
	if(!TDS_Valid_PTD(playerid, index))
		return 0;

	MEM_set_val(Pointer_PTDs[playerid][_:index], TD_INDEX_LX, _:lx);
	MEM_set_val(Pointer_PTDs[playerid][_:index], TD_INDEX_LY, _:ly);

	PlayerTextDrawLetterSize(playerid, index, lx, ly);
	return 1;		
}

stock TDS_GetLetterSize_PTD(playerid, PlayerText:index, &Float:lx, &Float:ly)
{
	if(!TDS_Valid_PTD(playerid, index))
		return 0;

	lx = Float:MEM_get_val(Pointer_PTDs[playerid][_:index], TD_INDEX_LX);
	ly = Float:MEM_get_val(Pointer_PTDs[playerid][_:index], TD_INDEX_LY);	
	return 1;		
}

stock TDS_SetTextSize_PTD(playerid, PlayerText:index, Float:tx, Float:ty)
{
	if(!TDS_Valid_PTD(playerid, index))
		return 0;

	MEM_set_val(Pointer_PTDs[playerid][_:index], TD_INDEX_TX, _:tx);
	MEM_set_val(Pointer_PTDs[playerid][_:index], TD_INDEX_TY, _:ty);

	PlayerTextDrawTextSize(playerid, index, tx, ty);
	return 1;		
}

stock TDS_GetTextSize_PTD(playerid, PlayerText:index, &Float:tx, &Float:ty)
{
	if(!TDS_Valid_PTD(playerid, index))
		return 0;

	tx = Float:MEM_get_val(Pointer_PTDs[playerid][_:index], TD_INDEX_TX);
	ty = Float:MEM_get_val(Pointer_PTDs[playerid][_:index], TD_INDEX_TY);

	return 1;		
}

stock TDS_SetColor_PTD(playerid, PlayerText:index, color)
{
	if(!TDS_Valid_PTD(playerid, index))
		return 0;

	MEM_set_val(Pointer_PTDs[playerid][_:index], TD_INDEX_COLOR, color);

	PlayerTextDrawColor(playerid, index, color);
	return 1;		
}

stock TDS_GetColor_PTD(playerid, PlayerText:index)
{
	if(!TDS_Valid_PTD(playerid, index))
		return 0;

	return MEM_get_val(Pointer_PTDs[playerid][_:index], TD_INDEX_COLOR);		
}

stock TDS_SetFont_PTD(playerid, PlayerText:index, font)
{
	if(!TDS_Valid_PTD(playerid, index))
		return 0;

	MEM_set_val(Pointer_PTDs[playerid][_:index], TD_INDEX_FONT, font);

	PlayerTextDrawFont(playerid, index, font);
	return 1;		
}

stock TDS_GetFont_PTD(playerid, PlayerText:index)
{
	if(!TDS_Valid_PTD(playerid, index))
		return 0;

	return MEM_get_val(Pointer_PTDs[playerid][_:index], TD_INDEX_FONT);		
}

stock TDS_SetBackColor_PTD(playerid, PlayerText:index, backcolor)
{
	if(!TDS_Valid_PTD(playerid, index))
		return 0;

	MEM_set_val(Pointer_PTDs[playerid][_:index], TD_INDEX_BACK_COLOR, backcolor);

	PlayerTextDrawBackgroundColor(playerid, index, backcolor);
	return 1;		
}

stock TDS_GetBackColor_PTD(playerid, PlayerText:index)
{
	if(!TDS_Valid_PTD(playerid, index))
		return 0;

	return MEM_get_val(Pointer_PTDs[playerid][_:index], TD_INDEX_BACK_COLOR);		
}

stock TDS_SetUseBox_PTD(playerid, PlayerText:index, usebox)
{
	if(!TDS_Valid_PTD(playerid, index))
		return 0;

	MEM_set_val(Pointer_PTDs[playerid][_:index], TD_INDEX_BOX, !!usebox);

	PlayerTextDrawUseBox(playerid, index, !!usebox);
	return 1;
}

stock TDS_GetUseBox_PTD(playerid, PlayerText:index)
{
	if(!TDS_Valid_PTD(playerid, index))
		return 0;

	return MEM_get_val(Pointer_PTDs[playerid][_:index], TD_INDEX_BOX);		
}

stock TDS_SetBoxColor_PTD(playerid, PlayerText:index, boxcolor)
{
	if(!TDS_Valid_PTD(playerid, index))
		return 0;

	MEM_set_val(Pointer_PTDs[playerid][_:index], TD_INDEX_BOX_COLOR, boxcolor);

	PlayerTextDrawBoxColor(playerid, index, boxcolor);
	return 1;		
}

stock TDS_GetBoxColor_PTD(playerid, PlayerText:index)
{
	if(!TDS_Valid_PTD(playerid, index))
		return 0;

	return MEM_get_val(Pointer_PTDs[playerid][_:index], TD_INDEX_BOX_COLOR);		
}

stock TDS_SetSelectable_PTD(playerid, PlayerText:index, select)
{
	if(!TDS_Valid_PTD(playerid, index))
		return 0;

	MEM_set_val(Pointer_PTDs[playerid][_:index], TD_INDEX_SELECTED, !!select);

	PlayerTextDrawSetSelectable(playerid, index, !!select);
	return 1;		
}

stock TDS_GetSelectable_PTD(playerid, PlayerText:index)
{
	if(!TDS_Valid_PTD(playerid, index))
		return 0;

	return MEM_get_val(Pointer_PTDs[playerid][_:index], TD_INDEX_SELECTED);		
}

stock TDS_SetProportional_PTD(playerid, PlayerText:index, proportional)
{
	if(!TDS_Valid_PTD(playerid, index))
		return 0;

	MEM_set_val(Pointer_PTDs[playerid][_:index], TD_INDEX_PROPORTIONAL, !!proportional);

	PlayerTextDrawSetProportional(playerid, index, !!proportional);
	return 1;		
}

stock TDS_GetProportional_PTD(playerid, PlayerText:index)
{
	if(!TDS_Valid_PTD(playerid, index))
		return 0;

	return MEM_get_val(Pointer_PTDs[playerid][_:index], TD_INDEX_SELECTED);		
}

stock TDS_SetAlignment_PTD(playerid, PlayerText:index, alignment)
{
	if(!TDS_Valid_PTD(playerid, index))
		return 0;

	MEM_set_val(Pointer_PTDs[playerid][_:index], TD_INDEX_ALIGNMENT, alignment);

	PlayerTextDrawAlignment(playerid, index, alignment);
	return 1;		
}

stock TDS_GetAlignment_PTD(playerid, PlayerText:index)
{
	if(!TDS_Valid_PTD(playerid, index))
		return 0;

	return MEM_get_val(Pointer_PTDs[playerid][_:index], TD_INDEX_ALIGNMENT);		
}

stock TDS_SetShadow_PTD(playerid, PlayerText:index, shadow)
{
	if(!TDS_Valid_PTD(playerid, index))
		return 0;

	MEM_set_val(Pointer_PTDs[playerid][_:index], TD_INDEX_SHADOW, shadow);

	PlayerTextDrawSetShadow(playerid, index, shadow);
	return 1;		
}

stock TDS_GetShadow_PTD(playerid, PlayerText:index)
{
	if(!TDS_Valid_PTD(playerid, index))
		return 0;

	return MEM_get_val(Pointer_PTDs[playerid][_:index], TD_INDEX_SHADOW);		
}

stock TDS_SetOutline_PTD(playerid, PlayerText:index, outline)
{
	if(!TDS_Valid_PTD(playerid, index))
		return 0;

	MEM_set_val(Pointer_PTDs[playerid][_:index], TD_INDEX_OUTLINE, outline);

	PlayerTextDrawSetOutline(playerid, index, outline);
	return 1;		
}

stock TDS_GetOutline_PTD(playerid, PlayerText:index)
{
	if(!TDS_Valid_PTD(playerid, index))
		return 0;

	return MEM_get_val(Pointer_PTDs[playerid][_:index], TD_INDEX_OUTLINE);		
}

stock TDS_SetPrevModel_PTD(playerid, PlayerText:index, modelid)
{
	if(!TDS_Valid_PTD(playerid, index))
		return 0;

	MEM_set_val(Pointer_PTDs[playerid][_:index], TD_INDEX_MODEL_ID, modelid);

	PlayerTextDrawSetPreviewModel(playerid, index, modelid);
	return 1;		
}

stock TDS_GetPrevModel_PTD(playerid, PlayerText:index)
{
	if(!TDS_Valid_PTD(playerid, index))
		return 0;

	return MEM_get_val(Pointer_PTDs[playerid][_:index], TD_INDEX_MODEL_ID);		
}

stock TDS_SetPrevModelRot_PTD(Text:index, Float:rx, Float:ry, Float:rz, Float:zoom = 1.0)
{
	if(!TDS_Valid_PTD(playerid, index))
		return 0;

	MEM_set_val(Pointer_PTDs[playerid][_:index], TD_INDEX_MODEL_RX, _:rx);
	MEM_set_val(Pointer_PTDs[playerid][_:index], TD_INDEX_MODEL_RY, _:ry);
	MEM_set_val(Pointer_PTDs[playerid][_:index], TD_INDEX_MODEL_RZ, _:rz);
	MEM_set_val(Pointer_PTDs[playerid][_:index], TD_INDEX_MODEL_ZOOM, _:zoom);

	PlayerTextDrawSetPreviewRot(playerid, index, rx, ry, rz, zoom);
	return 1;
}

stock TDS_GetPrevModelRot_PTD(playerid, PlayerText:index, &Float:rx, &Float:ry, &Float:rz, &Float:zoom)
{
	if(!TDS_Valid_PTD(playerid, index))
		return 0;

	rx = Float:MEM_get_val(Pointer_PTDs[playerid][_:index], TD_INDEX_MODEL_RX);
	ry = Float:MEM_get_val(Pointer_PTDs[playerid][_:index], TD_INDEX_MODEL_RY);
	rz = Float:MEM_get_val(Pointer_PTDs[playerid][_:index], TD_INDEX_MODEL_RZ);
	zoom = Float:MEM_get_val(Pointer_PTDs[playerid][_:index], TD_INDEX_MODEL_ZOOM);
	return 1;	
}

stock TDS_SetPrevModelVehCol_PTD(Text:index, color1, color2)
{
	if(!TDS_Valid_PTD(playerid, index))
		return 0;

	MEM_set_val(Pointer_PTDs[playerid][_:index], TD_INDEX_MODEL_VEH_1, color1);
	MEM_set_val(Pointer_PTDs[playerid][_:index], TD_INDEX_MODEL_VEH_2, color2);

	PlayerTextDrawSetPreviewVehCol(playerid, index, color1, color2);
	return 1;
}

stock TDS_GetPrevModelVehCol_PTD(playerid, PlayerText:index, &color1, &color2)
{
	if(!TDS_Valid_PTD(playerid, index))
		return 0;

	color1 = MEM_get_val(Pointer_PTDs[playerid][_:index], TD_INDEX_MODEL_VEH_1);
	color2 = MEM_get_val(Pointer_PTDs[playerid][_:index], TD_INDEX_MODEL_VEH_2);
	return 1;	
}

/* ————————————————————————————————————————————————————————————————————————————————————————————————

# INERNAL FUNCTIONS PLAYER TEXT DRAWS

————————————————————————————————————————————————————————————————————————————————————————————————— */

static stock PlayerText:TDS_Process_PTD(playerid, PlayerText:textid, bool:update_pos)
{
	new 
		Pointer:p = Pointer_PTDs[playerid][_:textid];


	if(update_pos)
	{
		PlayerTextDrawDestroy(playerid, textid);

		new string[MAX_TEXT_DRAW_LENGTH], Pointer:tmp = Pointer:MEM_get_val(p, TD_INDEX_TEXT);
		MEM_get_arr(tmp, 0, string, MEM_get_size(tmp));

		if((textid = CreatePlayerTextDraw(playerid, Float:MEM_get_val(p, TD_INDEX_POS_X), Float:MEM_get_val(p, TD_INDEX_POS_Y), string)) == INVALID_PLAYER_TEXT_DRAW)
			return INVALID_PLAYER_TEXT_DRAW;

	}

	new 
		box = MEM_get_val(p, TD_INDEX_BOX),
		font = MEM_get_val(p, TD_INDEX_FONT)
	;

	PlayerTextDrawColor(playerid, textid, 					    MEM_get_val(p, TD_INDEX_COLOR));
	PlayerTextDrawTextSize(playerid, textid, 				    Float:MEM_get_val(p, TD_INDEX_TX), Float:MEM_get_val(p, TD_INDEX_TY));
	PlayerTextDrawLetterSize(playerid, textid, 				    Float:MEM_get_val(p, TD_INDEX_LX), Float:MEM_get_val(p, TD_INDEX_LY));
	PlayerTextDrawBackgroundColor(playerid, textid, 		    MEM_get_val(p, TD_INDEX_BACK_COLOR));
	PlayerTextDrawSetSelectable(playerid, textid,	    	    MEM_get_val(p, TD_INDEX_SELECTED));
	PlayerTextDrawSetProportional(playerid, textid,			    MEM_get_val(p, TD_INDEX_PROPORTIONAL));
	PlayerTextDrawSetOutline(playerid, textid, 				    MEM_get_val(p, TD_INDEX_OUTLINE));
	PlayerTextDrawSetShadow(playerid, textid, 				    MEM_get_val(p, TD_INDEX_SHADOW));
	PlayerTextDrawFont(playerid, textid, 					    font);
	PlayerTextDrawUseBox(playerid, textid, 					    box);

	if(box)
		PlayerTextDrawBoxColor(playerid, textid, 				MEM_get_val(p, TD_INDEX_BOX_COLOR));

	if(font == 5)
	{
		PlayerTextDrawSetPreviewModel(playerid, textid, 		MEM_get_val(p, TD_INDEX_MODEL_ID));
		PlayerTextDrawSetPreviewRot(playerid, textid, 			Float:MEM_get_val(p, TD_INDEX_MODEL_RX), Float:MEM_get_val(p, TD_INDEX_MODEL_RY), Float:MEM_get_val(p, TD_INDEX_MODEL_RZ), Float:MEM_get_val(p, TD_INDEX_MODEL_ZOOM));
		PlayerTextDrawSetPreviewVehCol(playerid, textid, 		MEM_get_val(p, TD_INDEX_MODEL_VEH_1), MEM_get_val(p, TD_INDEX_MODEL_VEH_2));		
	}

	return textid;
}


/* ————————————————————————————————————————————————————————————————————————————————————————————————

# HOOKS PLAYER TEXT DRAWS

————————————————————————————————————————————————————————————————————————————————————————————————— */

#if defined _ALS_IsValidPlayerTextDraw
	#undef IsValidPlayerTextDraw
#else
	#define _ALS_IsValidPlayerTextDraw
#endif

#define IsValidPlayerTextDraw TDS_Valid_PTD


#if defined _ALS_CreatePlayerTextDraw
  #undef CreatePlayerTextDraw
#else
#define _ALS_CreatePlayerTextDraw
#endif
#define CreatePlayerTextDraw TDS_Create_PTD


#if defined _ALS_PlayerTextDrawDestroy
  #undef PlayerTextDrawDestroy
#else
#define _ALS_PlayerTextDrawDestroy
#endif
#define PlayerTextDrawDestroy TDS_Destroy_PTD


#if defined _ALS_PlayerTextDrawSetString
  #undef PlayerTextDrawSetString
#else
#define _ALS_PlayerTextDrawSetString
#endif
#define PlayerTextDrawSetString TDS_SetString_PTD


#if defined _ALS_PlayerTextDrawGetString
  #undef PlayerTextDrawGetString
#else
#define _ALS_PlayerTextDrawGetString
#endif
#define PlayerTextDrawGetString TDS_GetString_PTD


#if defined _ALS_PlayerTextDrawSetPos
  #undef PlayerTextDrawGetPos
#else
#define _ALS_PlayerTextDrawSetPos
#endif
#define PlayerTextDrawSetPos TDS_SetPos_PTD


#if defined _ALS_PlayerTextDrawGetPos
  #undef PlayerTextDrawGetPos
#else
#define _ALS_PlayerTextDrawGetPos
#endif
#define PlayerTextDrawGetPos TDS_GetPos_PTD


#if defined _ALS_PlayerTextDrawLetterSize
  #undef PlayerTextDrawLetterSize
#else
#define _ALS_PlayerTextDrawLetterSize
#endif
#define PlayerTextDrawLetterSize TDS_SetLetterSize_PTD


#if defined _ALS_GetLetterSize_PTD
  #undef PlayerTextDrawGetLetterSize
#else
#define _ALS_GetLetterSize_PTD
#endif
#define PlayerTextDrawGetLetterSize TDS_GetLetterSize_PTD		


#if defined _ALS_PlayerTextDrawTextSize
  #undef PlayerTextDrawTextSize
#else
#define _ALS_PlayerTextDrawTextSize
#endif
#define PlayerTextDrawTextSize TDS_SetTextSize_PTD


#if defined _ALS_GetTextSize_PTD
  #undef PlayerTextDrawGetTextSize
#else
#define _ALS_GetTextSize_PTD
#endif
#define PlayerTextDrawGetTextSize TDS_GetTextSize_PTD


#if defined _ALS_PlayerTextDrawColor
  #undef PlayerTextDrawColor
#else
#define _ALS_PlayerTextDrawColor
#endif
#define PlayerTextDrawColor TDS_SetColor_PTD


#if defined _ALS_PlayerTextDrawGetColor
  #undef PlayerTextDrawGetColor
#else
#define _ALS_PlayerTextDrawGetColor
#endif
#define PlayerTextDrawGetColor TDS_GetColor_PTD


#if defined _ALS_PlayerTextDrawFont
  #undef PlayerTextDrawFont
#else
#define _ALS_PlayerTextDrawFont
#endif
#define PlayerTextDrawFont TDS_SetFont_PTD


#if defined _ALS_PlayerTextDrawGetFont
  #undef PlayerTextDrawGetFont
#else
#define _ALS_PlayerTextDrawGetFont
#endif
#define PlayerTextDrawGetFont TDS_GetFont_PTD


#if defined _ALS_SetBackColor_PTD
  #undef PlayerTextDrawBackgroundColor
#else
#define _ALS_SetBackColor_PTD
#endif
#define PlayerTextDrawBackgroundColor TDS_SetBackColor_PTD


#if defined _ALS_PlayerTextDrawGetBackColor
  #undef PlayerTextDrawGetBackColor
#else
#define _ALS_PlayerTextDrawGetBackColor
#endif
#define PlayerTextDrawGetBackColor TDS_GetBackColor_PTD


#if defined _ALS_PlayerTextDrawUseBox
  #undef PlayerTextDrawUseBox
#else
#define _ALS_PlayerTextDrawUseBox
#endif
#define PlayerTextDrawUseBox TDS_SetUseBox_PTD


#if defined _ALS_PlayerTextDrawGetUseBox
  #undef PlayerTextDrawGetuseBox
#else
#define _ALS_PlayerTextDrawGetUseBox
#endif
#define PlayerTextDrawGetUseBox TDS_GetUseBox_PTD


#if defined _ALS_PlayerTextDrawBoxColor
  #undef PlayerTextDrawBoxColor
#else
#define _ALS_PlayerTextDrawBoxColor
#endif
#define PlayerTextDrawBoxColor TDS_SetBoxColor_PTD


#if defined _ALS_PlayerTextDrawGetBoxColor
  #undef PlayerTextDrawGetBoxColor
#else
#define _ALS_PlayerTextDrawGetBoxColor
#endif
#define PlayerTextDrawGetBoxColor TDS_GetBoxColor_PTD


#if defined _ALS_SetSelectable_PTD
  #undef PlayerTextDrawSetSelectable
#else
#define _ALS_SetSelectable_PTD
#endif
#define PlayerTextDrawSetSelectable TDS_SetSelectable_PTD


#if defined _ALS_GetSelectable_PTD
  #undef PlayerTextDrawGetSelectable
#else
#define _ALS_GetSelectable_PTD
#endif
#define PlayerTextDrawGetSelectable TDS_GetSelectable_PTD


#if defined _ALS_SetProportional_PTD
  #undef PlayerTextDrawSetProportional
#else
#define _ALS_SetProportional_PTD
#endif
#define PlayerTextDrawSetProportional TDS_SetProportional_PTD


#if defined _ALS_GetProportional_PTD
  #undef PlayerTextDrawGetProportional
#else
#define _ALS_GetProportional_PTD
#endif
#define PlayerTextDrawGetProportional TDS_GetProportional_PTD


#if defined _ALS_PlayerTextDrawAlignment
  #undef PlayerTextDrawAlignment
#else
#define _ALS_PlayerTextDrawAlignment
#endif
#define PlayerTextDrawAlignment TDS_SetAlignment_PTD


#if defined _ALS_PlayerTextDrawGetAlignment
  #undef PlayerTextDrawGetAlignment
#else
#define _ALS_PlayerTextDrawGetAlignment
#endif
#define PlayerTextDrawGetAlignment TDS_GetAlignment_PTD	


#if defined _ALS_PlayerTextDrawSetShadow
  #undef PlayerTextDrawSetShadow
#else
#define _ALS_PlayerTextDrawSetShadow
#endif
#define PlayerTextDrawSetShadow TDS_SetShadow_PTD


#if defined _ALS_PlayerTextDrawShadow
  #undef PlayerTextDrawgetShadow
#else
#define _ALS_PlayerTextDrawGetShadow
#endif
#define PlayerTextDrawGetShadow TDS_GetShadow_PTD


#if defined _ALS_PlayerTextDrawSetOutline
  #undef PlayerTextDrawSetOutline
#else
#define _ALS_PlayerTextDrawSetOutline
#endif
#define PlayerTextDrawSetOutline TDS_SetOutline_PTD


#if defined _ALS_PlayerTextDrawOutline
  #undef PlayerTextDrawgeOutline
#else
#define _ALS_PlayerTextDrawGetOutline
#endif
#define PlayerTextDrawGetOutline TDS_GetOutline_PTD


#if defined _ALS_SetPrevModel_PTD	
  #undef PlayerTextDrawSetPreviewModel
#else
#define _ALS_SetPrevModel_PTD
#endif
#define PlayerTextDrawSetPreviewModel TDS_SetPrevModel_PTD	


#if defined _ALS_GetPrevModel_PTD	
  #undef PlayerTextDrawGetPreviewModel
#else
#define _ALS_GetPrevModel_PTD
#endif
#define PlayerTextDrawGetPreviewModel TDS_GetPrevModel_PTD	


#if defined _ALS_SetPrevModelRot_PTD
  #undef PlayerTextDrawSetPreviewModelRot
#else
#define _ALS_SetPrevModelRot_PTD	
#endif
#define PlayerTextDrawSetPreviewModelRot TDS_SetPrevModelRot_PTD


#if defined _ALS_GetPrevModelRot_PTD
  #undef PlayerTextDrawGetPreviewModelRot
#else
#define _ALS_GetPrevModelRot_PTD	
#endif
#define PlayerTextDrawGetPreviewModelRot TDS_GetPrevModelRot_PTD


#if defined _ALS_SetPrevModelVehCol_PTD	
  #undef PlayerTextDrawSetPreviewVehCol
#else
#define _ALS_SetPrevModelVehCol_PTD	
#endif
#define PlayerTextDrawSetPreviewVehCol TDS_SetPrevModelVehCol_PTD


#if defined _ALS_GetPrevModelVehCol_PTD	
  #undef PlayerTextDrawGetPreviewVehCol
#else
#define _ALS_GetPrevModelVehCol_PTD	
#endif
#define PlayerTextDrawGetPreviewVehCol TDS_GetPrevModelVehCol_PTD


/* ————————————————————————————————————————————————————————————————————————————————————————————————

# FUNCTIONS GLOBAL TEXT DRAWS

————————————————————————————————————————————————————————————————————————————————————————————————— */

stock TDS_Valid_TD(Text:index)
	return Iter_Contains(Global_TDs, _:index);


stock Text:TDS_Create_TD(Float:x, Float:y, string[])
{
	new idx;
	if((idx = Iter_Alloc(Global_TDs)) > -1)
	{
		new Pointer:pointer = MEM_new(24);
		MEM_set_val(pointer, TD_INDEX_TEXT,         _:MEM_new_arr(string, strlen(string) + 1));
		MEM_set_val(pointer, TD_INDEX_POS_X,        _:x);
		MEM_set_val(pointer, TD_INDEX_POS_Y,        _:y);
		MEM_set_val(pointer, TD_INDEX_LX,           _:0.48);
		MEM_set_val(pointer, TD_INDEX_LY,           _:1.12);
		MEM_set_val(pointer, TD_INDEX_TX,		    _:1280.0);
		MEM_set_val(pointer, TD_INDEX_TY,		    _:1280.0);
		MEM_set_val(pointer, TD_INDEX_COLOR,        0xE1E1E1FF);
		MEM_set_val(pointer, TD_INDEX_FONT,         1);
		MEM_set_val(pointer, TD_INDEX_BOX_COLOR,    0x80808080);
		MEM_set_val(pointer, TD_INDEX_BACK_COLOR,   0x000000FF);
		MEM_set_val(pointer, TD_INDEX_BOX,			0);
		MEM_set_val(pointer, TD_INDEX_SELECTED,     0);
		MEM_set_val(pointer, TD_INDEX_PROPORTIONAL, 1);
		MEM_set_val(pointer, TD_INDEX_ALIGNMENT,    0);
		MEM_set_val(pointer, TD_INDEX_SHADOW,       2);
		MEM_set_val(pointer, TD_INDEX_OUTLINE,      0);
		MEM_set_val(pointer, TD_INDEX_MODEL_ID,     0);
		MEM_set_val(pointer, TD_INDEX_MODEL_RX,     _:0.0);
		MEM_set_val(pointer, TD_INDEX_MODEL_RY,     _:0.0);
		MEM_set_val(pointer, TD_INDEX_MODEL_RZ,     _:0.0);
		MEM_set_val(pointer, TD_INDEX_MODEL_ZOOM,   _:1.0);
		MEM_set_val(pointer, TD_INDEX_MODEL_VEH_1,	65535);
		MEM_set_val(pointer, TD_INDEX_MODEL_VEH_2,  65535);

		Pointer_GTDs[idx] = pointer;
		TDS_Process_TD(Text:idx, true);

		return Text:idx;
	}
	return INVALID_GLOBAL_TEXT_DRAW;
}

stock TDS_Destroy_TD(Text:index)
{
	if(!TDS_Valid_TD(index))
		return 0;

	TextDrawDestroy(index);

	MEM_delete(Pointer:MEM_get_val(Pointer_GTDs[_:index], TD_INDEX_TEXT));	  
	MEM_delete(Pointer_GTDs[index]);

	Pointer_GTDs[_:index] = MEM_NULLPTR;
	Iter_Remove(Global_TDs, _:index);
  	return 1;
}

stock TDS_SetString_TD(Text:index, string[])
{
	if(!TDS_Valid_TD(index))
		return 0;

	new 
		Pointer:p = Pointer_GTDs[_:index];

	MEM_delete(Pointer:MEM_get_val(p, TD_INDEX_TEXT));
	MEM_set_val(p, TD_INDEX_TEXT, _:MEM_new_arr(string, strlen(string) + 1));

	TextDrawSetString(index, string);
	return 1;   
}

stock TDS_GetString_TD(Text:index, string[])
{
	if(!TDS_Valid_TD(index))
		return 0;

	new Pointer:tmp = Pointer:MEM_get_val(Pointer_GTDs[_:index], TD_INDEX_TEXT);
	MEM_get_arr(tmp, 0, string, MEM_get_size(tmp));
	return 1;
}

stock TDS_SetPos_TD(Text:index, Float:newx, Float:newy)
{
	if(!TDS_Valid_TD(index))
		return 0;

	MEM_set_val(Pointer_GTDs[_:index], TD_INDEX_POS_X, _:newx);
	MEM_set_val(Pointer_GTDs[_:index], TD_INDEX_POS_Y, _:newy);

	return (TDS_Process_TD(index, true) != INVALID_GLOBAL_TEXT_DRAW);
}

stock TDS_GetPos_TD(Text:index, &Float:x, &Float:y)
{
	if(!TDS_Valid_TD(index))
		return 0;

	x = Float:MEM_get_val(Pointer_TDs[playeyrid][_:index], TD_INDEX_POS_X);
	y = Float:MEM_get_val(Pointer_TDs[playeyrid][_:index], TD_INDEX_POS_Y);
	return 1;
}

stock TDS_SetLetterSize_TD(Text:index, Float:lx, Float:ly)
{
	if(!TDS_Valid_TD(index))
		return 0;

	MEM_set_val(Pointer_GTDs[_:index], TD_INDEX_LX, _:lx);
	MEM_set_val(Pointer_GTDs[_:index], TD_INDEX_LY, _:ly);

	TextDrawLetterSize(index, lx, ly);
	return 1;   
}

stock TDS_GetLetterSize_TD(Text:index, &Float:lx, &Float:ly)
{
	if(!TDS_Valid_TD(index))
		return 0;

	lx = Float:MEM_get_val(Pointer_GTDs[_:index], TD_INDEX_LX);
	ly = Float:MEM_get_val(Pointer_GTDs[_:index], TD_INDEX_LY); 
	return 1;   
}

stock TDS_SetTextSize_TD(Text:index, Float:tx, Float:ty)
{
	if(!TDS_Valid_TD(index))
		return 0;

	MEM_set_val(Pointer_GTDs[_:index], TD_INDEX_TX, _:tx);
	MEM_set_val(Pointer_GTDs[_:index], TD_INDEX_TY, _:ty);

	TextDrawTextSize(index, tx, ty);
	return 1;   
}

stock TDS_GetTextSize_TD(Text:index, &Float:tx, &Float:ty)
{
	if(!TDS_Valid_TD(index))
		return 0;

	tx = Float:MEM_get_val(Pointer_GTDs[_:index], TD_INDEX_TX);
	ty = Float:MEM_get_val(Pointer_GTDs[_:index], TD_INDEX_TY);
	return 1;   
}

stock TDS_SetColor_TD(Text:index, color)
{
	if(!TDS_Valid_TD(index))
		return 0;

	MEM_set_val(Pointer_GTDs[_:index], TD_INDEX_COLOR, color);

	TextDrawColor(index, color);
	return 1;   
}

stock TDS_GetColor_TD(Text:index)
{
	if(!TDS_Valid_TD(index))
		return 0;

	return MEM_get_val(Pointer_GTDs[_:index], TD_INDEX_COLOR);    
}

stock TDS_SetFont_TD(Text:index, font)
{
	if(!TDS_Valid_TD(index))
		return 0;

	MEM_set_val(Pointer_GTDs[_:index], TD_INDEX_FONT, font);
	TextDrawFont(index, font);
	return 1;   
}

stock TDS_GetFont_TD(Text:index)
	{
	if(!TDS_Valid_TD(index))
		return 0;

	return MEM_get_val(Pointer_GTDs[_:index], TD_INDEX_FONT);   
}

stock TDS_SetBackColor_TD(Text:index, backcolor)
{
  	if(!TDS_Valid_TD(index))
		return 0;

	MEM_set_val(Pointer_GTDs[_:index], TD_INDEX_BACK_COLOR, backcolor);

	TextDrawBackgroundColor(index, backcolor);
	return 1;   
}

stock TDS_GetBackColor_TD(Text:index)
{
	if(!TDS_Valid_TD(index))
	return 0;

	return MEM_get_val(Pointer_GTDs[_:index], TD_INDEX_BACK_COLOR);   
}

stock TDS_SetUseBox_TD(Text:index, usebox)
{
	if(!TDS_Valid_TD(index))
		return 0;

	MEM_set_val(Pointer_GTDs[_:index], TD_INDEX_BOX, !!usebox);

	TextDrawUseBox(index, usebox);
	return 1;
}

stock TDS_GetUseBox_TD(Text:index)
{
	if(!TDS_Valid_TD(index))
		return 0;

	return MEM_get_val(Pointer_GTDs[_:index], TD_INDEX_BOX);   
}

stock TDS_SetBoxColor_TD(Text:index, boxcolor)
{
	if(!TDS_Valid_TD(index))
		return 0;

	MEM_set_val(Pointer_GTDs[_:index], TD_INDEX_BOX_COLOR, boxcolor);
	TextDrawBoxColor(index, boxcolor);
	return 1;   
}

stock TDS_GetBoxColor_TD(Text:index)
{
  if(!TDS_Valid_TD(index))
    return 0;

  return MEM_get_val(Pointer_GTDs[_:index], TD_INDEX_BOX_COLOR);    
}

stock TDS_SetSelectable_TD(Text:index, select)
{
  	if(!TDS_Valid_TD(index))
    	return 0;

	MEM_set_val(Pointer_GTDs[_:index], TD_INDEX_SELECTED, !!select);

	TextDrawSetSelectable(index, !!select);
  	return 1;   
}

stock TDS_GetSelectable_TD(Text:index)
{
	if(!TDS_Valid_TD(index))
		return 0;

	return MEM_get_val(Pointer_GTDs[_:index], TD_INDEX_SELECTED);   
}

stock TDS_SetProportional_TD(Text:index, proportional)
{
	if(!TDS_Valid_TD(index))
		return 0;

	MEM_set_val(Pointer_GTDs[_:index], TD_INDEX_PROPORTIONAL, !!proportional);
	TextDrawSetProportional(index, !!proportional);
	return 1;   
}

stock TDS_GetProportional_TD(Text:index)
{
	if(!TDS_Valid_TD(index))
		return 0;

	return MEM_get_val(Pointer_GTDs[_:index], TD_INDEX_SELECTED);   
}

stock TDS_SetAlignment_TD(Text:index, alignment)
{
	if(!TDS_Valid_TD(index))
		return 0;

	MEM_set_val(Pointer_GTDs[_:index], TD_INDEX_ALIGNMENT, alignment);
	TextDrawAlignment(index, alignment);
	return 1;   
}

stock TDS_GetAlignment_TD(Text:index)
{
	if(!TDS_Valid_TD(index))
		return 0;

	return MEM_get_val(Pointer_GTDs[_:index], TD_INDEX_ALIGNMENT);    
}

stock TDS_SetShadow_TD(Text:index, shadow)
{
  	if(!TDS_Valid_TD(index))
    	return 0;

	MEM_set_val(Pointer_GTDs[_:index], TD_INDEX_SHADOW, shadow);
	TextDrawSetShadow(index, shadow);
	return 1;   
}

stock TDS_GetShadow_TD(Text:index)
{
 	if(!TDS_Valid_TD(index))
    	return 0;

	return MEM_get_val(Pointer_GTDs[_:index], TD_INDEX_SHADOW);   
}

stock TDS_SetOutline_TD(Text:index, outline)
{
 	if(!TDS_Valid_TD(index))
    	return 0;

  	MEM_set_val(Pointer_GTDs[_:index], TD_INDEX_OUTLINE, outline);
	TextDrawSetOutline(index, outline);
  	return 1;   
}

stock TDS_GetOutline_TD(Text:index)
{
  if(!TDS_Valid_TD(index))
    return 0;

  return MEM_get_val(Pointer_GTDs[_:index], TD_INDEX_OUTLINE);    
}

stock TDS_SetPrevModel_TD(Text:index, modelid)
{
  	if(!TDS_Valid_TD(index))
    	return 0;

  	MEM_set_val(Pointer_GTDs[_:index], TD_INDEX_MODEL_ID, modelid);

	TextDrawSetPreviewModel(index, modelid);
  	return 1;   
}

stock TDS_GetPrevModel_TD(Text:index)
{
  	if(!TDS_Valid_TD(index))
		return 0;

  	return MEM_get_val(Pointer_GTDs[_:index], TD_INDEX_MODEL_ID);   
}

stock TDS_SetPrevModelRot_TD(Text:index, Float:rx, Float:ry, Float:rz, Float:zoom = 1.0)
{
  	if(!TDS_Valid_TD(index))
		return 0;

	MEM_set_val(Pointer_GTDs[_:index], TD_INDEX_MODEL_RX, _:rx);
	MEM_set_val(Pointer_GTDs[_:index], TD_INDEX_MODEL_RY, _:ry);
	MEM_set_val(Pointer_GTDs[_:index], TD_INDEX_MODEL_RZ, _:rz);
	MEM_set_val(Pointer_GTDs[_:index], TD_INDEX_MODEL_ZOOM, _:zoom);

	TextDrawSetPreviewRot(index, rx, ry, rz, zoom);
  	return 1;
}

stock TDS_GetPrevModelRot_TD(Text:index, &Float:rx, &Float:ry, &Float:rz, &Float:zoom)
{
	if(!TDS_Valid_TD(index))
		return 0;

	rx = Float:MEM_get_val(Pointer_GTDs[_:index], TD_INDEX_MODEL_RX);
	ry = Float:MEM_get_val(Pointer_GTDs[_:index], TD_INDEX_MODEL_RY);
	rz = Float:MEM_get_val(Pointer_GTDs[_:index], TD_INDEX_MODEL_RZ);
	zoom = Float:MEM_get_val(Pointer_GTDs[_:index], TD_INDEX_MODEL_ZOOM);
	return 1; 
}

stock TDS_SetPrevModelVehCol_TD(Text:index, color1, color2)
{
	if(!TDS_Valid_TD(index))
		return 0;

	MEM_set_val(Pointer_GTDs[_:index], TD_INDEX_MODEL_VEH_1, color1);
	MEM_set_val(Pointer_GTDs[_:index], TD_INDEX_MODEL_VEH_2, color2);

	TextDrawSetPreviewVehCol(index, color1, color2);
	return 1;
}

stock TDS_GetPrevModelVehCol_TD(Text:index, &color1, &color2)
{
	if(!TDS_Valid_TD(index))
	return 0;

	color1 = MEM_get_val(Pointer_GTDs[_:index], TD_INDEX_MODEL_VEH_1);
	color2 = MEM_get_val(Pointer_GTDs[_:index], TD_INDEX_MODEL_VEH_2);
	return 1; 
}



/* ————————————————————————————————————————————————————————————————————————————————————————————————

# INERNAL FUNCTIONS PLAYER TEXT DRAWS

————————————————————————————————————————————————————————————————————————————————————————————————— */

static stock Text:TDS_Process_TD(Text:textid, bool:update_pos)
{
	new 
		Pointer:p = Pointer_GTDs[_:textid];

	if(update_pos)
	{
		TextDrawDestroy(textid);

		new string[MAX_TEXT_DRAW_LENGTH], Pointer:tmp = Pointer:MEM_get_val(p, TD_INDEX_TEXT);
		MEM_get_arr(tmp, 0, string, MEM_get_size(tmp));

		textid = TextDrawCreate(Float:MEM_get_val(p, TD_INDEX_POS_X), Float:MEM_get_val(p, TD_INDEX_POS_Y), string);
	}


	new 
		box = MEM_get_val(p, TD_INDEX_BOX),
		font = MEM_get_val(p, TD_INDEX_FONT)
	;

	TextDrawColor(textid, 					    MEM_get_val(p, TD_INDEX_COLOR));
	TextDrawTextSize(textid, 				    Float:MEM_get_val(p, TD_INDEX_TX), Float:MEM_get_val(p, TD_INDEX_TY));
	TextDrawLetterSize(textid, 				    Float:MEM_get_val(p, TD_INDEX_LX), Float:MEM_get_val(p, TD_INDEX_LY));
	TextDrawBackgroundColor(textid, 		    MEM_get_val(p, TD_INDEX_BACK_COLOR));
	TextDrawSetSelectable(textid,	    	    MEM_get_val(p, TD_INDEX_SELECTED));
	TextDrawSetProportional(textid,			    MEM_get_val(p, TD_INDEX_PROPORTIONAL));
	TextDrawSetOutline(textid, 				    MEM_get_val(p, TD_INDEX_OUTLINE));
	TextDrawSetShadow(textid, 				    MEM_get_val(p, TD_INDEX_SHADOW));
	TextDrawFont(textid, 					    font);
	TextDrawUseBox(textid, 					    box);

	if(box)
		TextDrawBoxColor(textid, 				MEM_get_val(p, TD_INDEX_BOX_COLOR));

	if(font == 5)
	{
		TextDrawSetPreviewModel(textid, 		MEM_get_val(p, TD_INDEX_MODEL_ID));
		TextDrawSetPreviewRot(textid, 			Float:MEM_get_val(p, TD_INDEX_MODEL_RX), Float:MEM_get_val(p, TD_INDEX_MODEL_RY), Float:MEM_get_val(p, TD_INDEX_MODEL_RZ), Float:MEM_get_val(p, TD_INDEX_MODEL_ZOOM));
		TextDrawSetPreviewVehCol(textid, 		MEM_get_val(p, TD_INDEX_MODEL_VEH_1), MEM_get_val(p, TD_INDEX_MODEL_VEH_2));		
	}

	return textid;
}

/* ————————————————————————————————————————————————————————————————————————————————————————————————

# HOOKS GLOBAL TEXT DRAWS

————————————————————————————————————————————————————————————————————————————————————————————————— */


#if defined _ALS_IsValidTextDraw
	#undef IsValidTextDraw
#else
	#define _ALS_IsValidTextDraw
#endif

#define IsValidTextDraw TDS_Valid_TD


#if defined _ALS_TextDrawCreate
  #undef TextDrawCreate
#else
#define _ALS_TextDrawCreate
#endif
#define TextDrawCreate TDS_Create_TD


#if defined _ALS_TextDrawDestroy
  #undef TextDrawDestroy
#else
#define _ALS_TextDrawDestroy
#endif
#define TextDrawDestroy TDS_Destroy_TD


#if defined _ALS_TextDrawSetString
  #undef TextDrawSetString
#else
#define _ALS_TextDrawSetString
#endif
#define TextDrawSetString TDS_SetString_TD


#if defined _ALS_TextDrawGetString
  #undef TextDrawGetString
#else
#define _ALS_TextDrawGetString
#endif
#define TextDrawGetString TDS_GetString_TD


#if defined _ALS_TextDrawSetPos
  #undef TextDrawSetPos
#else
#define _ALS_TextDrawSetPos
#endif
#define TextDrawSetPos TDS_SetPos_TD


#if defined _ALS_TextDrawGetPos
  #undef TextDrawGetPos
#else
#define _ALS_TextDrawGetPos
#endif
#define TextDrawGetPos TDS_GetPos_TD


#if defined _ALS_TextDrawLetterSize
  #undef TextDrawLetterSize
#else
#define _ALS_TextDrawLetterSize
#endif
#define TextDrawLetterSize TDS_SetLetterSize_TD


#if defined _ALS_TextDrawGetLetterSize
  #undef TextDrawGetLetterSize
#else
#define _ALS_TextDrawGetLetterSize
#endif
#define TextDrawGetLetterSize TDS_GetLetterSize_TD		


#if defined _ALS_TextDrawTextSize
  #undef TextDrawTextSize
#else
#define _ALS_TextDrawTextSize
#endif
#define TextDrawTextSize TDS_SetTextSize_TD


#if defined _ALS_TextDrawGetTextSize
  #undef TextDrawGetTextSize
#else
#define _ALS_TextDrawGetTextSize
#endif
#define TextDrawGetTextSize TDS_GetTextSize_TD


#if defined _ALS_TextDrawColor
  #undef TextDrawColor
#else
#define _ALS_TextDrawColor
#endif
#define TextDrawColor TDS_SetColor_TD


#if defined _ALS_TextDrawGetColor
  #undef TextDrawGetColor
#else
#define _ALS_TextDrawGetColor
#endif
#define TextDrawGetColor TDS_GetColor_TD


#if defined _ALS_TextDrawFont
  #undef TextDrawFont
#else
#define _ALS_TextDrawFont
#endif
#define TextDrawFont TDS_SetFont_TD


#if defined _ALS_TextDrawGetFont
  #undef TextDrawGetFont
#else
#define _ALS_TextDrawGetFont
#endif
#define TextDrawGetFont TDS_GetFont_TD


#if defined _ALS_TextDrawBackgroundColor
  #undef TextDrawBackgroundColor
#else
#define _ALS_TextDrawBackgroundColor
#endif
#define TextDrawBackgroundColor TDS_SetBackColor_TD


#if defined _ALS_TextDrawGetBackColor
  #undef TextDrawGetBackColor
#else
#define _ALS_TextDrawGetBackColor
#endif
#define TextDrawGetBackColor TDS_GetBackColor_TD


#if defined _ALS_TextDrawUseBox
  #undef TextDrawUseBox
#else
#define _ALS_TextDrawUseBox
#endif
#define TextDrawUseBox TDS_SetUseBox_TD


#if defined _ALS_TextDrawGetUseBox
  #undef TextDrawGetuseBox
#else
#define _ALS_TextDrawGetUseBox
#endif
#define TextDrawGetUseBox TDS_GetUseBox_TD


#if defined _ALS_TextDrawBoxColor
  #undef TextDrawBoxColor
#else
#define _ALS_TextDrawBoxColor
#endif
#define TextDrawBoxColor TDS_SetBoxColor_TD


#if defined _ALS_TextDrawGetBoxColor
  #undef TextDrawGetBoxColor
#else
#define _ALS_TextDrawGetBoxColor
#endif
#define TextDrawGetBoxColor TDS_GetBoxColor_TD


#if defined _ALS_TextDrawSetSelectable
  #undef TextDrawSetSelectable
#else
#define _ALS_TextDrawSetSelectable
#endif
#define TextDrawSetSelectable TDS_SetSelectable_TD


#if defined _ALS_TextDrawGetSelectable
  #undef TextDrawGetSelectable
#else
#define _ALS_TextDrawGetSelectable
#endif
#define TextDrawGetSelectable TDS_GetSelectable_TD


#if defined _ALS_TextDrawSetProportional
  #undef TextDrawSetProportional
#else
#define _ALS_TextDrawSetProportional
#endif
#define TextDrawSetProportional TDS_SetProportional_TD


#if defined _ALS_TextDrawGetProportional
  #undef TextDrawGetProportional
#else
#define _ALS_TextDrawGetProportional
#endif
#define TextDrawGetProportional TDS_GetProportional_TD


#if defined _ALS_TextDrawAlignment
  #undef TextDrawAlignment
#else
#define _ALS_TextDrawAlignment
#endif
#define TextDrawAlignment TDS_SetAlignment_TD


#if defined _ALS_TextDrawGetAlignment
  #undef TextDrawGetAlignment
#else
#define _ALS_TextDrawGetAlignment
#endif
#define TextDrawGetAlignment TDS_GetAlignment_TD	


#if defined _ALS_TextDrawSetShadow
  #undef TextDrawSetShadow
#else
#define _ALS_TextDrawSetShadow
#endif
#define TextDrawSetShadow TDS_SetShadow_TD


#if defined _ALS_TextDrawGetShadow
  #undef TextDrawGetShadow
#else
#define _ALS_TextDrawGetShadow
#endif
#define TextDrawGetShadow TDS_GetShadow_TD


#if defined _ALS_TextDrawSetOutline
  #undef TextDrawSetOutline
#else
#define _ALS_TextDrawSetOutline
#endif
#define TextDrawSetOutline TDS_SetOutline_TD


#if defined _ALS_TextDrawGetOutline
  #undef TextDrawGetOutline
#else
#define _ALS_TextDrawGetOutline
#endif
#define TextDrawGetOutline TDS_GetOutline_TD


#if defined _ALS_TextDrawSetPreviewModel
  #undef TextDrawSetPreviewModel
#else
#define _ALS_TextDrawSetPreviewModel
#endif
#define TextDrawSetPreviewModel TDS_SetPrevModel_TD	


#if defined _ALS_TextDrawGetPreviewModel
  #undef TextDrawGetPreviewModel
#else
#define _ALS_TextDrawGetPreviewModel
#endif
#define TextDrawGetPreviewModel TDS_GetPrevModel_TD	


#if defined _ALS_TextDrawSetPreviewModelRot
  #undef TextDrawSetPreviewModelRot
#else
#define _ALS_TextDrawSetPreviewModelRot	
#endif
#define TextDrawSetPreviewModelRot TDS_SetPrevModelRot_TD


#if defined _ALS_TextDrawGetPreviewModelRot
  #undef TextDrawGetPreviewModelRot
#else
#define _ALS_TextDrawGetPreviewModelRot
#endif
#define TextDrawGetPreviewModelRot TDS_GetPrevModelRot_TD


#if defined _ALS_TextDrawSetPreviewVehCol
  #undef TextDrawSetPreviewVehCol
#else
#define _ALS_TextDrawSetPreviewVehCol
#endif
#define TextDrawSetPreviewVehCol TDS_SetPrevModelVehCol_TD


#if defined _ALS_TextDrawGetPreviewVehCol	
  #undef TextDrawGetPreviewVehCol
#else
#define _ALS_TextDrawGetPreviewVehCol	
#endif
#define TextDrawGetPreviewVehCol TDS_GetPrevModelVehCol_TD							