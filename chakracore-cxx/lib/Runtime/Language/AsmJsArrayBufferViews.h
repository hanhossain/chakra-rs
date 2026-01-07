//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft Corporation and contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
// Portions of this file are copyright 2014 Mozilla Foundation, available under the Apache 2.0 license.
//-------------------------------------------------------------------------------------------------------

#ifndef ARRAYBUFFER_VIEW
#define ARRAYBUFFER_VIEW(name, align, RegType, MemType, irSuffix)
#endif

#ifndef ARRAYBUFFER_VIEW_INT
#define ARRAYBUFFER_VIEW_INT(name, align, RegType, MemType, irSuffix) ARRAYBUFFER_VIEW(name, align, RegType, MemType, irSuffix)
#endif

#ifndef ARRAYBUFFER_VIEW_FLT
#define ARRAYBUFFER_VIEW_FLT(name, align, RegType, MemType, irSuffix) ARRAYBUFFER_VIEW(name, align, RegType, MemType, irSuffix)
#endif

//                  (Name            , Align , RegType, MemType , irSuffix )
//                                     (log2)
ARRAYBUFFER_VIEW_INT(INT8            , 0     , int32  , int8_t    , Int8     )
ARRAYBUFFER_VIEW_INT(UINT8           , 0     , int32  , uint8_t   , Uint8    )
ARRAYBUFFER_VIEW_INT(INT16           , 1     , int32  , int16   , Int16    )
ARRAYBUFFER_VIEW_INT(UINT16          , 1     , int32  , uint16  , Uint16   )
ARRAYBUFFER_VIEW_INT(INT32           , 2     , int32  , int32   , Int32    )
// TODO (hanhossain): remove in favor of uint32_t
ARRAYBUFFER_VIEW_INT(UINT32          , 2     , int32  , uint32_t  , Uint32   )
ARRAYBUFFER_VIEW_FLT(FLOAT32         , 2     , float  , float   , Float32  )
ARRAYBUFFER_VIEW_FLT(FLOAT64         , 3     , double , double  , Float64  )
// TODO (hanhossain): remove in favor of int64_t
ARRAYBUFFER_VIEW_INT(INT64           , 3     , long  , long   , Int64    )
ARRAYBUFFER_VIEW_INT(INT8_TO_INT64   , 0     , long  , int8_t    , Int8     )
ARRAYBUFFER_VIEW_INT(UINT8_TO_INT64  , 0     , long  , uint8_t   , Uint8    )
ARRAYBUFFER_VIEW_INT(INT16_TO_INT64  , 1     , long  , int16   , Int16    )
ARRAYBUFFER_VIEW_INT(UINT16_TO_INT64 , 1     , long  , uint16  , Uint16   )
ARRAYBUFFER_VIEW_INT(INT32_TO_INT64  , 2     , long  , int32   , Int32    )
ARRAYBUFFER_VIEW_INT(UINT32_TO_INT64 , 2     , long  , uint32_t  , Uint32   )

#undef ARRAYBUFFER_VIEW
#undef ARRAYBUFFER_VIEW_INT
#undef ARRAYBUFFER_VIEW_FLT
