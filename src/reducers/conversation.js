import _ from 'lodash';

const initialState = {
    [_.uniqueId()]: {
      draft: 'Hey, I just wanted to check in if ',
      messages: [0, 1]    // <-- access 'last' sent message (first in the Array) for sorting the conversation
    },
    [_.uniqueId()]: {
      draft: '',
      messages: [2, 3, 4]
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