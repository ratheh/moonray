// Copyright 2023-2024 DreamWorks Animation LLC
// SPDX-License-Identifier: Apache-2.0


#pragma once
#ifdef __APPLE__
#include <scene_rdl2/common/platform/platform.hh>
#endif
#include <scene_rdl2/common/platform/HybridVaryingData.hh>

//----------------------------------------------------------------------------

// instance0 is used in intersection test (instance attribute and motion vector)
// light is used in occlusion test (shadow linking)
// l2r: concatenated local to render space transform used by multilevel instance,
// be sure this is initialized to the identity.
#define MCRT_COMMON_RAY_DIFFERENTIAL_PADDING 4  /*Alignment: 8, Total size: 164, Padded size: 168 */

#define MCRT_COMMON_RAY_EXTENSION_MEMBERS                       /*  size */\
    HVD_MEMBER(int32_t, materialID);                            /*    4  */\
    HVD_MEMBER(int32_t, depth);                                 /*    8  */\
    HVD_PTR(void*, userData);                                   /*   16  */\
    HVD_PTR(void*, geomTls);                                    /*   24  */\
    HVD_PTR(const void*, priorityMaterial0);                    /*   32  */\
    HVD_PTR(const void*, priorityMaterial1);                    /*   40  */\
    HVD_PTR(const void*, priorityMaterial2);                    /*   48  */\
    HVD_PTR(const void*, priorityMaterial3);                    /*   56  */\
    HVD_MEMBER(int32_t, priorityMaterial0Count);                /*   60  */\
    HVD_MEMBER(int32_t, priorityMaterial1Count);                /*   64  */\
    HVD_MEMBER(int32_t, priorityMaterial2Count);                /*   68  */\
    HVD_MEMBER(int32_t, priorityMaterial3Count);                /*   72  */\
    HVD_PTR(const void*, instance0OrLight);                     /*   80  */\
    HVD_PTR(const void*, instance1);                            /*   88  */\
    HVD_PTR(const void*, instance2);                            /*   96  */\
    HVD_PTR(const void*, instance3);                            /*  104  */\
    HVD_MEMBER(int32_t, instanceAttributesDepth);               /*  108  */\
    HVD_MEMBER(HVD_NAMESPACE(scene_rdl2::math, Xform3f), l2r);  /*  156  */\
    HVD_MEMBER(int32_t, volumeInstanceState);                   /*  160  */\
    HVD_MEMBER(int32_t, shadowReceiverId);                      /*  164  */\
    HVD_ISPC_PAD(pad, MCRT_COMMON_RAY_DIFFERENTIAL_PADDING)     /*  168  */\
                                          /* macOS: 168 * 4 lanes = 672  */\
                                         /* linux: 168 * 8 lanes = 1344  */\

#define MCRT_COMMON_RAY_EXTENSION_VALIDATION(vlen)         \
    HVD_BEGIN_VALIDATION(RayExtension, vlen);              \
    HVD_VALIDATE(RayExtension, materialID);                \
    HVD_VALIDATE(RayExtension, depth);                     \
    HVD_VALIDATE(RayExtension, userData);                  \
    HVD_VALIDATE(RayExtension, geomTls);                   \
    HVD_VALIDATE(RayExtension, priorityMaterial0);         \
    HVD_VALIDATE(RayExtension, priorityMaterial1);         \
    HVD_VALIDATE(RayExtension, priorityMaterial2);         \
    HVD_VALIDATE(RayExtension, priorityMaterial3);         \
    HVD_VALIDATE(RayExtension, priorityMaterial0Count);    \
    HVD_VALIDATE(RayExtension, priorityMaterial1Count);    \
    HVD_VALIDATE(RayExtension, priorityMaterial2Count);    \
    HVD_VALIDATE(RayExtension, priorityMaterial3Count);    \
    HVD_VALIDATE(RayExtension, instance0OrLight);          \
    HVD_VALIDATE(RayExtension, instance1);                 \
    HVD_VALIDATE(RayExtension, instance2);                 \
    HVD_VALIDATE(RayExtension, instance3);                 \
    HVD_VALIDATE(RayExtension, instanceAttributesDepth);   \
    HVD_VALIDATE(RayExtension, l2r);                       \
    HVD_VALIDATE(RayExtension, volumeInstanceState);       \
    HVD_VALIDATE(RayExtension, shadowReceiverId);          \
    HVD_END_VALIDATION

