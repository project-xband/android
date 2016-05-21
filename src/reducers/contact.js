import _ from 'lodash'

const initialState = {
  '44.3.21': {
    name: 'Kazu'
  },
  '44.3.1': {
    name: 'Kevin'
  },
  '44.3.66': {
    name: 'Fero'
  },
  '44.3.69': {
    name: 'jden'
  },
  '44.3.5': {
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
