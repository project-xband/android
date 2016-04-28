import _ from 'lodash';
// import Immutable from 'immutable';

const initialState = {
    '44.111.2': {
      name: 'fero'
    },
    '44.3.21': {
      name: 'kevin'
    }
}

const actionsMap = {
}

export default (state = initialState, action) => {

	// map the "action.type" to an action within the "actionsMap" and create reduceFn
	const reduceFn = actionsMap[action.type];
 
	// return the original state if "action.type" doesn't map to any action
	if (!reduceFn) return state;

	// return reducer
	return reduceFn(state, action);
}