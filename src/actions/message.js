import UUID from 'uuid-js';

export function addMessage(payload) {
	console.log('msg action --> ', payload);

	return {
		type: 'addMessage',
		id: UUID.create().toString(),
		message: payload.message,
		conversation: payload.conversation
	};
}