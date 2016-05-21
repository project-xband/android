package com.xband.usbclient;

import android.util.Log;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReactContextBaseJavaModule;
import com.facebook.react.bridge.ReactMethod;
import com.facebook.react.bridge.ReadableMap;
import com.facebook.react.bridge.ReadableMapKeySetIterator;

import org.json.JSONObject;

import java.util.Iterator;

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
    public void send(ReadableMap message) {
      // Expects JSON Object
      // with params: from, to, msg

      byte from = new Byte(message.getString("from"));
      byte to = new Byte(message.getString("to"));
      String content = message.getString("msg");

      Log.d("sending",content);

      // Get the original bytes and
      // account for 2 address bytes
      byte[] msgbytes = content.getBytes();
      int msglength = msgbytes.length;

      // Create packet
      // 0    1    2    ...
      // byte byte string
      // from  to  content
      byte[] header = {to,from};
      byte[] packet = new byte[msglength+2];
      System.arraycopy(header, 0, packet, 0, 2);
      System.arraycopy(msgbytes, 0, packet, 2, msglength);


        if (UsbService.SERVICE_CONNECTED) {
            UsbService.write(framePacket(packet,packet.length));
        }
    }

    public byte[] framePacket(byte[] packetData,int packetLength) {
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
        return framedData;
    }
}
