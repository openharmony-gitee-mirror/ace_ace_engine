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

#include "core/components/stepper/flutter_render_stepper.h"

namespace OHOS::Ace {

using namespace Flutter;

RefPtr<RenderNode> RenderStepper::Create()
{
    return AceType::MakeRefPtr<FlutterRenderStepper>();
}

RenderLayer FlutterRenderStepper::GetRenderLayer()
{
    if (!layer_) {
        layer_ = AceType::MakeRefPtr<ClipLayer>(
            0.0, GetLayoutSize().Width(), 0.0, GetLayoutSize().Height(), Clip::HARD_EDGE);
    }
    return AceType::RawPtr(layer_);
}

void FlutterRenderStepper::Paint(RenderContext& context, const Offset& offset)
{
    layer_->SetClip(0.0, GetLayoutSize().Width(), 0.0, GetLayoutSize().Height(), Clip::HARD_EDGE);
    if (renderProgress_) {
        if (rightButtonData_.buttonStatus == StepperButtonStatus::WAITING) {
            renderProgress_->SetVisible(true);
        } else {
            renderProgress_->SetVisible(false);
        }
    }
    RenderNode::Paint(context, offset);
}

} // namespace OHOS::Ace
