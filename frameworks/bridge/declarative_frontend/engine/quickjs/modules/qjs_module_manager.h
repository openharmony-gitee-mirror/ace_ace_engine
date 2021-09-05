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

#ifndef FRAMEWORKS_BRIDGE_DECLARATIVE_FRONTEND_ENGINE_QUICKJS_MODULES_QJS_MODULE_MANAGER_H
#define FRAMEWORKS_BRIDGE_DECLARATIVE_FRONTEND_ENGINE_QUICKJS_MODULES_QJS_MODULE_MANAGER_H

#include <string>
#include <unordered_map>

#include "third_party/quickjs/quickjs.h"

#include "base/utils/noncopyable.h"

namespace OHOS::Ace::Framework {
class ModuleManager {
public:
    ModuleManager() = default;
    ~ModuleManager() = default;

    static ModuleManager* GetInstance();
    bool InitModule(JSContext* ctx, const std::string& moduleName, JSValue& jsObject);

private:
    ACE_DISALLOW_COPY_AND_MOVE(ModuleManager);
};
} // namespace OHOS::Ace::Framework

#endif // FRAMEWORKS_BRIDGE_DECLARATIVE_FRONTEND_ENGINE_QUICKJS_MODULES_QJS_MODULE_MANAGER_H
