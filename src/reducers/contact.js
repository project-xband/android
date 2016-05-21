import _ from 'lodash'

const initialState = {
  '97.03.09': {
    name: 'poopiepi'
  },
  '44.3.21': {
    name: 'Keviiin'
  },
  '44.3.66': {
    name: 'Jdoge'
  },
  '44.3.69': {
    name: 'Dennis'
  },
  '7777777': {
    name: 'Shannon'
  }
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
