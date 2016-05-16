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

import { Actions } from 'react-native-router-flux'

// how to animate the send button? what props do I pass down?
export default class AnimatedTextInput extends Component {

	static propTypes = {
		// text: PropTypes.string.isRequired,
		addMessage: PropTypes.func.isRequired,
		conversation: PropTypes.object.isRequired
	}

	constructor(props) {
		super(props)

		// console.log('textinput', this.props)

		this.state = {
			text: '',
			buttonState: true,
			colorStyle: styles.gray
		};
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

						this.props.addMessage({
							message: {
								from: '44.00.9.15',
								body: this.state.text,
								position: 'right',
								timestamp: 'now_or_never'
							},
							conversation: {
								...this.props.conversation
								// ^^^^ to edit the conversation so I can add the msg
							}
						});

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
});

export default AnimatedTextInput