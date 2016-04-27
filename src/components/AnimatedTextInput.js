import React, {
  Component,
  PropTypes,
  StyleSheet,
  Dimensions,
  // Animated,
  Text,
  TextInput,
  View,
  TouchableHighlight
} from 'react-native';
// import Button from 'react-native-button';

// how to animate the send button? what props do I pass down?
const AnimatedTextInput = ({
	text,
	sendMessage
}) =>
	<View style={styles.txtInputContainer}>
		<TextInput
			style={styles.txtInput}
			defaultValue={text}
			onChangeText={ (newText) => text=newText /* should send an action...*/}
		/>
		<TouchableHighlight 
			style={styles.sendButton}
			onPress={sendMessage(text)}>
			<Text style={styles.sendButtonText}>
				{'Send'}
			</Text>
		</TouchableHighlight>
	</View>

AnimatedTextInput.propTypes = {
	text: React.PropTypes.string.isRequired,
	sendMessage: React.PropTypes.func.isRequired
}

AnimatedTextInput.defaultProps = {
	...Component.defaultProps,
	text: 'xBand is swank!',
	sendMessage: function(text) {
		/* 
			send an action with the text? fuckkkkkkkkkkkk
			I should be using redux, just I can't piece it together
		*/

		console.log('message sent! --> ', text);
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
		marginRight: 10
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