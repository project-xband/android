import _ from 'lodash';

export const dummyData = {
  contact: {
    '44.111.2': {
      name: 'fero'
    },
    '44.3.21': {
      name: 'kevin'
    }
  },
  message: {
    [_.uniqueId()]: {
      from: '44.111.2',   // <-- to find who is in conversation look at the 'from',
      position: 'right',
      body: "Hey babe, how's xBand",
      timestamp: 'whenever'
    },
    [_.uniqueId()]: {
      from: '44.3.21',
      position: 'left',   // <-- this gets assigned when the message is received, if not me 'right', otherwise 'left'
      body: 'Hey, how are you doing habibi?',
      timestamp: 'whenever'
    },
    [_.uniqueId()]: {
      from: '44.3.21',
      position: 'left',
      body: "xBand is the coolest",
      timestamp: 'whenever'
    },
    [_.uniqueId()]: {
      from: '44.3.21',
      position: 'left',
      body: "xBand is swank",
      timestamp: 'whenever'
    },
  },
  conversation: {
    [_.uniqueId()]: {
      draft: 'Hey, I just wanted to check in if ',
      messages: [0, 1]    // <-- access 'last' sent message (first in the Array) for sorting the conversation
    },
    [_.uniqueId()]: {
      draft: '',
      messages: [2, 3, 4]
    }
  },
}