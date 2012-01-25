/*
    This file is part of the WebKit open source project.
    This file has been generated by generate-bindings.pl. DO NOT MODIFY!

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#include <glib-object.h>
#include "config.h"

#if ENABLE(DATABASE)

#include <wtf/GetPtr.h>
#include <wtf/RefPtr.h>
#include "DOMObjectCache.h"
#include "ExceptionCode.h"
#include "JSMainThreadExecState.h"
#include "TestCallback.h"
#include "WebKitDOMBinding.h"
#include "gobject/ConvertToUTF8String.h"
#include "webkit/WebKitDOMClass1.h"
#include "webkit/WebKitDOMClass1Private.h"
#include "webkit/WebKitDOMClass2.h"
#include "webkit/WebKitDOMClass2Private.h"
#include "webkit/WebKitDOMClass3.h"
#include "webkit/WebKitDOMClass3Private.h"
#include "webkit/WebKitDOMTestCallback.h"
#include "webkit/WebKitDOMTestCallbackPrivate.h"
#include "webkitmarshal.h"
#include "webkitprivate.h"

namespace WebKit {
    
WebKitDOMTestCallback* kit(WebCore::TestCallback* obj)
{
    g_return_val_if_fail(obj, 0);

    if (gpointer ret = DOMObjectCache::get(obj))
        return static_cast<WebKitDOMTestCallback*>(ret);

    return static_cast<WebKitDOMTestCallback*>(DOMObjectCache::put(obj, WebKit::wrapTestCallback(obj)));
}
    
} // namespace WebKit //

gboolean
webkit_dom_test_callback_callback_with_class1param(WebKitDOMTestCallback* self, WebKitDOMClass1* class1param)
{
    g_return_val_if_fail(self, 0);
    WebCore::JSMainThreadNullState state;
    WebCore::TestCallback * item = WebKit::core(self);
    g_return_val_if_fail(class1param, 0);
    WebCore::Class1 * converted_class1param = NULL;
    if (class1param != NULL) {
        converted_class1param = WebKit::core(class1param);
        g_return_val_if_fail(converted_class1param, 0);
    }
    gboolean res = item->callbackWithClass1Param(converted_class1param);
    return res;
}

gboolean
webkit_dom_test_callback_callback_with_class2param(WebKitDOMTestCallback* self, WebKitDOMClass2* class2param, const gchar* str_arg)
{
    g_return_val_if_fail(self, 0);
    WebCore::JSMainThreadNullState state;
    WebCore::TestCallback * item = WebKit::core(self);
    g_return_val_if_fail(class2param, 0);
    g_return_val_if_fail(str_arg, 0);
    WebCore::Class2 * converted_class2param = NULL;
    if (class2param != NULL) {
        converted_class2param = WebKit::core(class2param);
        g_return_val_if_fail(converted_class2param, 0);
    }
    WTF::String converted_str_arg = WTF::String::fromUTF8(str_arg);
    gboolean res = item->callbackWithClass2Param(converted_class2param, converted_str_arg);
    return res;
}

glong
webkit_dom_test_callback_callback_with_non_bool_return_type(WebKitDOMTestCallback* self, WebKitDOMClass3* class3param)
{
    g_return_val_if_fail(self, 0);
    WebCore::JSMainThreadNullState state;
    WebCore::TestCallback * item = WebKit::core(self);
    g_return_val_if_fail(class3param, 0);
    WebCore::Class3 * converted_class3param = NULL;
    if (class3param != NULL) {
        converted_class3param = WebKit::core(class3param);
        g_return_val_if_fail(converted_class3param, 0);
    }
    glong res = item->callbackWithNonBoolReturnType(converted_class3param);
    return res;
}


G_DEFINE_TYPE(WebKitDOMTestCallback, webkit_dom_test_callback, WEBKIT_TYPE_DOM_OBJECT)

namespace WebKit {

WebCore::TestCallback* core(WebKitDOMTestCallback* request)
{
    g_return_val_if_fail(request, 0);

    WebCore::TestCallback* coreObject = static_cast<WebCore::TestCallback*>(WEBKIT_DOM_OBJECT(request)->coreObject);
    g_return_val_if_fail(coreObject, 0);

    return coreObject;
}

} // namespace WebKit
enum {
    PROP_0,
};


static void webkit_dom_test_callback_finalize(GObject* object)
{
    WebKitDOMObject* dom_object = WEBKIT_DOM_OBJECT(object);
    
    if (dom_object->coreObject) {
        WebCore::TestCallback* coreObject = static_cast<WebCore::TestCallback *>(dom_object->coreObject);

        WebKit::DOMObjectCache::forget(coreObject);
        coreObject->deref();

        dom_object->coreObject = NULL;
    }

    G_OBJECT_CLASS(webkit_dom_test_callback_parent_class)->finalize(object);
}

static void webkit_dom_test_callback_set_property(GObject* object, guint prop_id, const GValue* value, GParamSpec* pspec)
{
    WebCore::JSMainThreadNullState state;
    switch (prop_id) {
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}


static void webkit_dom_test_callback_get_property(GObject* object, guint prop_id, GValue* value, GParamSpec* pspec)
{
    WebCore::JSMainThreadNullState state;
    switch (prop_id) {
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}


static void webkit_dom_test_callback_constructed(GObject* object)
{

    if (G_OBJECT_CLASS(webkit_dom_test_callback_parent_class)->constructed)
        G_OBJECT_CLASS(webkit_dom_test_callback_parent_class)->constructed(object);
}

static void webkit_dom_test_callback_class_init(WebKitDOMTestCallbackClass* requestClass)
{
    GObjectClass *gobjectClass = G_OBJECT_CLASS(requestClass);
    gobjectClass->finalize = webkit_dom_test_callback_finalize;
    gobjectClass->set_property = webkit_dom_test_callback_set_property;
    gobjectClass->get_property = webkit_dom_test_callback_get_property;
    gobjectClass->constructed = webkit_dom_test_callback_constructed;



}

static void webkit_dom_test_callback_init(WebKitDOMTestCallback* request)
{
}

namespace WebKit {
WebKitDOMTestCallback* wrapTestCallback(WebCore::TestCallback* coreObject)
{
    g_return_val_if_fail(coreObject, 0);

    /* We call ref() rather than using a C++ smart pointer because we can't store a C++ object
     * in a C-allocated GObject structure.  See the finalize() code for the
     * matching deref().
     */
    coreObject->ref();

    return  WEBKIT_DOM_TEST_CALLBACK(g_object_new(WEBKIT_TYPE_DOM_TEST_CALLBACK,
                                               "core-object", coreObject, NULL));
}
} // namespace WebKit
#endif /* ENABLE(DATABASE) */
