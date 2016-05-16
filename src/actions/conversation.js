export function addMessageKeyToTheConversation(payload) {
	console.log('addMessageKeyToTheConversation --> ', payload);

	return {
		type: 'addMessageKeyToTheConversation',
		conversationId: payload.conversationId
	};
}