{
	contacts: {
		'44.111.2': {
			name: 'fero'
		},
		'44.3.21': {
			name: 'kevin'
		}
	},
	messages: {
		0: {
			from: '44.111.2',   <-- to find who is in conversation look at the 'from',
			position: 'right'
			body: 'Hey babe',
			timestamp: 'whenever'
		},
		1: {
			from: '44.3.21',
			position: 'left',   <-- this gets assigned when the message is received, if not me 'right', otherwise 'left'
			body: 'Hey, how are you doing habibi?',
			timestamp: 'whenever'
		},
	},
	conversation: {
		0: {
			draft: 'Hey, I just wanted to check in if ',
			messages: [0, 1]    <-- access 'last' sent message (first in the Array) for sorting the conversation
		},
		1: {
			draft: '',
			messages: [10, 20, 55]
		}
	},
}
