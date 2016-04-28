import Immutable from 'immutable';
import { Platform } from 'react-native';
import { createStore, applyMiddleware, compose, combineReducers } from 'redux';
import thunk from 'redux-thunk';
import reducer from './reducers';
import createEngine from 'redux-storage-engine-reactnativeasyncstorage';
import * as storage from 'redux-storage';

// import { dummyData } from './dummyData';

let enhancer;

// create the engine with 'xband-to-moon' as the storage key
const engine = createEngine('xband-to-moons');

// add the storage engine middleware spice
const engineMiddleware = storage.createMiddleware(engine);
const middlewares = [thunk, engineMiddleware];

// if using remote-redux-devtools set them up
if (__DEV__) {
  const installDevTools = require('immutable-devtools');
  installDevTools(Immutable);

  const devTools = require('remote-redux-devtools');
  enhancer = compose(
    applyMiddleware(...middlewares),
    devTools({
      name: Platform.OS,
      hostname: 'localhost',
      port: 5678
    })
  );
} else {
  enhancer = applyMiddleware(...middlewares);
}

export default function configureStore(initialState={}) {
  const store = createStore(reducer, initialState, enhancer);

  // if using hot modules replace the reducers
  if (module.hot) {
    module.hot.accept(() => {
      store.replaceReducer(require('./reducers'));
    });
  }

  // At this stage the whole system is in place and every action will trigger
  // a save operation.

  // BUT (!) an existing old state HAS NOT been restored yet!\
  const load = storage.createLoader(engine);

  load(store)
    .then((loadedState) => console.log('Loaded state! ->', loadedState))
    .catch(() => console.log('Failed to load previous state'));

  return store;
}