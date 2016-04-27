import UUID from 'uuid-js';
// import Immutable from 'immutable';

const initialState = {
	[UUID.create().toString()]: {
      from: '44.111.2',   // <-- to find who is in conversation look at the 'from',
      position: 'right',  // <-- always 'right' on sent
      body: "Hey babe, how's xBand",
      timestamp: 'whenever'
    },
    [UUID.create().toString()]: {
      from: '44.3.21',
      position: 'left',   // <-- always 'left' on received
      body: 'Hey, how are you doing habibi?',
      timestamp: 'whenever'
    },
    [UUID.create().toString()]: {
      from: '44.3.21',
      position: 'left',
      body: "xBand is the coolest",
      timestamp: 'whenever'
    },
    [UUID.create().toString()]: {
      from: '44.3.21',
      position: 'left',
      body: "xBand is swank",
      timestamp: 'whenever'
    }
}

const actionsMap = {
	addMessage(state=initialState, action) {
		console.log('action --> ', action)
		console.log('state --> ', state)
		return {
			...state, 
			[UUID.create().toString()]: {
				from: action.payload.from,
				position: action.payload.position,
				body: action.payload.body,
				timestamp: action.payload.timestamp
			}
		}
	}
}

export default (state=initialState, action) => {

	// map the "action.type" to an action within the "actionsMap" and create reduceFn
	const reduceFn = actionsMap[action.type];

	// return the original state if "action.type" doesn't map to any action
	if (!reduceFn) return state;

	// return reducer
	return reduceFn(state, action);
}