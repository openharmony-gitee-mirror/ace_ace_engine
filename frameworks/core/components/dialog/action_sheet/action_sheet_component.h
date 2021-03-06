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

#ifndef FOUNDATION_ACE_FRAMEWORKS_CORE_COMPONENTS_ACTION_SHEET_ACTION_SHEET_COMPONENT_H
#define FOUNDATION_ACE_FRAMEWORKS_CORE_COMPONENTS_ACTION_SHEET_ACTION_SHEET_COMPONENT_H

#include "core/components/dialog/dialog_component.h"

namespace OHOS::Ace {

class ActionSheetComponent : public DialogComponent {
    DECLARE_ACE_TYPE(ActionSheetComponent, DialogComponent);

public:
    void BuildChild(const RefPtr<ThemeManager>& themeManager) override;

private:
    void BuildMenu(const RefPtr<ColumnComponent>& column) override;
    void BuildContent(const RefPtr<ColumnComponent>& column) override;
    void BuildActions(const RefPtr<ThemeManager>& themeManager, const RefPtr<ColumnComponent>& column) override;
};

} // namespace OHOS::Ace

#endif // FOUNDATION_ACE_FRAMEWORKS_CORE_COMPONENTS_ACTION_SHEET_ACTION_SHEET_COMPONENT_H
