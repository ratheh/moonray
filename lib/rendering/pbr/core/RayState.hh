// Copyright 2023-2024 DreamWorks Animation LLC
// SPDX-License-Identifier: Apache-2.0


#pragma once
#include <scene_rdl2/common/platform/HybridVaryingData.hh>


//----------------------------------------------------------------------------

// Identifies where the primary ray comes from
#define SUBPIXEL_MEMBERS                                                \
    /* Pixel location, this doubles as a 32-bit sort key for the */     \
    /* radiance queues. */                                              \
    HVD_MEMBER(uint32_t, mPixel);                                       \
    HVD_MEMBER(int, mSubpixelIndex);                                    \
    HVD_MEMBER(float, mSubpixelX);                                      \
    HVD_MEMBER(float, mSubpixelY);                                      \
    HVD_MEMBER(int, mPixelSamples);                                     \
    HVD_MEMBER(float, mSampleClampingValue);                            \
    HVD_MEMBER(float, mPrimaryRayDiffScale);                            \
    HVD_MEMBER(float, mTextureDiffScale)

#define SUBPIXEL_VALIDATION(vlen)                                       \
    HVD_BEGIN_VALIDATION(Subpixel, vlen);                               \
    HVD_VALIDATE(Subpixel, mPixel);                                     \
    HVD_VALIDATE(Subpixel, mSubpixelIndex);                             \
    HVD_VALIDATE(Subpixel, mSubpixelX);                                 \
    HVD_VALIDATE(Subpixel, mSubpixelY);                                 \
    HVD_VALIDATE(Subpixel, mPixelSamples);                              \
    HVD_VALIDATE(Subpixel, mSampleClampingValue);                       \
    HVD_VALIDATE(Subpixel, mPrimaryRayDiffScale);                       \
    HVD_VALIDATE(Subpixel, mTextureDiffScale);                          \
    HVD_END_VALIDATION


//----------------------------------------------------------------------------

#define PATH_VERTEX_MEMBERS                                                 /*  size */\
    HVD_MEMBER(HVD_NAMESPACE(scene_rdl2::math, Color), pathThroughput);     /*   12  */\
    /* Frame buffer path weight. */                                                    \
    HVD_MEMBER(float, pathPixelWeight);                                     /*   16  */\
    HVD_MEMBER(float, aovPathPixelWeight);                                  /*   20  */\
    HVD_MEMBER(float, pathDistance);                                        /*   24  */\
    HVD_MEMBER(HVD_NAMESPACE(scene_rdl2::math, Vec2f), minRoughness);       /*   32  */\
    HVD_MEMBER(int, diffuseDepth);                                          /*   36  */\
    HVD_MEMBER(int, volumeDepth);                                           /*   40  */\
    HVD_MEMBER(int, glossyDepth);                                           /*   44  */\
    HVD_MEMBER(int, mirrorDepth);                                           /*   48  */\
    HVD_MEMBER(int, nonMirrorDepth);                                        /*   52  */\
    HVD_MEMBER(int, presenceDepth);                                         /*   56  */\
    HVD_MEMBER(float, totalPresence);                                       /*   60  */\
    HVD_MEMBER(int, hairDepth);                                             /*   64  */\
    HVD_MEMBER(int, subsurfaceDepth);                                       /*   68  */\
    HVD_MEMBER(float, accumOpacity);                                        /*   72  */\
    /* for lpe aovs */                                                                 \
    HVD_MEMBER(int, lpeStateId);                                            /*   76  */\
    /* only used by bundling incoherent ray */                                         \
    /* queue, invalid in all other cases    */                                         \
    HVD_MEMBER(int, lpeStateIdLight);                                       /*   80  */\
    HVD_MEMBER(int, lobeType)                                               /*   84  */


#define PATH_VERTEX_VALIDATION(vlen)                \
    HVD_BEGIN_VALIDATION(PathVertex, vlen);         \
    HVD_VALIDATE(PathVertex, pathThroughput);       \
    HVD_VALIDATE(PathVertex, pathPixelWeight);      \
    HVD_VALIDATE(PathVertex, aovPathPixelWeight);   \
    HVD_VALIDATE(PathVertex, pathDistance);         \
    HVD_VALIDATE(PathVertex, minRoughness);         \
    HVD_VALIDATE(PathVertex, diffuseDepth);         \
    HVD_VALIDATE(PathVertex, volumeDepth);          \
    HVD_VALIDATE(PathVertex, glossyDepth);          \
    HVD_VALIDATE(PathVertex, mirrorDepth);          \
    HVD_VALIDATE(PathVertex, nonMirrorDepth);       \
    HVD_VALIDATE(PathVertex, presenceDepth);        \
    HVD_VALIDATE(PathVertex, totalPresence);        \
    HVD_VALIDATE(PathVertex, hairDepth);            \
    HVD_VALIDATE(PathVertex, subsurfaceDepth);      \
    HVD_VALIDATE(PathVertex, accumOpacity);         \
    HVD_VALIDATE(PathVertex, lpeStateId);           \
    HVD_VALIDATE(PathVertex, lpeStateIdLight);      \
    HVD_VALIDATE(PathVertex, lobeType);             \
    HVD_END_VALIDATION


//----------------------------------------------------------------------------


