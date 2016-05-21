import React, { Component } from 'react'
import {
	PropTypes,
	StyleSheet,
	Text,
	View,
	ScrollView,
  DeviceEventEmitter,
  Dimensions
} 													from 'react-native'
import _ 										from 'lodash'

// components
import Bubble 							from './Bubble'
import AnimatedTextInput 		from './AnimatedTextInput'

export default class Chat extends Component {

	static propTypes = {
		addMessage: 										PropTypes.func  .isRequired,
		addMessageKeyToTheConversation: PropTypes.func  .isRequired,
		conversation: 									PropTypes.object.isRequired,
		conversationKey: 								PropTypes.string.isRequired
	}

	// let's load in some bubbles
	constructor(props) {
		super(props)

		this.state = {
			viewFlex: styles.reading,
		}
	}

	render() {
		return(
			<View style = {{ marginTop: 64 }}>
				<ScrollView style = { this.state.viewFlex }>
					{
						_.map(this.props.conversation[this.props.conversationKey].messages, (msgId) => {
							return (
								<Bubble
									key      = { _.uniqueId() }
									text     = { this.props.messages[msgId].body }
								  position = { this.props.messages[msgId].position }/>
						  )
						})
					}
				</ScrollView>
				<AnimatedTextInput
					onFocus 											 = { (viewFlex) => {
																							this.setState({
																								viewFlex: styles[viewFlex]
																							})
																					 }}
					addMessage 										 = { this.props.addMessage }
					addMessageKeyToTheConversation = { this.props.addMessageKeyToTheConversation }
					conversationKey 							 = { this.props.conversationKey }/>
			</View>
		)
	}
}

const styles = StyleSheet.create({
	reading: {
		// flex: 8
		height: Dimensions.get('window').height * 0.84
	},
	typing: {
		// flex: 4
		height: Dimensions.get('window').height * 0.5
	}
})
