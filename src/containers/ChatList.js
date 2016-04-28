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

		console.log('props of chat list',this.props);

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

	conversationKey() {

	}
}

export default connect(mapStateToProps)(ChatList);