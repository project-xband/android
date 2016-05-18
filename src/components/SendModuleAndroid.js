'use strict'
/**
 * This exposes the native SendModuleAndroid module as a JS module. This has a
 * function 'send' which takes the following parameters:
 *
 * from,
 * to,
 * body,
 * timestamp
 *
 */
import { NativeModules } from 'react-native'
module.exports = NativeModules.SendModuleAndroid
