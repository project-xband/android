import { combineReducers } from 'redux'
import * as storage from 'redux-storage'

export default storage.reducer(combineReducers({
  message: require('./message'),
  contact: require('./contact'),
  conversation: require('./conversation'),
  routes: require('./routes'),
  storage: require('./storage')
}));
