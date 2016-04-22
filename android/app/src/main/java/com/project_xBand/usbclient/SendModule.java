package com.project_xBand.usbclient;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReactContextBaseJavaModule;
import com.facebook.react.bridge.ReactMethod;

public class SendModule extends ReactContextBaseJavaModule {

    public SendModule(ReactApplicationContext reactContext) {
        super(reactContext);
    }

    @Override
    public String getName() {
        return "SendModuleAndroid";
    }

    @ReactMethod
    public void send(String message) {

        if (UsbService.SERVICE_CONNECTED) {
            UsbService.write(message.getBytes());
        }
    }
}
