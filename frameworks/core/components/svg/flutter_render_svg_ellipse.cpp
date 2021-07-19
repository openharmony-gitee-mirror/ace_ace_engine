/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "frameworks/core/components/svg/flutter_render_svg_ellipse.h"

#include "frameworks/core/components/common/painter/flutter_svg_painter.h"
#include "frameworks/core/components/transform/flutter_render_transform.h"
#include "frameworks/core/pipeline/base/flutter_render_context.h"

namespace OHOS::Ace {

using namespace Flutter;

RefPtr<RenderNode> RenderSvgEllipse::Create()
{
    return AceType::MakeRefPtr<FlutterRenderSvgEllipse>();
}

RenderLayer FlutterRenderSvgEllipse::GetRenderLayer()
{
    if (!transformLayer_) {
        transformLayer_ = AceType::MakeRefPtr<Flutter::TransformLayer>(Matrix4::CreateIdentity(), 0.0, 0.0);
    }
    return AceType::RawPtr(transformLayer_);
}

void FlutterRenderSvgEllipse::Paint(RenderContext& context, const Offset& offset)
{
    const auto renderContext = static_cast<FlutterRenderContext*>(&context);
    flutter::Canvas* canvas = renderContext->GetCanvas();
    if (!canvas) {
        LOGE("Paint canvas is null");
        return;
    }
    SkCanvas* skCanvas = canvas->canvas();
    if (!skCanvas) {
        LOGE("Paint skCanvas is null");
        return;
    }

    SkPath path;
    double width = GetLayoutSize().Width();
    double height = GetLayoutSize().Height();
    double rx = ConvertDimensionToPx(rx_, width);
    double ry = ConvertDimensionToPx(ry_, height);
    SkRect rect = SkRect::MakeXYWH(ConvertDimensionToPx(cx_, width) - rx, ConvertDimensionToPx(cy_, height) - ry,
        rx + rx, ry + ry);
    path.addOval(rect);
    FlutterSvgPainter::SetFillStyle(skCanvas, path, fillState_, opacity_);
    FlutterSvgPainter::SetStrokeStyle(skCanvas, path, strokeState_, opacity_);
    RenderNode::Paint(context, offset);
}

void FlutterRenderSvgEllipse::UpdateMotion(const std::string& path, const std::string& rotate,
    double percent, const Point& point)
{
    if (!transformLayer_) {
        LOGE("transformLayer is null");
        return;
    }
    bool isSuccess = true;
    auto motionMatrix = FlutterSvgPainter::CreateMotionMatrix(path, rotate, point, percent, isSuccess);
    if (isSuccess) {
        auto transform = FlutterRenderTransform::GetTransformByOffset(motionMatrix, GetGlobalOffset());
        transformLayer_->Update(transform);
    }
}

bool FlutterRenderSvgEllipse::GetStartPoint(Point& point)
{
    double width = GetLayoutSize().Width();
    double height = GetLayoutSize().Height();
    point = Point(ConvertDimensionToPx(cx_, width), ConvertDimensionToPx(cy_, height));
    return true;
}

} // namespace OHOS::Ace