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

#ifndef FRAMEWORKS_BRIDGE_DECLARATIVE_FRONTEND_JS_VIEW_JS_SHAPE_ABSTRACT_H
#define FRAMEWORKS_BRIDGE_DECLARATIVE_FRONTEND_JS_VIEW_JS_SHAPE_ABSTRACT_H

#include "core/components/shape/shape_component.h"
#include "frameworks/bridge/declarative_frontend/jsview/js_view_abstract.h"

namespace OHOS::Ace::Framework {

class JSShapeAbstract : public AceType, public JSViewAbstract {
    DECLARE_ACE_TYPE(JSShapeAbstract, AceType);
public:
    static void SetStroke(const std::string& color);
    static void SetFill(const std::string& color);
    static void SetStrokeDashOffset(const JSCallbackInfo& info);
    static void SetStrokeDashArray(const JSCallbackInfo& info);
    static void SetStrokeLineCap(int lineCap);
    static void SetStrokeLineJoin(int lineJoin);
    static void SetStrokeMiterLimit(const std::string& strokeMiterLimit);
    static void SetStrokeOpacity(const std::string& strokeOpacity);
    static void SetFillOpacity(const std::string& fillOpacity);
    static void SetStrokeWidth(const std::string& strokeWidth);
    static void SetAntiAlias(bool antiAlias);
    static void JsWidth(const JSCallbackInfo& info);
    static void JsHeight(const JSCallbackInfo& info);

    static void JSBind();

    const RefPtr<BasicShape>& GetBasicShape() const
    {
        return basicShape_;
    }

    void SetBasicShape(const RefPtr<BasicShape>& basicShape)
    {
        basicShape_ = basicShape;
    }

    void ObjectWidth(const JSCallbackInfo& info);
    void ObjectHeight(const JSCallbackInfo& info);
    void ObjectOffset(const JSCallbackInfo& info);
    void ObjectFill(const JSCallbackInfo& info);
protected:
    static void SetSize(const JSCallbackInfo& info);
    RefPtr<BasicShape> basicShape_;
};

} // namespace OHOS::Ace::Framework
#endif // FRAMEWORKS_BRIDGE_DECLARATIVE_FRONTEND_JS_VIEW_JS_SHAPE_ABSTRACT_H
