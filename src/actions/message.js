export function addMessage(payload) {
	console.log('msg action --> ', message)

	return {
		type: 'addMessage',
		uniqueKey: message.uniqueKey,
		from: message.from,
		body: message.body,
		position: message.position,
		timestamp: message.timestamp
	};
}