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

#ifndef FOUNDATION_ACE_FRAMEWORKS_CORE_COMPONENTS_INDEXER_V2_RENDER_INDEXER_H
#define FOUNDATION_ACE_FRAMEWORKS_CORE_COMPONENTS_INDEXER_V2_RENDER_INDEXER_H

#include "base/geometry/size.h"
#include "core/components/box/render_box_base.h"
#include "core/components/common/rotation/rotation_node.h"
#include "core/components/display/render_display.h"
#include "core/components/focus_animation/render_focus_animation.h"
#include "core/components_v2/indexer/indexer_component.h"
#include "core/components_v2/indexer/render_indexer_item.h"
#include "core/gestures/drag_recognizer.h"
#include "core/gestures/raw_recognizer.h"
#include "core/pipeline/base/render_node.h"

namespace OHOS::Ace::V2 {
constexpr int32_t TABLE_ANGLE = 0;
constexpr int32_t TABLE_POSITION_X = 1;
constexpr int32_t TABLE_POSITION_Y = 2;
constexpr int32_t TABLE_INDEX_COUNT = 3;
constexpr int32_t INDEXER_BUBBLE_ANIMATION_DURATION = 3000;
constexpr double ROTATION_THRESHOLD = 90.0;
constexpr double HALF = 0.5;
constexpr double DOUBLE = 2.0;

class IndexerEventInfo : public BaseEventInfo, public EventToJSONStringAdapter {
    DECLARE_RELATIONSHIP_OF_CLASSES(IndexerEventInfo, BaseEventInfo, EventToJSONStringAdapter);
public:
    IndexerEventInfo(int32_t selectedIndex) : BaseEventInfo("indexer"), selectedIndex_(selectedIndex)
    {}

    ~IndexerEventInfo() = default;

    std::string ToJSONString() const override;

    int32_t GetSelectedIndex() const
    {
        return selectedIndex_;
    }

private:
    int32_t selectedIndex_ = 0;
};

class RenderIndexer : public RenderNode, public RotationNode {
    DECLARE_ACE_TYPE(RenderIndexer, RenderNode);

public:
    using OnSelectedFunc = std::function<void(std::shared_ptr<IndexerEventInfo>&)>;
    RenderIndexer();
    ~RenderIndexer() override = default;

    static RefPtr<RenderNode> Create();

    void Update(const RefPtr<Component>& component) override;
    void PerformLayout() override;
    void OnTouchTestHit(
        const Offset& coordinateOffset, const TouchRestrict& touchRestrict, TouchTestResult& result) override;
    virtual bool TouchTest(const Point& globalPoint, const Point& parentLocalPoint, const TouchRestrict& touchRestrict,
        TouchTestResult& result) override;
    virtual bool OnRotation(const RotationEvent& event) override
    {
        return true;
    }
    virtual void HandleTouchDown(const TouchEventInfo& info);
    virtual void HandleTouchUp(const TouchEventInfo& info);
    virtual void HandleTouchMove(const TouchEventInfo& info);
    virtual int32_t GetTouchedItemIndex(const Offset& touchPosition);
    virtual void HandleListMoveItems(int32_t curHeadIndex) {}
    virtual void UpdateCurrentSectionItem(int32_t curSection) {}
    virtual void BeginFocusAnimation(int32_t originIndex, int32_t targetIndex) {}
    virtual bool IsValidBubbleBox();

    virtual bool NeedProcess(const RefPtr<RenderNode>& item) const
    {
        return true;
    }
    virtual bool NeedRotation() const
    {
        return false;
    }

    int32_t GetFocusIndex() const
    {
        return focusedItem_;
    }

    void SetFocusIndex(int32_t index)
    {
        focusedItem_ = index;
    }

    void ClearFocusAnimation()
    {
        auto context = context_.Upgrade();
        if (!context) {
            LOGE("Pipeline context upgrade fail!");
            return;
        }
        if (!context->GetRenderFocusAnimation()) {
            LOGE("focusAnimation is null!");
            return;
        }
        context->CancelFocusAnimation();
    }

    RefPtr<RenderIndexerItem> GetSpecificItem(int32_t index) const;

    void HandleFocusAnimation(const Size& size, const Offset& offset);
    void MoveSectionWithIndexer(int32_t curSection);

protected:
    void HandleDragStart();
    void HandleDragEnd();
    void HandleTouched(const Offset& touchPosition);
    void MoveList(int32_t index);

    void InitFocusedItem();
    void UpdateItems();
    void UpdateBubbleText();
    void BuildBubbleAnimation();
    void BeginBubbleAnimation();
    int32_t GetItemIndex(int32_t index); // get first list item index in specified section

    void OnSelected(int32_t selected) const;

    int32_t focusedItem_ = -1; // the item which set high light
    int32_t nonItemCount_ = 0;
    int32_t itemCount_ = INDEXER_ITEM_MAX_COUNT;

    bool clicked_ = false;
    bool bubbleEnabled_ = true;

    double itemSize_ = INDEXER_CIRCLE_ITEM_SIZE;
    double itemSizeRender_ = INDEXER_CIRCLE_ITEM_SIZE;
    double paddingX_ = INDEXER_DEFAULT_PADDING_X;
    double paddingY_ = INDEXER_DEFAULT_PADDING_Y;
    AlignStyle alignStyle_ = AlignStyle::RIGHT;

    RefPtr<RawRecognizer> touchRecognizer_;
    RefPtr<DragRecognizer> dragRecognizer_;
    RefPtr<TextComponent> bubbleText_;
    RefPtr<RenderBox> bubbleBox_;                   // reference of the bubble box render node
    RefPtr<RenderDisplay> bubbleDisplay_;           // reference of the bubble box render node
    RefPtr<Animator> bubbleController_; // control bubble appear and disappear
    Offset touchPostion_;
    std::list<RefPtr<RenderNode>> items_;

    OnSelectedFunc selectedEventFun_;
};
} // namespace OHOS::Ace
#endif // FOUNDATION_ACE_FRAMEWORKS_CORE_COMPONENTS_INDEXER_V2_RENDER_INDEXER_H