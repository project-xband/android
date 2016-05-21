
const initialState = {
  address: '44.3.66'
}
/*
  ^ TODO:
  get rid of this altogether in production code once we
  can create a new conversation and send a message to it
*/

const actionsMap = {
  /**
   * TODO:
   * add contact reducers.
   *
   * addContact()
   */
}

export default (state = initialState, action) => {

	// map the "action.type" to an action within the "actionsMap" and create reduceFn
	const reduceFn = actionsMap[action.type];

	// return the original state if "action.type" doesn't map to any action
	if (!reduceFn) return state;

	// return reducer
	return reduceFn(state, action);
}
