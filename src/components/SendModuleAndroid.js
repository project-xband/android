'use strict'
/**
 * This exposes the native SendModuleAndroid module as a JS module. This has a
 * function 'send' which takes the following parameters:
 *
 * from,
 * // to, TODO: add this one later once we need it
 * conversationKey,
 * body,
 * timestamp
 */
import { NativeModules } from 'react-native'
module.exports = NativeModules.SendModuleAndroid

// TODO: get tokens from the SendModuleAndroid
