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

#ifndef FOUNDATION_ACE_FRAMEWORKS_BRIDGE_PA_BACKEND_ENGINE_QUICKJS_QJS_PA_ENGINE_H
#define FOUNDATION_ACE_FRAMEWORKS_BRIDGE_PA_BACKEND_ENGINE_QUICKJS_QJS_PA_ENGINE_H

#include <cstdlib>
#include <mutex>
#include <vector>

#include "third_party/quickjs/quickjs.h"

#include "base/memory/ace_type.h"
#include "base/utils/noncopyable.h"
#include "core/common/ace_page.h"
#include "core/common/js_message_dispatcher.h"

#include "frameworks/bridge/pa_backend/engine/common/js_backend_engine.h"
#include "frameworks/bridge/pa_backend/backend_delegate.h"

#include "napi/native_api.h"
#include "napi/native_node_api.h"
#include "native_engine/impl/quickjs/quickjs_native_engine.h"

#include "abs_shared_result_set.h"
#include "data_ability_predicates.h"
#include "result_set.h"
#include "uri.h"
#include "values_bucket.h"

namespace OHOS::Ace::Framework {
// Each JsFrontend holds only one QjsPaEngineInstance.
class QjsPaEngineInstance final : public AceType, public JsBackendEngineInstance {
public:
    explicit QjsPaEngineInstance(const RefPtr<BackendDelegate>& delegate, int32_t instanceId)
        : backendDelegate_(delegate), instanceId_(instanceId)
    {}
    ~QjsPaEngineInstance() override;

    bool InitJsEnv(JSRuntime* runtime, JSContext* context);

    JSRuntime* GetQjsRuntime() const
    {
        return runtime_;
    }

    JSContext* GetQjsContext() const
    {
        return context_;
    }

    void CallJs(const std::string& callbackId, const std::string& args, bool keepAlive = false, bool isGlobal = false);

    void SetJsMessageDispatcher(const RefPtr<JsMessageDispatcher>& dispatcher)
    {
        dispatcher_ = dispatcher;
    }

    RefPtr<BackendDelegate> GetDelegate() const
    {
        return backendDelegate_;
    }

    bool CallPlatformFunction(const std::string& channel, std::vector<uint8_t>&& data, int32_t id)
    {
        auto dispatcher = dispatcher_.Upgrade();
        if (dispatcher) {
            dispatcher->Dispatch(channel, std::move(data), id);
            return true;
        } else {
            LOGW("Dispatcher Upgrade fail when dispatch request mesaage to platform");
            return false;
        }
    }

    bool PluginErrorCallback(int32_t callbackId, int32_t errorCode, std::string&& errorMessage)
    {
        auto dispatcher = dispatcher_.Upgrade();
        if (dispatcher) {
            dispatcher->DispatchPluginError(callbackId, errorCode, std::move(errorMessage));
            return true;
        } else {
            LOGW("Dispatcher Upgrade fail when dispatch error mesaage to platform");
            return false;
        }
    }

    JSValue FireJsEvent(const std::string &eventId, const std::string &param);

private:
    JSRuntime* runtime_ = nullptr;
    JSContext* context_ = nullptr;
    RefPtr<BackendDelegate> backendDelegate_;
    int32_t instanceId_;

    WeakPtr<JsMessageDispatcher> dispatcher_;
    mutable std::mutex mutex_;

    ACE_DISALLOW_COPY_AND_MOVE(QjsPaEngineInstance);
};

using RdbValueBucketNewInstance = napi_value (*)(napi_env env, OHOS::NativeRdb::ValuesBucket& valueBucket);
using RdbValueBucketGetNativeObject = OHOS::NativeRdb::ValuesBucket* (*)(napi_env env, napi_value& value);
using RdbResultSetProxyNewInstance = napi_value (*)(napi_env env, OHOS::NativeRdb::AbsSharedResultSet* resultSet);
using RdbResultSetProxyGetNativeObject = OHOS::NativeRdb::AbsSharedResultSet* (*)(const napi_env env, const napi_value& arg);
using DataAbilityPredicatesNewInstance = napi_value (*)(napi_env env, OHOS::NativeRdb::DataAbilityPredicates* predicates);
using DataAbilityPredicatesGetNativeObject = OHOS::NativeRdb::DataAbilityPredicates* (*)(const napi_env env, const napi_value& arg);

class QjsPaEngine : public JsBackendEngine {
public:
    explicit QjsPaEngine(int32_t instanceId) : instanceId_(instanceId) {};
    ~QjsPaEngine() override;

