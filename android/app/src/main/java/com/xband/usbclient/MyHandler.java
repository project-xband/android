package com.xband.usbclient;

import android.os.Handler;
import android.os.Message;
import android.support.annotation.Nullable;

import com.xband.MainActivity;
import com.facebook.react.bridge.Arguments;
import com.facebook.react.bridge.ReactContext;
import com.facebook.react.bridge.WritableMap;
import com.facebook.react.modules.core.DeviceEventManagerModule;

import java.lang.ref.WeakReference;


public class MyHandler extends Handler {

    private final WeakReference<MainActivity> mActivity;
    private ReactContext someContext;
    private char[] buffer = new char[64];

    public MyHandler(MainActivity activity) {
        mActivity = new WeakReference<MainActivity>(activity);
        someContext = activity.getContext();
    }

    private void sendEvent(String eventName,
                           @Nullable WritableMap params)
    {
        mActivity.get().getContext()
                .getJSModule(DeviceEventManagerModule.RCTDeviceEventEmitter.class)
                .emit(eventName, params);
    }

    private final char CLEAR = 0;
    private final char STARTED = 1;
    private final char SYNC_CHARACTER = '{';
    private final char TERMINATOR_CHARACTER = '}';
    private final int INPUT_LENGTH = 50;

    private char incomingByte;   // for incoming serial data
    private char reliableLength;
    private char commandIndex = 0;
    private char commandState = CLEAR;
    private char[] commandBuffer = new char[INPUT_LENGTH + 2];

    @Override
    public void handleMessage(Message msg) {

        switch (msg.what) {
            case UsbService.MESSAGE_FROM_SERIAL_PORT:

                String data = (String) msg.obj;

                for (char incomingByte:data.toCharArray()) {
                    if (CLEAR == commandState){
                        if (SYNC_CHARACTER == incomingByte) {
                            commandState = STARTED;
                        }
                    }

                    if (STARTED == commandState) {
                        if (TERMINATOR_CHARACTER == incomingByte) {
                            commandBuffer[commandIndex] = 0;
                            WritableMap params = Arguments.createMap();
                            params.putString("content", new String(commandBuffer));
                            sendEvent("message", params);

                            commandIndex = 0;
                            commandState = CLEAR;
                        } else {
                            commandBuffer[commandIndex] = incomingByte;
                            commandIndex ++;
                            if (INPUT_LENGTH< commandIndex) {
                                WritableMap params = Arguments.createMap();
                                params.putString("content", "{too much data}");
                                sendEvent("message", params);

                                commandIndex = 0;
                                commandState = CLEAR;
                                return;
                            }
                        }
                    }
                }
                // some of this might crash the app because the context is null


                break;
        }
    }
}
