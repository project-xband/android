export function addMessageKeyToTheConversation(payload) {
	console.log('addMessageKeyToTheConversation --> ', payload)

	return {
		type: 'addMessageKeyToTheConversation',
		conversationKey: payload.conversationKey,
		messageKey: payload.uniqueKey
	};
}