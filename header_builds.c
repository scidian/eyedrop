//
// Copyright (C) 2021 Scididan Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Mon Feb 22 2021
//
//
//####################################################################################
//##
//##    Single Header Library Initialization
//##
//####################################################################################
#if defined(_WIN32)
    #include <Windows.h>
#endif


//####################################################################################
//##    Sokol Libraries
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

//####################################################################################
//##    Font Stash
//####################################################################################
#include <stdio.h>					// malloc, free, fopen, fclose, ftell, fseek, fread
#include <string.h>					// memset
#define FONTSTASH_IMPLEMENTATION	// Expands implementation
#include "3rd_party/fontstash.h"
#include "3rd_party/sokol/sokol_fontstash.h"


//####################################################################################
//##    Handmade Math Implementation
//####################################################################################
#define HANDMADE_MATH_IMPLEMENTATION
#define HANDMADE_MATH_NO_SSE
#include "3rd_party/handmade_math.h"


//####################################################################################
//##    STB Libraries
//####################################################################################
#define STB_IMAGE_IMPLEMENTATION
#include "3rd_party/stb/stb_image.h"
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "3rd_party/stb/stb_image_resize.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "3rd_party/stb/stb_image_write.h"

