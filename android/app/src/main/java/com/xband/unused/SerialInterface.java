//package com.clean_project;
//
//import android.support.annotation.Nullable;
//
//import com.facebook.react.bridge.ReactContext;
//import com.facebook.react.bridge.ReactContextBaseJavaModule;
//import com.facebook.react.bridge.NativeModule;
//import com.facebook.react.bridge.ReactApplicationContext;
//import com.facebook.react.bridge.ReactMethod;
//import com.facebook.react.bridge.WritableMap;
//import com.facebook.react.modules.core.DeviceEventManagerModule;
//
//public class SerialInterface extends ReactContextBaseJavaModule {
//
//    private final ReactApplicationContext mReactApplicationContext;
//
//    public SerialInterface(ReactApplicationContext mReactApplicationContext) {
//        super(mReactApplicationContext);
//        this.mReactApplicationContext = mReactApplicationContext;
//    }
//
//    public void sendEvent(ReactContext reactContext,
//                           String eventName,
//                           @Nullable WritableMap params)
//    {
//        reactContext
//                .getJSModule(DeviceEventManagerModule.RCTDeviceEventEmitter.class)
//                .emit(eventName, params);
//    }
//
//    @Override
//    public String getName() {
//        return "SerialInterface";
//    }
//}