// Embree4 RTCRayHit compatibility:
// Embree4 built with RTC_GEOMETRY_INSTANCE_ARRAY adds instPrimID[1] after instID[1].
// vcpkg's embree:x64-windows has this enabled by default.
// We add instPrimID unconditionally for compatibility.
//
// Struct layout must match RTCRayHit from embree4/rtcore_ray.h:
//   RTCRay (48 bytes): org(12), tnear(4), dir(12), time(4), tfar(4), mask(4), id(4), flags(4)
//   RTCHit (40 bytes): Ng(12), u(4), v(4), primID(4), geomID(4), instID[1](4), instPrimID[1](4)
//   Total RTCRayHit = 88 bytes
//
// With instPrimID, Ray members total 256 bytes on all platforms:
//   88 bytes (Embree portion + alignment) + 168 (ext) = 256
// Note: RayExtension has pointer members requiring 8-byte alignment in C++.
// This adds 4 bytes implicit padding after instPrimID that ISPC doesn't have.
// We add HVD_ISPC_PAD to match.

// Windows ISPC end-padding: C++ has 4 bytes padding before RayDifferential members.
// MSVC reuses Ray's tail padding for derived class members, but there's still a 4-byte gap
// after mFlags before mOriginX (likely due to alignment of Vec3f in derived class context).
// Note: ISPC uses __WIN32__ (passed via -D flag), MSVC defines _MSC_VER
#if defined(_MSC_VER) || defined(__WIN32__)
#define HVD_RAY_END_PAD ;HVD_ISPC_PAD(rayEndPad, 4)
#else
#define HVD_RAY_END_PAD
#endif

#define MCRT_COMMON_RAY_MEMBERS                                 /*  size */\
    HVD_MEMBER(HVD_NAMESPACE(scene_rdl2::math, Vec3f), org);    /*   12  */\
    HVD_MEMBER(float, tnear);                                   /*   16  */\
    HVD_MEMBER(HVD_NAMESPACE(scene_rdl2::math, Vec3f), dir);    /*   28  */\
    HVD_MEMBER(float, time);                                    /*   32  */\
    HVD_MEMBER(float, tfar);                                    /*   36  */\
    HVD_MEMBER(int32_t, mask);                                  /*   40  */\
    HVD_MEMBER(uint32_t, id);                                   /*   44  */\
    HVD_MEMBER(uint32_t, pad);                                  /*   48  */\
    HVD_MEMBER(HVD_NAMESPACE(scene_rdl2::math, Vec3f), Ng);     /*   60  */\
    HVD_MEMBER(float, u);                                       /*   64  */\
    HVD_MEMBER(float, v);                                       /*   68  */\
    HVD_MEMBER(int32_t, primID);                                /*   72  */\
    HVD_MEMBER(int32_t, geomID);                                /*   76  */\
    HVD_MEMBER(int32_t, instID);                                /*   80  */\
    HVD_MEMBER(int32_t, instPrimID);                            /*   84  */\
    HVD_ISPC_PAD(instPrimIDPad, 4);                             /*   88  (C++ aligns ext to 8-byte boundary) */\
    HVD_MEMBER(RayExtension, ext);                              /*  256  */\
    HVD_MEMBER(Flags, mFlags)                                   /*  260  */\
    HVD_RAY_END_PAD                                             /*  272  (Windows: C++ ALIGN(16) padding) */\
                                         /* macOS: 260 * 4 lanes = 1040  */\
                                         /* linux: 260 * 8 lanes = 2080  */\
                                         /* windows: 272 * 8 lanes = 2176  */\

