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

#ifndef FOUNDATION_ACE_FRAMEWORKS_CORE_COMPONENTS_BASE_COMPONENT_GROUP_ELEMENT_H
#define FOUNDATION_ACE_FRAMEWORKS_CORE_COMPONENTS_BASE_COMPONENT_GROUP_ELEMENT_H

#include "core/pipeline/base/render_element.h"
#include <map>

namespace OHOS::Ace {

using ComponentFunction = std::function<RefPtr<Component>(int32_t index)>;

class ACE_EXPORT ComponentGroupElement : public RenderElement {
    DECLARE_ACE_TYPE(ComponentGroupElement, RenderElement);

public:
    ComponentGroupElement() = default;
    ~ComponentGroupElement() override = default;

    static RefPtr<Element> Create();

    void PerformBuild() override;

    bool IsComposed(const std::list<RefPtr<Component>>& newComponents);

    std::list<RefPtr<Element>> UpdateCommonChildren(const std::list<RefPtr<Component>>& newComponents);
    void UpdateCommonChildrenOptimized(const std::list<RefPtr<Component>>& newComponents);

    std::list<RefPtr<Element>> UpdateComposedChildren(const std::list<RefPtr<Component>>& newComponents);

    // Updating the foreach's component
    // this will be called only when the foreach array is a observableobject and registers to foreach.
    std::list<RefPtr<Element>> UpdateComposedChildrenForEach(std::string oldFirstChildKey, int32_t oldChildCount,
        std::vector<std::string> newComponentsKeys, ComponentFunction&& func);

private:
    void UpdateComposedChildrenPreprocess(const std::vector<RefPtr<Element>>& oldChildren,
        const std::list<RefPtr<Component>>& newComponents, size_t& topLength, size_t& bottomLength, int32_t& slot);

    void UpdateComposedChildrenProcess(const std::vector<RefPtr<Element>>& oldChildren,
        const std::list<RefPtr<Component>>& newComponents, size_t topLength, size_t bottomLength, int32_t& slot,
        std::map<ComposeId, RefPtr<Element>>& oldKeyedChildren);

    void UpdateComposedChildrenPostprocess(const std::vector<RefPtr<Element>>& oldChildren,
        const std::list<RefPtr<Component>>& newComponents, size_t topLength, size_t bottomLength, int32_t& slot,
        std::map<ComposeId, RefPtr<Element>>& oldKeyedChildren);
};

} // namespace OHOS::Ace

#endif // FOUNDATION_ACE_FRAMEWORKS_CORE_COMPONENTS_BASE_COMPONENT_GROUP_ELEMENT_H