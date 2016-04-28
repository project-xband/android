import UUID from 'uuid-js';

const initialState = {
    [UUID.create().toString()]: {
      draft: 'Hey, I just wanted to check in if ',
      messages: [0, 1]    // <-- access 'last' sent message (first in the Array) for sorting the conversation
    },
    [UUID.create().toString()]: {
      draft: '',
      messages: [0]
    }
}

const actionsMap = {
	addMessage(state, action) {
		console.log('conversation action -->',action);
		console.log('conversation state  -->',state);
		return {
			// here I need to add the message to the conversation
			// action.conversation[]
		}
	}
}

export default (state = initialState, action) => {

	// map the "action.type" to an action within the "actionsMap" and create reduceFn
	const reduceFn = actionsMap[action.type];
 
	// return the original state if "action.type" doesn't map to any action
	if (!reduceFn) return state;

	// return reducer
	return reduceFn(state, action);
}