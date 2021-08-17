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

#ifndef FOUNDATION_ACE_FRAMEWORKS_CORE_COMPONENTS_SHAPE_RENDER_SHAPE_H
#define FOUNDATION_ACE_FRAMEWORKS_CORE_COMPONENTS_SHAPE_RENDER_SHAPE_H

#include "core/components/shape/shape_component.h"
#include "core/pipeline/base/render_node.h"

namespace OHOS::Ace {

class RenderShape : public RenderNode {
    DECLARE_ACE_TYPE(RenderShape, RenderNode);

public:
    static RefPtr<RenderNode> Create();
    void Update(const RefPtr<Component>& component) override;
    void PerformLayout() override;

    void OnAttachContext() override
    {
        const std::function<void()> callback = [weak = WeakClaim(this)] {
            auto renderShape = weak.Upgrade();
            if (renderShape) {
                renderShape->OnAnimationCallback();
            }
        };
        strokeState_.SetContextAndCallback(context_, callback);
        fillState_.SetContextAndCallback(context_, callback);
        width_.SetContextAndCallback(context_, callback);
        height_.SetContextAndCallback(context_, callback);
        topLeftRadius_.SetContextAndCallback(context_, callback);
        topRightRadius_.SetContextAndCallback(context_, callback);
        bottomRightRadius_.SetContextAndCallback(context_, callback);
        bottomLeftRadius_.SetContextAndCallback(context_, callback);
        pathCmd_.SetContextAndCallback(context_, callback);
    }

    virtual Size CalcSize() = 0;
    void OnAnimationCallback();

protected:
    AnimatableDimension width_ = AnimatableDimension(-1.0, DimensionUnit::PX);  // exclude margin
    AnimatableDimension height_ = AnimatableDimension(-1.0, DimensionUnit::PX); // exclude margin
    ShapeType shapeType_ = ShapeType::RECT;
    Radius topLeftRadius_;
    Radius topRightRadius_;
    Radius bottomRightRadius_;
    Radius bottomLeftRadius_;
    ShapePoint start_;
    ShapePoint end_;
    std::vector<ShapePoint> points_;
    AnimatablePath pathCmd_;
    FillState fillState_;
    StrokeState strokeState_;
    std::pair<bool, bool> antiAlias_ = std::make_pair(false, true);
};

} // namespace OHOS::Ace

#endif // FOUNDATION_ACE_FRAMEWORKS_CORE_COMPONENTS_SHAPE_RENDER_SHAPE_H
