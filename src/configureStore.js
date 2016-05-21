import { Platform, DeviceEventEmitter } from 'react-native'

import { createStore, applyMiddleware, compose, combineReducers } from 'redux'
import thunk from 'redux-thunk'
import reducer from './reducers'
import createEngine from 'redux-storage-engine-reactnativeasyncstorage'
import * as storage from 'redux-storage'
import filter from 'redux-storage-decorator-filter'

// actions
import addMessageKeyToTheConversation from './actions/conversation'
import addMessage from './actions/message'

let enhancer

// 'xband-to-moons' === storage key
let engine = createEngine('xband-to-moons')
engine = filter(engine, [], ['storage'])

// add storage engine middleware spice
const engineMiddleware = storage.createMiddleware(engine)
const middlewares = [thunk, engineMiddleware]

/*
  TODO:
  if using remote-redux-devtools set them up down hier.
 */
if (__DEV__) {

  const devTools = require('remote-redux-devtools')
  enhancer = compose(
    applyMiddleware(...middlewares),
    devTools({
      name: Platform.OS,
      hostname: 'localhost',
      port: 5678
    })
  )
} else {
  enhancer = applyMiddleware(...middlewares)
}

export default function configureStore(initialState={}) {
  const store = createStore(reducer, initialState, enhancer)

  var generateUniqueKey = function(str) {
    // TODO 32 BIT
    return hashFnv32a(str, true, undefined)
  }

  var hashFnv32a = function(str, asString, seed) {
    /*jshint bitwise:false */
    var i, l,
        hval = (seed === undefined) ? 0x811c9dc5 : seed

    for (i = 0, l = str.length; i < l; i++) {
        hval ^= str.charCodeAt(i)
        hval += (hval << 1) + (hval << 4) + (hval << 7) + (hval << 8) + (hval << 24)
    }
    if( asString ){
        // Convert to 8 digit hex string
        return ("0000000" + (hval >>> 0).toString(16)).substr(-8)
    }
    return hval >>> 0
  }

  // add the message listener here
  DeviceEventEmitter.addListener('message', (message) => {
    alert('Got a message! '+message.from+': ' + message.body)

    const uniqueKey = generateUniqueKey(message.body)
    // alert()
    store.dispatch(
      addMessage({
        uniqueKey: 2,
        from: message.from,
        to: message.to,
        conversationKey: 0,
        body: message.body,
        position: 'left',
        timestamp: 'Mon'
      })
    )

    store.dispatch(
      addMessageKeyToTheConversation ({
        uniqueKey: uniqueKey,
        conversationKey: message.conKey
      })
    )
  })

  // if using hot modules replace the reducers
  if (module.hot) {
    module.hot.accept(() => {
      store.replaceReducer(require('./reducers'))
    })
  }

  // At this stage the whole system is in place and every action will trigger
  // a save operation.

  // BUT (!) an existing old state HAS NOT been restored yet!
  const load = storage.createLoader(engine)

  load(store)
    .then((loadedState) => {
      // console.log('Loaded state! ->', loadedState)
      store.dispatch({
        type: 'IS_LOADED'
      })
      global.isLoaded = true
    })
    .catch(() => console.log('Failed to load previous state'))

  return store
}
