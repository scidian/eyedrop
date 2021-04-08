//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Mon Feb 22 2021
//
//
//####################################################################################
//##
//##    C Single Header Library Initialization
//##
//####################################################################################
#if defined(DROP_TARGET_WINDOWS)
    #include <Windows.h>
#endif


//####################################################################################
//##    Sokol Library Implmentations
//####################################################################################
#define SOKOL_IMPL
#include "3rd_party/sokol/sokol_app.h"
#include "3rd_party/sokol/sokol_gfx.h"
#define SOKOL_GL_IMPL
#include "3rd_party/sokol/sokol_gl.h"
#include "3rd_party/sokol/sokol_fetch.h"
#include "3rd_party/sokol/sokol_glue.h"
#include "3rd_party/sokol/sokol_time.h"
#include "3rd_party/sokol/sokol_audio.h"
#include "3rd_party/sokol/sokol_args.h"


//####################################################################################
//##    STB Library Implmentations
//####################################################################################
#define STB_IMAGE_IMPLEMENTATION
#include "3rd_party/stb/stb_image.h"
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "3rd_party/stb/stb_image_resize.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "3rd_party/stb/stb_image_write.h"
#define STB_RECT_PACK_IMPLEMENTATION
#include "3rd_party/stb/stb_rect_pack.h"                    // Include this BEFORE fontstash to stop a second definition of stb_rect_pack from within stb_truetype


//####################################################################################
//##    Font Stash Implmentations
//####################################################################################
#include <stdio.h>					                        // malloc, free, fopen, fclose, ftell, fseek, fread
#include <string.h>					                        // memset
#define FONTSTASH_IMPLEMENTATION	                        
#include "3rd_party/fontstash.h"
#include "3rd_party/sokol/sokol_fontstash.h"


//####################################################################################
//##    Handmade Math Implementation
//####################################################################################
#define HANDMADE_MATH_IMPLEMENTATION
#define HANDMADE_MATH_NO_SSE
#include "3rd_party/handmade_math.h"