// End padding for RayDifferential to match C++ struct size with alignment:
// - Windows: C++ RayDifferential is 336 bytes. ISPC needs 20 bytes padding:
//   264 (Ray w/rayEndPad) + 52 (diff members) = 316, need +20 to reach 336
// - ARM (CACHE_LINE_SIZE=128): Similar calculation
// - Linux (64 bytes cache line): 268 + 52 = 320 bytes, already cache-aligned
// Note: ISPC uses __WIN32__ (passed via -D flag), MSVC defines _MSC_VER
#if CACHE_LINE_SIZE == 128
#define HVD_RAY_DIFFERENTIAL_MEMEBERS_CACHE_PAD ;HVD_ARRAY(uint32_t, pad1, (5))
#elif defined(_MSC_VER) || defined(__WIN32__)
#define HVD_RAY_DIFFERENTIAL_MEMEBERS_CACHE_PAD ;HVD_ARRAY(uint32_t, pad1, (5))
#else
#define HVD_RAY_DIFFERENTIAL_MEMEBERS_CACHE_PAD
#endif

#define MCRT_COMMON_RAY_DIFFERENTIAL_MEMBERS                                    /*   size   */\
    HVD_MEMBER(HVD_NAMESPACE(scene_rdl2::math, Vec3f), mOriginX);               /*   276    */\
    HVD_MEMBER(HVD_NAMESPACE(scene_rdl2::math, Vec3f), mDirX);                  /*   288    */\
    HVD_MEMBER(HVD_NAMESPACE(scene_rdl2::math, Vec3f), mOriginY);               /*   300    */\
    HVD_MEMBER(HVD_NAMESPACE(scene_rdl2::math, Vec3f), mDirY);                  /*   312    */\
    HVD_MEMBER(float, mOrigTfar)                                                /*   316    */\
    HVD_RAY_DIFFERENTIAL_MEMEBERS_CACHE_PAD                                     /*   336 (win/mac) */\
                                                          /* windows: 336 * 8 lanes = 2688  */\
                                                          /* linux:   320 * 8 lanes = 2560  */\

#define MCRT_COMMON_RAY_DIFFERENTIAL_VALIDATION(vlen)       \
    HVD_BEGIN_VALIDATION(RayDifferential, vlen);            \
    HVD_VALIDATE(RayDifferential, org);                     \
    HVD_VALIDATE(RayDifferential, tnear);                   \
    HVD_VALIDATE(RayDifferential, dir);                     \
    HVD_VALIDATE(RayDifferential, time);                    \
    HVD_VALIDATE(RayDifferential, tfar);                    \
    HVD_VALIDATE(RayDifferential, mask);                    \
    HVD_VALIDATE(RayDifferential, id);                      \
    HVD_VALIDATE(RayDifferential, pad);                     \
    HVD_VALIDATE(RayDifferential, Ng);                      \
    HVD_VALIDATE(RayDifferential, u);                       \
    HVD_VALIDATE(RayDifferential, v);                       \
    HVD_VALIDATE(RayDifferential, primID);                  \
    HVD_VALIDATE(RayDifferential, geomID);                  \
    HVD_VALIDATE(RayDifferential, instID);                  \
    HVD_VALIDATE(RayDifferential, instPrimID);              \
    HVD_VALIDATE(RayDifferential, ext);                     \
    HVD_VALIDATE(RayDifferential, mFlags);                  \
    HVD_VALIDATE(RayDifferential, mOriginX);                \
    HVD_VALIDATE(RayDifferential, mDirX);                   \
    HVD_VALIDATE(RayDifferential, mOriginY);                \
    HVD_VALIDATE(RayDifferential, mDirY);                   \
    HVD_VALIDATE(RayDifferential, mOrigTfar);               \
    HVD_END_VALIDATION


//----------------------------------------------------------------------------



