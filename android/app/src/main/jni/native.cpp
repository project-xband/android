#include "native.hpp"

#include <jni.h>

extern "C" {
JNIEXPORT jstring JNICALL
Java_com_xband_MainActivity_testInitSetup(JNIEnv *env, jobject thiz)
{
    // TODO
    return (env)->NewStringUTF("Where is this message !..... i found uuuuu");
}

JNIEXPORT jbyteArray JNICALL
Java_com_xband_MainActivity_testByteArray(JNIEnv *env, jobject jobj)
{
    jbyteArray framedData;
    int i;

    const char *message[5] = {"first",
                              "second",
                              "third",
                              "fourth",
                              "fifth"};

    framedData = (jbyteArray) env->NewByteArray(5);

    for (i = 0; i < 5; i++)
    {
        framedData, i, env->NewStringUTF(message[i]);
    }
    return (framedData);

}

JNIEXPORT jstring JNICALL
Java_com_xband_MainActivity_testSendMessage(JNIEnv * env, jobject thiz, jstring rawData)
{

    const char * messageData = (env)->GetStringUTFChars(rawData , 0);
    // TODO
    return (env)->NewStringUTF(" message data from testsend");
}

JNIEXPORT jstring JNICALL
Java_com_xband_MainActivity_testGetMessage(JNIEnv * env, jobject thiz)
{
    // TODO
    return (env)->NewStringUTF(" message data from testget");
}
}
