import Immutable from 'immutable';

const initialState = Immutable.Map({ counter: 0 });

const actionsMap = {
  increment(state/*, action*/) {
    return state.update('counter', n => n + 1);
  },
  decrement(state/*, action*/) {
    return state.update('counter', n => n - 1);
  }
};

export default (state = initialState, action) => {
  
  // map the "action.type" to an action within the "actionsMap" and create reduceFn
  const reduceFn = actionsMap[action.type];

  // return the original state if "action.type" doesn't map to any action
  if (!reduceFn) return state;
  
  // return reducer
  return reduceFn(state, action);
};