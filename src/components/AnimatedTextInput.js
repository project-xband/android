import React, { Component } from 'react'
import {
  PropTypes,
  StyleSheet,
  Dimensions,
  Text,
  TextInput,
  View,
  TouchableHighlight
} 													from 'react-native'
import UUID 								from 'uuid-js'
import { Actions } 					from 'react-native-router-flux'
import SendModuleAndroid 		from './SendModuleAndroid'

class AnimatedTextInput extends Component {

	static propTypes = {
		/* text: PropTypes.string.isRequired,

			^ TODO:
			if the user has unfinished message,
			a draft, pass it down here and initialise
			the state with that prop
		*/
		onFocus 											: PropTypes.func,
		addMessage 										: PropTypes.func  .isRequired,
		conversationKey								: PropTypes.string.isRequired,
		address								        : PropTypes.string.isRequired,
		addMessageKeyToTheConversation: PropTypes.func  .isRequired
	}

	constructor(props) {
		super(props)

		// console.log('textinput', this.props)

		this.state = {
			address     : props.address,
			text        : '',
			buttonState : true,
			colorStyle  : styles.gray
		}
	}

	render() {
		return(
			<View style = { styles.txtInputContainer }>
				<TextInput
					style 			 = { styles.txtInput }
					defaultValue = { this.state.text }
					onFocus			 = { () => {
						this.props.onFocus('typing')
					}}
					onChangeText = { (text) => {
						this.setState({
							text: text
						})
						this.switchButton()
					}}
					onBlur = { () => {
						this.props.onFocus('reading')
					}}/>
				<TouchableHighlight
					disabled 			= { this.state.buttonState }
					underlayColor = 'transparent'
					style 				= { styles.sendButton }
					onPress 			= { () => {

						var hashprep = this.state.text + ":" 
						const msgkey = this.generateUniqueKey(hashprep)

						this.props.addMessage({
							uniqueKey 			: msgkey,
							from 						: '44.3.66',
							/*
								^ TODO:
								get the global address instead?
							*/
							conversationKey : this.props.conversationKey,
							body 						: this.state.text,
							position				: 'right',
							timestamp				: 'Mon'
							/*
								^ TODO:
								generate the timestamp based
								on shannons packet timestamp
								time formatting
							*/
						})

						const from = this.state //.address.replace(/.*?(\d+)[^\d]*$/,'$1')
						alert(from) 

						SendModuleAndroid.send({
							// uniqueKey 			: msgkey,
							from 						: '66',
							to              : '69',
							/*
								^ TODO:
								get the global address instead?
							*/
							conKey          : this.props.conversationKey,
							body 						: this.state.text,
							// position 				: 'right',
							// timestamp 			: 'Mon'
							/*
								^ TODO:
								generate the timestamp based
								on shannons packet timestamp
								time formatting
							*/
						})

						this.props.addMessageKeyToTheConversation({
							uniqueKey 		 : msgkey,
							conversationKey: this.props.conversationKey
						})

						this.setState({
							text 			 : '',
							colorStyle : styles.gray
						})
					}}>
					<Text style = { [
														styles.sendButtonText,
														this.state.colorStyle
													]}>
						{'Send'}
					</Text>
				</TouchableHighlight>
			</View>
		)
	}

	switchButton() {
		if (this.state.text != '') {
			this.setState({
				colorStyle : styles.blue,
				buttonState: false
			})
		} else {
			this.setState({
				colorStyle : styles.gray,
				buttonState: true
			})
		}
	}

	generateUniqueKey(str) {
		// TODO 32 BIT
		return this.hashFnv32a(str, true, undefined)
	}

	hashFnv32a(str, asString, seed) {
    /*jshint bitwise:false */
    var i, l,
        hval = (seed === undefined) ? 0x811c9dc5 : seed

    for (i = 0, l = str.length; i < l; i++) {
        hval ^= str.charCodeAt(i)
        hval += (hval << 1) + (hval << 4) + (hval << 7) + (hval << 8) + (hval << 24)
    }
    if( asString ){
        // Convert to 8 digit hex string
        return ("0000000" + (hval >>> 0).toString(16)).substr(-8)
    }
    return hval >>> 0
	}
}

const styles = StyleSheet.create({
	txtInputContainer : {
		flex							: 1,
		// height: Dimensions.get('window').height / 18,
		// width: Dimensions.get('window').width,
		flexDirection			: 'row',
		justifyContent		: 'center',

		borderTopColor    : '#BDBDBD',
		borderBottomColor : 'transparent',
		borderLeftColor		: 'transparent',
		borderRightColor	: 'transparent',
		borderWidth				: 1,

		padding 					: 4
	},
	txtInput					: {
		flex			    : 6,
		height				: 30,
		paddingLeft   : 20,
		paddingTop    : 5,
		paddingBottom : 5
	},
	sendButton			  : {
		flex 					 : 1,
		justifyContent : 'center',
		marginRight		 : 10,
	},
	sendButtonText    : {
		fontSize   : 19,
		textAlign	 : 'center',
		fontWeight : 'bold'
	},
	gray 							: {
		color: 'darkgray',
	},
	blue							: {
		color: '#536DFE',
	}
})

export default AnimatedTextInput
