/*
 * Copyright 2018 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

/**************************************************************************************************
 *** This file was autogenerated from GrRRectBlurEffect.fp; do not modify.
 **************************************************************************************************/
#ifndef GrRRectBlurEffect_DEFINED
#define GrRRectBlurEffect_DEFINED
#include "SkTypes.h"

#include "GrCaps.h"
#include "GrClip.h"
#include "GrPaint.h"
#include "GrProxyProvider.h"
#include "GrRecordingContext.h"
#include "GrRecordingContextPriv.h"
#include "GrRenderTargetContext.h"
#include "GrStyle.h"
#include "SkBlurMaskFilter.h"
#include "SkBlurPriv.h"
#include "SkGpuBlurUtils.h"
#include "SkRRectPriv.h"
#include "GrFragmentProcessor.h"
#include "GrCoordTransform.h"
class GrRRectBlurEffect : public GrFragmentProcessor {
public:
    static sk_sp<GrTextureProxy> find_or_create_rrect_blur_mask(GrRecordingContext* context,
                                                                const SkRRect& rrectToDraw,
                                                                const SkISize& size,
                                                                float xformedSigma) {
        static const GrUniqueKey::Domain kDomain = GrUniqueKey::GenerateDomain();
        GrUniqueKey key;
        GrUniqueKey::Builder builder(&key, kDomain, 9, "RoundRect Blur Mask");
        builder[0] = SkScalarCeilToInt(xformedSigma - 1 / 6.0f);

        int index = 1;
        for (auto c : {SkRRect::kUpperLeft_Corner, SkRRect::kUpperRight_Corner,
                       SkRRect::kLowerRight_Corner, SkRRect::kLowerLeft_Corner}) {
            SkASSERT(SkScalarIsInt(rrectToDraw.radii(c).fX) &&
                     SkScalarIsInt(rrectToDraw.radii(c).fY));
            builder[index++] = SkScalarCeilToInt(rrectToDraw.radii(c).fX);
            builder[index++] = SkScalarCeilToInt(rrectToDraw.radii(c).fY);
        }
        builder.finish();

        GrProxyProvider* proxyProvider = context->priv().proxyProvider();

        sk_sp<GrTextureProxy> mask(
                proxyProvider->findOrCreateProxyByUniqueKey(key, kBottomLeft_GrSurfaceOrigin));
        if (!mask) {
            GrBackendFormat format =
                    context->priv().caps()->getBackendFormatFromColorType(kAlpha_8_SkColorType);
            // TODO: this could be approx but the texture coords will need to be updated
            sk_sp<GrRenderTargetContext> rtc(
                    context->priv().makeDeferredRenderTargetContextWithFallback(
                            format, SkBackingFit::kExact, size.fWidth, size.fHeight,
                            kAlpha_8_GrPixelConfig, nullptr));
            if (!rtc) {
                return nullptr;
            }

            GrPaint paint;

            rtc->clear(nullptr, SK_PMColor4fTRANSPARENT,
                       GrRenderTargetContext::CanClearFullscreen::kYes);
            rtc->drawRRect(GrNoClip(), std::move(paint), GrAA::kYes, SkMatrix::I(), rrectToDraw,
                           GrStyle::SimpleFill());

            sk_sp<GrTextureProxy> srcProxy(rtc->asTextureProxyRef());
            if (!srcProxy) {
                return nullptr;
            }
            sk_sp<GrRenderTargetContext> rtc2(
                    SkGpuBlurUtils::GaussianBlur(context,
                                                 std::move(srcProxy),
                                                 nullptr,
                                                 SkIRect::MakeWH(size.fWidth, size.fHeight),
                                                 SkIRect::EmptyIRect(),
                                                 xformedSigma,
                                                 xformedSigma,
                                                 GrTextureDomain::kIgnore_Mode,
                                                 kPremul_SkAlphaType,
                                                 SkBackingFit::kExact));
            if (!rtc2) {
                return nullptr;
            }

            mask = rtc2->asTextureProxyRef();
            if (!mask) {
                return nullptr;
            }
            SkASSERT(mask->origin() == kBottomLeft_GrSurfaceOrigin);
            proxyProvider->assignUniqueKeyToProxy(key, mask.get());
        }

        return mask;
    }
    float sigma() const { return fSigma; }
    const SkRect& rect() const { return fRect; }
    float cornerRadius() const { return fCornerRadius; }

    static std::unique_ptr<GrFragmentProcessor> Make(GrRecordingContext* context,
                                                     float sigma,
                                                     float xformedSigma,
                                                     const SkRRect& srcRRect,
                                                     const SkRRect& devRRect);
    GrRRectBlurEffect(const GrRRectBlurEffect& src);
    std::unique_ptr<GrFragmentProcessor> clone() const override;
    const char* name() const override { return "RRectBlurEffect"; }

private:
    GrRRectBlurEffect(float sigma, SkRect rect, float cornerRadius,
                      sk_sp<GrTextureProxy> ninePatchSampler)
            : INHERITED(kGrRRectBlurEffect_ClassID,
                        (OptimizationFlags)kCompatibleWithCoverageAsAlpha_OptimizationFlag)
            , fSigma(sigma)
            , fRect(rect)
            , fCornerRadius(cornerRadius)
            , fNinePatchSampler(std::move(ninePatchSampler)) {
        this->setTextureSamplerCnt(1);
    }
    GrGLSLFragmentProcessor* onCreateGLSLInstance() const override;
    void onGetGLSLProcessorKey(const GrShaderCaps&, GrProcessorKeyBuilder*) const override;
    bool onIsEqual(const GrFragmentProcessor&) const override;
    const TextureSampler& onTextureSampler(int) const override;
    GR_DECLARE_FRAGMENT_PROCESSOR_TEST
    float fSigma;
    SkRect fRect;
    float fCornerRadius;
    TextureSampler fNinePatchSampler;
    typedef GrFragmentProcessor INHERITED;
};
#endif
