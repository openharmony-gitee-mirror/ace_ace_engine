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

#include "frameworks/bridge/declarative_frontend/engine/jsi/modules/jsi_router_module.h"

#include "base/json/json_util.h"
#include "base/log/log.h"
#include "frameworks/bridge/declarative_frontend/engine/jsi/jsi_declarative_engine.h"
#include "frameworks/bridge/js_frontend/engine/common/js_constants.h"

namespace OHOS::Ace::Framework {

std::string ParseRouteUrl(const shared_ptr<JsRuntime>& runtime, const shared_ptr<JsValue>& arg, const std::string& key)
{
    LOGD("ParseRouteUrl");
    std::string argStr = arg->GetJsonString(runtime);
    if (argStr.empty()) {
        return argStr;
    }

    std::string pageRoute;
    std::unique_ptr<JsonValue> argsPtr = JsonUtil::ParseJsonString(argStr);
    if (argsPtr != nullptr && argsPtr->GetValue(key) != nullptr && argsPtr->GetValue(key)->IsString()) {
        pageRoute = argsPtr->GetValue(key)->GetString();
    }
    LOGD("JsParseRouteUrl pageRoute = %{private}s", pageRoute.c_str());

    return pageRoute;
}

std::string ParseRouteParams(const shared_ptr<JsRuntime>& runtime, const shared_ptr<JsValue>& arg,
    const std::string& key)
{
    std::string argStr = arg->GetJsonString(runtime);
    std::unique_ptr<JsonValue> argsPtr = JsonUtil::ParseJsonString(argStr);
    std::string params;
    if (argsPtr != nullptr && argsPtr->Contains(key) && argsPtr->GetValue(key)->IsObject()) {
        params = argsPtr->GetValue(key)->ToString();
    }
    return params;
}

shared_ptr<JsValue> PagePush(const shared_ptr<JsRuntime>& runtime, const shared_ptr<JsValue>& thisObj,
    const std::vector<shared_ptr<JsValue>>& argv, int32_t argc)
{
    if (argc != 1) {
        LOGE("PagePush args count is invalid");
        return runtime->NewNull();
    }

    std::string uri = ParseRouteUrl(runtime, argv[0], ROUTE_KEY_URI);
    std::string params = ParseRouteParams(runtime, argv[0], ROUTE_KEY_PARAMS);
    auto instance = static_cast<JsiDeclarativeEngineInstance*>(runtime->GetEmbedderData());
    if (instance == nullptr) {
        LOGE("get jsi engine instance failed");
        return runtime->NewNull();
    }

    instance->GetDelegate()->Push(uri, params);
    return runtime->NewNull();
}

shared_ptr<JsValue> PageReplace(const shared_ptr<JsRuntime>& runtime, const shared_ptr<JsValue>& thisObj,
    const std::vector<shared_ptr<JsValue>>& argv, int32_t argc)
{
    if (argc != 1) {
        LOGE("PageReplace args count is invalid");
        return runtime->NewNull();
    }

    std::string uri = ParseRouteUrl(runtime, argv[0], ROUTE_KEY_URI);
    std::string params = ParseRouteParams(runtime, argv[0], ROUTE_KEY_PARAMS);
    auto instance = static_cast<JsiDeclarativeEngineInstance*>(runtime->GetEmbedderData());
    if (instance == nullptr) {
        LOGE("get jsi engine instance failed");
        return runtime->NewNull();
    }

    instance->GetDelegate()->Replace(uri, params);
    return runtime->NewNull();
}

shared_ptr<JsValue> PageBack(const shared_ptr<JsRuntime>& runtime, const shared_ptr<JsValue>& thisObj,
    const std::vector<shared_ptr<JsValue>>& argv, int32_t argc)
{
    if (argc != 1 && argc != 0) {
        LOGE("PageBack args count is invalid");
        return runtime->NewNull();
    }

    std::string uri;
    std::string params;
    if (argc == 1) {
        uri = ParseRouteUrl(runtime, argv[0], ROUTE_KEY_URI);
        params = ParseRouteParams(runtime, argv[0], ROUTE_KEY_PARAMS);
    }
    auto instance = static_cast<JsiDeclarativeEngineInstance*>(runtime->GetEmbedderData());
    if (instance == nullptr) {
        LOGE("get jsi engine instance failed");
        return runtime->NewNull();
    }

    instance->GetDelegate()->Back(uri, params);
    return runtime->NewNull();
}

shared_ptr<JsValue> PageClear(const shared_ptr<JsRuntime>& runtime, const shared_ptr<JsValue>& thisObj,
    const std::vector<shared_ptr<JsValue>>& argv, int32_t argc)
{
    auto instance = static_cast<JsiDeclarativeEngineInstance*>(runtime->GetEmbedderData());
    if (instance == nullptr) {
        LOGE("get jsi engine instance failed");
        return runtime->NewNull();
    }

    instance->GetDelegate()->Clear();
    return runtime->NewNull();
}

shared_ptr<JsValue> PageGetLength(const shared_ptr<JsRuntime>& runtime, const shared_ptr<JsValue>& thisObj,
    const std::vector<shared_ptr<JsValue>>& argv, int32_t argc)
{
    auto instance = static_cast<JsiDeclarativeEngineInstance*>(runtime->GetEmbedderData());
    if (instance == nullptr) {
        LOGE("get jsi engine instance failed");
        return runtime->NewNull();
    }

    int32_t routeLength = instance->GetDelegate()->GetStackSize();
    return runtime->NewString(std::to_string(routeLength));
}

shared_ptr<JsValue> PageGetState(const shared_ptr<JsRuntime>& runtime, const shared_ptr<JsValue>& thisObj,
    const std::vector<shared_ptr<JsValue>>& argv, int32_t argc)
{
    auto instance = static_cast<JsiDeclarativeEngineInstance*>(runtime->GetEmbedderData());
    if (instance == nullptr) {
        LOGE("get jsi engine instance failed");
        return runtime->NewNull();
    }

    int32_t routeIndex = 0;
    std::string routeName;
    std::string routePath;
    instance->GetDelegate()->GetState(routeIndex, routeName, routePath);
    shared_ptr<JsValue> jsState = runtime->NewObject();
    jsState->SetProperty(runtime, "index", runtime->NewNumber(routeIndex));
    jsState->SetProperty(runtime, "name", runtime->NewString(routeName));
    jsState->SetProperty(runtime, "path", runtime->NewString(routePath));
    return jsState;
}

shared_ptr<JsValue> PageGetParams(const shared_ptr<JsRuntime>& runtime, const shared_ptr<JsValue>& thisObj,
    const std::vector<shared_ptr<JsValue>>& argv, int32_t argc)
{
    auto instance = static_cast<JsiDeclarativeEngineInstance*>(runtime->GetEmbedderData());
    if (instance == nullptr) {
        LOGE("get jsi engine instance failed");
        return runtime->NewNull();
    }

    std::string paramsStr = instance->GetDelegate()->GetParams();
    if (paramsStr.empty()) {
        LOGI("PageGetParams params is null");
        return runtime->NewNull();
    }
    return runtime->ParseJson(paramsStr);
}

void InitRouterModule(const shared_ptr<JsRuntime>& runtime, shared_ptr<JsValue>& moduleObj)

{
    moduleObj->SetProperty(runtime, ROUTE_PAGE_PUSH, runtime->NewFunction(PagePush));
    moduleObj->SetProperty(runtime, ROUTE_PAGE_REPLACE, runtime->NewFunction(PageReplace));
    moduleObj->SetProperty(runtime, ROUTE_PAGE_BACK, runtime->NewFunction(PageBack));
    moduleObj->SetProperty(runtime, ROUTE_PAGE_CLEAR, runtime->NewFunction(PageClear));
    moduleObj->SetProperty(runtime, ROUTE_PAGE_GET_LENGTH, runtime->NewFunction(PageGetLength));
    moduleObj->SetProperty(runtime, ROUTE_PAGE_GET_STATE, runtime->NewFunction(PageGetState));
    moduleObj->SetProperty(runtime, ROUTE_PAGE_GET_PARAMS, runtime->NewFunction(PageGetParams));
}

} // namespace OHOS::Ace::Framework