// RayState padding to align to CACHE_LINE_SIZE boundary
// Note: ISPC uses __WIN32__ (passed via -D flag), MSVC defines _MSC_VER
//
// With Embree4 instPrimID and ALIGN(16) on Ray:
// - Windows/macOS: RayDifferential is 336 bytes (272 Ray + 52 diff + 12 pad1)
// - Linux: RayDifferential is 312 bytes (260 Ray + 52 diff, no cache pad)
// - Windows/macOS: RayState ends at 600 bytes, pad to 640 for cache alignment
// - Linux: RayState ends at 576 bytes (already aligned)
#if CACHE_LINE_SIZE == 128
// macOS ARM: 128-byte cache line alignment
// Total size: 600, Padded size: 640, Padding needed: 40
#define RAY_STATE_MEMBERS_PAD   40
#elif defined(_MSC_VER) || defined(__WIN32__)
// Windows: RayDifferential is 336 bytes
// Total size: 600, Padded to 640 for 64-byte cache line alignment
#define RAY_STATE_MEMBERS_PAD   40
#else
// Linux x86_64: 64-byte cache line alignment
// RayDifferential is 312 bytes (no pad1), Total size: 576, already aligned
#define RAY_STATE_MEMBERS_PAD   0
#endif

#define RAY_STATE_MEMBERS                                                   /*  linux   win    */\
    HVD_MEMBER(HVD_NAMESPACE(mcrt_common, RayDifferential), mRay);          /*    312    336   */\
    HVD_MEMBER(PathVertex, mPathVertex);                                    /*    396    420   */\
    HVD_MEMBER(uint32_t, mSequenceID);                                      /*    400    424   */\
    HVD_MEMBER(Subpixel, mSubpixel);                                        /*    432    456   */\
    HVD_MEMBER(uint32_t, mPad0);                                            /*    436    460   */\
    HVD_MEMBER(uint32_t, mTilePass);                                        /*    440    464   */\
    HVD_MEMBER(uint32_t, mRayStateIdx);                                     /*    444    468   */\
    HVD_ISPC_PAD(mPad1, 4);                                                 /*    448    472   */\
    HVD_PTR(HVD_NAMESPACE(shading, Intersection) *, mAOSIsect);             /*    456    480   */\
    HVD_MEMBER(uint32_t, mDeepDataHandle);                                  /*    460    484   */\
    HVD_MEMBER(uint32_t, mCryptomatteDataHandle);                           /*    464    488   */\
    HVD_MEMBER(HVD_NAMESPACE(scene_rdl2::math, Vec3f), mCryptoRefP);        /*    476    500   */\
    HVD_MEMBER(HVD_NAMESPACE(scene_rdl2::math, Vec3f), mCryptoP0);          /*    488    512   */\
    HVD_MEMBER(HVD_NAMESPACE(scene_rdl2::math, Vec3f), mCryptoRefN);        /*    500    524   */\
    HVD_MEMBER(HVD_NAMESPACE(scene_rdl2::math, Vec2f), mCryptoUV);          /*    508    532   */\
    HVD_MEMBER(HVD_NAMESPACE(scene_rdl2::math, Color), mVolRad);            /*    520    544   */\
    HVD_MEMBER(HVD_NAMESPACE(scene_rdl2::math, Color), mVolTr);             /*    532    556   */\
    HVD_MEMBER(HVD_NAMESPACE(scene_rdl2::math, Color), mVolTh);             /*    544    568   */\
    HVD_MEMBER(HVD_NAMESPACE(scene_rdl2::math, Color), mVolTalpha);         /*    556    580   */\
    HVD_MEMBER(HVD_NAMESPACE(scene_rdl2::math, Color), mVolTm);             /*    568    592   */\
    HVD_MEMBER(uint32_t, mVolHit);                                          /*    572    596   */\
    HVD_MEMBER(float, mVolumeSurfaceT);                                     /*    576    600   */\
    HVD_ISPC_PAD(pad, RAY_STATE_MEMBERS_PAD)                                /*    576    640   */\
                                                        /* win/macOS: 640 * 8 lanes = 5120   */\
                                                        /* linux:     576 * 8 lanes = 4608   */\


#define RAY_STATE_VALIDATION(vlen)                                          \
    HVD_BEGIN_VALIDATION(RayState, vlen);                                   \
    HVD_VALIDATE(RayState, mRay);                                           \
    HVD_VALIDATE(RayState, mPathVertex);                                    \
    HVD_VALIDATE(RayState, mSequenceID);                                    \
    HVD_VALIDATE(RayState, mSubpixel);                                      \
    HVD_VALIDATE(RayState, mPad0);                                          \
    HVD_VALIDATE(RayState, mTilePass);                                      \
    HVD_VALIDATE(RayState, mRayStateIdx);                                   \
    HVD_VALIDATE(RayState, mAOSIsect);                                      \
    HVD_VALIDATE(RayState, mDeepDataHandle);                                \
    HVD_VALIDATE(RayState, mCryptomatteDataHandle);                         \
    HVD_VALIDATE(RayState, mCryptoRefP);                                    \
    HVD_VALIDATE(RayState, mCryptoP0);                                      \
    HVD_VALIDATE(RayState, mCryptoRefN);                                    \
    HVD_VALIDATE(RayState, mCryptoUV);                                      \
    HVD_VALIDATE(RayState, mVolRad);                                        \
    HVD_VALIDATE(RayState, mVolTr);                                         \
    HVD_VALIDATE(RayState, mVolTh);                                         \
    HVD_VALIDATE(RayState, mVolTalpha);                                     \
    HVD_VALIDATE(RayState, mVolTm);                                         \
    HVD_VALIDATE(RayState, mVolHit);                                        \
    HVD_VALIDATE(RayState, mVolumeSurfaceT);                                \
    HVD_END_VALIDATION


//----------------------------------------------------------------------------

