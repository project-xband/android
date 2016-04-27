export function addMessage(payload) {
	return {
		type: 'addMessage',
		payload: payload
	};
}