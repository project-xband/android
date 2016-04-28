import React, { Component, PropTypes, View, ScrollView, TouchableOpacity, TouchableHighlight } from 'react-native'
import { bindActionCreators } from 'redux';
import { connect } from 'react-redux';
import _ from 'lodash';

import ChatCell from '../components/ChatCell';
import {Actions} from 'react-native-router-flux'

const mapStateToProps = state => {
   return {
   	  isLoaded: state.storage.isLoaded,
      conversation: state.conversation,
      messages: state.message,
   };
};

/* 
	TODO

	something in this file or it's children is fucking the store up. 

	if ChatCell fails to load the store loads itself fine
	try commenting the _.map({}) out and see for yourself
*/

class ChatList extends Component {
	render() {
		if (!this.props.isLoaded) {
			console.log('is not loaded')
			return <View/>
		}
		console.log('is loaded')
		
		// return ( <Text style={{marginTop: 100}}>Hey</Text>)
		return (
			<ScrollView style={{ marginTop: 60 }}>
				{
					/* 
						wrap in a touchableopacity to get the onTouch event
						and call and call action to route with the conversation key
					*/
					_.map(this.lastMessage(), (keys) => {
						console.log('keys', keys);
						conversationKey = keys.conversationKey
						message 		= this.props.messages[keys.messageKey]

						// console.log('conversationKey', conversationKey)
						// console.log('message', message)

						// the conversation keys don't get assigned when the cell instatiates
						// but at runtime when you press the cell, result -> all keys are the same one

						return (
							<TouchableOpacity onPress={ () => { Actions.ChatContainer({ conversationKey: conversationKey })}}
											  key={_.uniqueId()}>
								<ChatCell lastMessage={message}
										  key={_.uniqueId()}/>
							</TouchableOpacity>
						)
					})
				}
			</ScrollView>
		)
	}

	lastMessage() {
		// console.log('chat list props',this.props);
		lastMsgKeys = _.map(this.props.conversation, (conv, key) => {
			return {
				conversationKey: key,
				messageKey: conv.messages.pop()
			}
		})
		// this returns tuples of (conversationKey, messageKey)
		console.log('lastMsgKeys ', lastMsgKeys)
		return lastMsgKeys

		// console.log('lastMsgKeys',lastMsgKeys);
		// lastMsges = _.map(lastMsgKeys, (keys) => {
		// 	return this.props.messages[keys.messageKey]
		// })
		// console.log('last msges',lastMsges);
		// this returns the last messages of every conversation
		// return lastMsges
	}
}

export default connect(mapStateToProps)(ChatList);