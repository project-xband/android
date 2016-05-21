const initialState = {
  [0]: {
    from: '44.3.69',    // <-- to find who is in conversation look at the 'from',
    position: 'right',  // <-- always 'right' on sent
    body: "Hey, how's xBand?",
    timestamp: 'Mon'
  },
  [1]: {
    from: '44.3.21',
    position: 'left',   // <-- always 'left' on received
    body: 'Project xBand to the moon?',
    timestamp: 'Mon'
  },
  [2]: {
    from: '44.3.66',
    position: 'right',  // <-- always 'left' on received
    body: 'Yes',
    timestamp: 'Mon'
  },
  ['h4rdc0d3dm3ss4ge']: {
    from: '44.3.5',
    position: 'left',
    body: "Project xBand is THE coolest.",
    timestamp: 'Mon'
  },
  ['h4rdc0d3dm3nets']: {
    from: '44.3.1',
    position: 'right',
    body: "Agreed",
    timestamp: 'Mon'
  }
}
/*
  ^ TODO:
  get rid of this altogether in production coode
*/

const actionsMap = {
	addMessage(state=initialState, message) {
		// console.log('addMessage action --> ', action)
		// console.log('addMessage state  --> ', state)
		return {
			...state,
			[message.uniqueKey]: {
				from: message.from,
				position: message.position,
				body: message.body,
				timestamp: message.timestamp
			},
		}
	}
}

export default (state=initialState, action) => {

	// map the "action.type" to an action within the "actionsMap" and create reduceFn
	const reduceFn = actionsMap[action.type]

	// return the original state if "action.type" doesn't map to any action
	if (!reduceFn) return state

	// return reducer
	return reduceFn(state, action)
}
