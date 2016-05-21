const initialState = {
  ['h4rdc0d3d']: {
    draft: 'Hey, I just wanted to check in if ',
    participants: ['44.3.69', '44.3.66'],
    messages: [0, 2]    // <-- access 'last' sent message (first in the Array) for sorting the conversation
  },
  ['h4rdc0d3d2']: {
    draft: '',
    participants: ['44.3.21', '44.3.66'],
    messages: [1, 2]
  },
  ['h4rdc323d2']: {
    draft: '',
    participants: ['44.3.1', '44.3.5'],
    messages: ['h4rdc0d3dm3ss4ge', 'h4rdc0d3dm3nets']
  },
  ['h4rd1td3d2']: {
    draft: '',
    participants: ['44.3.69', '44.3.5'],
    messages: [0]
  }
}
/*
  ^ TODO:
  get rid of this altogether in production coode
*/

const actionsMap = {
	addMessageKeyToTheConversation(state, action) {
		console.log('conversation action -->',action)
		console.log('conversation state  -->',state)

    const newState = { ...state }
    /*
      ^ TODO:
      could be potentially harmful because it's just
      a shallow copy fo the state, just the pointers,
      it's not pure anc can couse fuego later?
     */

    // make a copy of the messages array and set it to the new array
    newMessageKeyArr = newState[action.conversationKey].messages.slice()
    newMessageKeyArr.push(action.messageKey)

    // swap the arrays <3
    newState[action.conversationKey].messages = newMessageKeyArr

		return { ...newState }
	}
}

export default (state = initialState, action) => {

	// map the "action.type" to an action within the "actionsMap" and create reduceFn
	const reduceFn = actionsMap[action.type]
 	// console.log('conversation reducer', state)
	// return the original state if "action.type" doesn't map to any action
	if (!reduceFn) return state;

	// return reducer
	return reduceFn(state, action)
}
