package com.xband.usbclient;

import java.util.Arrays;
import android.util.Log;
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
    private byte[] buffer = new byte[255];

    private int packetLength;
    private byte[] packetData;

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

    public byte[] extractPacket(byte[] framedPacketData,
                              int framePacketLength) {
        int index;
        int vindex = 0;
        int endOfFramedData;
        int consumedLength = 0;
        int calculatedLength = 0;
        boolean flag = false;
        byte[] framedData;
        byte[] unframedData;

        packetLength = 0;
        packetData = null;


        endOfFramedData = framePacketLength;
        framedData = framedPacketData;

        if ('{' == framedData[vindex]) {
            vindex++;

            for (; vindex < endOfFramedData; vindex++) {
                if ('%' == framedData[vindex]) {
                    vindex++;
                } else {
                    if ('}' == framedData[vindex]) {
                        consumedLength = vindex - 1;
                        flag = true;
                        break;
                    }
                }
                calculatedLength++;
            }
            if (!flag) return null;

            unframedData = new byte[calculatedLength + 2];

            vindex = 1;

            for (index = 0; index < calculatedLength; index++) {
                if ('%' == framedData[vindex]) {
                    vindex++;
                    switch (framedData[vindex]) {
                        case '[':
                            unframedData[index] = '{';
                            break;
                        case ']':
                            unframedData[index] = '}';
                            break;
                        default:
                            unframedData[index] = framedData[vindex];
                            break;
                    }
                    vindex++;
                } else {
                    unframedData[index] = framedData[vindex++];
                }
            }

            packetLength = calculatedLength;
            return unframedData;
        }
        return null;
    }

    private final byte CLEAR = 0;
    private final byte STARTED = 1;
    private final byte SYNC_CHARACTER = '{';
    private final byte TERMINATOR_CHARACTER = '}';
    private final int INPUT_LENGTH = 250;

    private char incomingByte;   // for incoming serial data
    private char reliableLength;
    private byte commandIndex = 0;
    private byte commandState = CLEAR;
    private byte[] commandBuffer = new byte[INPUT_LENGTH + 2];

    @Override
    public void handleMessage(Message msg) {

        switch (msg.what) {
            case UsbService.MESSAGE_FROM_SERIAL_PORT:

                String data = (String) msg.obj;

                for (byte incomingByte:data.getBytes()) {
                    if (CLEAR == commandState){
                        if (SYNC_CHARACTER == incomingByte) {
                            commandState = STARTED;
                        }
                    }

                    if (STARTED == commandState) {
                        if (TERMINATOR_CHARACTER == incomingByte) {
                            commandBuffer[commandIndex] = incomingByte;
                            commandIndex++;
                            
                            byte[] temp = extractPacket(commandBuffer, commandIndex);
                            byte to = temp[0];
                            byte from = temp[1];
                            byte conKey = temp[2];

                            temp[0] = temp[1] = temp[2] = 0;

                            String message = new String(temp);
                            
                            message = message.trim();
                            
                            Log.d("headers", "from:"+from+"|to:"+to+"|conKey:"+conKey+"|msg:"+message+"|");
                            
                            WritableMap params = Arguments.createMap();
                            params.putString("body",   message);
                            params.putString("from",   Byte.toString(from));
                            params.putString("to",     Byte.toString(to));
                            params.putString("conKey", Byte.toString(to));
                            
                            sendEvent("message", params);

                            commandIndex = 0;
                            commandState = CLEAR;
                        } else {
                            commandBuffer[commandIndex] = incomingByte;
                            commandIndex++;
                            if (INPUT_LENGTH < commandIndex) {
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
