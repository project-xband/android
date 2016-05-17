import React, {
	Component,
	PropTypes,
	StyleSheet,
	Text,
	View,
	ScrollView,
  DeviceEventEmitter
} from 'react-native'

/*
	^ there is a new format they said (new RN 0.25.1) 
	where the import is like -->

	````
		import React, { Component } from 'react'
		import { 
			PropTypes,
			StyleSheet,
			Dimensions,
			Text,
			TextInput,
			View,
			TouchableHighlight
		} from 'react-native'
	````

	(https://github.com/facebook/react-native/releases/tag/v0.25.1)
*/

import Bubble from './Bubble'
import AnimatedTextInput from './AnimatedTextInput'
import SendModuleAndroid from './SendModuleAndroid'
import _ from 'lodash'

export default class Chat extends Component {

	static propTypes = {
		addMessage: PropTypes.func.isRequired,
		addMessageKeyToTheConversation: PropTypes.func.isRequired,
		conversation: PropTypes.object.isRequired,
		conversationKey: PropTypes.string.isRequired
	}

	// let's load in some bubbles
	constructor(props) {	
		super(props)

		console.log('chat',this.props)
	}

	render() {
		return(
			<View style={{ marginTop: 64 }}>
				<ScrollView style={{}}>
					{
						_.map(this.props.conversation[this.props.conversationKey].messages, (msgId) => {
							return (
								<Bubble
									key = { _.uniqueId() }
									text = { this.props.messages[msgId].body }
								  position = { this.props.messages[msgId].position }/>
						  )
						})
					}
				</ScrollView>
				<AnimatedTextInput
					addMessage = { this.props.addMessage }
					addMessageKeyToTheConversation = { this.props.addMessageKeyToTheConversation }
					conversation = { this.props.conversation }/>
			</View>
		)
	}
}