    bool Initialize(const RefPtr<BackendDelegate>& delegate) override;

    // Load and initialize a JS bundle into the JS Framework
    void LoadJs(const std::string& url) override;

    void SetJsMessageDispatcher(const RefPtr<JsMessageDispatcher>& dispatcher) override;

    // destroy application instance according packageName
    void DestroyApplication(const std::string &packageName) override;

    // Fire AsyncEvent on JS
    void FireAsyncEvent(const std::string &eventId, const std::string &param) override;

    // Fire SyncEvent on JS
    void FireSyncEvent(const std::string &eventId, const std::string &param) override;

    int32_t Insert(const Uri& uri, const OHOS::NativeRdb::ValuesBucket& value) override;
    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> Query(
        const Uri& uri, const std::vector<std::string>& columns,
        const OHOS::NativeRdb::DataAbilityPredicates& predicates) override;
    int32_t Update(const Uri& uri, const OHOS::NativeRdb::ValuesBucket& value,
        const OHOS::NativeRdb::DataAbilityPredicates& predicates) override;
    int32_t Delete(const Uri& uri, const OHOS::NativeRdb::DataAbilityPredicates& predicates) override;

    int32_t BatchInsert(const Uri& uri, const std::vector<OHOS::NativeRdb::ValuesBucket>& values) override;
    std::string GetType(const Uri& uri) override;
    std::vector<std::string> GetFileTypes(const Uri& uri, const std::string& mimeTypeFilter) override;
    int32_t OpenFile(const Uri& uri, const std::string& mode) override;
    int32_t OpenRawFile(const Uri& uri, const std::string& mode) override;
    Uri NormalizeUri(const Uri& uri) override;
    Uri DenormalizeUri(const Uri& uri) override;
    sptr<IRemoteObject> OnConnectService(const std::string &want) override;
    void OnDisconnectService(const std::string &want) override;

private:
    void GetLoadOptions(std::string& optionStr);
    JSValue GetPaFunc(const std::string& funcName);

    void LoadLibrary();
    void UnloadLibrary();

    RefPtr<QjsPaEngineInstance> engineInstance_;
    int32_t instanceId_;
    QuickJSNativeEngine* nativeEngine_ = nullptr;

    void* libRdb_ = nullptr;
    void* libDataAbility_ = nullptr;
    RdbValueBucketNewInstance rdbValueBucketNewInstance_;
    RdbValueBucketGetNativeObject rdbValueBucketGetNativeObject_;
    RdbResultSetProxyNewInstance rdbResultSetProxyNewInstance_;
    RdbResultSetProxyGetNativeObject rdbResultSetProxyGetNativeObject_;
    DataAbilityPredicatesNewInstance dataAbilityPredicatesNewInstance_;
    DataAbilityPredicatesGetNativeObject dataAbilityPredicatesGetNativeObject_;

    ACE_DISALLOW_COPY_AND_MOVE(QjsPaEngine);
};

class FromBindingData : public AceType {
    DECLARE_ACE_TYPE(FromBindingData, AceType);

public:
    FromBindingData(JSContext* ctx, JSValue value);
    void Constructor(const std::string& param);
    void UpdateData(const std::string& data);
    std::string GetDataString() const;
    void AddImageData(const std::string& name, void* buffer);
    void RemoveImageData(const std::string& name);

private:
    JSContext* ctx_;
    JSValue valueFromJs_;
    ACE_DISALLOW_COPY_AND_MOVE(FromBindingData);
};

} // namespace OHOS::Ace::Framework
#endif // FOUNDATION_ACE_FRAMEWORKS_BRIDGE_PA_BACKEND_ENGINE_QUICKJS_QJS_PA_ENGINE_H
