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

#ifndef FOUNDATION_ACE_ADAPTER_PREVIEW_INSPECTOR_INSPECT_MENU_H
#define FOUNDATION_ACE_ADAPTER_PREVIEW_INSPECTOR_INSPECT_MENU_H

#include "adapter/preview/inspector/inspect_node.h"

namespace OHOS::Ace::Framework {
class InspectMenu : public InspectNode {
    DECLARE_ACE_TYPE(InspectMenu, InspectNode);

public:
    InspectMenu(NodeId nodeId, const std::string& nodeName);
    ~InspectMenu() override = default;
    void PackAttrAndStyle() override;
};
} // namespace OHOS::Ace::Framework

#endif // FOUNDATION_ACE_ADAPTER_PREVIEW_INSPECTOR_INSPECT_MENU_H
