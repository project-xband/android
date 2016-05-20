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

// 'xband-to-moon' === storage key
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

  // add the message listener here
  DeviceEventEmitter.addListener('message', (message) => {

    store.dispatch(addMessage({
      uniqueKey: message.uniqueKey,
      from: message.from,
      conversationKey: message.conversationKey,
      body: message.body,
      position: 'left',
      timestamp: message.timestamp
    }))

    store.dispatch(addMessageKeyToTheConversation({
      uniqueKey: message.uniqueKey,
      conversationKey: message.conversationKey
    }))
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
