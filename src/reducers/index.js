import { combineReducers } from 'redux';
import * as storage from 'redux-storage';

// add the redux-storage reducers on top of any other reducers that get combined
export default storage.reducer(combineReducers({
  message: require('./message'),
  contact: require('./contact'),
  conversation: require('./conversation'),
  routes: require('./routes'),
}));
