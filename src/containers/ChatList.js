import React, { Component, PropTypes, View, Text, ScrollView } from 'react-native'
import { bindActionCreators } from 'redux';
import { connect } from 'react-redux';
import _ from 'lodash';

import ChatCell from '../components/ChatCell';

const mapStateToProps = state => {
   return {
      conversation: state.conversation,
      messages: state.message
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
		
		// return ( <Text style={{marginTop: 100}}>Hey</Text>)
		return (
			<ScrollView style={{ marginTop: 60 }}>
				{
					/* 
						wrap in a touchableopacity to get the onTouch event
						and call and call action to route with the conversation key
					*/
					_.map(this.lastMessage(), (message) => {
						console.log('message',message);
						return <ChatCell lastMessage={message}
										 key={_.uniqueId()}/>
					})
				}
			</ScrollView>
		)
	}

	lastMessage() {

		console.log('chat list props',this.props);

		lastMsgKeys = _.map(this.props.conversation, (conv, key) => {
			return {
				conversationKey: key,
				messageKey: conv.messages.pop()
			}
		})

		console.log('lastMsgKeys',lastMsgKeys);

		lastMsges = _.map(lastMsgKeys, (keys) => {
			return this.props.messages[keys.messageKey]
		})

		console.log('last msges',lastMsges);

		return lastMsges
	}
}

export default connect(mapStateToProps)(ChatList);