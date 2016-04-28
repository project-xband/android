import React, {
  Component,
  PropTypes,
  StyleSheet,
  Dimensions,
  Text,
  TextInput,
  View,
  TouchableHighlight
} from 'react-native';
// import Button from 'react-native-button';

import {Actions} from 'react-native-router-flux'

// how to animate the send button? what props do I pass down?
export default class AnimatedTextInput extends Component {

	static propTypes = {
		// text: PropTypes.string.isRequired,
		addMessage: PropTypes.func.isRequired,
		conversation: PropTypes.object.isRequired
	}

	constructor(props) {
		super(props)

		console.log('textinput', this.props)

		this.state = {
			text: ''
		};
	}

	render() {
		return(
			<View style={styles.txtInputContainer}>
				<TextInput
					style={styles.txtInput}
					defaultValue={ this.state.text }
					onChangeText={ (text) => this.state = { text: text } /*TODO: send an action and store draft for each conversation*/}
				/>
				<TouchableHighlight
					style={styles.sendButton}
					onPress={ () => {

						Actions.ChatList()
						console.log('Actions',Actions)
						// console.log('Actions chatlist', Actions.ChatList())
						console.log('Actions chatlist', Actions.ChatList)

						this.props.addMessage({
							message: {
								from: '44.00.9.15',
								body: this.state.text,
								position: 'right',
								timestamp: 'now_or_never'
							},
							conversation: {
								...this.props.conversation
							}
						});

						this.setState({ text: '' });
					}}>
					<Text style={styles.sendButtonText}>
						{'Send'}
					</Text>
				</TouchableHighlight>
			</View>
		)
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
		color: '#536DFE',
		fontWeight: 'bold'
	}
});

export default AnimatedTextInput