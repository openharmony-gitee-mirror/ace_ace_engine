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

#include "core/components/picker/picker_text_component.h"

namespace OHOS::Ace {

PickerTextComponent::PickerTextComponent()
{
    auto column = AceType::MakeRefPtr<PickerColumnComponent>();
    column->SetColumnTag(PICKER_TEXT_COLUMN);
    AppendColumn(column);
}

void PickerTextComponent::OnTitleBuilding()
{
    PickerBaseComponent::OnTitleBuilding();
    SetHasTitle(false);
}

void PickerTextComponent::OnColumnsBuilding()
{
    auto column = GetColumn(PICKER_TEXT_COLUMN);
    if (!column) {
        LOGE("can not get column of text");
        return;
    }

    column->ClearOption();
    for (const auto& item : range_) {
        column->AppendOption(item);
    }
    selectedIndex_ = range_.empty() ? 0 : selectedIndex_ % range_.size();
    column->SetCurrentIndex(selectedIndex_);
}

std::string PickerTextComponent::GetSelectedObject(bool isColumnChange, const std::string& changeColumnTag) const
{
    if (isColumnChange) {
        LOGW("text picker has no column change event.");
        return "";
    }
    return std::string("{\"newValue\":\"") +
        selectedValue_ + "\",\"newSelected\":" + std::to_string(selectedIndex_) + "}";
}

void PickerTextComponent::OnSelectedSaving()
{
    auto column = GetColumn(PICKER_TEXT_COLUMN);
    if (!column) {
        LOGE("can not get column of text");
        return;
    }
    selectedIndex_ = column->GetCurrentIndex();
    selectedValue_ = column->GetCurrentText();
}

void PickerTextComponent::HandleSelectedChange()
{
    if (!IsDialogShowed()) {
        return;
    }
    auto column = GetColumn(PICKER_TEXT_COLUMN);
    if (!column) {
        LOGE("can not get column of text");
        return;
    }

    OnColumnsBuilding();
    column->HandleChangeCallback(true, false);
}

void PickerTextComponent::HandleRangeChange()
{
    if (!IsDialogShowed()) {
        return;
    }

    auto column = GetColumn(PICKER_TEXT_COLUMN);
    if (!column) {
        LOGE("can not get column of text");
        return;
    }

    auto backupIndex = selectedIndex_;
    auto backupValue = selectedValue_;
    OnSelectedSaving();
    OnColumnsBuilding();
    column->HandleChangeCallback(true, false);
    selectedIndex_ = backupIndex;
    selectedValue_ = backupValue;
}

} // namespace OHOS::Ace
