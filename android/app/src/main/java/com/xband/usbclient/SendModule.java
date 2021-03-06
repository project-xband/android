package com.xband.usbclient;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReactContextBaseJavaModule;
import com.facebook.react.bridge.ReactMethod;

public class SendModule extends ReactContextBaseJavaModule {

    public SendModule(ReactApplicationContext reactContext) {
        super(reactContext);
    }

    private int framedLength;
    private byte[] framedData;

    @Override
    public String getName() {
        return "SendModuleAndroid";
    }

    @ReactMethod
    public void send(String message) {

        if (UsbService.SERVICE_CONNECTED) {
            framePacket(message.getBytes(),message.length());
            UsbService.write(framedData);
        }
    }

    public void framePacket(byte[] packetData,int packetLength) {
        int index;
        int pData;
        int calculatedLength;

        for (index = calculatedLength = pData = 0; index < packetLength; index++, pData++) {
            switch (packetData[pData]){
                case '%':
                case '{':
                case '}':
                    calculatedLength += 2;
                    break;
                default:
                    calculatedLength++;
            }
        }
        calculatedLength += 2;

        framedData = new byte[calculatedLength + 2];

        // assign globally -> framedData;

        framedData[0] = '{';

        int vindex = 1;
        for (index = pData = 0; index < packetLength; index++, pData++){
            switch (packetData[pData]) {
                case '%':
                    framedData[vindex++] = '%';
                    framedData[vindex++] = '%';
                    break;
                case '{':
                    framedData[vindex++] = '%';
                    framedData[vindex++] = '[';
                    break;
                case '}':
                    framedData[vindex++] = '%';
                    framedData[vindex++] = ']';
                    break;
                default:
                    framedData[vindex++] = packetData[pData];
                    break;
            }
        }
        framedData[vindex++] = '}';
        framedLength = calculatedLength;
    }
}
