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

#include "core/components/stack/stack_element.h"

#include "core/components/bubble/bubble_element.h"
#include "core/components/dialog/dialog_element.h"
#include "core/components/drop_filter/drop_filter_element.h"
#include "core/components/page/page_element.h"
#include "core/components/picker/picker_base_element.h"
#include "core/components/popup/popup_component.h"
#include "core/components/select_popup/select_popup_element.h"
#include "core/components/text_overlay/text_overlay_element.h"
namespace OHOS::Ace {

void StackElement::PushInstant(const RefPtr<Component>& newComponent, bool disableTouchEvent)
{
    PopupComponentInfo pushComponentInfo = { -1, "-1", Operation::DIRECT_PUSH, newComponent };
    popupComponentInfos_.emplace_back(pushComponentInfo);
    disableTouchEvent_ = disableTouchEvent;
    PerformBuild();
}

void StackElement::PushComponent(const RefPtr<Component>& newComponent, bool disableTouchEvent)
{
    PopupComponentInfo pushComponentInfo = { -1, "-1", Operation::DIRECT_PUSH, newComponent };
    popupComponentInfos_.emplace_back(pushComponentInfo);
    disableTouchEvent_ = disableTouchEvent;
    MarkDirty();
}

void StackElement::PopComponent()
{
    PopupComponentInfo popComponentInfo = { -1, "-1", Operation::DIRECT_POP, nullptr };
    popupComponentInfos_.emplace_back(popComponentInfo);
    MarkDirty();
}

void StackElement::PushToastComponent(const RefPtr<Component>& newComponent, int32_t toastId)
{
    PopupComponentInfo pushComponentInfo = { toastId, "-1", Operation::TOAST_PUSH, newComponent };
    popupComponentInfos_.emplace_back(pushComponentInfo);
    MarkDirty();
}

void StackElement::PopToastComponent(int32_t toastPopId)
{
    PopupComponentInfo popComponentInfo = { toastPopId, "-1", Operation::TOAST_POP, nullptr };
    popupComponentInfos_.emplace_back(popComponentInfo);
    MarkDirty();
}

void StackElement::PushPanel(const RefPtr<Component>& newComponent, bool disableTouchEvent)
{
    PopupComponentInfo pushComponentInfo = { -1, "-1", Operation::PANEL_PUSH, newComponent };
    popupComponentInfos_.emplace_back(pushComponentInfo);
    disableTouchEvent_ = disableTouchEvent;
    MarkDirty();
}

void StackElement::PopPanel()
{
    PopDialog();
}

bool StackElement::PushDialog(const RefPtr<Component>& newComponent, bool disableTouchEvent)
{
    auto context = context_.Upgrade();
    if (context) {
        AccessibilityEvent stackEvent;
        stackEvent.eventType = "ejectdismiss";
        context->SendEventToAccessibility(stackEvent);
    }
    PopupComponentInfo pushComponentInfo = { -1, "-1", Operation::DIALOG_PUSH, newComponent };
    popupComponentInfos_.emplace_back(pushComponentInfo);
    disableTouchEvent_ = disableTouchEvent;
    MarkDirty();
    return true;
}

bool StackElement::PopDialog(int32_t id)
{
    auto context = context_.Upgrade();
    if (context) {
        AccessibilityEvent stackEvent;
        stackEvent.eventType = "ejectdismiss";
        context->SendEventToAccessibility(stackEvent);
    }
    PopupComponentInfo popComponentInfo = { id, "-1", Operation::DIALOG_POP, nullptr };
    popupComponentInfos_.emplace_back(popComponentInfo);
    MarkDirty();
    return true;
}

void StackElement::PopTextOverlay()
{
    PopupComponentInfo popComponentInfo = { -1, "-1", Operation::TEXT_OVERLAY_POP, nullptr };
    popupComponentInfos_.emplace_back(popComponentInfo);
    MarkDirty();
}

void StackElement::PopPopup(const ComposeId& id)
{
    PopupComponentInfo popComponentInfo = { -1, id, Operation::POPUP_POP, nullptr };
    popupComponentInfos_.emplace_back(popComponentInfo);
    MarkDirty();
}

void StackElement::PopMenu()
{
    PopupComponentInfo popComponentInfo = { -1, "-1", Operation::MENU_POP, nullptr };
    popupComponentInfos_.emplace_back(popComponentInfo);
    MarkDirty();
}

void StackElement::PopInstant ()
{
    auto child = children_.end();
    if (child != children_.begin()) {
        child--;
        UpdateChild(*child, nullptr);
    }
    EnableTouchEventAndRequestFocus();
}

void StackElement::PerformBuild()
{
    // rebuild popup component
    for (auto& info : popupComponentInfos_) {
        PerformPopupChild(info);
    }

    ComponentGroupElement::PerformBuild();
    popupComponentInfos_.clear();
}

void StackElement::PerformPopupChild(PopupComponentInfo& popupComponentInfo)
{
    switch (popupComponentInfo.operation) {
        case Operation::TOAST_PUSH:
            PerformPushToast(popupComponentInfo);
            break;
        case Operation::DIRECT_PUSH:
        case Operation::DIALOG_PUSH:
        case Operation::PANEL_PUSH:
            PerformPushChild(popupComponentInfo);
            break;
        case Operation::TOAST_POP:
            PerformPopToastById(popupComponentInfo.popId);
            break;
        case Operation::DIALOG_POP:
            PerformPopDialog(popupComponentInfo.popId);
            break;
        case Operation::TEXT_OVERLAY_POP:
            PerformPopTextOverlay();
            break;
        case Operation::POPUP_POP:
            PerformPopPopup(popupComponentInfo.id);
            break;
        case Operation::MENU_POP:
            PerformPopMenu();
            break;
        case Operation::DIRECT_POP:
            PerformDirectPop();
            break;
        default:
            LOGD("Dont't need pop popup component");
    }
}

void StackElement::PerformPushToast(PopupComponentInfo& popupComponentInfo)
{
    if (!popupComponentInfo.IsValid() || popupComponentInfo.operation != Operation::TOAST_PUSH) {
        return;
    }
    PerformPopToast();
    // store toast element
    RefPtr<Element> toastElement = UpdateChild(nullptr, popupComponentInfo.component);
    if (toastElement) {
        ToastInfo toastInfo = { popupComponentInfo.popId, toastElement };
        toastStack_.emplace_back(toastInfo);
    }
    popupComponentInfo.component = nullptr;
}

void StackElement::PerformPushChild(PopupComponentInfo& popupComponentInfo)
{
    if (!popupComponentInfo.IsValid()) {
        return;
    }
    // store toast element
    if (!UpdateChild(nullptr, popupComponentInfo.component)) {
        return;
    }
    for (auto child = (++children_.rbegin()); child != children_.rend(); ++child) {
        auto renderNode = (*child)->GetRenderNode();
        if (renderNode) {
            renderNode->SetDisableTouchEvent(disableTouchEvent_);
        }
    }
    auto renderNode = GetRenderNode();
    if (!renderNode) {
        return;
    }
    renderNode->MarkNeedLayout();
    if (!focusNodes_.empty() && focusNodes_.back()->IsFocusable()) {
        focusNodes_.back()->RequestFocus();
    }
    popupComponentInfo.component = nullptr;
}

void StackElement::PerformPopToastById(int32_t toastId)
{
    if (toastStack_.empty()) {
        return;
    }
    for (auto iter = toastStack_.end() - 1; iter >= toastStack_.begin(); --iter) {
        if (iter->toastId == toastId) {
            UpdateChild(iter->child, nullptr);
            toastStack_.erase(iter);
            break;
        }
    }
    EnableTouchEventAndRequestFocus();
}

void StackElement::PerformPopToast()
{
    LOGD("PerformPopToast");
    if (!toastStack_.empty()) {
        UpdateChild(toastStack_.back().child, nullptr);
        toastStack_.pop_back();
    }
}

void StackElement::PerformPopDialog(int32_t id)
{
    if (id >= 0) {
        PerformPopDialogById(id);
        return;
    }

    bool hasDialog = std::any_of(children_.begin(), children_.end(), [](const RefPtr<Element>& child) {
        return AceType::InstanceOf<DialogElement>(child) || AceType::InstanceOf<PickerBaseElement>(child) ||
               AceType::InstanceOf<DropFilterElement>(child);
    });
    if (!hasDialog) {
        EnableTouchEventAndRequestFocus();
        return;
    }
    for (auto iter = children_.rbegin(); iter != children_.rend(); ++iter) {
        if (AceType::InstanceOf<DialogElement>(*iter) || AceType::InstanceOf<PickerBaseElement>(*iter) ||
            AceType::InstanceOf<DropFilterElement>(*iter)) {
            UpdateChild(*iter, nullptr);
            break;
        }
    }
    EnableTouchEventAndRequestFocus();
}

void StackElement::PerformPopDialogById(int32_t id)
{
    bool hasDialog = std::any_of(children_.begin(), children_.end(),
        [](const RefPtr<Element>& child) { return AceType::InstanceOf<DialogElement>(child); });
    if (!hasDialog) {
        EnableTouchEventAndRequestFocus();
        return;
    }
    for (auto iter = children_.rbegin(); iter != children_.rend(); ++iter) {
        auto dialogElement = AceType::DynamicCast<DialogElement>(*iter);
        if (dialogElement && dialogElement->GetDialogId() == id) {
            UpdateChild(*iter, nullptr);
            break;
        }
    }
    EnableTouchEventAndRequestFocus();
}

void StackElement::PerformPopTextOverlay()
{
    for (auto iter = children_.rbegin(); iter != children_.rend(); ++iter) {
        if (AceType::InstanceOf<TextOverlayElement>(*iter)) {
            UpdateChild(*iter, nullptr);
            break;
        }
    }
    if (IsFocusable()) {
        RequestFocus();
    }
    EnableTouchEventAndRequestFocus();
}

void StackElement::PerformPopPopup(const ComposeId& id)
{
    for (auto iter = children_.rbegin(); iter != children_.rend(); ++iter) {
        auto child = DynamicCast<BubbleElement>(*iter);
        if (child && child->GetId() == id) {
            child->FirePopEvent();
            UpdateChild(child, nullptr);
            break;
        }
    }
    if (IsFocusable()) {
        RequestFocus();
    }
    EnableTouchEventAndRequestFocus();
}

void StackElement::PerformPopMenu()
{
    for (auto iter = children_.rbegin(); iter != children_.rend(); ++iter) {
        if (AceType::InstanceOf<SelectPopupElement>(*iter)) {
            UpdateChild(*iter, nullptr);
            break;
        }
    }
    if (IsFocusable()) {
        RequestFocus();
    }
    EnableTouchEventAndRequestFocus();
}

void StackElement::PerformDirectPop()
{
    auto child = children_.end();
    while (child != children_.begin()) {
        child--;
        bool isNotToast = std::none_of(
            toastStack_.begin(), toastStack_.end(), [child](const ToastInfo& toast) { return toast.child == *child; });
        if (isNotToast) {
            UpdateChild(*child, nullptr);
            break;
        }
    }
    EnableTouchEventAndRequestFocus();
}

bool StackElement::RequestNextFocus(bool vertical, bool reverse, const Rect& rect)
{
    // Only consider the top node.
    return false;
}

void StackElement::OnFocus()
{
    if (focusNodes_.empty()) {
        itLastFocusNode_ = focusNodes_.end();
        return;
    }
    // Only focus on the top focusable child.
    if (itLastFocusNode_ != focusNodes_.end()) {
        return;
    }
    while (itLastFocusNode_ != focusNodes_.begin()) {
        --itLastFocusNode_;
        if ((*itLastFocusNode_)->RequestFocusImmediately()) {
            FocusNode::OnFocus();
            return;
        }
    }

    // Not found any focusable node, clear focus.
    itLastFocusNode_ = focusNodes_.end();
}

void StackElement::EnableTouchEventAndRequestFocus()
{
    for (auto& child : children_) {
        auto renderNode = child->GetRenderNode();
        if (renderNode) {
            renderNode->SetDisableTouchEvent(false);
        }
    }
    if (IsFocusable()) {
        RequestFocus();
    }
}

} // namespace OHOS::Ace
