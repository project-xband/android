const IS_LOADED = 'IS_LOADED'

const initialState = {
	isLoaded: false
}

export default (state = initialState, action = {}) => {
	switch (action.type) {
		case IS_LOADED:
			return {
				isLoaded: true
			}
		default:
			return state
	}
}
