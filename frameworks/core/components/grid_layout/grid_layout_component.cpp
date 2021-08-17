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

#include "core/components/grid_layout/grid_layout_component.h"

#include "core/components/grid_layout/grid_layout_element.h"
#include "core/components/grid_layout/render_grid_layout.h"
#include "core/components/grid_layout/render_grid_scroll_layout.h"
#include "core/pipeline/base/multi_composed_component.h"

namespace OHOS::Ace {

RefPtr<Element> GridLayoutComponent::CreateElement()
{
    return AceType::MakeRefPtr<GridLayoutElement>();
}

RefPtr<RenderNode> GridLayoutComponent::CreateRenderNode()
{
    if (rowsArgs_.empty() || columnsArgs_.empty()) {
        return RenderGridScrollLayout::Create();
    }

    return RenderGridLayout::Create();
}

void GridLayoutComponent::AppendChild(const RefPtr<Component>& child)
{
    if (AceType::InstanceOf<V2::LazyForEachComponent>(child)) {
        auto lazyForEach = AceType::DynamicCast<V2::LazyForEachComponent>(child);
        lazyForEachComponent_ = lazyForEach;
        return;
    }

    auto multiComposed = AceType::DynamicCast<MultiComposedComponent>(child);
    if (!multiComposed) {
        ComponentGroup::AppendChild(child);
        return;
    }

    for (const auto& childComponent : multiComposed->GetChildren()) {
        AppendChild(childComponent);
    }
}

void GridLayoutComponent::SetDirection(FlexDirection direction)
{
    if (direction < FlexDirection::ROW || direction > FlexDirection::COLUMN_REVERSE) {
        LOGW("Invalid direction %{public}d", direction);
        return;
    }
    direction_ = direction;
}

void GridLayoutComponent::SetFlexAlign(FlexAlign flexAlign)
{
    if (flexAlign < FlexAlign::FLEX_START || flexAlign > FlexAlign::STRETCH) {
        LOGW("Invalid flexAlign %{public}d", flexAlign);
        return;
    }
    flexAlign_ = flexAlign;
}

void GridLayoutComponent::SetColumnCount(int32_t count)
{
    if (count <= 0) {
        LOGW("Invalid ColumnCount %{public}d", count);
        return;
    }
    columnCount_ = count;
}

void GridLayoutComponent::SetRowCount(int32_t count)
{
    if (count <= 0) {
        LOGW("Invalid RowCount %{public}d", count);
        return;
    }
    rowCount_ = count;
}

void GridLayoutComponent::SetWidth(double width)
{
    if (width <= 0.0) {
        LOGW("Invalid Width %{public}lf", width);
        return;
    }
    width_ = width;
}

void GridLayoutComponent::SetHeight(double height)
{
    if (height <= 0.0) {
        LOGW("Invalid Height %{public}lf", height);
        return;
    }
    height_ = height;
}

void GridLayoutComponent::SetColumnsArgs(const std::string& columnsArgs)
{
    columnsArgs_ = columnsArgs;
}

void GridLayoutComponent::SetRowsArgs(const std::string& rowsArgs)
{
    rowsArgs_ = rowsArgs;
}

void GridLayoutComponent::SetColumnGap(const Dimension& columnGap)
{
    if (columnGap.Value() < 0.0) {
        LOGW("Invalid RowGap, use 0px");
        columnGap_ = 0.0_px;
        return;
    }
    columnGap_ = columnGap;
}

void GridLayoutComponent::SetRowGap(const Dimension& rowGap)
{
    if (rowGap.Value() < 0.0) {
        LOGW("Invalid RowGap, use 0px");
        rowGap_ = 0.0_px;
        return;
    }
    rowGap_ = rowGap;
}

void GridLayoutComponent::SetRightToLeft(bool rightToLeft)
{
    LOGD("SetRightToLeft to %{public}d.", rightToLeft);
    rightToLeft_ = rightToLeft;
}

} // namespace OHOS::Ace
