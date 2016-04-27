import { combineReducers } from 'redux';
import * as storage from 'redux-storage';

// add the redux-storage reducers on top of any other reducers that get combined
export default storage.reducer(combineReducers({
  counter: require('./counter').default,
  message: require('./message').default
}));
