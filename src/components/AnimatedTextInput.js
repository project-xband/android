import React, {
  Component,
  PropTypes,
  StyleSheet,
  Dimensions,
  Text,
  TextInput,
  View,
  TouchableHighlight
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

import UUID from 'uuid-js'
import { Actions } from 'react-native-router-flux'

export default class AnimatedTextInput extends Component {

	static propTypes = {
		/* text: PropTypes.string.isRequired,

			^ TODO:
			if the user has unfinished message,
			a draft, pass it down here and initialise
			the state with that prop
		*/
		addMessage: PropTypes.func.isRequired,
		conversationKey: PropTypes.string.isRequired,
		addMessageKeyToTheConversation: PropTypes.func.isRequired
	}

	constructor(props) {
		super(props)

		// console.log('textinput', this.props)

		this.state = {
			text: '',
			buttonState: true,
			colorStyle: styles.gray
		}
	}

	render() {
		return(
			<View style={styles.txtInputContainer}>
				<TextInput
					style={styles.txtInput}
					defaultValue={ this.state.text }
					onChangeText={ (text) => {
						this.setState({
							text: text,
						})
						this.switchButton()
					}
				}/>
				<TouchableHighlight
					disabled={this.state.buttonState}
					underlayColor='transparent'
					style={styles.sendButton}
					onPress={ () => {

						const msgkey = this.generateUniqueKey()

						this.props.addMessage({
							uniqueKey: msgkey,
							from: '44.00.9.15',
							/*
								^ TODO:
								get the global address instead?
							*/
							body: this.state.text,
							position: 'right',
							// ^ this will aways be so when sending <3
							timestamp: 'now_or_never'

							/*
								^ TODO:
								generate the timestamp based
								on shannons packet timestamp
								time formatting
							*/
						});

						this.props.addMessageKeyToTheConversation({
							uniqueKey: msgkey,
							conversationKey: this.props.conversationKey
						})

						this.setState({ text: '' })
					}}>
					<Text style={ [styles.sendButtonText, this.state.colorStyle] }>
						{'Send'}
					</Text>
				</TouchableHighlight>
			</View>
		)
	}

	switchButton() {
		if (this.state.text != '') {
			this.setState({
				colorStyle: styles.blue,
				buttonState: false
			})
		} else {
			this.setState({
				colorStyle: styles.gray,
				buttonState: true
			})
		}
	}

	generateUniqueKey() {
		return UUID.create().toString()
	}
}

const styles = StyleSheet.create({
	txtInputContainer: {
		height: Dimensions.get('window').height / 18,
		width: Dimensions.get('window').width,
		flexDirection: 'row',
		justifyContent: 'center',
		borderTopColor: '#BDBDBD',
		borderBottomColor: 'transparent',
		borderLeftColor: 'transparent',
		borderRightColor: 'transparent',
		borderWidth: 1,
		// paddingTop: 2
	},
	txtInput: {
		flex: 6,
		paddingLeft: 20
	},
	sendButton: {
		flex: 1,
		justifyContent: 'center',
		marginRight: 10,
		// backgroundColor: '#757575'
	},
	sendButtonText: {
		fontSize: 19,
		textAlign: 'center',
		fontWeight: 'bold'
	},
	gray: {
		color: 'darkgray',
	},
	blue: {
		color: '#536DFE',
	}
})

export default AnimatedTextInput
