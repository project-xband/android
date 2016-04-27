import Immutable from 'immutable';

const initialState = Immutable.Map({ 
	from: '44.111.2',  // <-- to find who is in conversation look at the 'from',
	position: 'left',
	body: 'xBand to the Moon',
	timestamp: 'whenever'
});

const actionsMap = {
	addMessage(state, action) {
		return Object.assign({}, state, {
			messages: [
				...state.messages, {
					from: action.payload.from,
					position: action.payload.position,
					body: action.payload.body,
					timestamp: action.payload.timestamp
				}
			]
		});
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